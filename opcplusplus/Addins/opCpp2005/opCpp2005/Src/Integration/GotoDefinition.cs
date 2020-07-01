///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: GotoDefinition.cs
/// Date: 09/26/2007
///
/// Description:
///
/// Go To Definition / Go To Original Code File
///****************************************************************

using System;
using EnvDTE;
using System.Collections.Generic;
using Extensibility;
using EnvDTE80;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.VCProject;
using Microsoft.VisualStudio.VCProjectEngine;
using Microsoft.VisualStudio.VCCodeModel;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using System.IO;
using System.Windows.Forms;

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
// 		private bool ParseLineDirective(string directive, ref int linenumber, ref string filename)
// 		{
// 			if (!directive.StartsWith("#line "))
// 				return false;
// 
// 			int findquote = directive.IndexOf('"');
// 			if (findquote == -1 || findquote < 7)
// 				return false;
// 
// 			string linestring = directive.Substring(6, findquote - 6);
// 			linenumber = Convert.ToInt32(linestring);
// 
// 			int findlastquote = directive.LastIndexOf('"');
// 			if (findlastquote == findquote || findlastquote >= directive.Length)
// 				return false;
// 
// 			filename = directive.Substring(findquote + 1, findlastquote - findquote - 1);
// 
// 			return true;
// 		}

		// Returns if the current position is valid, and whether the redirected file exists
		//private bool CheckGotoOriginal(out string redirectedfile, out int redirectedline)
		//{
			//TODO: well, it would be nice to have this,
			//      but I have to first find the #line --without-- moving the cursor
			//		which would be nice but I don't know how to do that currently.
			//does FindPattern maybe work?

		//}

		private CodeElement FindBestElement(VirtualPoint activepoint, CodeElements elements)
		{

			foreach (CodeElement ce in elements)
			{
// 				if (ce.EndPoint.LessThan(activepoint))
// 				{
// 					continue;
// 				}
// 				else if (ce.StartPoint.GreaterThan(activepoint))
// 				{
// 					break;
// 				}

				//ok, on the right line now
				//now find the correct range

				if (ce.StartPoint.EqualTo(activepoint) || ce.EndPoint.EqualTo(activepoint)
				|| (ce.StartPoint.LessThan(activepoint) && ce.EndPoint.GreaterThan(activepoint)))
				{
					//this is the element we want, actually we should keep looking to see if we get a better match
					CodeElement childce = FindBestElement(activepoint, ce.Children);

					if (childce != null)
						return childce;
					
					return ce;
				}
			}

			return null;
		}

		/*

		// Called when Goto Definition command is executed.
		private void GotoDefinitionCallback(object sender, EventArgs e)
		{
			Document doc = App().ActiveDocument;
			if (doc != null)
			{
				TextDocument text = (TextDocument)doc.Object("");

				//this is the current selection's top line #
				int line = text.Selection.CurrentLine;
				int col = text.Selection.CurrentColumn;

				VirtualPoint activepoint = text.Selection.ActivePoint;

				EditPoint point = text.Selection.ActivePoint.CreateEditPoint();
				
				//TODO: need to grab the VCFileCodeModel!
				//		the normal one doesn't give us enough info??

				//doc.ProjectItem.ContainingProject.CodeModel

				CodeElement bestmatch = null;
				
				//NOTE: need to grab the filecodemodel
				FileCodeModel model = doc.ProjectItem.FileCodeModel;
				if(model != null)
				{
					bestmatch = FindBestElement(activepoint, model.CodeElements);

					if(bestmatch != null)
					{
						VCCodeElement vcelement = (VCCodeElement)bestmatch;
// 
// 						foreach(VCCodeElement element in vcelement.Collection)
// 						{
// 							string name = element.Name;
// 						}

						string location = vcelement.get_Location(vsCMWhere.vsCMWhereDeclaration);
						string defaultlocation = vcelement.get_Location(vsCMWhere.vsCMWhereDefault);
						string definitionlocation = vcelement.get_Location(vsCMWhere.vsCMWhereDefinition);

						///VCCodeFunction fcn = (VCCodeFunction)vcelement;

// 						CodeFunction2 fcn;
//						
// 						CodeClass celement = (CodeClass)bestmatch;
// 						if (celement != null)
// 						{
// 						}
					}
				}

			}
		}

		private void GotoDefinitionStatus(object sender, EventArgs e)
		{
		}
		*/

		// Called when Goto Note command is executed.
		private void GotoNoteCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			string actualfile = "";
			int actualline = 0;

			//get the goto info
			bool bResult = GetGotoInfo(ref actualfile, ref actualline);

			if (bResult)
			{
				bool bExists = System.IO.File.Exists(actualfile);
				if (!bExists)
					return;
				
 				//TODO: check if the line number is valid, and only proceed if it is
 				Window docwindow = App().ItemOperations.OpenFile(actualfile, EnvDTE.Constants.vsViewKindTextView);
 				TextDocument newdoc = (TextDocument)docwindow.Document.Object("");

				newdoc.Selection.GotoLine(actualline, true);
 				newdoc.Selection.ActivePoint.TryToShow(vsPaneShowHow.vsPaneShowCentered, null);
			}
		}

		private void GotoNoteStatus(object sender, EventArgs e)
		{
			try
			{
				if (DocumentCommandsUnavailable())
					throw new Exception();
				
				if (IsInGeneratedCode())
				{
					string file = "";
					int line = 0;

					if (GetGotoInfo(ref file, ref line))
					{
						Document doc = App().ActiveDocument;
						string myname = StringUtility.RLeft(doc.Name, ".");
						if (!file.Contains(myname))
						{
							GotoNoteCommand.Visible = true;

							bool bExists = System.IO.File.Exists(file);
							if (bExists)
								GotoNoteCommand.Enabled = true;
							else
								GotoNoteCommand.Enabled = false;

							return;
						}
					}
				}
			}
			catch(Exception) {}

			GotoNoteCommand.Visible = false;
			GotoNoteCommand.Enabled = false;
		}

		private bool GetGotoInfo(ref string actualfile, ref int actualline)
		{
			int original = 0;
			int gendepth = 0;
			int filedepth = 0;
			return GetGotoInfo(ref actualfile, ref actualline, ref original, ref gendepth, ref filedepth);
		}

		//NOTE: should not check the file for existance...
		private bool GetGotoInfo(ref string actualfile, ref int actualline, ref int originalline, ref int gendepth, ref int filedepth)
		{
			//first we need to find the text thing
			Document doc = App().ActiveDocument;
			if (doc != null)
			{
				TextDocument text = (TextDocument)doc.Object("");

				//this is the current selection's top line #
				int line = text.Selection.CurrentLine;
				int col = text.Selection.CurrentColumn;

				EditPoint point = text.Selection.ActivePoint.CreateEditPoint();
				point.EndOfLine();

				EditPoint endfound = null;
				TextRanges found = null;
				bool result = point.FindPattern("\\#line{.*}\\\"{.*}\\\"//\\[{.*}\\]", (int)(vsFindOptions.vsFindOptionsBackwards | vsFindOptions.vsFindOptionsRegularExpression), ref endfound, ref found);

				if (result)
				{
					int foundline = endfound.Line;

					//NOTE: I think this is correct...
					int offset = line - foundline - 1;

					TextRange linematch = found.Item(2);
					string linematchtext = linematch.StartPoint.GetText(linematch.EndPoint);

					TextRange filematch = found.Item(3);
					string filematchtext = filematch.StartPoint.GetText(filematch.EndPoint);

					TextRange optionmatch = found.Item(4);
					string optionstext = optionmatch.StartPoint.GetText(optionmatch.EndPoint);

					//parse the line directive
					int redirectedline = Convert.ToInt32(linematchtext);
					string redirectedfile = filematchtext;

					//parse the additional information
					string[] options = optionstext.Split(',');

					gendepth = 0;
					originalline = -1;
					filedepth = 0;

					//first number: the depth of the generated path
					if(options.Length > 0)
					{
						string generatedDepth = options[0];
						gendepth = Convert.ToInt32(generatedDepth);
					}

					//second number: the depth of the file path (relative to the generated?)
					if (options.Length > 1)
					{
						string fileDepth = options[1];
						filedepth = Convert.ToInt32(fileDepth);
					}

					//third number: the original line number
					if (options.Length > 2)
					{
						string originline = options[2];
						originalline = Convert.ToInt32(originline);
					}

					if (offset == -1)
						offset = 0;

					if (offset >= 0)
					{
						string dots = "";

						for(int i = 0; i < gendepth; i++)
						{
							dots += "..\\";
						}

						string myfile = StringUtility.RLeft(doc.FullName, "\\");

						if (!System.IO.Path.IsPathRooted(redirectedfile))
						{
							actualfile = myfile + "\\" + dots + redirectedfile;
						}
						else
							actualfile = redirectedfile;

						actualfile = System.IO.Path.GetFullPath(actualfile);

						actualline = redirectedline + offset;

						return true;
					}
				}
			}

			return false;
		}


		// Called when Goto Original command is executed.
		private void GotoOriginalCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;
			
			string actualfile = "";
			int actualline = 0;
			int originalline = 0;
			int gendepth = 0;
			int filedepth = 0;

			//get the goto info
			bool bResult = GetGotoInfo(ref actualfile, ref actualline, ref originalline, ref gendepth, ref filedepth);

			if (bResult)
			{
				string realpath = actualfile;

				//if we have no original line, then 
				if (originalline != -1)
				{
					Document doc = App().ActiveDocument;

					string docpath = doc.FullName;
					string originalpath = Paths.GeneratedToOriginalPath(docpath, gendepth, filedepth);
					realpath = originalpath;
				}
				else
					originalline = actualline;

				realpath = Path.GetFullPath(realpath);

				bool bExists = System.IO.File.Exists(realpath);
				if (bExists)
				{
					Window docwindow = App().ItemOperations.OpenFile(realpath, EnvDTE.Constants.vsViewKindTextView);
					TextDocument newdoc = (TextDocument)docwindow.Document.Object("");

					newdoc.Selection.GotoLine(originalline, true);
					newdoc.Selection.ActivePoint.TryToShow(vsPaneShowHow.vsPaneShowCentered, null);
				}
			}
		}
		
		private bool IsInGeneratedCode()
		{
			if (App().ActiveDocument != null)
			{
				string extension = StringUtility.RRight(App().ActiveDocument.Name, ".");
				if (extension == "ooh" || extension == "ocpp")
				{
					return true;
				}
			}

			return false;
		}

		private void GotoOriginalStatus(object sender, EventArgs e)
		{
			try
			{
				if (DocumentCommandsUnavailable())
					throw new Exception();

				if (IsInGeneratedCode())
				{
					string file = "";
					int line = 0;
					int original = 0;
					int gendepth = 0;
					int filedepth = 0;

					if (GetGotoInfo(ref file, ref line, ref original, ref gendepth, ref filedepth))
					{
						GotoOriginalCommand.Visible = true;

						string realpath = file;

						//if we have an original line, then 
						if (original != -1)
						{
							Document doc = App().ActiveDocument;

							string docpath = doc.FullName;
							string originalpath = Paths.GeneratedToOriginalPath(docpath, gendepth, filedepth);
							realpath = originalpath;
							original = line;
						}

						realpath = Path.GetFullPath(realpath);

						bool bExists = System.IO.File.Exists(realpath);

						if (bExists)
							GotoOriginalCommand.Enabled = true;
						else
							GotoOriginalCommand.Enabled = false;

						return;
					}
				}
			}
			catch (Exception) { }

			GotoOriginalCommand.Visible = false;
			GotoOriginalCommand.Enabled = false;
		}
	};
}





