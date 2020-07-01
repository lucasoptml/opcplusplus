///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: CustomEditors.cs
/// Date: 09/19/2007
///
/// Description:
///
/// Some C# editor subclasses.
///****************************************************************

using System;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Drawing.Design;
using System.ComponentModel;
using System.Collections.Generic;
using System.Drawing;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// FindExecutableEditor
    ///==========================================
    
    // This find file editor is used to find the location of opCpp.exe only.
    public class FindExecutableEditor : UITypeEditor
    {
        /*=== overrides ===*/

        // Tells the designer what kind of editing style this editor uses.
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        // This method fires when the user wishes to edit a value.
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            StringOption   option = (StringOption) value;
            OpenFileDialog form   = new OpenFileDialog();

            form.Title  = "Find opCpp.exe";
            form.Filter = "opC++ Executable (opCpp.exe) | opCpp.exe";

            if (form.ShowDialog() == DialogResult.OK)
                option.Value = form.FileName;

            return option;
        }
    };

    ///==========================================
    /// FindLicenseEditor
    ///==========================================
    
    // This find file editor is used to find the location of opCpp.oplicense only.
    public class FindLicenseEditor : UITypeEditor
    {
        /*=== overrides ===*/

        // Tells the designer what kind of editing style this editor uses.
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        // This method fires when the user wishes to edit a value.
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            StringOption   option = (StringOption) value;
            OpenFileDialog form   = new OpenFileDialog();

            form.Title  = "Find opC++.oplicense";
            form.Filter = "opC++ License (opC++.oplicense) | opC++.oplicense";

            if (form.ShowDialog() == DialogResult.OK)
                option.Value = form.FileName;

            return option;
        }
    };

    ///==========================================
    /// StringListEditor
    ///==========================================
   
    // This editor will pop up a dialog for populating strings.
    public abstract class StringListEditor : UITypeEditor
    {
        /*=== virtuals ===*/

        // Override this to initialize the dialog.
        protected virtual void InitializeDialog(StringListForm form)
        {
            
        }

        // Override this to initialize the strings added (via some dialog).
        public virtual List<string> GetNewStrings()
        {
            List<string> strings = new List<string>();

            return strings;
        }

        /*=== overrides ===*/

        // Tells the designer what kind of editing style this editor uses.
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        // This method fires when the user wishes to edit a value.
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            StringListOption option = (StringListOption) value;
            StringListForm   form   = new StringListForm();

            InitializeDialog(form);
            
            form.Editor = this;
            form.AddFiles(option.StringList);

            if (form.ShowDialog() == DialogResult.OK)
            {
                option.StringList = form.Strings;

                return option;
            }

            return option;
        }
    };

    ///==========================================
    /// OhFilesEditor
    ///==========================================
    
    public class OhFilesEditor : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Additional Header Files";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string>   strings = new List<string>();
            OpenFileDialog form    = new OpenFileDialog();

            form.Title       = "Find Header Files";
            form.Filter      = "Header Files (*.oh)|*.oh|All Files (*.*)|*.*";
            form.Multiselect = true;

            if (form.ShowDialog() == DialogResult.OK)
            {
                string[] files = form.FileNames;

                foreach (string s in files)
                    strings.Add(s);
            }
            
            return strings;
        }
    };

    ///==========================================
    /// DohFilesEditor
    ///==========================================
    
    public class DohFilesEditor : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Additional Dialect Files";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string> strings = new List<string>();
            OpenFileDialog form  = new OpenFileDialog();

            form.Title       = "Find Dialect Files";
            form.Filter      = "Dialect Files (*.doh)|*.doh|All Files (*.*)|*.*";
            form.Multiselect = true;

            if (form.ShowDialog() == DialogResult.OK)
            {
                string[] files = form.FileNames;

                foreach (string s in files)
                    strings.Add(s);
            }

            return strings;
        }
    };

    ///==========================================
    /// DependencyFilesEditor
    ///==========================================
    
    public class DependencyFilesEditor : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Additional Dependencies";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string>   strings = new List<string>();
            OpenFileDialog form    = new OpenFileDialog();

            form.Title  = "Find Dependencies";
            form.Filter = "All Files (*.*)|*.*";
            form.Multiselect = true;

            if (form.ShowDialog() == DialogResult.OK)
            {
                string[] files = form.FileNames;

                foreach (string s in files)
                    strings.Add(s);
            }

            return strings;
        }
    };

    ///==========================================
    /// IncludeDirectoriesEditor
    ///==========================================
    
    public class IncludeDirectoriesEditor : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Additional Include Directories";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string>        strings = new List<string>();
            FolderBrowserDialog form    = new FolderBrowserDialog();

            if (form.ShowDialog() == DialogResult.OK)
                strings.Add(form.SelectedPath);

            return strings;
        }
    };

    ///==========================================
    /// OhCompileDirectories
    ///==========================================

    public class OhCompileDirectories : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Header Compile Directories";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string>        strings = new List<string>();
            FolderBrowserDialog form    = new FolderBrowserDialog();

            if (form.ShowDialog() == DialogResult.OK)
                strings.Add(form.SelectedPath);

            return strings;
        }
    };

    ///==========================================
    /// FeatureDirectoriesEditor
    ///==========================================

    public class FeatureDirectoriesEditor : StringListEditor
    {
        /*=== overrides ===*/

        // Override this to initialize the dialog.
        protected override void InitializeDialog(StringListForm form)
        {
            form.Text = "Additional Feature Directories";
        }

        // Override this to initialize the strings added (via some dialog).
        public override List<string> GetNewStrings()
        {
            List<string>        strings = new List<string>();
            FolderBrowserDialog form    = new FolderBrowserDialog();

            if (form.ShowDialog() == DialogResult.OK)
                strings.Add(form.SelectedPath);

            return strings;
        }
    };
}