// VsPkg.cs : Implementation of opCpp2005
//

using System;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using Microsoft.Win32;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using System.Xml;
using System.Xml.Serialization;
using EnvDTE80;
using EnvDTE;
using System.IO;
using System.Collections.Generic;
using System.Reflection;
using System.Windows.Forms;

namespace opGamesLLC.opCpp2005
{
	//[ProvideAutoLoad("{F1536EF8-92EC-443C-9ED7-FDADF150DA82}")]
	[ProvideAutoLoad(ContextGuids.vsContextGuidSolutionExists)]
	/// <summary>
    /// This is the class that implements the package exposed by this assembly.
    ///
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the 
    /// IVsPackage interface and uses the registration attributes defined in the framework to 
    /// register itself and its components with the shell.
    /// </summary>
    // This attribute tells the registration utility (regpkg.exe) that this class needs
    // to be registered as package.
    [PackageRegistration(UseManagedResourcesOnly = true)]
    // A Visual Studio component can be registered under different registry roots; for instance
    // when you debug your package you want to register it in the experimental hive. This
    // attribute specifies the registry root to use if no one is provided to regpkg.exe with
    // the /root switch.
	[DefaultRegistryRoot("Software\\Microsoft\\VisualStudio\\8.0")]
    // This attribute is used to register the informations needed to show the this package
    // in the Help/About dialog of Visual Studio.
	[InstalledProductRegistration(true, null, null, null)]
	// In order be loaded inside Visual Studio in a machine that has not the VS SDK installed, 
    // package needs to have a valid load key (it can be requested at 
    // http://msdn.microsoft.com/vstudio/extend/). This attributes tells the shell that this 
    // package has a load key embedded in its resources.
    [ProvideLoadKey("Standard", "1.0", "opC++ Compiler", "opGames LLC", 113)]
	// This attribute is needed to let the shell know that this package exposes some menus.
    [ProvideMenuResource(1000, 1)]
    [Guid(GuidList.guidopCpp2005PkgString)]
	public sealed partial class opCpp2005 : Package, IVsInstalledProduct//, IOleCommandTarget
    {
        /// <summary>
        /// Default constructor of the package.
        /// Inside this method you can place any initialization code that does not require 
        /// any Visual Studio service because at this point the package object is created but 
        /// not sited yet inside Visual Studio environment. The place to do all the other 
        /// initialization is the Initialize method.
        /// </summary>
        public opCpp2005()
        {
            Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Entering constructor for: {0}", this.ToString()));
            instance = this;
        }

         /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        protected override void Initialize()
        {
			try
			{
// 				try
// 				{
// 					TextWriter log = new StreamWriter("opcpp-addin.log");
// 
// 					log.WriteLine("Starting addin : " + System.DateTime.Now.ToString());
// 					log.Flush();

				// happens in /setup mode
				if (App() == null)
					return;

// 					log.WriteLine("app is not null");
// 					log.Flush();
// 
// 				}
// 				catch (Exception)
// 				{
// 				}

				Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Entering Initialize() of: {0}", this.ToString()));
				base.Initialize();

				// Add our command handlers for menu (commands must exist in the .ctc file)
				// Setup the Toolbar, Commands
				CreateCommands();
				CreateTextMarkers();

                // If this is xp or greater, check to see if the opCpp application data path is valid.
                if (Paths.IsXPFolderHierarchy())
                {
                    string opCppUserPath = Paths.GetGlobalOptionsPath();

                    if (!Directory.Exists(opCppUserPath))
                        Directory.CreateDirectory(opCppUserPath);
                }

                Log("Starting opC++ Addin: " + System.DateTime.Now.ToString());
                Log("-----------------------------------------");

                if (opBeta.IsBeta)
                {
                    // do nothing
                }
				// First, check and see if there is a valid license.
				else if ( !opLicenseUtility.LicenseIsValid() )
				{
					// Check to see if we're in trial mode.
					if (opLicenseUtility.InTrialMode())
					{
						TrialVersionForm form = new TrialVersionForm();
						string runsRemaining = "";

						runsRemaining += (opLicenseUtility.MaxTrialRuns - opLicenseUtility.NumTrialRuns);

						form.RunsRemaining = runsRemaining;

						form.ShowDialog();
					}
					else
					{
						// We're not in trial mode, so display a message and lock down.
						LockAddin();

						AddinLockedForm form = new AddinLockedForm();

						form.ShowDialog();
					}
				}
			}
			catch (Exception e)
			{
				LogException(e);
			}
		}

