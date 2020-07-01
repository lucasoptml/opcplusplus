namespace opGamesLLC.opCpp2005
{
    partial class AddinLockedForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddinLockedForm));
			this.btnOk = new System.Windows.Forms.Button();
			this.btnUnlock = new System.Windows.Forms.Button();
			this.lnkWebsite = new System.Windows.Forms.LinkLabel();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.picLogo = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picLogo)).BeginInit();
			this.SuspendLayout();
			// 
			// btnOk
			// 
			this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOk.Location = new System.Drawing.Point(430, 105);
			this.btnOk.Name = "btnOk";
			this.btnOk.Size = new System.Drawing.Size(75, 23);
			this.btnOk.TabIndex = 2;
			this.btnOk.Text = "Ok";
			this.btnOk.UseVisualStyleBackColor = true;
			// 
			// btnUnlock
			// 
			this.btnUnlock.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnUnlock.Location = new System.Drawing.Point(349, 105);
			this.btnUnlock.Name = "btnUnlock";
			this.btnUnlock.Size = new System.Drawing.Size(75, 23);
			this.btnUnlock.TabIndex = 3;
			this.btnUnlock.Text = "Unlock";
			this.btnUnlock.UseVisualStyleBackColor = true;
			this.btnUnlock.Click += new System.EventHandler(this.btnUnlock_Click);
			// 
			// lnkWebsite
			// 
			this.lnkWebsite.AutoSize = true;
			this.lnkWebsite.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lnkWebsite.Location = new System.Drawing.Point(139, 111);
			this.lnkWebsite.Name = "lnkWebsite";
			this.lnkWebsite.Size = new System.Drawing.Size(111, 13);
			this.lnkWebsite.TabIndex = 9;
			this.lnkWebsite.TabStop = true;
			this.lnkWebsite.Text = "www.opcpp.com";
			this.lnkWebsite.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkWebsite_LinkClicked);
			// 
			// textBox1
			// 
			this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.textBox1.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.textBox1.Location = new System.Drawing.Point(142, 12);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.Size = new System.Drawing.Size(364, 63);
			this.textBox1.TabIndex = 10;
			this.textBox1.Text = "The trial period for the opC++ Addin has expired.  To unlock, please purchase a l" +
				"icense key and enter it into the unlock dialog below.";
			// 
			// picLogo
			// 
			this.picLogo.Image = global::opGamesLLC.opCpp2005.MyResources.addin_small;
			this.picLogo.Location = new System.Drawing.Point(12, 12);
			this.picLogo.Name = "picLogo";
			this.picLogo.Size = new System.Drawing.Size(121, 114);
			this.picLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picLogo.TabIndex = 0;
			this.picLogo.TabStop = false;
			// 
			// AddinLockedForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(517, 140);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.lnkWebsite);
			this.Controls.Add(this.btnUnlock);
			this.Controls.Add(this.btnOk);
			this.Controls.Add(this.picLogo);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AddinLockedForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Addin Locked";
			((System.ComponentModel.ISupportInitialize)(this.picLogo)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox picLogo;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnUnlock;
        private System.Windows.Forms.LinkLabel lnkWebsite;
        private System.Windows.Forms.TextBox textBox1;
    }
}