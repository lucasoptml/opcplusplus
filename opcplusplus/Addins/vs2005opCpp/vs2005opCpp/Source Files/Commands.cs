///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
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

namespace vs2005opCpp
{
	public partial class Connect
	{
        ///==========================================
        /// Events
        ///==========================================

		//NOTE: this does not catch debugger startup,
		//		for that I need to catch the event via BeforeExecute
		public void OnBuildBegin(vsBuildScope Scope, vsBuildAction Action)
		{
			//NOTE: we may want to use these, just for copying the error buffer
			//		to the other output pane (nothing else...)

//			GetBuildOutputPane().OutputString("OnBuildBegin...\n");
// 			GetDebugOutputPane().Clear();
// 
// 			string ActionScope = Scope.ToString();
// 			string ActionType = Action.ToString();
// 
// 			GetDebugOutputPane().OutputString("Build Event Begin: Scope=" + ActionScope + " Action=" + ActionType + "\n");
// 	        
// 			if(Scope == vsBuildScope.vsBuildScopeSolution)
// 			{
// 				if (Action == vsBuildAction.vsBuildActionBuild
// 				||	Action == vsBuildAction.vsBuildActionDeploy
// 				||	Action == vsBuildAction.vsBuildActionRebuildAll)
// 					CatchBuildSolution();
// 			}
		}

		public void OnBuildDone(vsBuildScope Scope, vsBuildAction Action)
		{
//			GetBuildOutputPane().OutputString("OnBuildDone...\n");

// 			string ActionScope = Scope.ToString();
// 			string ActionType = Action.ToString();
// 
// 			GetDebugOutputPane().OutputString("Build Event Done: Scope=" + ActionScope + " Action=" + ActionType + "\n");
		}

		public void OnBuildProjConfigBegin(string Project, string ProjectConfig, string Platform, string SolutionConfig)
		{
//			GetBuildOutputPane().OutputString("OnBuildProjConfigBegin...\n");
			
// 			GetDebugOutputPane().OutputString("Build Event Begin: (project configuration)" 
// 			+ " Project=" + Project
// 			+ " ProjectConfig=" + ProjectConfig
// 			+ " Platform=" + Platform
// 			+ " SolutionConfig=" + SolutionConfig
// 			+ "\n"
// 			);
		}

		public void OnBuildProjConfigDone(string Project, string ProjectConfig, string Platform, string SolutionConfig, bool Success)
		{
//			GetBuildOutputPane().OutputString("OnBuildProjConfigDone...\n");

// 			GetDebugOutputPane().OutputString("Build Event Done: (project configuration)"
// 			+ " Project=" + Project
// 			+ " ProjectConfig=" + ProjectConfig
// 			+ " Platform=" + Platform
// 			+ " SolutionConfig=" + SolutionConfig
// 			+ " Success=" + Success.ToString()
// 			+ "\n"
// 			);
		}

        ///==========================================
        /// Command Utility
        ///==========================================

        // This method is called before a command is executed.
		void BeforeExecute(string Guid, int ID, object CustomIn, object CustomOut, ref bool CancelDefault)
		{
			string commandName = "";

			try
			{
				commandName = App().Commands.Item(Guid, ID).Name;
			}
			catch (System.Exception)
			{
			}
			
			if(commandName != "")
			{
				//don't execute in debug mode (for now?)
				if (App().Mode == vsIDEMode.vsIDEModeDebug)
					return;

				//catch debug.start
				if (commandName == "Debug.Start")
					CancelDefault = !CatchDebugStart();
				else if (commandName == "Debug.StartWithoutDebugging")
					CancelDefault = !CatchNoDebugStart();
				else if (commandName == "Build.BuildSolution")
					CancelDefault = !CatchBuildSolution();
				else if (commandName == "Build.BuildSelection")
					CancelDefault = !CatchBuildSelection();
				//TODO: need to catch all Clean events
				//TODO: need to catch all rebuild events
				//?? batches
			}

			if (commandName != "")
			{
				Log("CommandEvents, BeforeExecute");
				Log("\tCommand name: " + commandName);
				Log("\tCommand GUID/ID: " + Guid + ", " + ID.ToString());
			}

		}

