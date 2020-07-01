///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Arguments.cs
/// Date: 10/17/2007
///
/// Description:
///
/// Arguments Code File
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
	partial class opCpp2005
	{
		OleMenuCommand[] ArgumentCommands;
		List<string> Arguments = new List<string>();

		OleMenuCommand ArgumentLabelCommand;



		void ArgumentLabelStatus(object sender, EventArgs e)
		{
			OleMenuCommand command = sender as OleMenuCommand;

			try
			{
				if (DocumentCommandsUnavailable())
					throw new Exception();

				UpdateArguments();

				if (Arguments.Count > 0)
				{
					command.Visible = true;

					return;
				}

			}
			catch(Exception) {}

			command.Visible = false;
		}

		void ArgumentLabelCallback(object sender, EventArgs e)
		{

		}



		void ArgumentStatus(object sender, EventArgs e)
		{
			OleMenuCommand command = sender as OleMenuCommand;

			try
			{
				//figure out the index based on the command id
				int id = command.CommandID.ID;
				int index = id - (int)PkgCmdIDList.cmdArgumentStart;

				if (index < Arguments.Count)
				{
					command.Text = Arguments[index];

					command.Visible = true;
					return;
				}
			}
			catch (Exception) { }

			command.Visible = false;
		}

		void ArgumentCallback(object sender, EventArgs e)
		{
			OleMenuCommand command = sender as OleMenuCommand;

			try
			{
				//figure out the index based on the command id
				int id = command.CommandID.ID;
				int index = id - (int)PkgCmdIDList.cmdArgumentStart;

				if (index < Arguments.Count)
				{
					string text = "";
					if (NoteArguments.Length > 0
					&& NoteArguments[0] != "")
						text += ",";

					text += Arguments[index];

					ArgumentEnd.Insert(text);
					
					return;
				}
			}
			catch (Exception) { }
		}

		bool UpdateXml(ref string path)
		{
			bool result = IsGeneratedFileAvailable(".xml", ref path);

			if (!result)
				return false;

			if(path == XmlPath)
			{
				DateTime Timestamp = System.IO.File.GetLastWriteTime(path);
				if (XmlTimestamp == Timestamp)
					return true;
			}

			XmlTimestamp = System.IO.File.GetLastWriteTime(path);
			XmlPath = path;

			XmlFile.Reload(path);

			return true;
		}

		DateTime XmlTimestamp;
		string XmlPath;
		XmlDialectLoader XmlFile = new XmlDialectLoader();

		//these are available if status is visible...
		EditPoint ArgumentEnd = null;
		string[] NoteArguments = null;

		void UpdateArguments()
		{
			Document doc = App().ActiveDocument;

			//whats the xml file's path?
			string xmlpath = "";
			bool available = UpdateXml(ref xmlpath);

			if(!available)
			{
				Arguments.Clear();
				return;
			}
			
			TextDocument text = (TextDocument)doc.Object("");
			VirtualPoint currentpoint = text.Selection.ActivePoint;

			EditPoint point = currentpoint.CreateEditPoint();
			point.EndOfLine();

			EditPoint endfound = null;
			TextRanges found = null;

			Arguments.Clear();
			
			bool result = point.FindPattern("note[ \t]+{.*}\\({.*}\\)", (int)(vsFindOptions.vsFindOptionsBackwards | vsFindOptions.vsFindOptionsRegularExpression), ref endfound, ref found);

			if(result)
			{
				TextRange fullmatch = found.Item(1);
				TextRange pathmatch = found.Item(2);
				TextRange argmatch = found.Item(3);

				//only want it if the point was in the range
				if (fullmatch.EndPoint.LessThan(currentpoint)
				|| fullmatch.StartPoint.GreaterThan(currentpoint))
					return;

				ArgumentEnd = argmatch.EndPoint;

				string arguments = argmatch.StartPoint.GetText(argmatch.EndPoint).Trim();
				
				NoteArguments = arguments.Split(',');

				string notepath = pathmatch.StartPoint.GetText(pathmatch.EndPoint).Trim();
			
				XmlNoteNode note = XmlFile.GetNote(notepath);

				if (note != null)
				{
                    Arguments = new List<string>();

                    foreach (XmlArgumentNode n in note.Arguments)
                        Arguments.Add(n.Name);
				}
			}
		}
	};
}









