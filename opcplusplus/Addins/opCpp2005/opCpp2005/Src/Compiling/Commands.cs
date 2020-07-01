///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Commands.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Command-related stuff.
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
using System.Text.RegularExpressions;

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
        /*=== code for locking/unlocking the addin ===*/

        private bool bAddinLocked = false;

		public void LockAddin()
		{
			bAddinLocked = true;
		}

        public void UnlockAddin()
        {
            bAddinLocked = false;
        }
		
		//
		// All the commands
		//
		private OleMenuCommand GlobalSettingsCommand;
		private OleMenuCommand ProjectSettingsCommand;
		private OleMenuCommand BuildSolutionCommand;
		private OleMenuCommand BuildProjectCommand;
		private OleMenuCommand CleanSolutionCommand;
		private OleMenuCommand GotoOriginalCommand;
		private OleMenuCommand FeatureManagerCommand;
		private OleMenuCommand GotoNoteCommand;
		//private OleMenuCommand GotoDefinitionCommand;
		private OleMenuCommand OpenIncludeCommand;
		private OleMenuCommand GotoOOHCommand;
		private OleMenuCommand GotoOCPPCommand;
		private OleMenuCommand VisualizeCommand;
		private OleMenuCommand StopBuildCommand;

		//TODO: figure out a fast way to make sure statuses are not
		//      queried in c# for example...
