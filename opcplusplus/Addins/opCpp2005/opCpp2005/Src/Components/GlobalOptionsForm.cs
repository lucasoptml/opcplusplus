///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: GlobalOptionsForm.cs
/// Date: 09/23/2007
///
/// Description:
///
/// The form for changing global options.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace opGamesLLC.opCpp2005
{
    // This is the global options dialog class.
    public partial class GlobalOptionsForm : Form
    {
        /*=== construction ===*/

        public GlobalOptionsForm()
        {
            InitializeComponent();      

            // Initialize the property grid.
            this.propertyGrid.RefreshGrid();
        }

        /*=== events ===*/

        // When Ok button is clicked, save the global options.
        private void btnOk_Click(object sender, EventArgs e)
        {
            this.propertyGrid.Save();
        }

        // Launch the about dialog.
        private void btnAbout_Click(object sender, EventArgs e)
        {
            AboutForm    form    = new AboutForm();
            string       license = opLicenseUtility.FullLicenseFileName;
            string       message = "";
            StreamReader r       = null;

            try
            {
                r = new StreamReader(license);

                message = r.ReadToEnd();
                r.Close();
            }
            catch (Exception)
            {
                message = "The opC++ license file '" + license + "' is either " +
                          "missing or corrupted.";
            }

            form.License = message;
            form.ShowDialog();
        }

        // When defaults button is clicked, reset globals to their defaults.
        private void btnDefaults_Click(object sender, EventArgs e)
        {
            this.propertyGrid.Defaults();
            this.propertyGrid.RefreshGrid();
        }

        // Fired when the user clicks the unlock button.
        private void btnUnlock_Click(object sender, EventArgs e)
        {
            UnlockForm form = new UnlockForm();

            form.ShowDialog();
        }
    }
}