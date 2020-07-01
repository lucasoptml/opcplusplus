///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Projects.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Code for looking at the solution/project tree.
///****************************************************************
///

using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.VCProjectEngine;
using EnvDTE;
using System.Windows.Forms;

namespace vs2005opCpp
{
    public partial class Connect
    {
        // Return code for the opCpp addin.
        enum returncode
        {
            None,
            Success,
            Errors,
        }

        // Builds a list of projects in the current solution. 
        // TODO - does this work?
        returncode BuildProjects(List<Project> projects)
        {
            returncode totalresult = returncode.None;

            bool bforce = false;

            //directly save the files
            foreach (Document doc in App().Documents)
            {
                if ((doc.FullName.EndsWith(".oh")
                || doc.FullName.EndsWith(".doh"))
                && doc.Saved == false)
                {
                    if (doc.FullName.EndsWith(".doh"))
                        bforce = true;

                    doc.Save(doc.FullName);
                }
            }

            GetOutputPane().Clear();
            GetOutputPane().Activate();
            Application.DoEvents();

            foreach (Project project in projects)
            {
                returncode result = BuildProject(project, bforce);

                if (result == returncode.Errors)
                {
                    totalresult = returncode.Errors;
                    return totalresult;
                }
                else if (result == returncode.Success)
                    totalresult = returncode.Success;
            }

            return totalresult;
        }

        // Builds the project but doesn't force a rebuild.
        returncode BuildProject(Project project)
        {
            return BuildProject(project, false);
        }

        // Builds the project.
        returncode BuildProject(Project project, bool bforce)
        {
            List<VCFile> ohfiles = FindProjectFiles(project);
            FilterFiles(ref ohfiles, ".oh");

            List<VCFile> dohfiles = FindProjectFiles(project);
            FilterFiles(ref dohfiles, ".doh");

            if (ohfiles.Count == 0)
                return returncode.None;

            if (dohfiles.Count == 0)
                return returncode.None;

            //build the active configuration name
            string activeconfig = ProjectActiveConfig(project);

            FilterActiveFiles(ref ohfiles, activeconfig);
            FilterActiveFiles(ref dohfiles, activeconfig);

            if (dohfiles.Count == 0)
                return returncode.None;

            if (ohfiles.Count == 0)
                return returncode.None;

            //now we want to aggregate all the files
            //into a list for opcpp
            //format: "a","b","c","d"
            string ohfilestring = BuildFileString(ohfiles);
            string oharguments = "-oh " + ohfilestring;

            string dohfilestring = BuildFileString(dohfiles);
            string doharguments = "-doh " + dohfilestring;

            string arguments = oharguments + " " + doharguments;

            // 			if (bverbose)
            // 				arguments += " -verbose";
            if (bforce)
                arguments += " -force";

            arguments += " -globmode";

            //add the -gd directory argument
            arguments += " -gd ";
            arguments += "\"Generated\\" + project.Name + "\"";

            string workingdir = project.FileName.Substring(0, project.FileName.LastIndexOf('\\'));


            arguments = GetOpCppArguments() + " " + arguments;

            runthread = new opCppThread(GetOpCppPath(), workingdir, arguments);
            runthread.OnEnd += opCppFinished;
            runthread.OnReadLine += opCppLog;
            runthread.Start();

            //Thread thread = new Thread(new ThreadStart());
            //return returncode.None;

            //if (result)
            return returncode.Success;
            //else
            //	return returncode.Errors;
        }

        // Utility method.
        String ProjectActiveConfig(Project p)
        {
            return p.ConfigurationManager.ActiveConfiguration.ConfigurationName
                + "|"
                + p.ConfigurationManager.ActiveConfiguration.PlatformName;
        }

        // Builds list of files for the command line arguments.
        String BuildFileString(List<VCFile> files)
        {
            String value = "";
            foreach (VCFile file in files)
            {
                value += "\"" + file.RelativePath + "\"";
                value += ',';
            }

            value = value.TrimEnd(',');
            return value;
        }

        // Filters files based on extension.
        void FilterFiles(ref List<VCFile> inoutfiles, string suffix)
        {
            List<VCFile> files = new List<VCFile>(inoutfiles);
            inoutfiles.Clear();

            foreach (VCFile file in files)
            {
                if (file.Extension == suffix)
                    inoutfiles.Add(file);
            }
        }

        // Filters out inactive files.
        void FilterActiveFiles(ref List<VCFile> inoutfiles, string activeconfig)
        {
            List<VCFile> files = new List<VCFile>(inoutfiles);
            inoutfiles.Clear();

            foreach (VCFile file in files)
            {
                IVCCollection fileconfigs = (IVCCollection)file.FileConfigurations;
                foreach (VCFileConfiguration config in fileconfigs)
                {
                    if (config.Name == activeconfig)
                    {
                        if (!config.ExcludedFromBuild)
                        {
                            inoutfiles.Add(file);
                            break;
                        }
                    }
                }
            }
        }

        // Returns a list of project files.
        List<VCFile> FindProjectFiles(Project project)
        {
            List<VCFile> files = new List<VCFile>();

            //use my file finder code.
            RecursiveFiles(project.ProjectItems, ref files);

            return files;
        }

        //recursively find the files in a project
        //utility function
        void RecursiveFiles(ProjectItems projectitems, ref List<VCFile> files)
        {
            if (projectitems == null)
                return;

            foreach (ProjectItem projectitem in projectitems)
            {
                if (projectitem.Kind == EnvDTE.Constants.vsProjectItemKindPhysicalFile)
                {
                    VCFile file = (VCFile)projectitem.Object;

                    if (file != null)
                        files.Add(file);
                }
                RecursiveFiles(projectitem.ProjectItems, ref files);
            }
        }

        //FindProjects
        //get the projects list from the solution.
        List<Project> FindProjects()
        {
            List<Project> projects = new List<Project>();
            foreach (Project p in App().Solution.Projects)
                projects.Add(p);
            return projects;
        }

        //FindCurrentProjects
        //get the currently selected project and its dependencies.
        List<Project> FindCurrentProjects()
        {
            //TODO: this doesn't get the full dependencies list, only the project.
            List<Project> projects = new List<Project>();
            System.Array currentprojects = (System.Array)App().ActiveSolutionProjects;
            foreach (Project p in currentprojects)
                projects.Add(p);
            return projects;
        }
    }
}