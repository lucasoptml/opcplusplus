///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
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

namespace vs2005opCpp
{
    // Generic thread object.
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

		public void Stop()
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

		public opCppThread(string inexe, string inworkingdir, string inarguments)
		{
			exe        = inexe;
			workingdir = inworkingdir;
			arguments  = inarguments;
		}
		
        /*=== utility ===*/

		public override void Running()
		{
			bool bErrored = false;
			
			try
			{
#if DEBUG
   				OnReadLine("debug: working dir = " + workingdir);
                OnReadLine("debug: exepath = " + exe);
				OnReadLine("debug: arguments = " + arguments);
#endif
				//create a new process
				process = new System.Diagnostics.Process();

				process.StartInfo.WorkingDirectory = workingdir;

				//this path is from the registry
				//NOTE: this should be integrated with the installer
				process.StartInfo.FileName               = exe;
				process.StartInfo.Arguments              = arguments;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.UseShellExecute        = false;
				process.StartInfo.CreateNoWindow         = true;
				process.StartInfo.ErrorDialog            = false;
				process.EnableRaisingEvents				 = true;

				//process.
				process.OutputDataReceived += new DataReceivedEventHandler(a_OutputDataReceived);

				process.Start();

				//begin async output reading
				process.BeginOutputReadLine();

                //wait only 10 seconds (temporary?)
				process.WaitForExit(1000*10);

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
                    OnReadLine("error: opcpp may not have finished in 10 seconds.");
                }

				if(process.ExitCode != 0)
					bErrored = true;
			}
			catch(Exception)
			{
				bErrored = true;
			}
			
			OnEnd(bErrored);
		}

		void a_OutputDataReceived(object sender, DataReceivedEventArgs e)
		{
			OnReadLine(e.Data);
		}

		public delegate void EndThread(bool bErrored);
		public delegate void ReadLine(string text);

		//called when the thread is done
		public EndThread OnEnd;

		//called when we read a new line of text
		public ReadLine OnReadLine;

		private string exe;
		private string workingdir;
		private string arguments;

		private Process process;
	};

} // end namespace vs2005opCpp
