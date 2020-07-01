///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ToolOptions.cs
/// Date: 09/19/2007
///
/// Description:
///
/// Code for the global options dialog.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualStudio.Shell;
using System.Runtime.InteropServices;

namespace vs2005opCpp
{
	/*
	public partial class OptionsPage : UserControl, EnvDTE.IDTToolsOptionsPage
	{
		static OptionPageProperties _propertiesObject = new OptionPageProperties();

		public OptionsPage()
		{
			InitializeComponent();
		}

		public static string GetValueFromRegistry(string key, string value)
		{
			Microsoft.Win32.RegistryKey registryKey;
			registryKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(key, false);
			string registryValue = (string)registryKey.GetValue(value);
			return registryValue;
		}

		public static void SetValueToRegistry(string key, string value, string setting)
		{
 			Microsoft.Win32.RegistryKey registryKey;
			registryKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(key, true);
			registryKey.SetValue(value, setting, Microsoft.Win32.RegistryValueKind.String);
		}

		#region IDTToolsOptionsPage Members

		public void GetProperties(ref object PropertiesObject)
		{
			//Return an object which is accessed by the method call DTE.Properties(category, subcategory)
			PropertiesObject = _propertiesObject;
		}

		public void OnAfterCreated(EnvDTE.DTE DTEObject)
		{
            opCppPath.Text = GetValueFromRegistry(@"SOFTWARE\OP Games\opCpp","Location");
            Arguments.Text = GetValueFromRegistry(@"SOFTWARE\OP Games\opCpp", "GlobalArguments");
		}

		public void OnCancel()
		{
		}

		public void OnHelp()
		{
			//System.Windows.Forms.MessageBox.Show("TODO: Display Help");
		}

		public void OnOK()
		{
			SetValueToRegistry(@"SOFTWARE\OP Games\opCpp","Location",opCppPath.Text);
            SetValueToRegistry(@"SOFTWARE\OP Games\opCpp","GlobalArguments",Arguments.Text);
		}

		#endregion

        private void FindExeButton_Click(object sender, EventArgs e)
        {

        }

    }

    //this is for setting values via add-in calls, instead of touching the registry directly
	[System.Runtime.InteropServices.ComVisible(true)]
	[System.Runtime.InteropServices.ClassInterface(System.Runtime.InteropServices.ClassInterfaceType.AutoDual)]
	public class OptionPageProperties
	{
		public string opCppPath
		{
			get
			{
                return OptionsPage.GetValueFromRegistry(@"SOFTWARE\OP Games\opCpp","Location");
			}
			set
			{
				OptionsPage.SetValueToRegistry(@"SOFTWARE\OP Games\opCpp","Location",value);
			}
		}

	}
 */
	[Guid(GuidStrings.GuidPageCustom)]
	public class newOptionsPage : Microsoft.VisualStudio.Shell.DialogPage, EnvDTE.IDTToolsOptionsPage
	{
		public newOptionsPage()
		{
			OnCancel();
		}
		
		public void GetProperties(ref object PropertiesObject)
		{
			//Return an object which is accessed by the method call DTE.Properties(category, subcategory)
//			PropertiesObject = _propertiesObject;
		}

		public void OnAfterCreated(EnvDTE.DTE DTEObject)
		{
// 			opCppPath.Text = GetValueFromRegistry(@"SOFTWARE\OP Games\opCpp", "Location");
// 			Arguments.Text = GetValueFromRegistry(@"SOFTWARE\OP Games\opCpp", "GlobalArguments");
		}

		public void OnCancel()
		{
		}

		public void OnHelp()
		{
			//System.Windows.Forms.MessageBox.Show("TODO: Display Help");
		}

		public void OnOK()
		{
// 			SetValueToRegistry(@"SOFTWARE\OP Games\opCpp", "Location", opCppPath.Text);
// 			SetValueToRegistry(@"SOFTWARE\OP Games\opCpp", "GlobalArguments", Arguments.Text);
		}	
		
		private string optionCustomString = "weee";
		
		
		[Category("String Options")]
		[Description("My string option")]
		public string OptionString
		{
			get
			{
				return optionCustomString;
			}
			set
			{
				optionCustomString = value;
			}
		}

		protected override void OnActivate(CancelEventArgs e)
		{
// 			DialogResult result = WinFormsHelper.ShowMessageBox(Resources.MessageOnActivateEntered, Resources.MessageOnActivateEntered, MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
// 
// 			if (result == DialogResult.Cancel)
// 			{
// 				Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Cancelled the OnActivate event"));
// 				e.Cancel = true;
// 			}

			base.OnActivate(e);
		}
		
		protected override void OnClosed(EventArgs e)
		{
			//WinFormsHelper.ShowMessageBox(Resources.MessageOnClosed);
		}	
		
		protected override void OnDeactivate(CancelEventArgs e)
		{
// 			DialogResult result = WinFormsHelper.ShowMessageBox(Resources.MessageOnDeactivateEntered, Resources.MessageOnDeactivateEntered, MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
// 
// 			if (result == DialogResult.Cancel)
// 			{
// 				Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Cancelled the OnDeactivate event"));
// 				e.Cancel = true;
// 			}
		}

		protected override void OnApply(PageApplyEventArgs e)
		{
// 			DialogResult result = WinFormsHelper.ShowMessageBox(Resources.MessageOnApplyEntered);
// 
// 			if (result == DialogResult.Cancel)
// 			{
// 				Trace.WriteLine(string.Format(CultureInfo.CurrentCulture, "Cancelled the OnApply event"));
// 				e.ApplyBehavior = ApplyKind.Cancel;
// 			}
// 			else
// 			{
 				base.OnApply(e);
// 			}
// 
// 			WinFormsHelper.ShowMessageBox(Resources.MessageOnApply);
		}
	}

	public static class GuidStrings
	{
		/// <summary>
		/// Guid for the Package class.
		/// </summary>
		public const string GuidPackage = "B0002DC2-56EE-4931-93F7-70D6E9863940";
		/// <summary>
		/// Guid for the PageGeneral class.
		/// </summary>
		public const string GuidPageGeneral = "E6717D0B-111E-4a5b-9834-076CA319ED59";
		/// <summary>
		/// Guid for the PageCustom class.
		/// </summary>
		public const string GuidPageCustom = "0A9F3920-3881-4f50-8986-9EDEC7B33566";
	}
}
