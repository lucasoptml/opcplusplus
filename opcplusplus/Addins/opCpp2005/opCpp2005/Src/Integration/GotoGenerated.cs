///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: GotoGenerated.cs
/// Date: 10/01/2007
///
/// Description:
///
/// Goto Generated Code
///****************************************************************

using System;
using EnvDTE;
using System.Collections.Generic;
using Extensibility;
using EnvDTE80;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.VCProject;
using Microsoft.VisualStudio.VCProjectEngine;
using Microsoft.VisualStudio.VCCodeModel;
using System.Runtime.InteropServices;
using System.ComponentModel.Design;
using System.IO;
using System.Windows.Forms;

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
		public void GotoOOHCallback(object sender, EventArgs e)
		{
			string filename = "";

			if(IsGeneratedFileAvailable(".ooh",ref filename))
			{
				App().ItemOperations.OpenFile(filename, EnvDTE.Constants.vsViewKindTextView);
				return;
			}
		}

		public void GotoOCPPCallback(object sender, EventArgs e)
		{
			string filename = "";

			if(IsGeneratedFileAvailable(".ocpp",ref filename))
			{
				App().ItemOperations.OpenFile(filename, EnvDTE.Constants.vsViewKindTextView);
				return;
			}
		}

		public bool DocumentCommandsUnavailable()
		{
			if (bAddinLocked)
				return true;
			if (App() == null)
				return true;
			if (App().ActiveDocument == null)
				return true;

			if (App().ActiveDocument.Language == "C/C++"
			||  App().ActiveDocument.Name.EndsWith(".oh")
			||  App().ActiveDocument.Name.EndsWith(".doh")
			||  App().ActiveDocument.Name.EndsWith(".ooh")
			||  App().ActiveDocument.Name.EndsWith(".ocpp")
			||  App().ActiveDocument.Name.EndsWith(".oohindex")
			||  App().ActiveDocument.Name.EndsWith(".ocppindex"))
				return false;

			return true;
		}

		public bool CodeCommandsUnavailable()
		{
			if (bAddinLocked)
				return true;
			if (App() == null)
				return true;
			if (App().ActiveDocument == null)
				return true;

			if (App().ActiveDocument.Name.EndsWith(".oh")
			||  App().ActiveDocument.Name.EndsWith(".doh"))
				return false;

			return true;
		}

		public void GotoOOHStatus(object sender, EventArgs e)
		{
			try
			{
				if (CodeCommandsUnavailable())
					throw new Exception();

				string filename = "";
				
				GotoOOHCommand.Visible = true;
				
				if(IsGeneratedFileAvailable(".ooh",ref filename))
				{
					GotoOOHCommand.Enabled = true;

					return;
				}

				GotoOOHCommand.Enabled = false;
				return;
			}
			catch (Exception) { }

			GotoOOHCommand.Visible = false;
			GotoOOHCommand.Enabled = false;
		}

		public void GotoOCPPStatus(object sender, EventArgs e)
		{
			try
			{
				if (CodeCommandsUnavailable())
					throw new Exception();

				string filename = "";
				
				GotoOCPPCommand.Visible = true;

				if(IsGeneratedFileAvailable(".ocpp", ref filename))
				{
					GotoOCPPCommand.Enabled = true;

					return;
				}

				GotoOCPPCommand.Enabled = false;
				return;
			}
			catch (Exception) { }

			GotoOCPPCommand.Visible = false;
			GotoOCPPCommand.Enabled = false;
		}
	}
}