// 		bool AllCommandsUnavailable()
// 		{
// 			if (App() == null)
// 				return true;
// 			
// 			
// 			
// 			if ()
// 			{
// 			
// 			}
// 		}


		//
		// Command callbacks
		//

		// Called when Global Settings command is executed.
		private void GlobalSettingsCallback(object sender, EventArgs e)
		{
			//don't lockdown global settings...

			GlobalOptionsForm form = new GlobalOptionsForm();
			form.ShowDialog();
		}

		// Called when Project Settings command is executed.
		private void ProjectSettingsCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;
			
			ProjectOptionsForm form = new ProjectOptionsForm();

            form.ShowDialog();
		}

		// Called when Feature Manager command is executed.
		private void FeatureManagerCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			FeatureManagerForm form = new FeatureManagerForm();

			form.ShowDialog();
		}

		// Called when opCpp Build Solution command is executed.
		private void BuildSolutionCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			HandleBuildSolution(false);
		}

		// Called when opCpp Build Project is executed.
		private void BuildProjectCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			HandleBuildProject(false);
		}

		// Called when opCpp Clean Solution command is executed.
		private void CleanSolutionCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			HandleCleanSolution(false);
		}
		
		// Called when opCpp Stop Build command is executed
		private void StopBuildCallback(object sender, EventArgs e)
		{
			if (bAddinLocked)
				return;

			//Stop the build if it's going...
			if (CurrentThread != null)
			{
				CurrentThread.Stop();
				CurrentThread = null;
			}

			LogCompile("opC++ Build Stopped.");
		}

		//
		// Command status callbacks
		//

		private void ButtonStatus(object sender, EventArgs e)
		{
			if (bAddinLocked)
			{
				DisableCompileButtons();
				return;
			}

			bool hasOhFiles = ProjectUtility.SolutionHasOhFiles(App().Solution);
			
			if (IsCompiling() || hasOhFiles == false)
			{
				DisableCompileButtons();
			}
			else
				EnableCompileButtons();

		}

		private void StopStatus(object sender, EventArgs e)
		{
			if (bAddinLocked)
			{
				StopBuildCommand.Enabled = false;
				return;
			}

			if(IsCompiling())
			{
				StopBuildCommand.Enabled = true;
			}
			else
			{
				StopBuildCommand.Enabled = false;
			}
		}

		private void DisableCompileButtons()
		{
			BuildSolutionCommand.Enabled = false;
			BuildProjectCommand.Enabled = false;
			CleanSolutionCommand.Enabled = false;
		}

		private void EnableCompileButtons()
		{
			BuildSolutionCommand.Enabled = true;
			BuildProjectCommand.Enabled = true;
			CleanSolutionCommand.Enabled = true;

		}

		private void GlobalSettingsStatus(object sender, EventArgs e)
		{

		}

		private void FeatureManagerStatus(object sender, EventArgs e)
		{
			if (bAddinLocked)
			{
				FeatureManagerCommand.Enabled = false;
				return;
			}

#if DEBUG
			bool hasOhFiles = ProjectUtility.SolutionHasOhFiles(App().Solution);

			FeatureManagerCommand.Enabled = hasOhFiles;
#else
			FeatureManagerCommand.Enabled = false;
#endif
		}

		private void ProjectSettingsStatus(object sender, EventArgs e)
		{
			if (bAddinLocked)
			{
				ProjectSettingsCommand.Enabled = false;
				return;
			}
						
			bool hasOhFiles = ProjectUtility.SolutionHasOhFiles(App().Solution);

			ProjectSettingsCommand.Enabled = hasOhFiles;
		}


		// Handle No Debug Start (may be called directly or intercepted)
		void HandleNoDebugStart(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.NoDebug, SelectionTypes.Solution, bIntercepted));
		}

		// Handle Debug Start (may be called directly or intercepted)
		void HandleDebugStart(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Debug, SelectionTypes.Solution, bIntercepted));
		}

		// Handle opCpp Build Solution (may be called directly or intercepted)
		void HandleBuildSolution(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Build, SelectionTypes.Solution, bIntercepted));
		}

		// Handle opCpp Build Project (may be called directly or intercepted)
		void HandleBuildProject(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Build, SelectionTypes.Project, bIntercepted));
		}

		// Handle opCpp Build Project Only (intercepted)
		void HandleBuildProjectOnly(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Build, SelectionTypes.ProjectOnly, bIntercepted));
		}

		// Handle opCpp Rebuild Solution (intercepted)
		void HandleRebuildSolution(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Rebuild, SelectionTypes.Solution, bIntercepted));
		}

		// Handle opCpp Rebuild Project (intercepted)
		void HandleRebuildProject(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Rebuild, SelectionTypes.Project, bIntercepted));
		}

		// Handle opCpp Rebuild Project Only (intercepted)
		void HandleRebuildProjectOnly(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Rebuild, SelectionTypes.ProjectOnly, bIntercepted));
		}

		// Handle opCpp Clean Solution (may be called directly or intercepted)
		void HandleCleanSolution(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Clean, SelectionTypes.Solution, bIntercepted));
		}

		// Handle opCpp Clean Project (intercepted)
		void HandleCleanProject(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Clean, SelectionTypes.Project, bIntercepted));
		}

		// Handle opCpp Clean Project Only (intercepted)
		void HandleCleanProjectOnly(bool bIntercepted)
		{
			StartCompile(new CompileMode(CompileTypes.Clean, SelectionTypes.ProjectOnly, bIntercepted));
		}

		public enum SelectionTypes
		{
			None,
			Solution,
			Project,
			ProjectOnly,
		}

		public enum CompileTypes
		{
			None,
			Build,
			Rebuild,
			Clean,
			Debug,
			NoDebug,
		}

		public class CompileMode
		{
			public CompileMode(CompileTypes action, SelectionTypes selection, bool Intercepted)
			{
				Selection = selection;
				Action = action;
				bIntercepted = Intercepted;
			}

			public SelectionTypes Selection = SelectionTypes.None;
			public CompileTypes Action = CompileTypes.None;
			public bool bIntercepted = false;
		};

		bool IsCompiling()
		{
			return CurrentThread != null;
		}

		bool IsVSCompiling()
		{
			return App().Solution.SolutionBuild.BuildState == vsBuildState.vsBuildStateInProgress;
		}

		public struct CommandSetting
		{
			public Project Project;
			public string WorkingDir;
			public string ExePath;
			public string Arguments;
		};

		//Current Status
		private CompileMode CurrentMode;
		private opCppThread CurrentThread = null;

		void StartCompile(CompileMode mode)
		{
			// a compile is running already
			if (IsCompiling())
			{
				//TODO: should halt compiling really
				return;
				//CurrentThread.Stop();
			}

            if(mode.bIntercepted)
            {
                Options options = OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options;

                bool bExtendCommands = options.ExtendCommands.Value;

                if (!bExtendCommands)
                    return;
            }

			if(IsVSCompiling())
			{
				//cancel it
				App().ExecuteCommand("Build.Cancel", "");
			}

			// activate the output pane...
			GetOutputPane().Clear();
			ActivateOutputPane();

			// ok, lets attempt to start compiling now.

			CurrentThread = null;
			PostEvent = "";

			// no solution loaded
			DTE2 app = App();
			Solution solution = app.Solution;
			if (solution == null)
				return;

			// first save all the open files - this is a global setting
			SaveOpenFiles();

			List<Project> Projects;
			
			// find the selections we need (projects)
			if (mode.Selection == SelectionTypes.Solution)
				Projects = ProjectUtility.GetSolutionProjects();
			else if (mode.Selection == SelectionTypes.Project)
				Projects = ProjectUtility.GetActiveProjects();
			else// if (mode.Selection == SelectionTypes.ProjectOnly)
				Projects = ProjectUtility.GetActiveProjectOnly();


			//
			// Mode Arguments
			//
			// clean : send it -clean
			// rebuild : send it -force
			// build : none...
			string ModeArguments = "";
			if (mode.Action == CompileTypes.Rebuild)
				ModeArguments += "-force";
			else if (mode.Action == CompileTypes.Clean)
				ModeArguments += "-clean";
			//else if Debug || NoDebug || Build ... no additional needed

			//TODO: need the global options path
			string GlobalArguments = "-dependencies \"" + Paths.GetGlobalOptionsFilename() + "\"";

			//no projects to compile
			if (Projects.Count == 0)
				return;

			List<CommandSetting> CommandSettings = new List<CommandSetting>();

			int numprojects = Projects.Count;
			for (int ip = 0; ip < numprojects; ip++)
			{
				Project project = Projects[ip];

				string ActiveConfiguration = ProjectUtility.ProjectActiveConfiguration(project);

				List<VCFile> ohfiles = ProjectUtility.GetOhFiles(project);

				// does it contain active oh files? if not skip it.
				ProjectUtility.FilterActiveFiles(ref ohfiles, ActiveConfiguration);
				if (ohfiles.Count == 0)
					continue;

				// are the project settings set to enable opcpp?
				bool bProjectEnabled = true;
				if (!bProjectEnabled)
					continue;

				//3. do we have .doh files in the project?
				List<VCFile> dohfiles = ProjectUtility.GetDohFiles(project);
				ProjectUtility.FilterActiveFiles(ref dohfiles, ActiveConfiguration);
				// 				if (dohfiles.Count == 0)
				// 					continue;

				//fetch the project doh files from the global/project doh settings
				List<string> ProjectDohFiles = new List<string>();
				// 				if(ProjectDohFiles.Count + dohfiles.Count == 0)
				// 				{
				// 					//print a message "Project has oh files but no dialect was found or specified"
				// 					continue;
				// 				}

				//now build the settings
				CommandSetting setting = new CommandSetting();

				setting.Project = project;
				setting.WorkingDir = StringUtility.RLeft(project.FileName, "\\");

				//build the oh files list from the arguments
				//build the doh files list from the arguments
				string ohfilestring = "";
				for (int i = 0; i < ohfiles.Count; i++)
				{
					VCFile file = ohfiles[i];

					ohfilestring += '"';
					ohfilestring += file.FullPath;
					ohfilestring += '"';

					if (i + 1 < ohfiles.Count)
						ohfilestring += ',';
				}

				string dohfilestring = "";
				for (int i = 0; i < dohfiles.Count; i++)
				{
					VCFile file = dohfiles[i];
					dohfilestring += '"';
					dohfilestring += file.FullPath;
					dohfilestring += '"';

					if (i + 1 < dohfiles.Count)
						dohfilestring += ',';
				}

				for (int i = 0; i < ProjectDohFiles.Count; i++)
				{
					dohfilestring += '"';
					dohfilestring += ProjectDohFiles[i];
					dohfilestring += '"';

					if (i + 1 < ProjectDohFiles.Count)
						dohfilestring += ',';
				}

				string FileArguments = "";

				if (ohfilestring.Length > 0)
					FileArguments += " -oh " + ohfilestring;

				if (dohfilestring.Length > 0)
					FileArguments += " -doh " + dohfilestring;

				// fetch the global + overloaded project arguments
				CommandLineInfo info = OptionsManager.GetCommandLineInfo(project);
				setting.ExePath = info.ExecutablePath;

				FileArguments += " -dependencies \"" + Paths.GetProjectOptionsFilename(project) + "\"";

				if (setting.ExePath == "")
				{
					LogCompile("Error: opCpp exe path is undefined, please define in global and/or project settings");
					return;
				}

				string ProjectArguments = info.Arguments;

				string MacroArguments = FileArguments + " " + ProjectArguments + " " + ModeArguments + " " + GlobalArguments;

				if (opBeta.IsBeta)
					MacroArguments += " -beta";

				setting.Arguments = Paths.ResolveVisualStudioMacros(project, MacroArguments);

				//verify the macros worked...
				Regex findmacros = new Regex("$\\(*.\\)");
				Match result = findmacros.Match(setting.Arguments);
				if (result.Success)
				{
					string foundstring = setting.Arguments.Substring(result.Index, result.Length);
					LogCompile("Error: bad macro found in settings - " + foundstring);
					return;
				}

				CommandSettings.Add(setting);
			}
                            
			//no commands to execute
			if (CommandSettings.Count == 0)
			{
// 				CurrentMode = null;
// 				FinishedCompile(false);
				return;
			}

			//execute commands
			CurrentMode = mode;
			
			CurrentThread = new opCppThread(CommandSettings);
			CurrentThread.OnReadLine = LogCompile;
			CurrentThread.OnEnd = FinishedCompile;
			CurrentThread.Start();
		}

		public void LogCompile(string s)
		{
			PrintDebugLine(s);
			GetOutputPane().OutputString(s + "\n");
			Application.DoEvents();
		}
		
		//TODO: need to add debug, nodebug catching
		void FinishedCompile(bool bErrored)
		{
			PrintDebugLine("Finished opC++ Build.");
			//handle post-build operations
			CurrentThread = null;

			EnableCompileButtons();

			PostEvent = "";

			if (bErrored)
				return;

			if(CurrentMode != null)
			{
				// if the command was from the toolbar, we can customize the post build event using settings
				if (!CurrentMode.bIntercepted)
				{
					Options options = OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options;

					if (CurrentMode.Action == CompileTypes.Build)
					{
						if (CurrentMode.Selection == SelectionTypes.Solution)
						{
							//TODO: grab the command
							PostEvent = options.PostBuildSolution.Value;
						}
						else if (CurrentMode.Selection == SelectionTypes.Project)
						{
							//TODO: grab the command
							PostEvent = options.PostBuildProject.Value;
						}
					}
					else if (CurrentMode.Action == CompileTypes.Clean)
					{
						//TODO: grab the command
						PostEvent = options.PostCleanSolution.Value;
					}
				}
				else if (CurrentMode.Action == CompileTypes.Debug)
				{
					PostEvent = "Debug.Start";
				}
				else if (CurrentMode.Action == CompileTypes.NoDebug)
				{
					PostEvent = "Debug.StartWithoutDebugging";
				}
				else
				{
					//setup the post-build event
					if (CurrentMode.Action == CompileTypes.Build)
					{
						PostEvent = "Build.Build";
					}
					else if (CurrentMode.Action == CompileTypes.Rebuild)
					{
						PostEvent = "Build.Rebuild";
					}
					else if (CurrentMode.Action == CompileTypes.Clean)
					{
						PostEvent = "Build.Clean";
					}

					if (CurrentMode.Selection == SelectionTypes.Project)
					{
						PostEvent += "Selection";
					}
					else if (CurrentMode.Selection == SelectionTypes.ProjectOnly)
					{
						PostEvent += "OnlyProject";
					}
					else if (CurrentMode.Selection == SelectionTypes.Solution)
					{
						PostEvent += "Solution";
					}
				}
			}

			if(PostEvent.Length != 0)
				App().ExecuteCommand(PostEvent, "");
		}

		private string PostEvent;



		void AfterExecute(string Guid, int ID, object CustomIn, object CustomOut)
		{
			string commandName = "";

			try
			{
				commandName = App().Commands.Item(Guid, ID).Name;
			}
			catch (Exception)
			{
			}

			if (commandName != "")
			{
				if (bAddinLocked)
					return;

				if (commandName == PostEvent)
				{
					//TODO: fix this... do we need to use AfterExecute maybe?
					PostEvent = "";
					return;
				}
			}

			if (commandName != "")
			{
				PrintDebugLine("CommandEvents, AfterExecute");
				PrintDebugLine("\tCommand name: " + commandName);
				PrintDebugLine("\tCommand GUID/ID: " + Guid + ", " + ID.ToString());
			}
		}

		bool CompileCommandsUnavailable()
		{
			if ( App() == null )
				return true;

			Projects projects = App().Solution.Projects;

			foreach( Project p in projects )
			{
				VCProject project = p.Object as VCProject;
				
				if (project != null)
				{
					string config = ProjectUtility.ProjectActiveConfiguration(p);

					List<VCFile> ohs = ProjectUtility.GetOhFiles(p);
					ProjectUtility.FilterActiveFiles(ref ohs, config);

					if (ohs.Count != 0)
						return false;
				}
			}
			
			return true;
		}

		void BeforeExecute(string Guid, int ID, object CustomIn, object CustomOut, ref bool CancelDefault)
		{
			string commandName = "";

			try
			{
				commandName = App().Commands.Item(Guid, ID).Name;
			}
			catch (Exception)
			{
			}

			if (commandName != "")
			{
				if (bAddinLocked)
					return;

				if (commandName == PostEvent)
				{
					//TODO: fix this... do we need to use AfterExecute maybe?
					//PostEvent = "";
					return;
				}

				if (CompileCommandsUnavailable())
					return;

				//our handled events...

				//don't execute in debug mode (for now?)
				if (App().Mode == vsIDEMode.vsIDEModeDebug)
					return;

				//TODO: where is the interception option???

				//catch debug.start
				if (commandName == "Debug.Start")
				{
					HandleDebugStart(true);
					CancelDefault = true;
				}
				else if (commandName == "Debug.StartWithoutDebugging")
				{
					HandleNoDebugStart(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.BuildSolution")
				{
					HandleBuildSolution(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.BuildSelection")
				{
					HandleBuildProject(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.BuildOnlyProject")
				{
					HandleBuildProjectOnly(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.RebuildSolution")
				{
					HandleRebuildSolution(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.RebuildSelection")
				{
					HandleRebuildProject(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.RebuildOnlyProject")
				{
					HandleRebuildProjectOnly(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.CleanSolution")
				{
					HandleCleanSolution(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.CleanSelection")
				{
					HandleCleanProject(true);
					CancelDefault = true;
				}
				else if (commandName == "Build.CleanOnlyProject")
				{
					HandleCleanProjectOnly(true);
					CancelDefault = true;
				}
				
				//TODO: ?? batches
			}

			if (commandName != "")
			{
				PrintDebugLine("CommandEvents, BeforeExecute");
				PrintDebugLine("\tCommand name: " + commandName);
				PrintDebugLine("\tCommand GUID/ID: " + Guid + ", " + ID.ToString());
			}
		}

		//
		// Utilities
		//

		void SaveOpenFiles()
		{
			//TODO: get this setting from global options
			bool bSaveFiles = true;
			if (bSaveFiles)
			{
				foreach (Document d in App().Documents)
				{
					if (d.Name.EndsWith(".doh")
					|| d.Name.EndsWith(".oh"))
					{
						if (d.Saved == false)
							d.Save(d.FullName);
					}
				}
			}
		}
	};
}


