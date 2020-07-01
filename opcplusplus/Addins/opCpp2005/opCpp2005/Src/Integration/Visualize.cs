///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Visualize.cs
/// Date: 10/15/2007
///
/// Description:
///
/// Visualize Command
///****************************************************************


using System;
using Extensibility;
using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using System.Windows.Forms;
using System.IO;
using SHDocVw;




namespace opGamesLLC.opCpp2005
{

	
	partial class opCpp2005
	{
		enum INTERNETFEATURELIST
		{
			FEATURE_ZONE_ELEVATION = 1,
			FEATURE_MIME_HANDLING = 2,
			FEATURE_MIME_SNIFFING = 3,
			FEATURE_WINDOW_RESTRICTIONS = 4,
			FEATURE_WEBOC_POPUPMANAGEMENT = 5,
			FEATURE_BEHAVIORS = 6,
			FEATURE_DISABLE_MK_PROTOCOL = 7,
			FEATURE_LOCALMACHINE_LOCKDOWN = 8,
			FEATURE_SECURITYBAND = 9,
			FEATURE_RESTRICT_ACTIVEXINSTALL = 10,
			FEATURE_VALIDATE_NAVIGATE_URL = 11,
			FEATURE_RESTRICT_FILEDOWNLOAD = 12,
			FEATURE_ADDON_MANAGEMENT = 13,
			FEATURE_PROTOCOL_LOCKDOWN = 14,
			FEATURE_HTTP_USERNAME_PASSWORD_DISABLE = 15,
			FEATURE_SAFE_BINDTOOBJECT = 16,
			FEATURE_UNC_SAVEDFILECHECK = 17,
			FEATURE_GET_URL_DOM_FILEPATH_UNENCODED = 18,
			FEATURE_TABBED_BROWSING = 19,
			FEATURE_SSLUX = 20,
			FEATURE_DISABLE_NAVIGATION_SOUNDS = 21,
			FEATURE_DISABLE_LEGACY_COMPRESSION = 22,
			FEATURE_FORCE_ADDR_AND_STATUS = 23,
			FEATURE_XMLHTTP = 24,
			FEATURE_DISABLE_TELNET_PROTOCOL = 25,
			FEATURE_FEEDS = 26,
			FEATURE_BLOCK_INPUT_PROMPTS = 27,
			FEATURE_ENTRY_COUNT = 28
		}
		
		
		private const int SET_FEATURE_ON_THREAD = 0x00000001;
		private const int SET_FEATURE_ON_PROCESS = 0x00000002;
		private const int SET_FEATURE_IN_REGISTRY = 0x00000004;
		private const int SET_FEATURE_ON_THREAD_LOCALMACHINE = 0x00000008;
		private const int SET_FEATURE_ON_THREAD_INTRANET = 0x00000010;
		private const int SET_FEATURE_ON_THREAD_TRUSTED = 0x00000020;
		private const int SET_FEATURE_ON_THREAD_INTERNET = 0x00000040;
		private const int SET_FEATURE_ON_THREAD_RESTRICTED = 0x00000080;

		[DllImport("urlmon.dll")]
		[PreserveSig]
		[return: MarshalAs(UnmanagedType.Error)]
		static extern int CoInternetSetFeatureEnabled(
			 INTERNETFEATURELIST FeatureEntry,
			 [MarshalAs(UnmanagedType.U4)] int dwFlags,
			 bool fEnable);
		
		
		void VisualizeCallback(object sender, EventArgs e)
		{
			//here we need to open the xml file
			//we can either open it using a system command (open a webpage)
			//or we can open it in visual studio

			//2. figure out how to open via normal web browser
			// easy, just call start process
			try
			{
				if (CodeCommandsUnavailable())
					throw new Exception();

				string filename = "";

				if (IsGeneratedFileAvailable(".xml",ref filename))
				{
					string url = "file://" + filename;

					//before navigating, apparently you have to do this.
					CoInternetSetFeatureEnabled(INTERNETFEATURELIST.FEATURE_LOCALMACHINE_LOCKDOWN, SET_FEATURE_ON_PROCESS, false);

					Window window = App().ItemOperations.Navigate(url, vsNavigateOptions.vsNavigateOptionsDefault);
					window.Caption = "opC++ Visualization";

					SHDocVw.WebBrowser browser = window.Object as SHDocVw.WebBrowser;
					
					if(browser != null)
					{
						mshtml.HTMLDocument				 doc	= browser.Document as mshtml.HTMLDocument;
						mshtml.HTMLDocumentEvents2_Event ievent = doc as mshtml.HTMLDocumentEvents2_Event;

						//NOTE: theres some issue with this
						//		it seems to work the first time but doesn't after that
						//		I suspect this could be a garbage collection issue.
						if(ievent != null)
						{
							ievent.onclick += new mshtml.HTMLDocumentEvents2_onclickEventHandler(ClickVisualize);
						}
						
					}

					return;
				}
			}
			catch (Exception) { }
			//Now we can add our event handler

			//m_hostedBrowser.NavigateComplete2 += new SHDocVw.DWebBrowserEvents2_NavigateComplete2EventHandler(NavigateComplete2);
		}

