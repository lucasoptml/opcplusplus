///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Panes.cs
/// Date: 09/19/2007
///
/// Description:
///
/// Output pane code.
///****************************************************************

using System;
using EnvDTE;
using EnvDTE80;

namespace opGamesLLC.opCpp2005
{
	public partial class opCpp2005
	{
        /*=== data ===*/

        private OutputWindowPane DebugOutputPane;
        private OutputWindowPane OutputPane;
        private OutputWindowPane BuildOutputPane;

        /*=== utility ===*/

        // Looks for an output pane by name.
        OutputWindowPane FindOutputPane(string name)
        {
            OutputWindow outputwindow = App().ToolWindows.OutputWindow;

            foreach (OutputWindowPane pane in outputwindow.OutputWindowPanes)
            {
                if (pane.Name == name)
                    return pane;
            }

            return outputwindow.OutputWindowPanes.Add(name);
        }

		void ActivateOutputPane()
		{

			Window window = App().Windows.Item(ContextGuids.vsContextGuidOutput);

			if (window != null)
				window.Activate();

			//this brings up the pane, but not the window
			GetOutputPane().Activate();


		}

        // Gets and caches the opcpp build pane.
        OutputWindowPane GetOutputPane()
        {
            if (OutputPane == null)
                OutputPane = FindOutputPane("opC++ Build");

            return OutputPane;
        }

        // Get build output pane.
        OutputWindowPane GetBuildOutputPane()
        {
            if (BuildOutputPane == null)
                BuildOutputPane = FindOutputPane("Build");

            return BuildOutputPane;
        }

        // Get debug output pane.
        OutputWindowPane GetDebugOutputPane()
        {
            if (DebugOutputPane == null)
                DebugOutputPane = FindOutputPane("opC++ Debug Output");

            return DebugOutputPane;
        }
    }
}
 