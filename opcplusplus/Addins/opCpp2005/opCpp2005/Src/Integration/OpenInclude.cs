///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: OpenInclude.cs
/// Date: 09/28/2007
///
/// Description:
///
/// Open Include Commands
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
		private void OpenIncludeStatus(object sender, EventArgs e)
		{
			try
			{
				if (DocumentCommandsUnavailable())
					throw new Exception();

				string includefile = "";

				//check it
				if (GetIncludeInfo(ref includefile))
				{
					OpenIncludeCommand.Visible = true;
					OpenIncludeCommand.Text = "Open \"" + includefile + "\"";

					if (ResolveIncludePath(ref includefile))
					{
						OpenIncludeCommand.Enabled = true;
					}
					else
						OpenIncludeCommand.Enabled = false;

					return;
				}
			}
			catch (Exception) { }

			OpenIncludeCommand.Visible = false;
		}

		private void OpenIncludeCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			//run it

			string includefile = "";

			//check it
			if (GetIncludeInfo(ref includefile))
			{
				if (ResolveIncludePath(ref includefile))
				{
					Window docwindow = App().ItemOperations.OpenFile(includefile, EnvDTE.Constants.vsViewKindTextView);
					//TextDocument newdoc = (TextDocument)docwindow.Document.Object("");
				}

				return;
			}
		}

		private bool GetIncludeInfo(ref string path)
		{
			//first we need to find the text thing
			Document doc = App().ActiveDocument;
			if (doc != null)
			{
				TextDocument text = (TextDocument)doc.Object("");

				//this is the current selection's top line #
				int line = text.Selection.CurrentLine;
				int col = text.Selection.CurrentColumn;

				VirtualPoint activepoint = text.Selection.ActivePoint;
				EditPoint point = activepoint.CreateEditPoint();
				point.EndOfLine();

				EditPoint endfound = null;
				TextRanges found = null;
				bool result = point.FindPattern("opinclude[ \t]+\\\"{.*}\\\"", (int)(vsFindOptions.vsFindOptionsBackwards | vsFindOptions.vsFindOptionsRegularExpression), ref endfound, ref found);

				if (result)
				{
					int foundline = endfound.Line;

					//NOTE: I think this is correct...
					int offset = line - foundline - 1;//TODO: get this 1 to be constant in macro expansion!

					TextRange fullmatch = found.Item(1);

					// need to break out if the active point wasn't in the line
					if (fullmatch.StartPoint.Line != activepoint.Line)
						return false;

					TextRange filematch = found.Item(2);
 					string filetext = filematch.StartPoint.GetText(filematch.EndPoint);

					//NOTE: this returns what was in the opinclude, but does not resolve the path
					path = filetext;

					return true;
				}
			}

			return false;
		}

		// resolve the path to an absolute path, verify that the file exists
		private bool ResolveIncludePath(ref string filepath)
		{
			string docpath = App().ActiveDocument.FullName;
			
			//1. attempt to find it using the current file first
			string current = Paths.RelativePath(docpath, filepath);

			if (System.IO.File.Exists(current))
			{
				filepath = current.ToLower();
				return true;
			}

			//2. could attempt to find it using the standard directories
			//find it using the standard directories...
			string opcppdir = Paths.GetFullAppPath();

			string stddir = opcppdir + "..\\..\\..\\opcpp\\dialects\\";

			string testpath = stddir + filepath;
			testpath = System.IO.Path.GetFullPath(testpath);

			if(System.IO.File.Exists(testpath))
			{
				filepath = testpath.ToLower();
				return true;
			}

			//TODO: add these other cases...

			//3. could attempt to find it using the directory settings/project directories...
			


			return false;
		}

	}
}