        // This method is called after an event is executed.
		void AfterExecute(string Guid, int ID, object CustomIn, object CustomOut)
		{
			string commandName = "";

			try
			{
				commandName = App().Commands.Item(Guid, ID).Name;
			}
			catch (System.Exception)
			{
			}
			
			if (commandName != "")
			{
				Log("CommandEvents, AfterExecute");
				Log("\tCommand name: " + commandName);
				Log("\tCommand GUID/ID: " + Guid + ", " + ID.ToString());
			}
        }

        // This method allows you to lookup a command by name.
		Command FindCommand(string name)
		{
			string fullcommandname = _addInInstance.ProgID + "." + name;
			Command newCommand = App().Commands.Item(fullcommandname, 0);
			return newCommand;
		}
        
        // This method allows you to create a new command.
		Command CreateCommand(string name, string description, int iconindex)
		{
			Object[] objs = null;

			Command newCommand = App().Commands.AddNamedCommand(
				_addInInstance,
				name,
				name,
				description,
				false,
				iconindex,
				ref objs,
				(int) vsCommandStatus.vsCommandStatusSupported | (int) vsCommandStatus.vsCommandStatusEnabled
				);

			return newCommand;
		}

        // Utility method.
		bool CommandCompare(string command, string comparename)
		{
			return command == _addInInstance.ProgID + "." + comparename;
		}

        // This method returns true if the command is supported.
		bool SupportsCommand(string command)
		{
			if (CommandCompare(command, "buildsolution"))
			{
				return true;
			}
			else if (CommandCompare(command, "buildproject"))
			{
				return true;
			}
			else if (CommandCompare(command, "cleansolution"))
			{
				return true;
			}
			return false;
		}

        // This method runs the given command.
		bool RunCommand(string command)
		{
			if (CommandCompare(command, "buildsolution"))
			{
				RunBuildSolution();
				return true;
			}
			if (CommandCompare(command, "buildproject"))
			{
				RunBuildSelection();
				return true;
			}
			if (CommandCompare(command, "cleansolution"))
			{
				RunCleanSolution();
				return true;
			}

			return false;
		}

        ///==========================================
        /// Visual Studio Commands
        ///==========================================

        // This method handles the 'debug start' command.
		bool CatchDebugStart()
		{
			//Ok to compile c++ now
			if (bCompileCpp)
			{
				bCompileCpp = false;

				GetBuildOutputPane().OutputString("Built opC++ Successfully...\n");

				return true;
			}

			//don't compile c++ if we have opcpp running
			if (runthread != null)
				return false;

			mode = CompileMode.DebugStart;

			List<Project> projects = FindProjects();
			returncode totalresult = BuildProjects(projects);

			//don't compile c++, we're running opcpp
			if (totalresult == returncode.None)
				return true;
			return false;
		}

        // This handles the 'start without debugging' command.
		bool CatchNoDebugStart()
		{
			//Ok to compile c++ now
			if (bCompileCpp)
			{
				bCompileCpp = false;

				GetBuildOutputPane().OutputString("Built opC++ Successfully...\n");

				return true;
			}

			//don't compile c++ if we have opcpp running
			if (runthread != null)
				return false;

			mode = CompileMode.NoDebugStart;

			List<Project> projects = FindProjects();
			returncode totalresult = BuildProjects(projects);

			//don't compile c++, we're running opcpp
			if (totalresult == returncode.None)
				return true;
			return false;
		}		

