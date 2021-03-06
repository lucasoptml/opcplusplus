///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Connect.cs
/// Date: 09/18/2007
///
/// Description:
///
/// This class allows the addin to connect to visual studio's IDE.
///****************************************************************

using System;
using Extensibility;
using EnvDTE;
using EnvDTE80;

namespace vs2005opCpp
{
	// The connect class.
    public partial class Connect : IDTExtensibility2, IDTCommandTarget
	{
        // This method gets the application object.
        DTE2 App()
        {
            return _applicationObject;
        }

        // This method updates the status bar.
        void SetStatusText(string text)
        {
            App().StatusBar.Text = text;
        }

		/// <summary>Implements the OnConnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being loaded.</summary>
		/// <param term='application'>Root object of the host application.</param>
		/// <param term='connectMode'>Describes how the Add-in is being loaded.</param>
		/// <param term='addInInst'>Object representing this Add-in.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnConnection(object application, ext_ConnectMode connectMode, object addInInst, ref Array custom)
		{
			//retreive application objects
			_applicationObject = (DTE2)application;
			_addInInstance     = (AddIn)addInInst;

			if(connectMode == ext_ConnectMode.ext_cm_UISetup)
			{
				//this happens only once per add-in, its not a good idea to 
				//use it unless you have once-only behavior
				StartupAddin();
			}
			
			if(connectMode == ext_ConnectMode.ext_cm_Startup
			|| connectMode == ext_ConnectMode.ext_cm_AfterStartup)
			{
				//this is the real events for add-in startup
				//you have to make sure you don't redo things,
				//so everything should be built on the assumption
				//it could be called multiple times
				StartupAddin();
			}

		}

        // This method sets up the addin event handlers.
		void StartupAddin()
		{
			//events added twice will throw an exception
			try
			{
				if(_commandEvents == null)
				{
					_buildEvents = App().Events.BuildEvents;

 					_buildEvents.OnBuildBegin           += new _dispBuildEvents_OnBuildBeginEventHandler(this.OnBuildBegin);
 					_buildEvents.OnBuildDone            += new _dispBuildEvents_OnBuildDoneEventHandler(this.OnBuildDone);
 					_buildEvents.OnBuildProjConfigBegin += new _dispBuildEvents_OnBuildProjConfigBeginEventHandler(this.OnBuildProjConfigBegin);
 					_buildEvents.OnBuildProjConfigDone  += new _dispBuildEvents_OnBuildProjConfigDoneEventHandler(this.OnBuildProjConfigDone);
					
					_commandEvents = App().Events.get_CommandEvents("{00000000-0000-0000-0000-000000000000}", 0);

 					_commandEvents.BeforeExecute += new _dispCommandEvents_BeforeExecuteEventHandler(BeforeExecute);
 					_commandEvents.AfterExecute  += new _dispCommandEvents_AfterExecuteEventHandler(AfterExecute);
				}
			}
			catch(Exception) {}

			CreateToolbar();
		}

        // This method closes the addin.
		void CloseAddin()
		{
             _buildEvents.OnBuildBegin           -= this.OnBuildBegin;
             _buildEvents.OnBuildDone            -= this.OnBuildDone;
             _buildEvents.OnBuildProjConfigBegin -= this.OnBuildProjConfigBegin;
             _buildEvents.OnBuildProjConfigDone  -= this.OnBuildProjConfigDone;

            _commandEvents.BeforeExecute -= BeforeExecute;
            _commandEvents.AfterExecute  -= AfterExecute;

            CloseToolbar();
		}

		/// <summary>Implements the OnDisconnection method of the IDTExtensibility2 interface. Receives notification that the Add-in is being unloaded.</summary>
		/// <param term='disconnectMode'>Describes how the Add-in is being unloaded.</param>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnDisconnection(ext_DisconnectMode disconnectMode, ref Array custom)
		{
			//NOTE: I know I dont want to close upon exit of visual studio
            if (disconnectMode == ext_DisconnectMode.ext_dm_UserClosed)
            {
                CloseAddin();
            }
		}

		/// <summary>Implements the OnAddInsUpdate method of the IDTExtensibility2 interface. Receives notification when the collection of Add-ins has changed.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />		
		public void OnAddInsUpdate(ref Array custom)
		{
		}

		/// <summary>Implements the OnStartupComplete method of the IDTExtensibility2 interface. Receives notification that the host application has completed loading.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnStartupComplete(ref Array custom)
		{
			//NOTE: Startup is complete by now.
			//_applicationObject.StatusBar.Text = "opcpp startup complete!";
		}

		/// <summary>Implements the OnBeginShutdown method of the IDTExtensibility2 interface. Receives notification that the host application is being unloaded.</summary>
		/// <param term='custom'>Array of parameters that are host application specific.</param>
		/// <seealso class='IDTExtensibility2' />
		public void OnBeginShutdown(ref Array custom)
		{
		}
		
		/// <summary>Implements the QueryStatus method of the IDTCommandTarget interface. This is called when the command's availability is updated</summary>
		/// <param term='commandName'>The name of the command to determine state for.</param>
		/// <param term='neededText'>Text that is needed for the command.</param>
		/// <param term='status'>The state of the command in the user interface.</param>
		/// <param term='commandText'>Text requested by the neededText parameter.</param>
		/// <seealso class='Exec' />
		public void QueryStatus(string commandName, vsCommandStatusTextWanted neededText, ref vsCommandStatus status, ref object commandText)
		{
			if (neededText == vsCommandStatusTextWanted.vsCommandStatusTextWantedNone)
			{
				status = SupportsCommand(commandName)
					? (vsCommandStatus)vsCommandStatus.vsCommandStatusSupported|vsCommandStatus.vsCommandStatusEnabled
					: (vsCommandStatus)vsCommandStatus.vsCommandStatusUnsupported;
			}
		}

		/// <summary>Implements the Exec method of the IDTCommandTarget interface. This is called when the command is invoked.</summary>
		/// <param term='commandName'>The name of the command to execute.</param>
		/// <param term='executeOption'>Describes how the command should be run.</param>
		/// <param term='varIn'>Parameters passed from the caller to the command handler.</param>
		/// <param term='varOut'>Parameters passed from the command handler to the caller.</param>
		/// <param term='handled'>Informs the caller if the command was handled or not.</param>
		/// <seealso class='Exec' />
		public void Exec(string commandName, vsCommandExecOption executeOption, ref object varIn, ref object varOut, ref bool handled)
		{
			handled = false;

			if(executeOption == vsCommandExecOption.vsCommandExecOptionDoDefault)
			{
				handled = RunCommand(commandName);
			}
		}
	
        /*=== data ===*/

        private DTE2          _applicationObject;
		private string        CommandPrefix = "opCpp";
		private BuildEvents   _buildEvents;
		private CommandEvents _commandEvents;
	}




}