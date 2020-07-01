///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Version.cs
/// Date: 12/12/2007
///
/// Description:
///
/// Class to manage addin version.
///****************************************************************

using System;
using System.IO;
using System.Windows.Forms;

namespace opGamesLLC.opCpp2005
{
    public class opVersion
    {
        /*=== data ===*/

        private static float version = 0.91f;

        public float Version
        {
            get { return version; }
        }

        private static string versionstring = "0.9.1";

        public string VersionString
        {
            get { return versionstring; }
        }

        /*=== utility ===*/

        // Returns version string.
        public static string GetVersionString()
        {
            string   s;
            DateTime date = File.GetLastWriteTime( System.Reflection.Assembly.GetExecutingAssembly().Location );

            s = "opC++ MSVS 2005 Addin v. " + versionstring + "\r\n";
            s += "Build Date: " + date.ToString("MMM") + " " + date.Day + " " + date.Year + 
                " @ " + StringUtility.RLeft( date.TimeOfDay.ToString(), "." ) + "\r\n";
            s += "Copyright 2008 opGames LLC";

            return s;
        }
    }
}
