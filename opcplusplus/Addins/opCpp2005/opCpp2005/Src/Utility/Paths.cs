///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Paths.cs
/// Date: 09/22/2007
///
/// Description:
///
/// Contains utility methods for getting different paths.
///****************************************************************

using System;
using System.IO;
using System.Reflection;
using EnvDTE;

namespace opGamesLLC.opCpp2005
{
	public class Paths
	{
		/*=== utility ===*/

		// Returns the location of the running application (path and filename).
		public static string GetFullAppName()
		{
			Assembly a = Assembly.GetExecutingAssembly();

			return a.Location;
		}

		// Returns the location of the running application (path only).
		public static string GetFullAppPath()
		{
			string s = GetFullAppName();
			return StringUtility.RLeft(s, "\\") + "\\";
		}

		// Returns true if we're to use program files and not the
		// application data folder.
		public static bool IsXPFolderHierarchy()
		{
			OperatingSystem osInfo = Environment.OSVersion;

			return osInfo.Version.Major > 5
				|| (osInfo.Version.Major == 5 && osInfo.Version.Minor >= 1);
		}

		// Returns the application data folder for the current user.
		public static string GetApplicationDataPath()
		{
			return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
		}

		// Returns the directory that global options are written to.
		public static string GetGlobalOptionsPath()
		{
			string path;

			// If this is a newer os, write the global options to the the user's application 
			// data folder.  Otherwise, write to the compiler directory.
			if (IsXPFolderHierarchy())
				path = GetApplicationDataPath() + "\\opC++\\";
			else
				path = GetFullAppPath();

			return path;
		}

		// Returns the path of the global options file.
		public static string GetGlobalOptionsFilename()
		{
			return GetGlobalOptionsPath() + GlobalOptionsFile;
		}

		// Returns the path of the project options file for the input project.
		public static string GetProjectOptionsFilename(Project project)
		{
			if (project == null)
				return "";

			return ProjectUtility.GetFullProjectPath(project) + ProjectUtility.GetProjectName(project) + ProjectOptionsFile;
		}

		// Resolves a path with visual studio macros in it.
		public static string ResolveVisualStudioMacros(Project project, string path)
		{
			// Replace $(PlatformName) with the platform name.
			path = path.Replace("$(PlatformName)", ProjectUtility.GetActivePlatform(project));

			// Replace $(ConfigurationName) with the configuration name.
			path = path.Replace("$(ConfigurationName)", ProjectUtility.GetActiveConfiguration(project));

			// Replace $(ProjectName) with the project name.
			path = path.Replace("$(ProjectName)", ProjectUtility.GetProjectName(project));

			// Replace $(SolutionName) with the solution name.
			path = path.Replace("$(SolutionName)", ProjectUtility.GetSolutionName());

			// Replace $(ProjectDir) with the project directory.
			path = path.Replace("$(ProjectDir)", ProjectUtility.GetFullProjectPath(project));

			// Replace $(SolutionDir) with the solution directory.
			path = path.Replace("$(SolutionDir)", ProjectUtility.GetFullSolutionPath());

			//if we still have $() it should error really!

			return path;
		}

		public static string GetParentPath(string path, int depth)
		{
			string filename;
			return GetParentPath(path, depth, out filename);
		}

		public static string GetParentPath(string path, int depth, out string filename)
		{
			//if we start with a '\', return it minus the slash
			//
			if (path.Length > 0 && path[path.Length - 1] == '\\')
			{
				filename = "";
				return path.Substring(0, path.Length - 1);
			}

			int position = path.Length - 1;

			while (depth >= 0)
			{
				while (position >= 0)
				{
					bool found = path[position] == '\\';

					position--;

					if (found)
						break;
				}

				depth--;
			}

			position++;

			string dir = path.Substring(0, position);
			filename = path.Substring(position + 1, path.Length - position - 1);

			return dir;
		}

