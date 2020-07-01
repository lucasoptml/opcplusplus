//*/***************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
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

namespace opGamesLLC.opCpp2005
{
    // Microsoft visual studio project-related methods.
    public class ProjectUtility
    {
        /*=== utility ===*/

		public static bool SolutionHasOhFiles(Solution solution)
		{
			Projects projects = solution.Projects;
			try
			{

				foreach (Project p in projects)
				{
					if (HasOhFiles(p))
						return true;
				}
			}
			catch (Exception) { }

			return false;
		}
		
// 		public class ProjectInfo
// 		{
// 			ProjectInfo(BuildDependency d)
// 			{
// 				Project = d.Project;
// 				
// 				foreach(Project p in d.RequiredProjects as Array)
// 				{
// 					Dependencies.Add(p);
// 				}
// 			}
// 			
// 			public Project		  Project;
// 			public List<Project> Dependencies;
// 		};
// 
// 		private static void CompleteDependencies(ref List<ProjectInfo> projects)
// 		{
// 			foreach(ProjectInfo p in projects)
// 				CompleteDependency(projects, p);
// 		}
// 
// 		private static void CompleteDependency(List<ProjectInfo> projects, ProjectInfo project)
// 		{
// 			foreach (Project depend in project.Dependencies)
// 			{
// 				ProjectInfo found = projects.Find(
// 					delegate(ProjectInfo other)
// 					{
// 						if (other.Project == depend)
// 							return true;
// 						return false;
// 					}
// 				);
// 
// 				if (found != null)
// 				{
// 					CompleteDependency(projects,found);
// 					
// 					foreach (Project p in found.Dependencies)
// 						project.Dependencies.Add(p);
// 				}
// 			}
// 		}

		// This method returns a list of all projects in the current solution. 
        public static List<Project> GetSolutionProjects()
        {
            List<Project> projects = new List<Project>();
			try
			{

				List<BuildDependency> dependencies = new List<BuildDependency>();

				foreach(BuildDependency b in opCpp2005.App().Solution.SolutionBuild.BuildDependencies)
				{
					dependencies.Add(b);
				}

				// sort by dependency order
				dependencies.Sort( 
					
					delegate(BuildDependency a, BuildDependency b)
					{
						if (a == b)
							return 0;

						//if a should go before b
						Array req = a.RequiredProjects as Array;
						foreach (Project p in req)
						{
							if (p == b.Project)
								return 1;//a < b
						}

						//if b should go before a
						return -1;//b < a
					}
				);
				
				//build the final list
				foreach (BuildDependency b in dependencies)
				{
					if (b.Project.Kind == VcProjGuid)
						projects.Add(b.Project);
				}

			}
			catch (Exception) { }

            return projects;
        }        

		public static List<Project> GetActiveProjects()
		{
			//rely on GetSolutionProjects to get the ordered list,
			List<Project> order = GetSolutionProjects();

			List<Project> projects = new List<Project>();

			Array AllProjects = opCpp2005.App().ActiveSolutionProjects as Array;

			foreach (Project p in AllProjects)
			{
				if (p.Kind == VcProjGuid)
				{
					//now add all the dependencies
					BuildDependency dep = opCpp2005.App().Solution.SolutionBuild.BuildDependencies.Item(p);
					if(dep != null)
					{
						Array required = dep.RequiredProjects as Array;
						foreach(Project dp in required)
						{
							projects.Add(dp);
						}
					}

					//add the project
					projects.Add(p);
				}
			}

			//Only want the ordered projects that are in the projects list.
			order.RemoveAll(
				delegate(Project p)
				{
					bool found = projects.Exists(delegate(Project match) { return match == p; } );
					if (found)
						return false;
					return true;
				}
			);
			
			return order;
		}

        // This method returns 
		public static List<Project> GetActiveProjectOnly()
		{
			List<Project> projects = new List<Project>();

			Array AllProjects = opCpp2005.App().ActiveSolutionProjects as Array;

			foreach (Project p in AllProjects)
			{
				if (p.Kind == VcProjGuid)
					projects.Add(p);
			}

			return projects;
		}

        // Given a project, this returns the project name.
        public static string GetProjectName(Project p)
        {
            return p.Name;
        }

        // Given a project, returns the full project name (path + name).
        public static string GetFullProjectName(Project p)
        {
            return p.FullName;
        }

