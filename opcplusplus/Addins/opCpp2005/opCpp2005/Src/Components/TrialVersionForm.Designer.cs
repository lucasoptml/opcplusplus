namespace opGamesLLC.opCpp2005
{
    partial class TrialVersionForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TrialVersionForm));
			this.btnOk = new System.Windows.Forms.Button();
			this.lnkWebsite = new System.Windows.Forms.LinkLabel();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.txt1 = new System.Windows.Forms.TextBox();
			this.txtRunsRemaining = new System.Windows.Forms.TextBox();
			this.txt3 = new System.Windows.Forms.TextBox();
			this.btnUnlock = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.SuspendLayout();
			// 
			// btnOk
			// 
			this.btnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOk.Location = new System.Drawing.Point(541, 129);
			this.btnOk.Name = "btnOk";
			this.btnOk.Size = new System.Drawing.Size(75, 23);
			this.btnOk.TabIndex = 1;
			this.btnOk.Text = "Ok";
			this.btnOk.UseVisualStyleBackColor = true;
			// 
			// lnkWebsite
			// 
			this.lnkWebsite.AutoSize = true;
			this.lnkWebsite.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lnkWebsite.Location = new System.Drawing.Point(18, 130);
			this.lnkWebsite.Name = "lnkWebsite";
			this.lnkWebsite.Size = new System.Drawing.Size(111, 13);
			this.lnkWebsite.TabIndex = 8;
			this.lnkWebsite.TabStop = true;
			this.lnkWebsite.Text = "www.opcpp.com";
			this.lnkWebsite.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkWebsite_LinkClicked);
			// 
			// pictureBox1
			// 
			this.pictureBox1.Image = global::opGamesLLC.opCpp2005.MyResources.addin_small;
			this.pictureBox1.Location = new System.Drawing.Point(12, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(121, 114);
			this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.pictureBox1.TabIndex = 9;
			this.pictureBox1.TabStop = false;
			// 
			// txt1
			// 
			this.txt1.BackColor = System.Drawing.SystemColors.Control;
			this.txt1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txt1.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txt1.Location = new System.Drawing.Point(138, 55);
			this.txt1.Multiline = true;
			this.txt1.Name = "txt1";
			this.txt1.ReadOnly = true;
			this.txt1.Size = new System.Drawing.Size(436, 33);
			this.txt1.TabIndex = 10;
			this.txt1.Text = "You are currently running the trial version of opC++";
			// 
			// txtRunsRemaining
			// 
			this.txtRunsRemaining.BackColor = System.Drawing.SystemColors.Control;
			this.txtRunsRemaining.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txtRunsRemaining.Font = new System.Drawing.Font("Lucida Console", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtRunsRemaining.Location = new System.Drawing.Point(412, 12);
			this.txtRunsRemaining.Name = "txtRunsRemaining";
			this.txtRunsRemaining.ReadOnly = true;
			this.txtRunsRemaining.Size = new System.Drawing.Size(52, 27);
			this.txtRunsRemaining.TabIndex = 11;
			this.txtRunsRemaining.Text = "#";
			// 
			// txt3
			// 
			this.txt3.BackColor = System.Drawing.SystemColors.Control;
			this.txt3.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txt3.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txt3.Location = new System.Drawing.Point(138, 92);
			this.txt3.Multiline = true;
			this.txt3.Name = "txt3";
			this.txt3.ReadOnly = true;
			this.txt3.Size = new System.Drawing.Size(478, 34);
			this.txt3.TabIndex = 13;
			this.txt3.Text = "To unlock, please purchase a license and enter it into the unlock dialog below.";
			// 
			// btnUnlock
			// 
			this.btnUnlock.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnUnlock.Location = new System.Drawing.Point(460, 129);
			this.btnUnlock.Name = "btnUnlock";
			this.btnUnlock.Size = new System.Drawing.Size(75, 23);
			this.btnUnlock.TabIndex = 14;
			this.btnUnlock.Text = "Unlock";
			this.btnUnlock.UseVisualStyleBackColor = true;
			this.btnUnlock.Click += new System.EventHandler(this.btnUnlock_Click);
			// 
			// textBox1
			// 
			this.textBox1.BackColor = System.Drawing.SystemColors.Control;
			this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.textBox1.Font = new System.Drawing.Font("Lucida Console", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.textBox1.Location = new System.Drawing.Point(139, 12);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.Size = new System.Drawing.Size(343, 42);
			this.textBox1.TabIndex = 15;
			this.textBox1.Text = "Runs Remaining : ";
			// 
			// TrialVersionForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Control;
			this.ClientSize = new System.Drawing.Size(628, 164);
			this.Controls.Add(this.btnOk);
			this.Controls.Add(this.btnUnlock);
			this.Controls.Add(this.txt3);
			this.Controls.Add(this.txtRunsRemaining);
			this.Controls.Add(this.txt1);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.lnkWebsite);
			this.Controls.Add(this.textBox1);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TrialVersionForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Trial Version";
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.LinkLabel lnkWebsite;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TextBox txt1;
		private System.Windows.Forms.TextBox txtRunsRemaining;
        private System.Windows.Forms.TextBox txt3;
        private System.Windows.Forms.Button btnUnlock;
		private System.Windows.Forms.TextBox textBox1;
    }
}