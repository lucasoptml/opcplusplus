namespace opGamesLLC.opCpp2005
{
    partial class ProjectOptionsForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProjectOptionsForm));
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOk = new System.Windows.Forms.Button();
            this.lblConfiguration = new System.Windows.Forms.Label();
            this.lblPlatform = new System.Windows.Forms.Label();
            this.comboConfigurations = new System.Windows.Forms.ComboBox();
            this.comboPlatforms = new System.Windows.Forms.ComboBox();
            this.btnDefaults = new System.Windows.Forms.Button();
            this.propertyGrid = new opGamesLLC.opCpp2005.ProjectOptionsPropertyGrid();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(545, 442);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 0;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOk
            // 
            this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOk.Location = new System.Drawing.Point(464, 442);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 1;
            this.btnOk.Text = "Ok";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // lblConfiguration
            // 
            this.lblConfiguration.AutoSize = true;
            this.lblConfiguration.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.lblConfiguration.Location = new System.Drawing.Point(12, 9);
            this.lblConfiguration.Name = "lblConfiguration";
            this.lblConfiguration.Size = new System.Drawing.Size(111, 13);
            this.lblConfiguration.TabIndex = 3;
            this.lblConfiguration.Text = "Configuration";
            // 
            // lblPlatform
            // 
            this.lblPlatform.AutoSize = true;
            this.lblPlatform.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.lblPlatform.Location = new System.Drawing.Point(328, 9);
            this.lblPlatform.Name = "lblPlatform";
            this.lblPlatform.Size = new System.Drawing.Size(71, 13);
            this.lblPlatform.TabIndex = 4;
            this.lblPlatform.Text = "Platform";
            // 
            // comboConfigurations
            // 
            this.comboConfigurations.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboConfigurations.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.comboConfigurations.Location = new System.Drawing.Point(129, 5);
            this.comboConfigurations.MaxDropDownItems = 10;
            this.comboConfigurations.Name = "comboConfigurations";
            this.comboConfigurations.Size = new System.Drawing.Size(190, 21);
            this.comboConfigurations.Sorted = true;
            this.comboConfigurations.TabIndex = 5;
            this.comboConfigurations.SelectedIndexChanged += new System.EventHandler(this.comboConfigurations_SelectedIndexChanged);
            // 
            // comboPlatforms
            // 
            this.comboPlatforms.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboPlatforms.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.comboPlatforms.Location = new System.Drawing.Point(405, 5);
            this.comboPlatforms.MaxDropDownItems = 10;
            this.comboPlatforms.Name = "comboPlatforms";
            this.comboPlatforms.Size = new System.Drawing.Size(190, 21);
            this.comboPlatforms.Sorted = true;
            this.comboPlatforms.TabIndex = 6;
            this.comboPlatforms.SelectedIndexChanged += new System.EventHandler(this.comboPlatforms_SelectedIndexChanged);
            // 
            // btnDefaults
            // 
            this.btnDefaults.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnDefaults.Location = new System.Drawing.Point(12, 442);
            this.btnDefaults.Name = "btnDefaults";
            this.btnDefaults.Size = new System.Drawing.Size(75, 23);
            this.btnDefaults.TabIndex = 7;
            this.btnDefaults.Text = "Defaults";
            this.btnDefaults.UseVisualStyleBackColor = true;
            this.btnDefaults.Click += new System.EventHandler(this.btnDefaults_Click);
            // 
            // propertyGrid
            // 
            this.propertyGrid.Anchor = ((System.Windows.Forms.AnchorStyles) ((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGrid.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.propertyGrid.Location = new System.Drawing.Point(12, 35);
            this.propertyGrid.Name = "propertyGrid";
            this.propertyGrid.Size = new System.Drawing.Size(608, 401);
            this.propertyGrid.TabIndex = 2;
            // 
            // ProjectOptionsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(632, 477);
            this.Controls.Add(this.btnDefaults);
            this.Controls.Add(this.comboPlatforms);
            this.Controls.Add(this.comboConfigurations);
            this.Controls.Add(this.lblPlatform);
            this.Controls.Add(this.lblConfiguration);
            this.Controls.Add(this.propertyGrid);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.btnCancel);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon) (resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProjectOptionsForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "opC++ Project Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOk;
        private ProjectOptionsPropertyGrid propertyGrid;
        private System.Windows.Forms.Label lblConfiguration;
        private System.Windows.Forms.Label lblPlatform;
        private System.Windows.Forms.ComboBox comboConfigurations;
        private System.Windows.Forms.ComboBox comboPlatforms;
        private System.Windows.Forms.Button btnDefaults;
    }
}