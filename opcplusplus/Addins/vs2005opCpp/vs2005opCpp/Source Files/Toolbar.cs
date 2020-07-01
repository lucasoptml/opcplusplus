///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Toolbar.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Code for manipulating the toolbar.
///****************************************************************

using System;
using EnvDTE;
using Microsoft.VisualStudio.CommandBars;
using System.Windows.Forms;

namespace vs2005opCpp
{
    public partial class Connect
    {
        // This method creates the a new visual studio command bar object.
        CommandBar CreateCommandBar(string name, MsoBarPosition position)
        {
            CommandBars cbars = (CommandBars) App().CommandBars;

            try
            {
                //this adds the toolbar.
                CommandBar toolbar = cbars.Add("opCppToolbar", MsoBarPosition.msoBarTop, System.Type.Missing, false);
                return toolbar;
            }
            catch (System.Exception)
            {
                //find existing one?
                CommandBar toolbar = cbars["opCppToolbar"];
                return toolbar;
            }
        }

        // This method adds a button to the visual studio command bar.
        private void AddButton(CommandBar toolbar, String commandname, String captionname, int iconindex)
        {
            Command newCommand = null;

            try
            {
                newCommand = FindCommand(commandname);
            }
            catch (System.Exception)
            {
                newCommand = CreateCommand(commandname, captionname, iconindex);
            }

            //see if the button exists already, create it if it doesn't
            //this does have the intended effect!
            foreach (CommandBarControl c in toolbar.Controls)
            {
                if (c.Caption == captionname)
                {
                    return;
                }
            }

            //doesn't exist, create it
            CommandBarControl control = (CommandBarControl)newCommand.AddControl(toolbar, 1);
            CommandBarButton button = (CommandBarButton)control;

            button.Caption = captionname;
            button.Style = MsoButtonStyle.msoButtonIcon;
        }

        // This method creates the toolbar.
        public void CreateToolbar()
        {
            CommandBar toolbar = CreateCommandBar("opCppToolbar", MsoBarPosition.msoBarTop);

            toolbar.Visible = true;

            //add all the buttons/commands
            AddButton(toolbar, "buildsolution", "build solution", 1);
            AddButton(toolbar, "buildproject",  "build project",  2);
            AddButton(toolbar, "cleansolution", "clean solution", 3);

            try
            {
                debugstream = new System.IO.StreamWriter("c:\\spheroid\\opcpp\\addins\\debugopcpp.txt");
            }
            catch (Exception) {}
        }

        // This method closes the toolbar.
        public void CloseToolbar()
        {
            CommandBars cbars = (CommandBars) _applicationObject.CommandBars;

            try
            {
                cbars["opCppToolbar"].Delete();
            }
            catch (Exception) {}
        }
    }
}