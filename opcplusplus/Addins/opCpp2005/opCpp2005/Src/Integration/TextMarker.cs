///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: TextMarker.cs
/// Date: 10/02/2007
///
/// Description:
///
/// Text Marker Code
///****************************************************************


//NOTE: trying to figure out how to register a new custom code marker...

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
using Microsoft.VisualStudio.TextManager.Interop;


namespace opGamesLLC.opCpp2005
{
	
	public partial class opCpp2005
	{
		void CreateTextMarkers()
		{
			textmarkerservice = new MyTextMarkers();
			
			IServiceContainer serviceContainer = (IServiceContainer)this;
			serviceContainer.AddService(typeof(MyTextMarkers), textmarkerservice, true);
		}

		static MyTextMarkers textmarkerservice;
	}

	[Guid("40F475EB-DE8B-4ef7-B2BA-E8B2FFAACD7B")]
	public class MyTextMarkers : IVsTextMarkerTypeProvider, IVsPackageDefinedTextMarkerType
	{
		#region IVsTextMarkerTypeProvider Members

		public int GetTextMarkerType(ref Guid pguidMarker, out IVsPackageDefinedTextMarkerType ppMarkerType)
		{
			//When a package registers an external marker type, this interface is 
			//implemented once by the specified service. This method passes you a 
			//GUID that matches the GUID of a marker that you have registered under 
			//"External Markers." You then need to pass back a pointer to your 
			//IVsPackageDefinedTextMarkerType implementation for this marker type.
			
			ppMarkerType = this;
			
			return 0;
		}

		#endregion

		#region IVsPackageDefinedTextMarkerType Members

		public int DrawGlyphWithColors(IntPtr hdc, RECT[] pRect, int iMarkerType, IVsTextMarkerColorSet pMarkerColors, uint dwGlyphDrawFlags, int iLineHeight)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetBehaviorFlags(out uint pdwFlags)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetDefaultColors(COLORINDEX[] piForeground, COLORINDEX[] piBackground)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetDefaultFontFlags(out uint pdwFontFlags)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetDefaultLineStyle(COLORINDEX[] piLineColor, LINESTYLE[] piLineIndex)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetPriorityIndex(out int piPriorityIndex)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public int GetVisualStyle(out uint pdwVisualFlags)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		#endregion
	};

}