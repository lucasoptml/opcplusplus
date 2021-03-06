///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Connect.cs
/// Date: 09/18/2007
///
/// Description:
///
/// This class allows the addin to connect to visual studio's IDE.
///****************************************************************

using System;
using Extensibility;
using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using System.IO;
using System.Reflection;

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
        // This method updates the status bar.
        void SetStatusText(string text)
        {
            App().StatusBar.Text = text;
        }

		// Create all the commands and link them to callbacks.
		public void CreateCommands()
		{
			GlobalSettingsCommand = CreateCommand(PkgCmdIDList.cmdopGlobalSettings, GlobalSettingsCallback, GlobalSettingsStatus);
			ProjectSettingsCommand = CreateCommand(PkgCmdIDList.cmdopProjectSettings, ProjectSettingsCallback, ProjectSettingsStatus);
			FeatureManagerCommand = CreateCommand(PkgCmdIDList.cmdopFeatureManager, FeatureManagerCallback, FeatureManagerStatus);

			BuildProjectCommand = CreateCommand(PkgCmdIDList.cmdopBuildProject, BuildProjectCallback, ButtonStatus);
			BuildSolutionCommand = CreateCommand(PkgCmdIDList.cmdopBuildSolution, BuildSolutionCallback, ButtonStatus);
			CleanSolutionCommand = CreateCommand(PkgCmdIDList.cmdopCleanSolution, CleanSolutionCallback, ButtonStatus);
			StopBuildCommand = CreateCommand(PkgCmdIDList.cmdStop, StopBuildCallback, StopStatus);

			GotoOriginalCommand = CreateCommand(PkgCmdIDList.cmdGotoOriginal, GotoOriginalCallback, GotoOriginalStatus);
			//GotoDefinitionCommand = CreateCommand(PkgCmdIDList.cmdGotoDefinition, GotoDefinitionCallback, GotoDefinitionStatus);
			GotoNoteCommand = CreateCommand(PkgCmdIDList.cmdGotoNote, GotoNoteCallback, GotoNoteStatus);

			GotoOOHCommand = CreateCommand(PkgCmdIDList.cmdGotoOOH, GotoOOHCallback, GotoOOHStatus);
			GotoOCPPCommand = CreateCommand(PkgCmdIDList.cmdGotoOCPP, GotoOCPPCallback, GotoOCPPStatus);

			OpenIncludeCommand = CreateCommand(PkgCmdIDList.cmdOpenInclude, OpenIncludeCallback, OpenIncludeStatus);

			VisualizeCommand = CreateCommand(PkgCmdIDList.cmdVisualize, VisualizeCallback, VisualizeStatus);

			//Argument Commands...

			int numargs = 99;
			
			ArgumentCommands = new OleMenuCommand[numargs];

			for (int i = 0; i < numargs; i++)
			{
				//setup the argument commands to call using the correct index
				int index = i;
				
				ArgumentCommands[i] = 
				CreateCommand(
					(uint)PkgCmdIDList.cmdArgumentStart + (uint)i,
					ArgumentCallback,
					ArgumentStatus
					);
			}
			
			ArgumentLabelCommand = CreateCommand(PkgCmdIDList.opCppArgumentMenu, ArgumentLabelCallback, ArgumentLabelStatus);

			// 			CreateCommand((uint)VSConstants.VSStd97CmdID.BuildSln, BuildSolutionCallback, ButtonStatus, new Guid(GuidList.guidVisualStudioCommands) );

			commandEvents = App().Events.get_CommandEvents("{00000000-0000-0000-0000-000000000000}", 0);
			commandEvents.BeforeExecute += new _dispCommandEvents_BeforeExecuteEventHandler(BeforeExecute);
			commandEvents.AfterExecute  += new _dispCommandEvents_AfterExecuteEventHandler(AfterExecute);
		}

		public OleMenuCommand CreateCommand(uint commandid, EventHandler callback, EventHandler statuscallback, Guid guid)
		{
			OleMenuCommandService mcs = GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
			if (mcs != null)
			{
				// Create the command for the menu item.
				CommandID menuCommandID = new CommandID(guid, (int)commandid);
				OleMenuCommand menuItem = new OleMenuCommand(new EventHandler(callback), menuCommandID);
				mcs.AddCommand(menuItem);

				menuItem.BeforeQueryStatus += new EventHandler(statuscallback);

				return menuItem;
			}

			return null;
		}

		//Create a command and link it to a callback function
		public OleMenuCommand CreateCommand(uint commandid, EventHandler callback, EventHandler statuscallback)
		{
			return CreateCommand(commandid, callback, statuscallback, GuidList.guidopCpp2005CmdSet);
		}

		// CommandEvents Object
		private CommandEvents commandEvents;
	}
}