		#region IVsInstalledProduct Members

		//NOTE: no longer called in vs2005 (vs2003 relic)
		public int IdBmpSplash(out uint pIdBmp)
		{
			pIdBmp = 400;
			return VSConstants.S_OK;
		}

		public int IdIcoLogoForAboutbox(out uint pIdIco)
		{
			Log("asking for icon: " + (App() != null ? "about box" : "startup screen"));

			if (App() != null) //about box
				pIdIco = 400; //32 x 32 (Help box)
			else
				pIdIco = 258; //16 x 16 during devenv /setup
			return VSConstants.S_OK;
		}

		public int OfficialName(out string pbstrName)
		{
			pbstrName = GetResourceString("@110");
			return VSConstants.S_OK;
		}

		public int ProductDetails(out string pbstrProductDetails)
		{
			pbstrProductDetails = GetResourceString("@112");
			return VSConstants.S_OK;
		}

		public int ProductID(out string pbstrPID)
		{
			pbstrPID = GetResourceString("@114");
			return VSConstants.S_OK;
		}

		#endregion

		/// <summary>
		/// This method loads a localized string based on the specified resource.
		/// </summary>
		/// <param name="resourceName">Resource to load</param>
		/// <returns>String loaded for the specified resource</returns>
		public string GetResourceString(string resourceName)
		{
			string resourceValue;
			IVsResourceManager resourceManager = (IVsResourceManager)GetService(typeof(SVsResourceManager));
			if (resourceManager == null)
			{
				throw new InvalidOperationException("Could not get SVsResourceManager service. Make sure the package is Sited before calling this method");
			}
			Guid packageGuid = this.GetType().GUID;
			int hr = resourceManager.LoadResourceString(ref packageGuid, -1, resourceName, out resourceValue);
			Microsoft.VisualStudio.ErrorHandler.ThrowOnFailure(hr);
			return resourceValue;
		}

        // Returns the current application object.
		public static DTE2 App()
		{
			if (ApplicationObject == null)
				ApplicationObject = (DTE2) GetGlobalService(typeof(SDTE));

			return ApplicationObject;
		}

		private static DTE2 ApplicationObject;

        // A reference to an opCpp2005 instance.
        private static opCpp2005 instance;

        public static opCpp2005 GetInstance()
        {
            return instance;
        }

// 		#region IOleCommandTarget Members
// 
// 		int Exec(ref Guid pguidCmdGroup, uint nCmdID, uint nCmdexecopt, IntPtr pvaIn, IntPtr pvaOut)
// 		{
// 			if (pguidCmdGroup == new Guid(GuidList.guidVisualStudioCommands))
// 			{
// 				if(nCmdID == (uint)VSConstants.VSStd97CmdID.BuildSln)
// 				{
// 					nCmdID = 0;
// 				}
// 				
// 
// 
// 				// OLECMDERR_E_UNKNOWNGROUP 
// 				// The pguidCmdGroup parameter is not NULL but does not specify a recognized command group. 
// 				//
// 				// OLECMDERR_E_NOTSUPPORTED 
// 				// The nCmdID parameter is not a valid command in the group identified by pguidCmdGroup. 
// 				//
// 				// OLECMDERR_E_DISABLED 
// 				// The command identified by nCmdID is currently disabled and cannot be executed. 
// 				//
// 				// OLECMDERR_E_NOHELP 
// 				// The caller has asked for help on the command identified by nCmdID, but no help is available. 
// 				//
// 				// OLECMDERR_E_CANCELED 
// 				// The user canceled the execution of the command. 
// 
// 				//return Microsoft.VisualStudio.OLE.Interop.Constants.
// 			}
// 
// 			return VSConstants.S_OK;
// 			//throw new Exception("The method or operation is not implemented.");
// 		}
// 
// 		int QueryStatus(ref Guid pguidCmdGroup, uint cCmds, OLECMD[] prgCmds, IntPtr pCmdText)
// 		{
// 			//throw new Exception("The method or operation is not implemented.");
// 
// 			return VSConstants.S_OK;
// 		}
// 
// 		#endregion
	}
}
