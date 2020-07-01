///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: AboutForm.cs
/// Date: 09/23/2007
///
/// Description:
///
/// About dialog class.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Resources;
using System.IO;

namespace opGamesLLC.opCpp2005
{
    public partial class AboutForm : Form
    {
        /*=== construction ===*/

        public AboutForm()
        {
            InitializeComponent();

            // Set the compiler version (we must actually run opcpp with
            // -version and grab the standard output).
            try
            {
                Process process = new Process();

				//NOTE: not using the options file location.
				process.StartInfo.FileName = Path.GetFullPath( Paths.GetFullAppPath() + "..\\Release\\opCpp.exe" );
                process.StartInfo.Arguments = " -version";

                if (opLicenseUtility.FullLicenseFileName != "")
                    process.StartInfo.Arguments += " -license \"" + opLicenseUtility.FullLicenseFileName + "\"";

                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.ErrorDialog = false;

                process.Start();
                process.WaitForExit(1000 * 10);
                process.WaitForExit();

                string output = process.StandardOutput.ReadToEnd().Trim();

                this.txtCompilerVersion.Text = output;
            }
            catch (Exception)
            {
                this.txtCompilerVersion.Text = "Unable to query compiler version.";
            }

            // Set addin version string.
            this.txtAddinVersion.Text = opVersion.GetVersionString();
        }

        /*=== data ===*/

        public string License
        {
            set { txtLicense.Text = value; }
        }

        /*=== events ===*/

        // Open our site in a new browser.
        private void lnkWebsite_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.opcpp.com");
        }
    }
}