		bool ClickVisualize(mshtml.IHTMLEventObj pEvtObj)
		{
			return true;
			/*
			mshtml.IHTMLElement element = pEvtObj.srcElement;
			if(element.className == "gotobutton")
			{
				mshtml.IHTMLElementCollection a = element.children as mshtml.IHTMLElementCollection;

				string foundpath = "";
				int    foundline = -1;

				foreach (mshtml.IHTMLElement e in a)
				{
					if(e.className == "gotopath")
					{
						//TODO: hook up code to generate the path and line to xml.
						//		so we can use the goto code button.
						string path = e.innerText.Trim();
						string dir  = App().ActiveDocument.ProjectItem.ContainingProject.FullName;
						dir = StringUtility.RLeft(dir, "\\") + "\\";

						string filepath = dir + path;
						filepath = System.IO.Path.GetFullPath(filepath);

						if(System.IO.File.Exists(filepath))
						{
							foundpath = filepath;
						}
					}
					if(e.className == "gotoline")
					{
						string line = e.innerText.Trim();
						foundline = Convert.ToInt32(line);
					}
				}

				if(foundpath != ""
				&& foundline != -1)
				{
					Window docwindow = App().ItemOperations.OpenFile(foundpath, EnvDTE.Constants.vsViewKindTextView);
					TextDocument newdoc = (TextDocument)docwindow.Document.Object("");

					newdoc.Selection.GotoLine(foundline, true);
					newdoc.Selection.ActivePoint.TryToShow(vsPaneShowHow.vsPaneShowCentered, null);
				}
			}

			
			return false;
			 */
		}

		void VisualizeStatus(object sender, EventArgs e)
		{
			//here we need to find the xml file (alongside the output file)
			//and see if it exists (actually...maybe up to date)

			//1. find the togenerated code used to find ooh files
			//use this to find the xml file

			try
			{
				if (CodeCommandsUnavailable())
					throw new Exception();
				
				string filename = "";

				VisualizeCommand.Visible = true;
				VisualizeCommand.Enabled = false;

				if(IsGeneratedFileAvailable(".xml", ref filename))
				{
					VisualizeCommand.Enabled = true;	
					return;
				}

				return;
			}
			catch (Exception) { }

			VisualizeCommand.Enabled = false;
			VisualizeCommand.Visible = false;
		}

		// check if a generated file is available (from oh or doh etc)
		// extension, ie .xml
		bool IsGeneratedFileAvailable(string extension, ref string filename)
		{
			string filepath = App().ActiveDocument.FullName;

			Project fileproject = App().ActiveDocument.ProjectItem.ContainingProject;
			if (fileproject == null || fileproject.FullName == "")
			{
				//attempt to grab the fileproject from the active projects...
				Array activeprojects = (Array)App().ActiveSolutionProjects;

				foreach (Project p in activeprojects)
				{
					if (fileproject.FullName != "")
					{
						fileproject = p;
						break;
					}
				}
			}

			if (fileproject != null && fileproject.FullName != "")
			{
				string generatedpath = Paths.OriginalToGeneratedPath(fileproject, filepath);

				generatedpath += extension;

				//check if it exists?
				if (File.Exists(generatedpath))
				{
					filename = generatedpath;
					return true;
				}
			}

			return false;
		}
	};
}