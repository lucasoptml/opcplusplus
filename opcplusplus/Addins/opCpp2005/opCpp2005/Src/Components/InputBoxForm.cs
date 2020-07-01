///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: InputBoxForm.cs
/// Date: 09/25/2007
///
/// Description:
///
/// Class for getting input from the user (a string).
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
    public partial class InputBoxForm : Form
    {
        /*=== construction ===*/

        public InputBoxForm()
        {
            InitializeComponent();

            // Give the input box the focus.
            txtInput.Focus();
        }

        /*=== data ===*/

        // The text in the input text box.

        public string Input
        {
            get
            {
                return txtInput.Text;
            }
            set
            {
                txtInput.Text = value;
            }
        }
    }
}