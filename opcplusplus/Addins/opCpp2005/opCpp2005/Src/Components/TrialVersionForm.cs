///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: TrialVersionForm.cs
/// Date: 09/28/2007
///
/// Description:
///
/// This form tells the user they're in trial mode.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace opGamesLLC.opCpp2005
{
    public partial class TrialVersionForm : Form
    {
        /*=== construction ===*/

        public TrialVersionForm()
        {
            InitializeComponent();
        }

        /*=== data ===*/

        public string RunsRemaining
        {
            set { txtRunsRemaining.Text = value; }
        }

        /*=== events ===*/

        // Fired when the user clicks on our website link.
        private void lnkWebsite_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.opcpp.com");
        }

        // Fired when the unlock button is clicked.
        private void btnUnlock_Click(object sender, EventArgs e)
        {
            UnlockForm form = new UnlockForm();

            if (form.ShowDialog() == DialogResult.OK)
                this.Close();
        }

    }
}