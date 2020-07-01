///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: AddinLockedForm.cs
/// Date: 09/28/2007
///
/// Description:
///
/// Form shown when the addin goes into lockdown.
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
    public partial class AddinLockedForm : Form
    {
        /*=== construction ===*/

        public AddinLockedForm()
        {
            InitializeComponent();
        }

        /*=== events ===*/

        // Fired when the unlock button is clicked.
        private void btnUnlock_Click(object sender, EventArgs e)
        {
            UnlockForm form = new UnlockForm();

            if (form.ShowDialog() == DialogResult.OK)
                this.Close();
        }

        // Opens our website when the link is clicked.
        private void lnkWebsite_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.opcpp.com");
        }
    }
}