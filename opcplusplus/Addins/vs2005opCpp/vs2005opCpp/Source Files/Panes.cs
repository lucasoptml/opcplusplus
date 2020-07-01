///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
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

namespace vs2005opCpp
{
    public partial class Connect
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

        // Gets and caches the opcpp build pane.
        OutputWindowPane GetOutputPane()
        {
            if (OutputPane == null)
                OutputPane = FindOutputPane("opCpp Build");

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
                DebugOutputPane = FindOutputPane("opCpp Debug Output");

            return DebugOutputPane;
        }
    }
}
 