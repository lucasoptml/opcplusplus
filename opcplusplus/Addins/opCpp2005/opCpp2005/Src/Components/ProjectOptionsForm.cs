///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: ProjectOptionsForm.cs
/// Date: 09/25/2007
///
/// Description:
///
/// The form for the project options.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EnvDTE;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// ProjectOptionsForm
    ///==========================================
    
    // Form class for project options.
    public partial class ProjectOptionsForm : Form
    {
        /*=== construction ===*/

        public ProjectOptionsForm()
        {
            InitializeComponent();

            /*=== Grab the current project. ===*/

            List<Project> projects = ProjectUtility.GetActiveProjectOnly();
            Project       project  = projects[0];

            this.Text = project.Name + " : " + this.Text;

            /*=== Populate the combo boxes. ===*/
            
            List<string> configurations = ProjectUtility.GetConfigurations(project);
            List<string> platforms      = ProjectUtility.GetPlatforms(project);

            foreach (string s in configurations)
                comboConfigurations.Items.Add(s);

            foreach (string s in platforms)
                comboPlatforms.Items.Add(s);

            comboConfigurations.SelectedItem = ProjectUtility.GetActiveConfiguration(project);
            comboPlatforms.SelectedItem      = ProjectUtility.GetActivePlatform(project);
            
            /*=== Setup the property grid. ===*/           
            
            this.propertyGrid.Initialize(project);
            this.propertyGrid.RefreshGrid((string) comboConfigurations.SelectedItem, (string) comboPlatforms.SelectedItem);
        }

        /*=== events ===*/

        // When Ok button is clicked, save the project options.
        private void btnOk_Click(object sender, EventArgs e)
        {
            this.propertyGrid.Save();
        }

        // Fired when the configuration is changed.
        private void comboConfigurations_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.propertyGrid.RefreshGrid((string) comboConfigurations.SelectedItem, (string) comboPlatforms.SelectedItem);
        }

        // Fired when the platform is changed.
        private void comboPlatforms_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.propertyGrid.RefreshGrid((string) comboConfigurations.SelectedItem, (string) comboPlatforms.SelectedItem);
        }

        // Fired when the defaults button is clicked.
        private void btnDefaults_Click(object sender, EventArgs e)
        {
            propertyGrid.Defaults();
            propertyGrid.RefreshGrid((string) comboConfigurations.SelectedItem, (string) comboPlatforms.SelectedItem);
        }
    }
}