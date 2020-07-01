///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Log.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Code for logging information.
///****************************************************************

using System;
using System.Windows.Forms;
using System.IO;
using EnvDTE80; 

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
        /*=== data ===*/

        private StreamWriter debugstream = null;

        /*=== utility ===*/

		public void Log(string s)
		{
			DebugLog(s);
			PrintDebugLine(s);
		}

		public void LogException(Exception e)
		{
			Log("Exception: " + e.Message + "\n" + e.StackTrace);
		}

        // This method logs strings.
        public void DebugLog(string s)
        {
            try
			{
				if (debugstream == null)
				{		
                    string path = Paths.GetGlobalOptionsPath() + "opC++.oplog";
					debugstream = new StreamWriter(path);
				}

				debugstream.Write(s + "\n");
                debugstream.Flush();
			}
			catch(Exception)
			{
			}
        }

        // Prints a line to the console.
        public void PrintDebugLine(string s)
        {
            System.Diagnostics.Debugger.Log(0, "", s + '\n');
        }
    }
}