namespace Field3D
{
	partial class FormProgress
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
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// progressBar1
			// 
			this.progressBar1.Dock = System.Windows.Forms.DockStyle.Top;
			this.progressBar1.Location = new System.Drawing.Point(0, 0);
			this.progressBar1.Name = "progressBar1";
			this.progressBar1.Size = new System.Drawing.Size(508, 35);
			this.progressBar1.TabIndex = 0;
			this.progressBar1.Click += new System.EventHandler(this.progressBar1_Click);
			// 
			// label1
			// 
			this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.label1.Location = new System.Drawing.Point(0, 38);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(508, 26);
			this.label1.TabIndex = 1;
			this.label1.Text = "loading ...";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// FormProgress
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(508, 64);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.progressBar1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "FormProgress";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "FormProgress";
			this.TopMost = true;
			this.Load += new System.EventHandler(this.FormProgress_Load);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.Label label1;
	}
}