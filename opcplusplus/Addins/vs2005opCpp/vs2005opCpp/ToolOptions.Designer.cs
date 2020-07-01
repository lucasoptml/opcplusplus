namespace vs2005opCpp
{

	partial class OptionsPage
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.opCppPath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.Arguments = new System.Windows.Forms.TextBox();
            this.FindExeButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // opCppPath
            // 
            this.opCppPath.Location = new System.Drawing.Point(20, 30);
            this.opCppPath.Name = "opCppPath";
            this.opCppPath.Size = new System.Drawing.Size(361, 20);
            this.opCppPath.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(119, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "opCpp Executable Path";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(124, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "opCpp Global Arguments";
            // 
            // Arguments
            // 
            this.Arguments.Location = new System.Drawing.Point(20, 69);
            this.Arguments.Name = "Arguments";
            this.Arguments.Size = new System.Drawing.Size(361, 20);
            this.Arguments.TabIndex = 3;
            // 
            // FindExeButton
            // 
            this.FindExeButton.Location = new System.Drawing.Point(387, 27);
            this.FindExeButton.Name = "FindExeButton";
            this.FindExeButton.Size = new System.Drawing.Size(30, 23);
            this.FindExeButton.TabIndex = 4;
            this.FindExeButton.Text = "...";
            this.FindExeButton.UseVisualStyleBackColor = true;
            this.FindExeButton.Click += new System.EventHandler(this.FindExeButton_Click);
            // 
            // OptionsPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.FindExeButton);
            this.Controls.Add(this.Arguments);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.opCppPath);
            this.Name = "OptionsPage";
            this.Size = new System.Drawing.Size(424, 170);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox opCppPath;
		private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox Arguments;
        private System.Windows.Forms.Button FindExeButton;
	}
	 */
}
