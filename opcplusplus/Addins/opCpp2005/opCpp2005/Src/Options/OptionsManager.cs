///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: OptionsManager.cs
/// Date: 09/20/2007
///
/// Description:
///
/// Class to manage all (global and project) options.
///****************************************************************

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Microsoft.VisualStudio.VCProjectEngine;
using EnvDTE;
using System.Reflection;
using System.IO;

namespace opGamesLLC.opCpp2005
{
    // This data structure is returned from 'GetCommandLineInfo' in the 
    // 'OptionsManager' class.
    public class CommandLineInfo
    {
        // Path to the opCpp.exe executable.

        private string executablePath;

        public string ExecutablePath
        {
            get { return executablePath; }
            set { executablePath = value; }
        }

        // Command line arguments string.

        private string arguments;

        public string Arguments
        {
            get { return arguments; }
            set { arguments = value; }
        }
    };

    // This class contains some static methods to retrieve 
    // command line-related information.
    public class OptionsManager
    {
        // This method returns the command line argument string.
        public static CommandLineInfo GetCommandLineInfo(Project p)
        {
            /*=== Read the options. ===*/

            Options gOptions = GetGlobalOptions().GetGlobalConfiguration().Options;
            Options pOptions = GetProjectOptions(p).GetActiveConfiguration().Options;

            /*=== Use reflection to build the command line string. ===*/
            
            string      cmd = "";
            FieldInfo[] oFields;
            Type        oType     = typeof(Options);
            char[]      trimchars = {'"'};

            // Get the fields for type 'Options'.
            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);
            
            /*=== Loop through the data members in the 'Options' object. ===*/

            for (int i = 0; i < oFields.Length; i++)
            {
                /*=== If the current field is not an option, ignore it. ===*/

                if (!oFields[i].FieldType.IsSubclassOf(typeof(OptionBase)))
                    continue;

                /*=== Parse this option's attributes. ===*/

                List<Attribute> attrs = AttributeUtility.GetAllAttributes(oFields[i], false);
                OptionInfo      info  = new OptionInfo(attrs);

                // If a commandline syntax isn't specified, ignore this option.
                if (info.CommandLine == "")
                    continue;

                string tag = info.CommandLine;

                // Handle string list options.
                if (oFields[i].FieldType == typeof(StringListOption))
                {
                    StringListOption gOption = (StringListOption) oFields[i].GetValue(gOptions);
                    StringListOption pOption = (StringListOption) oFields[i].GetValue(pOptions);
                    int              gsize   = gOption.StringList.Count;
                    int              psize   = pOption.StringList.Count;

                    if (gsize > 0)
                    {
                        cmd += " " + tag + " ";
                        cmd += '"' + gOption.StringList[0].Trim(trimchars) + '"';

                        for (int j = 1; j < gsize; j++)
                            cmd += "," + '"' + gOption.StringList[j].Trim(trimchars) + '"';
                    }

                    if (psize > 0)
                    {
                        int j = 0;

                        if (gsize < 1)
                        {
                            cmd += " " + tag + " ";
                            cmd += '"' + pOption.StringList[0].Trim(trimchars) + '"';
                            j    = 1;
                        }

                        for (; j < psize; j++)
                            cmd += "," + '"' + pOption.StringList[j].Trim(trimchars) + '"';
                    }
                }
                // Handle string options.
                else if (oFields[i].FieldType == typeof(StringOption))
                {
                    StringOption gOption = (StringOption) oFields[i].GetValue(gOptions);
                    StringOption pOption = (StringOption) oFields[i].GetValue(pOptions);

                    if (pOption.Value != "")
                        cmd += " " + tag + " " + '"' + pOption.Value.Trim(trimchars) + '"';
                    else if (gOption.Value != "")
                        cmd += " " + tag + " " + '"' + gOption.Value.Trim(trimchars) + '"';
                }
                // Handle boolean options.
                else if (oFields[i].FieldType == typeof(BoolOption))
                {
                    BoolOption gOption = (BoolOption) oFields[i].GetValue(gOptions);
                    BoolOption pOption = (BoolOption) oFields[i].GetValue(pOptions);
                    bool       on      = pOption.UseDefault ? gOption.Value : pOption.Value;
                    
                    if (on)
                        cmd += " " + tag;
                }
                // Handle int options.
                else if (oFields[i].FieldType == typeof(IntOption))
                {
                    IntOption gOption = (IntOption) oFields[i].GetValue(gOptions);
                    IntOption pOption = (IntOption) oFields[i].GetValue(pOptions);
                    int       val     = pOption.UseDefault ? gOption.Value : pOption.Value;

                    cmd += " " + tag + " " + val;
                }
            }

            /*=== Setup the command line info data structure. ===*/

            CommandLineInfo cmdinfo = new CommandLineInfo();

            /*=== set the license path ===*/

            if (pOptions.LicensePath.Value != "")
                cmd += " -license " + '"' + pOptions.LicensePath.Value.Trim(trimchars) + '"';
            else if (gOptions.LicensePath.Value != "")
                cmd += " -license " + '"' + gOptions.LicensePath.Value.Trim(trimchars) + '"';
            else
                cmd += " -license " + '"' + opLicenseUtility.FullLicenseFileName + '"';
            
            cmdinfo.Arguments = cmd;

            /*=== set the executable path ===*/

            if (pOptions.ExecutablePath.Value != "")
                cmdinfo.ExecutablePath = pOptions.ExecutablePath.Value.Trim(trimchars);
            else if (gOptions.ExecutablePath.Value != "")
                cmdinfo.ExecutablePath = gOptions.ExecutablePath.Value.Trim(trimchars);
            else
                cmdinfo.ExecutablePath = Path.GetFullPath( Paths.GetFullAppPath() + "..\\Release\\opCpp.exe" );

            return cmdinfo;
        }

        /*=== data ===*/

        // The global options set.

        private static GlobalOptionsSet GlobalOptions = null;

        public static GlobalOptionsSet GetGlobalOptions()
        {
            if (GlobalOptions == null)
            {
                GlobalOptions = new GlobalOptionsSet();

                GlobalOptions.Load();
            }

            return GlobalOptions;
        }

        // The project options sets.

        private static Dictionary<Project, ProjectOptionsSet> ProjectOptions = new Dictionary<Project, ProjectOptionsSet>();

        public static ProjectOptionsSet GetProjectOptions(Project p)
        {
            if (p == null)
                return null;

            ProjectOptionsSet project = null;

            if (!ProjectOptions.TryGetValue(p, out project))
            {
                project = new ProjectOptionsSet(p);

                project.Load();

                ProjectOptions.Add(p, project);
            }

            return project;
        }
    };
}