///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
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

namespace vs2005opCpp
{
    public partial class Connect
    {
        /*=== data ===*/

        private System.IO.StreamWriter debugstream;

        /*=== utility ===*/

        // This method logs strings.
        public void Log(string s)
        {
            if (debugstream != null)
                debugstream.Write(s + "\n");
        }

        // This method logs text to the opCpp pane.
        void opCppLog(string text)
        {
            GetOutputPane().OutputString(text + "\n");
            Application.DoEvents();
        }

        // Prints a line to the console.
        public void PrintDebugLine(string s)
        {
            System.Diagnostics.Debugger.Log(0, "", s + '\n');
        }
    }
}