        // This method handles the 'build solution' command.
		bool CatchBuildSolution()
		{
			//TODO:
			//this should really check if any files haven't been saved yet
			//if thats so, we should ignore bCompileCpp
			//also we should Abort runthread, and restart

			//NOTE: I dont think we can do this... because dependency checking happens too late
			//		but maybe I'm wrong there.
			//		I suppose maybe the best way is to add a custom command,
			//		and one that lists all the dependencies correctly, then it
			//		*might* work (could test using manually input stuff)
// 			If your custom build task is a wrapper around command line util 
// 			(compiler or similar executable) and you want to show output 
// 			from it in Output Window panel you should inherit your custom 
// 			build task from ToolTask base class. ToolTask class is able to 
// 				write output from command line utility line by line.

 			//Ok to compile c++ now
			if (bCompileCpp)
			{
				bCompileCpp = false;

				GetBuildOutputPane().OutputString("Built opC++ Successfully...\n");
				
				return true;
			}
			
			//don't compile c++ if we have opcpp running
			if (runthread != null)
				return false;

			mode = CompileMode.BuildSolution;

            List<Project> projects = FindProjects();
			returncode totalresult = BuildProjects(projects);

			//don't compile c++, we're running opcpp
			if (totalresult == returncode.None)
				return true;
			return false;
		}

        // This method handles the 'build selection' command.
		bool CatchBuildSelection()
		{
			//TODO:
			//this should really check if any files haven't been saved yet
			//if thats so, we should ignore bCompileCpp
			//also we should Abort runthread, and restart

			//Ok to compile c++ now
			if (bCompileCpp)
			{
				bCompileCpp = false;
				GetBuildOutputPane().OutputString("Built opC++ Successfully...\n");
				return true;
			}
			
			//don't compile c++ if we have opcpp running
			if (runthread != null)
				return false;

			mode = CompileMode.BuildSelection;

			List<Project> projects = FindCurrentProjects();
			returncode totalresult = BuildProjects(projects);

			//don't compile c++, we're running opcpp
			if (totalresult == returncode.None)
				return true;
			return false;
		}

        // This runs the 'build solution' command.
		void RunBuildSolution()
		{
			//run the actual solution build
			//(this means run opcpp on all oh files in all projects)
			App().ExecuteCommand("Build.BuildSolution", "");
		}

        // This runs the 'build selection' command.
		void RunBuildSelection()
		{
			//run the selected build
			App().ExecuteCommand("Build.BuildSelection", "");
		}
		
        // This runs the 'degug start' command.
		void RunDebugStart()
		{
			//run the actual solution build
			//(this means run opcpp on all oh files in all projects)
			
			//have to invoke it this way, dunno why?
			App().Solution.SolutionBuild.Debug();
		}
		
        // This runs the 'start without debugging' command.
		void RunNoDebugStart()
		{
			App().ExecuteCommand("Debug.StartWithoutDebugging", "");
		}

        // Enumeration for compilation mode.
		enum CompileMode
		{
			DebugStart,
			BuildSolution,
			BuildSelection,
			NoDebugStart,
		};

        /*=== data ===*/

		private CompileMode mode;
        private bool bCompileCpp = false;
        private opCppThread runthread;

        // This method is called when opCpp is finished with it's thread.
		void opCppFinished(bool bErrored)
		{
			runthread = null;
			
			if(!bErrored)
			{
				bCompileCpp = true;

				if (mode == CompileMode.BuildSolution)
					RunBuildSolution();
				else if (mode == CompileMode.DebugStart)
					RunDebugStart();
				else if (mode == CompileMode.NoDebugStart)
					RunNoDebugStart();
				else if (mode == CompileMode.BuildSelection)
					RunBuildSelection();

			}
		}


        // Cleans the solution.
		void RunCleanSolution()
		{
            // TODO - implement this!
			SetStatusText("Ran opCpp Solution Clean! :) ");
		}

		// Build the list of .oh files for command line arguments.
		string BuildOhString( ref List<string> found )
		{
			string filestring = "";

			foreach(string foundfile in found)
			{
				filestring += "\"" + foundfile + "\",";
			}

			filestring = filestring.TrimEnd(',');

			return filestring;
		}
	};

}


