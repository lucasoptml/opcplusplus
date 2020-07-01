namespace opGamesLLC.opCpp2005
{
    partial class UnlockForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UnlockForm));
			this.btnUnlock = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.lblName = new System.Windows.Forms.Label();
			this.lblLicenseId = new System.Windows.Forms.Label();
			this.lblSerial = new System.Windows.Forms.Label();
			this.txtName = new System.Windows.Forms.TextBox();
			this.txtLicenseId = new System.Windows.Forms.TextBox();
			this.txtSerial = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// btnUnlock
			// 
			this.btnUnlock.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnUnlock.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnUnlock.Location = new System.Drawing.Point(279, 90);
			this.btnUnlock.Name = "btnUnlock";
			this.btnUnlock.Size = new System.Drawing.Size(75, 23);
			this.btnUnlock.TabIndex = 0;
			this.btnUnlock.Text = "Unlock";
			this.btnUnlock.UseVisualStyleBackColor = true;
			this.btnUnlock.Click += new System.EventHandler(this.btnUnlock_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(360, 90);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(75, 23);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// lblName
			// 
			this.lblName.AutoSize = true;
			this.lblName.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblName.Location = new System.Drawing.Point(12, 12);
			this.lblName.Name = "lblName";
			this.lblName.Size = new System.Drawing.Size(39, 13);
			this.lblName.TabIndex = 2;
			this.lblName.Text = "Name";
			// 
			// lblLicenseId
			// 
			this.lblLicenseId.AutoSize = true;
			this.lblLicenseId.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblLicenseId.Location = new System.Drawing.Point(12, 39);
			this.lblLicenseId.Name = "lblLicenseId";
			this.lblLicenseId.Size = new System.Drawing.Size(87, 13);
			this.lblLicenseId.TabIndex = 3;
			this.lblLicenseId.Text = "License Id";
			// 
			// lblSerial
			// 
			this.lblSerial.AutoSize = true;
			this.lblSerial.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblSerial.Location = new System.Drawing.Point(12, 66);
			this.lblSerial.Name = "lblSerial";
			this.lblSerial.Size = new System.Drawing.Size(55, 13);
			this.lblSerial.TabIndex = 4;
			this.lblSerial.Text = "Serial";
			// 
			// txtName
			// 
			this.txtName.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtName.Location = new System.Drawing.Point(108, 5);
			this.txtName.Name = "txtName";
			this.txtName.Size = new System.Drawing.Size(327, 20);
			this.txtName.TabIndex = 5;
			// 
			// txtLicenseId
			// 
			this.txtLicenseId.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtLicenseId.Location = new System.Drawing.Point(108, 32);
			this.txtLicenseId.Name = "txtLicenseId";
			this.txtLicenseId.Size = new System.Drawing.Size(327, 20);
			this.txtLicenseId.TabIndex = 6;
			// 
			// txtSerial
			// 
			this.txtSerial.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtSerial.Location = new System.Drawing.Point(108, 59);
			this.txtSerial.Name = "txtSerial";
			this.txtSerial.Size = new System.Drawing.Size(327, 20);
			this.txtSerial.TabIndex = 7;
			// 
			// UnlockForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Control;
			this.ClientSize = new System.Drawing.Size(447, 125);
			this.Controls.Add(this.txtSerial);
			this.Controls.Add(this.txtLicenseId);
			this.Controls.Add(this.txtName);
			this.Controls.Add(this.lblSerial);
			this.Controls.Add(this.lblLicenseId);
			this.Controls.Add(this.lblName);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnUnlock);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "UnlockForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Unlock opC++ Addin";
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnUnlock;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblLicenseId;
        private System.Windows.Forms.Label lblSerial;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtLicenseId;
        private System.Windows.Forms.TextBox txtSerial;
    }
}