		public static string GeneratedToOriginalPath(string generated, int gendepth, int mydepth)
		{
			//mydepth = depth from generated directory start
			//gendepth = depth from project folder

			//first, utilizing mydepth, can get the resolvable path
			//second, utilizing gendepth, can get the absolute project path
			string filename;
			string generatedpath = GetParentPath(generated, mydepth - gendepth, out filename);

			string projectpath = GetParentPath(generatedpath, gendepth - 1);

			filename = StringUtility.RLeft(filename, ".");

			string newpath = filename.Replace("_/", "../");

			// windows absolute path 
			if (projectpath.StartsWith("_win/"))
			{
				newpath = StringUtility.Right(newpath, 5);
				newpath.Insert(1, ":");
			}
			// linux root path
			else if (newpath.StartsWith("_root/"))
			{
				newpath = StringUtility.Right(newpath, 5);
			}
			// network share path
			else if (newpath.StartsWith("_net/"))
			{
				newpath = "/" + StringUtility.Right(newpath, 4);
			}

			string resolvedpath = newpath;

			//third, once the resolvable path is available, can append relative to project path
			//also can just utilize absolute paths
			if (System.IO.Path.IsPathRooted(resolvedpath))
			{
				//absolute case
				return resolvedpath;
			}

			//relative case
			return projectpath + "\\" + resolvedpath;
		}

		// original path must be absolute
		public static string OriginalToGeneratedPath(Project project, string originalpath)
		{
			string projectpath = StringUtility.RLeft(project.FullName, "\\") + "\\";

			string pathstring = Paths.EvaluateRelativePath(projectpath, originalpath);

			if (Path.IsPathRooted(pathstring))
			{
				//cases: //
				//       /
				//       c:/

				if(pathstring.StartsWith("/"))
				{
					pathstring = "_root" + pathstring;
				}
				else if(pathstring.StartsWith("//"))
				{
					pathstring = "_net" + StringUtility.Right(pathstring,1);
				}
				else
				{
					//c:/
					pathstring.Replace(":","");
					pathstring = "_win/" + pathstring;
				}
			}

			string relativepath = pathstring.Replace("..\\", "_\\");
			
			// 3. if it's a complete network path, I need to handle the root

			//TODO: get the generated directory from the project settings
			string generated = OptionsManager.GetProjectOptions(project).GetActiveConfiguration().Options.GeneratedDirectory.Value;

			if(generated == "")
				generated = OptionsManager.GetGlobalOptions().GetGlobalConfiguration().Options.GeneratedDirectory.Value;

			generated = ResolveVisualStudioMacros(project, generated);

			generated = MakeValidPath(generated);

			//resolve the macros
			return projectpath + generated + relativepath;
		}

		//make a directory path valid (have a \ on the end)
		public static string MakeValidPath(string s)
		{
			if (s.EndsWith("\\"))
				return s;
			return s + "\\";
		}

		// gets the path of a file relative to another file
		public static string RelativePath(string basefile, string relativefile)
		{
			basefile = StringUtility.RLeft(basefile, "\\");
			string newpath = basefile + "\\" + relativefile;
			return System.IO.Path.GetFullPath(newpath);
		}

		public static string EvaluateRelativePath(string mainDirPath, string absoluteFilePath)
		{
			string[] firstPathParts = mainDirPath.Trim(Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar);
			string[]
			secondPathParts = absoluteFilePath.Trim(Path.DirectorySeparatorChar).Split(Path.DirectorySeparatorChar);

			int sameCounter = 0;
			for (int i = 0; i < Math.Min(firstPathParts.Length,secondPathParts.Length); i++)
			{
				if ( !firstPathParts[i].ToLower().Equals(secondPathParts[i].ToLower()) )
				{
					break;
				}
				sameCounter++;
			}

			if (sameCounter == 0)
			{
				return absoluteFilePath;
			}

			string newPath = "";
			for (int i = sameCounter; i < firstPathParts.Length; i++)
			{
				if (i > sameCounter)
				{
					newPath += Path.DirectorySeparatorChar;
				}
				newPath += "..";
			}
			if (newPath.Length == 0)
			{
				newPath = ".";
			}
			for (int i = sameCounter; i < secondPathParts.Length; i++)
			{
				newPath += Path.DirectorySeparatorChar;
				newPath += secondPathParts[i];
			}
			return newPath;
		}

		/*=== data ===*/

		// Global options file name.

		private static readonly string GlobalOptionsFile = "GlobalOptions.opoptions";

		// Project options file name.

		private static readonly string ProjectOptionsFile = "Options.opoptions";
	};
}