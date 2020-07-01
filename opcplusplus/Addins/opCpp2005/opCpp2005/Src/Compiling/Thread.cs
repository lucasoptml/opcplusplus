///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Thread.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Threading stuff.
///****************************************************************

using System;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;

namespace opGamesLLC.opCpp2005
{
	abstract class WorkerThread
	{
        /*=== utility ===*/

		public void Start()
		{
			if (thread == null)
			{
				thread = new Thread(new ThreadStart(Run));
				thread.Start();
			}
		}

		private void Run()
		{
			Running();
		}

		public virtual void Running()
		{
		}

		public virtual void Stop()
		{
			thread.Abort();
		}

        /*=== data ===*/

		private Thread thread;
	}

    // The opCpp thread.
	class opCppThread : WorkerThread
	{
        /*=== construction ===*/

		public opCppThread(List<opCpp2005.CommandSetting> commands)
		{
			Commands = new List<opCpp2005.CommandSetting>(commands);
		}

        /*=== utility ===*/

		public override void Running()
		{
			bool bErrored = false;
			
			foreach(opCpp2005.CommandSetting command in Commands)
			{
				try
				{
					Options options = OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options;

                    // Print out the project that's currently compiling.
                    if ( !command.Arguments.Contains("-clean") )
                    {
                        string configuration = ProjectUtility.GetActiveConfiguration(command.Project);
                        string platform      = ProjectUtility.GetActivePlatform(command.Project);
                        string message       = "------ opC++ Build started: Project: " + command.Project.Name + ", Configuration: " + configuration + " " + platform + " ------";

                        OnReadLine( message );
                    }

					if (options.OutputCommandline.Value)//TODO: hook into global options
					{
						string argstring = command.Arguments.Replace("-beta", "");
						
						OnReadLine("---------------------------------------------");
						OnReadLine("Working Directory = " + command.WorkingDir);
						OnReadLine("Path = " + command.ExePath);
						OnReadLine("Arguments = " + argstring);
						OnReadLine("::::::::::::::::::::::::::::::::::::::::::::;");
					}

					//create a new process
					process = new System.Diagnostics.Process();

					process.StartInfo.WorkingDirectory = command.WorkingDir;

					//this path is from the registry
					//NOTE: this should be integrated with the installer
					process.StartInfo.FileName = command.ExePath;
					process.StartInfo.Arguments = command.Arguments;
					process.StartInfo.RedirectStandardOutput = true;
					process.StartInfo.UseShellExecute = false;
					process.StartInfo.CreateNoWindow = true;
					process.StartInfo.ErrorDialog = false;
					//process.EnableRaisingEvents = true;

					//process.
					process.OutputDataReceived += new DataReceivedEventHandler(a_OutputDataReceived);

					process.Start();

					//begin async output reading
					process.BeginOutputReadLine();

// 					while(!process.HasExited)
// 					{
// 						string value;
// 						while( (value = process.StandardOutput.ReadLine()) != null )
// 							OnReadLine(value);
// 					}

					//while (!process.HasExited) ;

					//wait only 10 seconds (temporary?)
					process.WaitForExit(1000 * 10);

					//the second call makes sure the async output is all finished before proceeding.
					process.WaitForExit();

					if (process.HasExited)
					{
						//success
					}
					else
					{
						bErrored = true;

						//check status...this should be temporary!
						OnReadLine("Error: opC++ may not have finished in 10 seconds.");
						break;
					}

					if (process.ExitCode != 0)
					{
						bErrored = true;
						break;
					}
				}
				catch (Exception e)
				{
					OnReadLine("exception: " + e.Message + "\nStack Trace: " + e.StackTrace);
					bErrored = true;
					break;
				}
			}

			OnEnd(bErrored);
		}

		public override void Stop()
		{
			if(!process.HasExited)
				process.Kill();
		}

		void a_OutputDataReceived(object sender, DataReceivedEventArgs e)
		{
			OnReadLine(e.Data);
		}

		public delegate void EndThread(bool bErrored);
		public delegate void ReadLine(string text);

		//called when the thread is done
		public EndThread OnEnd;

		private List<opCpp2005.CommandSetting> Commands;

		//called when we read a new line of text
		public ReadLine OnReadLine;

		private Process process;
	};

} // end namespace vs2005opCpp
