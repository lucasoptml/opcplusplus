namespace opGamesLLC.opCpp2005
{
    partial class AboutForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutForm));
			this.btnOk = new System.Windows.Forms.Button();
			this.lnkWebsite = new System.Windows.Forms.LinkLabel();
			this.lblLicense = new System.Windows.Forms.Label();
			this.txtLicense = new System.Windows.Forms.TextBox();
			this.lblCompilerVersion = new System.Windows.Forms.Label();
			this.txtCompilerVersion = new System.Windows.Forms.TextBox();
			this.lblAddinVersion = new System.Windows.Forms.Label();
			this.txtAddinVersion = new System.Windows.Forms.TextBox();
			this.picLogo = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picLogo)).BeginInit();
			this.SuspendLayout();
			// 
			// btnOk
			// 
			this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOk.Location = new System.Drawing.Point(775, 284);
			this.btnOk.Name = "btnOk";
			this.btnOk.Size = new System.Drawing.Size(75, 23);
			this.btnOk.TabIndex = 0;
			this.btnOk.Text = "Ok";
			this.btnOk.UseVisualStyleBackColor = true;
			// 
			// lnkWebsite
			// 
			this.lnkWebsite.AutoSize = true;
			this.lnkWebsite.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lnkWebsite.Location = new System.Drawing.Point(37, 235);
			this.lnkWebsite.Name = "lnkWebsite";
			this.lnkWebsite.Size = new System.Drawing.Size(167, 13);
			this.lnkWebsite.TabIndex = 4;
			this.lnkWebsite.TabStop = true;
			this.lnkWebsite.Text = "http://www.opcpp.com";
			this.lnkWebsite.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkWebsite_LinkClicked);
			// 
			// lblLicense
			// 
			this.lblLicense.AutoSize = true;
			this.lblLicense.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblLicense.Location = new System.Drawing.Point(241, 8);
			this.lblLicense.Name = "lblLicense";
			this.lblLicense.Size = new System.Drawing.Size(63, 13);
			this.lblLicense.TabIndex = 7;
			this.lblLicense.Text = "License";
			// 
			// txtLicense
			// 
			this.txtLicense.BackColor = System.Drawing.Color.White;
			this.txtLicense.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtLicense.Location = new System.Drawing.Point(241, 24);
			this.txtLicense.Multiline = true;
			this.txtLicense.Name = "txtLicense";
			this.txtLicense.ReadOnly = true;
			this.txtLicense.Size = new System.Drawing.Size(609, 67);
			this.txtLicense.TabIndex = 8;
			// 
			// lblCompilerVersion
			// 
			this.lblCompilerVersion.AutoSize = true;
			this.lblCompilerVersion.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblCompilerVersion.Location = new System.Drawing.Point(241, 96);
			this.lblCompilerVersion.Name = "lblCompilerVersion";
			this.lblCompilerVersion.Size = new System.Drawing.Size(135, 13);
			this.lblCompilerVersion.TabIndex = 10;
			this.lblCompilerVersion.Text = "Compiler Version";
			// 
			// txtCompilerVersion
			// 
			this.txtCompilerVersion.BackColor = System.Drawing.Color.White;
			this.txtCompilerVersion.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtCompilerVersion.Location = new System.Drawing.Point(241, 112);
			this.txtCompilerVersion.Multiline = true;
			this.txtCompilerVersion.Name = "txtCompilerVersion";
			this.txtCompilerVersion.ReadOnly = true;
			this.txtCompilerVersion.Size = new System.Drawing.Size(609, 72);
			this.txtCompilerVersion.TabIndex = 11;
			// 
			// lblAddinVersion
			// 
			this.lblAddinVersion.AutoSize = true;
			this.lblAddinVersion.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblAddinVersion.Location = new System.Drawing.Point(241, 190);
			this.lblAddinVersion.Name = "lblAddinVersion";
			this.lblAddinVersion.Size = new System.Drawing.Size(111, 13);
			this.lblAddinVersion.TabIndex = 12;
			this.lblAddinVersion.Text = "Addin Version";
			// 
			// txtAddinVersion
			// 
			this.txtAddinVersion.BackColor = System.Drawing.Color.White;
			this.txtAddinVersion.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtAddinVersion.Location = new System.Drawing.Point(241, 206);
			this.txtAddinVersion.Multiline = true;
			this.txtAddinVersion.Name = "txtAddinVersion";
			this.txtAddinVersion.ReadOnly = true;
			this.txtAddinVersion.Size = new System.Drawing.Size(609, 67);
			this.txtAddinVersion.TabIndex = 13;
			// 
			// picLogo
			// 
			this.picLogo.Image = global::opGamesLLC.opCpp2005.MyResources.addin_logo;
			this.picLogo.Location = new System.Drawing.Point(12, 24);
			this.picLogo.Name = "picLogo";
			this.picLogo.Size = new System.Drawing.Size(220, 208);
			this.picLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picLogo.TabIndex = 9;
			this.picLogo.TabStop = false;
			// 
			// AboutForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(862, 319);
			this.Controls.Add(this.txtAddinVersion);
			this.Controls.Add(this.lblAddinVersion);
			this.Controls.Add(this.txtCompilerVersion);
			this.Controls.Add(this.lblCompilerVersion);
			this.Controls.Add(this.picLogo);
			this.Controls.Add(this.txtLicense);
			this.Controls.Add(this.lblLicense);
			this.Controls.Add(this.lnkWebsite);
			this.Controls.Add(this.btnOk);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AboutForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About opC++";
			((System.ComponentModel.ISupportInitialize)(this.picLogo)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.LinkLabel lnkWebsite;
        private System.Windows.Forms.Label lblLicense;
        private System.Windows.Forms.TextBox txtLicense;
        private System.Windows.Forms.PictureBox picLogo;
        private System.Windows.Forms.Label lblCompilerVersion;
        private System.Windows.Forms.TextBox txtCompilerVersion;
        private System.Windows.Forms.Label lblAddinVersion;
        private System.Windows.Forms.TextBox txtAddinVersion;
    }
}