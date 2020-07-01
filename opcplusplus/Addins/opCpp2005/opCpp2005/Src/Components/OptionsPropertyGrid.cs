///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: OptionsPropertyGrid.cs
/// Date: 09/23/2007
///
/// Description:
///
/// Contains the special property grid that reflects the global/project options.
///****************************************************************

using System;
using System.Reflection;
using System.Windows.Forms;
using System.ComponentModel;
using Flobbster.Windows.Forms;
using System.Collections.Generic;
using EnvDTE;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// OptionsPropertyGrid
    ///==========================================
    
    // Abstract class representing an options property grid.
    // You should inherit from this to create property grid classes.
    public abstract class OptionsPropertyGrid : PropertyGrid
    {
        /*=== construction ===*/

        public OptionsPropertyGrid() : base()
        {
            bag = new PropertyBag();

            // Setup the event handlers.
            bag.GetValue += new PropertySpecEventHandler(this.GetValue);
            bag.SetValue += new PropertySpecEventHandler(this.SetValue);
        }

        /*=== virtuals ===*/

        // Override this method to save options.
        public virtual void Save()
        {

        }

        // Override this method to set options to their defaults.
        public virtual void Defaults()
        {

        }

        // Called when a value is needed.
        protected virtual void GetValue(object sender, PropertySpecEventArgs e)
        {

        }

        // Called when a value is changed.
        protected virtual void SetValue(object sender, PropertySpecEventArgs e)
        {

        }

        /*=== data ===*/

        // The property bag.

        protected PropertyBag bag;
    };

    ///==========================================
    /// GlobalOptionsPropertyGrid
    ///==========================================
    
    // This is the property grid control for global options.
    public class GlobalOptionsPropertyGrid : OptionsPropertyGrid
    {
        /*=== data ===*/

        Options aOptions = null;
        Options gOptions = null;

        /*=== construction ===*/

        public GlobalOptionsPropertyGrid() : base()
        {
            aOptions = new Options();
            gOptions = new Options(OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options);
        }

        /*=== utility ===*/

        // This method refreshes the property grid.
        public void RefreshGrid()
        {
            bag.Properties.Clear();

            /*=== Setup the property bag. ===*/

            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>  attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionInfo       info  = new OptionInfo(attrs);

                // Ignore options that shouldn't show up in the global options dialog.
                if (!info.IsGlobal)
                    continue;

                OptionBase   aOption = (OptionBase) oFields[i].GetValue(aOptions);
                OptionBase   gOption = (OptionBase) oFields[i].GetValue(gOptions);
                PropertySpec ps      = new PropertySpec(info.Name, gOption.GetOptionType());

                ps.Category          = info.Category;
                ps.Description       = info.Description;
                ps.EditorTypeName    = info.EditorType;
                ps.ConverterTypeName = info.GlobalTypeConverter;
                ps.DefaultValue      = aOption; 

                bag.Properties.Add(ps);
            }

            // Set the selected object to be the bag.
            this.SelectedObject = bag;
        }

        /*=== overrides ===*/

        // Override this method to set options to their defaults.
        public override void Defaults()
        {
            gOptions = new Options();
        }

        // Override this method to save options.
        public override void Save()
        {
            OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options = new Options(gOptions);
            OptionsManager.GetGlobalOptions().Save();
        }

        // Called when a value is needed.
        protected override void GetValue(object sender, PropertySpecEventArgs e)
        {
            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>     attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionNameAttribute oname = (OptionNameAttribute) AttributeUtility.GetAttribute<OptionNameAttribute>(attrs);
                string              name  = (oname != null) ? oname.OptionName : "";

                if (e.Property.Name == name)
                {
                    e.Value = oFields[i].GetValue(gOptions);

                    return;
                }
            }
        }

        // Called when a value is changed.
        protected override void SetValue(object sender, PropertySpecEventArgs e)
        {
            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>     attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionNameAttribute oname = (OptionNameAttribute) AttributeUtility.GetAttribute<OptionNameAttribute>(attrs);
                string              name  = (oname != null) ? oname.OptionName : "";

                if (e.Property.Name == name)
                {
                    oFields[i].SetValue(gOptions, e.Value);

                    return;
                }
            }
        }
    };

    ///==========================================
    /// ProjectOptionsPropertyGrid
    ///==========================================
    
    public class ProjectOptionsPropertyGrid : OptionsPropertyGrid
    {
        /*=== data ===*/

        private Options           gOptions       = null;
        private Options           pOptions       = null;
        private ProjectOptionsSet ProjectOptions = null;
        private Project           project        = null;  
        private string            Configuration  = "";           
        private string            Platform       = "";              

        /*=== construction ===*/

        public ProjectOptionsPropertyGrid() : base()
        {
           
        }

        /*=== utility ===*/

        // Call this to initialize settings.
        public virtual void Initialize(Project p)
        {
            project        = p;
            gOptions       = new Options(OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options);
            ProjectOptions = new ProjectOptionsSet(project);

            /*=== add the existing options ===*/

            List<OptionsElement> options = OptionsManager.GetProjectOptions(p).Options;

            foreach (OptionsElement oe in options)
            {
                OptionsElement newOE = new OptionsElement();
                newOE.Configuration  = oe.Configuration;
                newOE.Platform       = oe.Platform;
                newOE.Options        = new Options(oe.Options);

                ProjectOptions.AddConfiguration(newOE);
            }
        }

        // This method refreshes the property grid.
        public void RefreshGrid(string configuration, string platform)
        {
            // If we don't have a project yet, don't run this method.
            if (project == null)
                return;

            bag.Properties.Clear();

            Configuration = configuration;
            Platform      = platform;

            // Setup the new configuration.  If it doesn't exist, add it.
            pOptions = ProjectOptions.GetNewConfiguration(Configuration, Platform).Options;

            /*=== Setup the property bag. ===*/
            
            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>  attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionInfo       info  = new OptionInfo(attrs);

                // Ignore options that shouldn't show up in the project options dialog.
                if (!info.IsProject)
                    continue;

                OptionBase   gOption = (OptionBase) oFields[i].GetValue(gOptions);
                OptionBase   pOption = (OptionBase) oFields[i].GetValue(pOptions);
                PropertySpec ps      = new PropertySpec(info.Name, pOption.GetOptionType());

                ps.Category          = info.Category;
                ps.Description       = info.Description;
                ps.EditorTypeName    = info.EditorType;
                ps.ConverterTypeName = info.ProjectTypeConverter;
                ps.DefaultValue      = gOption; 
                
                bag.Properties.Add(ps);
            }

            // Set the selected object to be the bag.
            this.SelectedObject = bag;
        }

        /*=== overrides ===*/

        // Override this method to set options to their defaults.
        public override void Defaults()
        {
            pOptions.SetDefaults();
        }

        // Override this method to save options.
        public override void Save()
        {
            /*=== update the global project options, then save ===*/

            List<OptionsElement> options    = ProjectOptions.Options;
            ProjectOptionsSet    optionsSet = OptionsManager.GetProjectOptions(project);

            foreach (OptionsElement oe in options)
                optionsSet.GetNewConfiguration(oe.Configuration, oe.Platform).Options = new Options(oe.Options);

            optionsSet.Save();
        }

        // Called when a value is needed.
        protected override void GetValue(object sender, PropertySpecEventArgs e)
        {
            if (project == null)
                return;

            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>     attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionNameAttribute oname = (OptionNameAttribute) AttributeUtility.GetAttribute<OptionNameAttribute>(attrs);
                string              name  = (oname != null) ? oname.OptionName : "";

                if (e.Property.Name == name)
                {
                    e.Value = oFields[i].GetValue(pOptions);

                    return;
                }
            }
        }

        // Called when a value is changed.
        protected override void SetValue(object sender, PropertySpecEventArgs e)
        {
            if (project == null)
                return;

            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            // Loop through all the fields in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // If the current field is not an option, ignore it.
                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                List<Attribute>     attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionNameAttribute oname = (OptionNameAttribute) AttributeUtility.GetAttribute<OptionNameAttribute>(attrs);
                string              name  = (oname != null) ? oname.OptionName : "";

                if (e.Property.Name == name)
                {
                    oFields[i].SetValue(pOptions, e.Value);

                    return;
                }
            }
        }
    };
}