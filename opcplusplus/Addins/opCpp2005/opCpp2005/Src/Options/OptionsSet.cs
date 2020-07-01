///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ProjectOptions.cs
/// Date: 09/20/2007
///
/// Description:
///
/// Contains classes for project options.
///****************************************************************

using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using Microsoft.VisualStudio.VCProjectEngine;
using EnvDTE;
using System.Windows.Forms;
using Microsoft.Win32;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// Represents a set of options for one configuration.
    ///==========================================
    
    public class OptionsElement
    {
        /*=== data ===*/

        // Configuration name.

        private string configuration;

        public string Configuration
        {
            get { return configuration; }
            set { configuration = value; }
        }

        // Platform name.

        private string platform = "";

        public string Platform 
        {
            get { return platform; }
            set { platform = value; }
        }

        // Configuration options.

        private Options options = new Options();

        public Options Options
        {
            get { return options; }
            set { options = value; }
        }
    };

    ///==========================================
    /// Represents a list of options for multiple configurations.
    ///==========================================

    public abstract class OptionsSet
    {
        /*=== construction ===*/

        public OptionsSet()
        {
            project = null;
            options = new List<OptionsElement>();
        }

        public OptionsSet(Project p)
        {
            project = p;
            options = new List<OptionsElement>();
        }

        /*=== virtuals ===*/

        // Reloads this options set.
        public abstract void Load();

        // Saves this option set.
        public abstract void Save();

        /*=== utility ===*/

        // Add a set of options for a particular configuration to the options set.
        public void AddConfiguration(OptionsElement optionsElement)
        {
            RemoveConfiguration(optionsElement.Configuration, optionsElement.Platform);
            options.Add(optionsElement);
        }

        // Remove a set of options for a particular configuration from the options set.
        public void RemoveConfiguration(string configuration, string platform)
        {
            options.RemoveAll(
                delegate(OptionsElement x) 
                { 
                    return x.Configuration == configuration && x.Platform == platform; 
                }
                             );
        }

        // Removes all options.
        public void ClearConfigurations()
        {
            options.Clear();
        }

        // Returns true if the specified configuration exists.
        public bool ConfigurationExists(string configuration, string platform)
        {
            OptionsElement oe = options.Find(
                delegate(OptionsElement x)
                {
                    return x.Configuration == configuration && x.Platform == platform;
                }
                                            );

            return oe != null;
        }

        // Get an option set.
        public OptionsElement GetConfiguration(string configuration, string platform) 
        {
            return options.Find(
                delegate(OptionsElement x) 
                { 
                    return x.Configuration == configuration && x.Platform == platform; 
                }
                               );
        }

        /*=== data ===*/

        protected Project project;

        [XmlArray]
        private List<OptionsElement> options;

        public List<OptionsElement> Options
        {
            get { return options; }
            set { options = value; }
        }
    };

    ///==========================================
    /// Represents a set of global options.
    ///==========================================
    
    public class GlobalOptionsSet : OptionsSet
    {
        /*=== constructor ===*/

        public GlobalOptionsSet() : base()
        {

        }

        /*=== overrides ===*/

        // Reload the global options.
        public override void Load()
        {
            string           file = Paths.GetGlobalOptionsFilename();
            GlobalOptionsSet os   = null;

            // Load the global options.
            XmlUtility.LoadXml(file, ref os);

            if (os == null)
                this.Options = new List<OptionsElement>();
            else 
                this.Options = os.Options;

            // If the 'GlobalOptionsConfigurationName' configuration does not exist, add it.
            if (GetConfiguration(GlobalConfigurationName, GlobalPlatformName) == null)
            {
                OptionsElement oe = new OptionsElement();

                oe.Configuration = GlobalConfigurationName;
                oe.Platform      = GlobalPlatformName;

                AddConfiguration(oe);

                // Save the new global options.
                Save();
            }
        }

        // Save the global options.
        public override void Save()
        {
            string           file = Paths.GetGlobalOptionsFilename();
            GlobalOptionsSet os   = this;

            if (!XmlUtility.SaveXml(file, ref os))
                MessageBox.Show("opC++ unable to save global options file '" + file + "'.");
        }

        /*=== utility ===*/

        // Returns the single global configuration.
        public OptionsElement GetGlobalConfiguration()
        {
            return GetConfiguration(GlobalConfigurationName, GlobalPlatformName);
        }

		// Use this dll's path to find opCpp.exe if none is defined
		public static string GetDefaultExecutablePath()
		{
			return Path.GetFullPath( Paths.GetFullAppPath() + "..\\Release\\opCpp.exe" );
		}

        /*=== data ===*/

        // Global options configuration name.

        private static readonly string GlobalConfigurationName = "All";

        // Global options platform name.

        private static readonly string GlobalPlatformName = "All";
    };

    ///==========================================
    /// Represents a set of project options.
    ///==========================================

    public class ProjectOptionsSet : OptionsSet
    {
        /*=== construction ===*/

        public ProjectOptionsSet() : base() 
        {

        }

        public ProjectOptionsSet(Project p) : base()
        {
            project = p;
        }

        /*=== overrides ===*/

        // Reload this project's options file.
        public override void Load()
        {
            if (project == null)
                return;

            string            file = Paths.GetProjectOptionsFilename(project);
            ProjectOptionsSet os   = null;

            // Load the project options.
            XmlUtility.LoadXml(file, ref os);

            if (os == null)
                this.Options = new List<OptionsElement>();
            else
                this.Options = os.Options;

            string activeConfiguration = ProjectUtility.GetActiveConfiguration(project);
            string activePlatform      = ProjectUtility.GetActivePlatform(project);

            if (GetConfiguration(activeConfiguration, activePlatform) == null)
            {
                OptionsElement oe = new OptionsElement();

                oe.Configuration = activeConfiguration;
                oe.Platform      = activePlatform;
                oe.Options.SetDefaults();

                AddConfiguration(oe);

                // Save the new project options.
                Save();
            }
        }

        // Save this project's options file.
        public override void Save()
        {
            if (project == null)
                return;

            string            file = Paths.GetProjectOptionsFilename(project);
            ProjectOptionsSet os   = this;

            if (!XmlUtility.SaveXml(file, ref os))
                MessageBox.Show("opC++ unable to save project options file '" + file + "'.");
        }

        /*=== utility ===*/

        // Gets a new configuration (if it doesn't already exist).
        public OptionsElement GetNewConfiguration(string configuration, string platform)
        {
            OptionsElement oe = GetConfiguration(configuration, platform);

            if (oe == null)
            {
                oe = new OptionsElement();

                oe.Configuration = configuration;
                oe.Platform      = platform;
                oe.Options.SetDefaults();

                AddConfiguration(oe);
            }

            return oe;
        }

        // Returns the active configuration.
        public OptionsElement GetActiveConfiguration()
        {
            string activeConfiguration = ProjectUtility.GetActiveConfiguration(project);
            string activePlatform      = ProjectUtility.GetActivePlatform(project);

            return GetNewConfiguration(activeConfiguration, activePlatform);
        }
    };
}