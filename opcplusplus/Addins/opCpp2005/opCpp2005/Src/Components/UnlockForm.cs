///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: UnlockForm.cs
/// Date: 09/28/2007
///
/// Description:
///
/// Form to unlock the opCpp addin.
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
    public partial class UnlockForm : Form
    {
        /*=== construction ===*/

        public UnlockForm()
        {
            InitializeComponent();

            // Set the focus to the name text box.
            txtName.Focus();

            // Dim the unlock button.
            btnUnlock.Enabled = false;

            // Setup event handlers for the text boxes.
            txtName.TextChanged      += new EventHandler(this.txtName_TextChanged);
            txtLicenseId.TextChanged += new EventHandler(this.txtLicenseId_TextChanged);
            txtSerial.TextChanged    += new EventHandler(this.txtSerial_TextChanged);
        }

        /*=== events ===*/

        // Fired when text is changed in the 'Name' text box.
        private void txtName_TextChanged(object sender, EventArgs e)
        {
            if (opLicenseUtility.IsValidLicenseCombination(txtName.Text, txtLicenseId.Text, txtSerial.Text))
                btnUnlock.Enabled = true;
            else
                btnUnlock.Enabled = false;
        }

        // Fired when text is changed in the 'License Id' text box.
        private void txtLicenseId_TextChanged(object sender, EventArgs e)
        {
            if (opLicenseUtility.IsValidLicenseCombination(txtName.Text, txtLicenseId.Text, txtSerial.Text))
                btnUnlock.Enabled = true;
            else
                btnUnlock.Enabled = false;
        }

        // Fired when text is changed in the 'Serial' text box.
        private void txtSerial_TextChanged(object sender, EventArgs e)
        {
            if (opLicenseUtility.IsValidLicenseCombination(txtName.Text, txtLicenseId.Text, txtSerial.Text))
                btnUnlock.Enabled = true;
            else
                btnUnlock.Enabled = false;
        }

        // Fired when the unlock button is clicked.
        private void btnUnlock_Click(object sender, EventArgs e)
        {
            if (!opLicenseUtility.GenerateLicenseFile(txtName.Text, txtLicenseId.Text, txtSerial.Text))
                MessageBox.Show("Unable to generate the license file '" + opLicenseUtility.FullLicenseFileName + "'.");
            else
            {
                this.Close();

                opCpp2005.GetInstance().UnlockAddin();

                MessageBox.Show("The opC++ addin has been successfully unlocked.");
            }
        }
    }
}