        // Given a project, returns the full project path (path only).
        public static string GetFullProjectPath(Project p)
        {
            string s = p.FullName;
            int    i = s.LastIndexOf("\\");

            return s.Substring(0, i + 1);
        }

        // Returns the name of the active solution.
        public static string GetSolutionName()
        {
            string solution = opCpp2005.App().Solution.FileName;

            solution = StringUtility.RRight(solution, "\\");
            solution = StringUtility.RLeft(solution, ".");
            
            return solution;
        }

        // Returns the full name of the active solution.
        public static string GetFullSolutionName()
        {
            return opCpp2005.App().Solution.FileName;
        }

        // Returns the full path of the active solution.
        public static string GetFullSolutionPath()
        {
            string path = opCpp2005.App().Solution.FileName;

            path = StringUtility.RLeft(path, "\\");
            
            return path + "\\";
        }

        // This method returns a list of project files for the given project.
        public static List<VCFile> GetProjectFiles(Project p)
        {
            List<VCFile> files = new List<VCFile>();

            RecursiveFiles(p.ProjectItems, ref files);

            return files;
        }

        // Get project files, of a certain extension.
        public static List<VCFile> GetProjectFiles(Project p, string extension)
        {
            List<VCFile> files    = GetProjectFiles(p);
            List<VCFile> outFiles = new List<VCFile>();

            foreach (VCFile f in files)
            {
                if (f.Extension == extension)
                    outFiles.Add(f);
            }

            return outFiles;
        }

		// Filters out inactive files.
		public static void FilterActiveFiles(ref List<VCFile> inoutfiles, string activeconfig)
		{
			List<VCFile> files = new List<VCFile>(inoutfiles);
			inoutfiles.Clear();

			foreach (VCFile file in files)
			{
				IVCCollection fileconfigs = file.FileConfigurations as IVCCollection;
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

        // This method recursively finds files in a project.
        private static void RecursiveFiles(ProjectItems projectitems, ref List<VCFile> files)
        {
            if (projectitems == null)
                return;

            foreach (ProjectItem projectitem in projectitems)
            {
                if (projectitem.Kind == EnvDTE.Constants.vsProjectItemKindPhysicalFile)
                {
                    VCFile file = projectitem.Object as VCFile;

                    if (file != null)
                        files.Add(file);
                }

                RecursiveFiles(projectitem.ProjectItems, ref files);
            }
        }

        // This method returns all .doh files in the given project.
        public static List<VCFile> GetDohFiles(Project p)
        {
            return GetProjectFiles(p, ".doh");
        }

        // This method returns all .oh files in the given project.
        public static List<VCFile> GetOhFiles(Project p)
        {
            return GetProjectFiles(p, ".oh");
        }

        // If true, the project has .doh files.
        public static bool HasDohFiles(Project p)
        {
            return GetDohFiles(p).Count > 0;
        }

        // If true, the project has .oh files.
        public static bool HasOhFiles(Project p)
        {
            return GetOhFiles(p).Count > 0;
        }

        // Returns a list of strings that are the current project's configurations.
        public static List<string> GetConfigurations(Project p)
        {
            List<string> configurations = new List<string>();
            Array        a              = p.ConfigurationManager.ConfigurationRowNames as Array;

            foreach (string c in a)
                configurations.Add(c);

            return configurations;
        }

        // Returns a list of strings that are the current project's platforms.
        public static List<string> GetPlatforms(Project p)
        {
            List<string> platforms = new List<string>();
            Array        a         = p.ConfigurationManager.PlatformNames as Array;

            foreach (string c in a)
            {
                platforms.Add(c);
            }

            return platforms;
        }

        // Returns the active configuration name for the project.
        public static string GetActiveConfiguration(Project p)
        {
            return p.ConfigurationManager.ActiveConfiguration.ConfigurationName;
        }

		// Returns the active platform name for the project
		public static string GetActivePlatform(Project p)
		{
			return p.ConfigurationManager.ActiveConfiguration.PlatformName;
		}

		// Returns the full (config + platform) active configuration name for the project
		public static string ProjectActiveConfiguration(Project p)
		{
			return p.ConfigurationManager.ActiveConfiguration.ConfigurationName
				+ "|"
				+ p.ConfigurationManager.ActiveConfiguration.PlatformName;
		}

        /*=== data ===*/

        // Unique guid for .vcproj files.

        private static readonly string VcProjGuid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";
    };
}
