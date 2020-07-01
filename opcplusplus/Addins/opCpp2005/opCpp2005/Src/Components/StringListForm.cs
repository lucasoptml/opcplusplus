///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: FindFiles.cs
/// Date: 09/24/2007
///
/// Description:
///
/// Contains a class for finding a list of strings.
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
    ///==========================================
    /// FindFilesForm
    ///==========================================
    
    public partial class StringListForm : Form
    {
        /*=== construction ===*/

        public StringListForm()
        {
            InitializeComponent();
            Initialize();
        }

        /*=== utility ===*/

        // Initialization code.
        private void Initialize()
        {
            /*=== Initialize the form. ===*/

            this.Resize += new System.EventHandler(this.form_Resize);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.form_MouseDown);

            /*=== Initialize the list box. ===*/

            lstStrings.Focus();

            this.lstStrings.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lstStrings_MouseDown);
            this.lstStrings.DoubleClick += new System.EventHandler(this.lstStrings_DoubleClick);
            this.lstStrings.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lstStrings_KeyDown);
            
            /*=== Initialize the edit box. ===*/

            editBox             = new TextBox();
            editBox.Location    = new Point(0, 0);
            editBox.Size        = new Size(0, 0);
            editBox.Text        = "";
            editBox.BackColor   = Color.Beige;
            editBox.BorderStyle = BorderStyle.FixedSingle; 
            editBox.KeyDown    += new System.Windows.Forms.KeyEventHandler(this.editBox_KeyDown);
            editBox.Hide();

            lstStrings.Controls.AddRange(new System.Windows.Forms.Control[] { editBox });
        }

        // Adds some strings to the dialog.
        public void AddFiles(List<string> _strings)
        {
            foreach (string s in _strings)
            {
                strings.Add(s);
                lstStrings.Items.Add(s);
            }

            if (strings.Count > 0)
                lstStrings.SelectedIndex = 0;
            else
                btnRemove.Enabled = false;
        }

        // Initializes and shows the edit box.
        private void ShowEditBox()
        {
            int index = lstStrings.SelectedIndex;

            if (!editBox.Visible && index > -1)
            {
                Rectangle rect     = lstStrings.GetItemRectangle(index);
                Point     loc      = lstStrings.Location;
                string    itemText = (string) lstStrings.SelectedItem;
                editBox.Location   = new Point(rect.X, rect.Y);
                editBox.Size       = new Size(rect.Width, rect.Height);
                editBox.Text       = itemText;
                editBox.Show();

                editBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.editBox_KeyDown);

                editBox.Focus();
                editBox.SelectAll();

                LastIndex = index;
            }
        }

        // Hides the edit box and resets the data.
        private void HideEditBox()
        {
            if (editBox.Visible)
            {
                string oldText = (string) lstStrings.Items[LastIndex];
                string newText = editBox.Text;

                strings.Remove(oldText);
                strings.Add(newText);
                
                editBox.Hide();

                lstStrings.Items[LastIndex] = newText;
            }
        }

        /*=== data ===*/

        // A reference to the StringListEditor object.

        StringListEditor editor = null;

        public StringListEditor Editor
        {
            get { return editor; }
            set { editor = value; }
        }

        // The list of strings.

        private List<string> strings = new List<string>();
        
        public List<string> Strings
        {
        	get { return strings; }
        }

        // The editbox for entering path names.

        private TextBox editBox;

        // The last index being edited.

        private int LastIndex;

        /*=== events ===*/

        // Event fired when add button is clicked.
        private void btnAdd_Click(object sender, EventArgs e)
        {
            HideEditBox();

            List<string> _strings = Editor.GetNewStrings();
            int          size     = _strings.Count;
            
            if (size > 0)
            {
                string f = _strings[0];

                strings.Add(f);
                lstStrings.SelectedIndex = lstStrings.Items.Add(f);

                for (int i = 1; i < size; i++)
                {
                    f = _strings[i];

                    strings.Add(f);
                    lstStrings.Items.Add(f);
                }

                btnRemove.Enabled = true;
            }
        }

        // Event fired when remove button is clicked.
        private void btnRemove_Click(object sender, EventArgs e)
        {
            HideEditBox();

            int index = lstStrings.SelectedIndex;

            if (index > -1)
            {
                string s = (string) lstStrings.SelectedItem;

                strings.Remove(s);
                lstStrings.Items.RemoveAt(index);

                int size = lstStrings.Items.Count;

                if (size > 0)
                {
                    if (index < size)
                        lstStrings.SelectedIndex = index;
                    else
                        lstStrings.SelectedIndex = --index;
                }
                else
                    btnRemove.Enabled = false;
            }
        }

        // Event fired when the user double clicks the list box.
        private void lstStrings_DoubleClick(object sender, EventArgs e)
        {
            ShowEditBox();
        }

        // Event fired when the user presses a key in the list box.
        private void lstStrings_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.F2 || e.KeyData == Keys.Enter)
                ShowEditBox();
        }

        // Event fired when the user clicks in the list box.
        private void lstStrings_MouseDown(object sender, MouseEventArgs e)
        {
            int index = lstStrings.SelectedIndex;

            if (editBox.Visible && index > -1)
            {
                // Only close the edit box if we clicked off of the edit box
                // elsewhere on the list box.
                Rectangle rect = editBox.Bounds;

                if (!rect.Contains(e.Location))
                    HideEditBox();
            }
        }

        // Event fired when the user hits a key in the edit box.
        private void editBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Enter)
                HideEditBox();
        }

        // Event fired when the form is resized.
        private void form_Resize(object sender, EventArgs e)
        {
            HideEditBox();
        }

        // Event fired when the mouse is clicked on the form.
        private void form_MouseDown(object sender, MouseEventArgs e)
        {
            HideEditBox();
        }
    };
}