namespace Field3D
{
	partial class FormDataFilesControl
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
			this.label1 = new System.Windows.Forms.Label();
			this.nupTimeStepIndex = new System.Windows.Forms.NumericUpDown();
			this.hsbLengthZoom = new System.Windows.Forms.HScrollBar();
			this.hsbLocationZoom = new System.Windows.Forms.HScrollBar();
			this.txtLengthZoom = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.txtSpaceZoom = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.nupTimeStepIndex)).BeginInit();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(84, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Time step index:";
			// 
			// nupTimeStepIndex
			// 
			this.nupTimeStepIndex.Location = new System.Drawing.Point(102, 7);
			this.nupTimeStepIndex.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
			this.nupTimeStepIndex.Name = "nupTimeStepIndex";
			this.nupTimeStepIndex.Size = new System.Drawing.Size(144, 20);
			this.nupTimeStepIndex.TabIndex = 2;
			this.nupTimeStepIndex.ValueChanged += new System.EventHandler(this.nupTimeStepIndex_ValueChanged);
			// 
			// hsbLengthZoom
			// 
			this.hsbLengthZoom.Location = new System.Drawing.Point(16, 98);
			this.hsbLengthZoom.Name = "hsbLengthZoom";
			this.hsbLengthZoom.Size = new System.Drawing.Size(230, 10);
			this.hsbLengthZoom.TabIndex = 31;
			this.hsbLengthZoom.Value = 50;
			this.hsbLengthZoom.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbLengthZoom_Scroll);
			// 
			// hsbLocationZoom
			// 
			this.hsbLocationZoom.Location = new System.Drawing.Point(16, 56);
			this.hsbLocationZoom.Name = "hsbLocationZoom";
			this.hsbLocationZoom.Size = new System.Drawing.Size(230, 10);
			this.hsbLocationZoom.TabIndex = 30;
			this.hsbLocationZoom.Value = 50;
			this.hsbLocationZoom.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbLocationZoom_Scroll);
			// 
			// txtLengthZoom
			// 
			this.txtLengthZoom.Location = new System.Drawing.Point(206, 75);
			this.txtLengthZoom.Name = "txtLengthZoom";
			this.txtLengthZoom.Size = new System.Drawing.Size(40, 20);
			this.txtLengthZoom.TabIndex = 29;
			this.txtLengthZoom.Text = "50";
			this.txtLengthZoom.TextChanged += new System.EventHandler(this.txtLengthZoom_TextChanged);
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(13, 78);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(166, 13);
			this.label5.TabIndex = 28;
			this.label5.Text = "Vector length view-size factor (%):";
			// 
			// txtSpaceZoom
			// 
			this.txtSpaceZoom.Location = new System.Drawing.Point(206, 33);
			this.txtSpaceZoom.Name = "txtSpaceZoom";
			this.txtSpaceZoom.Size = new System.Drawing.Size(40, 20);
			this.txtSpaceZoom.TabIndex = 27;
			this.txtSpaceZoom.Text = "50";
			this.txtSpaceZoom.TextChanged += new System.EventHandler(this.txtSpaceZoom_TextChanged);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(13, 36);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(177, 13);
			this.label4.TabIndex = 26;
			this.label4.Text = "Vector location view-size factor (%): ";
			// 
			// FormDataFilesControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(262, 119);
			this.ControlBox = false;
			this.Controls.Add(this.hsbLengthZoom);
			this.Controls.Add(this.hsbLocationZoom);
			this.Controls.Add(this.txtLengthZoom);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.txtSpaceZoom);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.nupTimeStepIndex);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "FormDataFilesControl";
			this.Opacity = 0.5D;
			this.ShowInTaskbar = false;
			this.Text = "EM Data Files Control";
			((System.ComponentModel.ISupportInitialize)(this.nupTimeStepIndex)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.NumericUpDown nupTimeStepIndex;
		private System.Windows.Forms.HScrollBar hsbLengthZoom;
		private System.Windows.Forms.HScrollBar hsbLocationZoom;
		private System.Windows.Forms.TextBox txtLengthZoom;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox txtSpaceZoom;
		private System.Windows.Forms.Label label4;
	}
}