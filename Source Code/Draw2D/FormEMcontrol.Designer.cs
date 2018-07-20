namespace Draw2D
{
	partial class FormEMcontrol
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
			this.nudTime = new System.Windows.Forms.NumericUpDown();
			this.vs1 = new System.Windows.Forms.VScrollBar();
			this.vs2 = new System.Windows.Forms.VScrollBar();
			this.txtScale1 = new System.Windows.Forms.TextBox();
			this.txtScale2 = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.txtExcludeR = new System.Windows.Forms.TextBox();
			((System.ComponentModel.ISupportInitialize)(this.nudTime)).BeginInit();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(61, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Time index:";
			// 
			// nudTime
			// 
			this.nudTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.nudTime.Location = new System.Drawing.Point(79, 2);
			this.nudTime.Name = "nudTime";
			this.nudTime.Size = new System.Drawing.Size(56, 26);
			this.nudTime.TabIndex = 1;
			this.nudTime.ValueChanged += new System.EventHandler(this.nudTime_ValueChanged);
			// 
			// vs1
			// 
			this.vs1.Location = new System.Drawing.Point(15, 105);
			this.vs1.Maximum = 1000;
			this.vs1.Minimum = 1;
			this.vs1.Name = "vs1";
			this.vs1.Size = new System.Drawing.Size(56, 111);
			this.vs1.TabIndex = 2;
			this.vs1.Value = 1;
			this.vs1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vs1_Scroll);
			// 
			// vs2
			// 
			this.vs2.Location = new System.Drawing.Point(79, 105);
			this.vs2.Maximum = 1000;
			this.vs2.Minimum = 1;
			this.vs2.Name = "vs2";
			this.vs2.Size = new System.Drawing.Size(56, 111);
			this.vs2.TabIndex = 3;
			this.vs2.Value = 1;
			this.vs2.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vs2_Scroll);
			// 
			// txtScale1
			// 
			this.txtScale1.Location = new System.Drawing.Point(15, 82);
			this.txtScale1.Name = "txtScale1";
			this.txtScale1.Size = new System.Drawing.Size(56, 20);
			this.txtScale1.TabIndex = 4;
			this.txtScale1.Text = "100";
			this.txtScale1.TextChanged += new System.EventHandler(this.txtScale1_TextChanged);
			// 
			// txtScale2
			// 
			this.txtScale2.Location = new System.Drawing.Point(79, 82);
			this.txtScale2.Name = "txtScale2";
			this.txtScale2.Size = new System.Drawing.Size(56, 20);
			this.txtScale2.TabIndex = 5;
			this.txtScale2.Text = "100";
			this.txtScale2.TextChanged += new System.EventHandler(this.txtScale2_TextChanged);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 66);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(46, 13);
			this.label2.TabIndex = 6;
			this.label2.Text = "Scale 1:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(76, 66);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(46, 13);
			this.label3.TabIndex = 7;
			this.label3.Text = "Scale 2:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 38);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(79, 13);
			this.label4.TabIndex = 8;
			this.label4.Text = "Exclude radius:";
			// 
			// txtExcludeR
			// 
			this.txtExcludeR.Location = new System.Drawing.Point(93, 31);
			this.txtExcludeR.Name = "txtExcludeR";
			this.txtExcludeR.Size = new System.Drawing.Size(42, 20);
			this.txtExcludeR.TabIndex = 9;
			this.txtExcludeR.Text = "0";
			this.txtExcludeR.TextChanged += new System.EventHandler(this.txtExcludeR_TextChanged);
			// 
			// FormEMcontrol
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(146, 221);
			this.Controls.Add(this.txtExcludeR);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.txtScale2);
			this.Controls.Add(this.txtScale1);
			this.Controls.Add(this.vs2);
			this.Controls.Add(this.vs1);
			this.Controls.Add(this.nudTime);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormEMcontrol";
			this.Opacity = 0.5D;
			this.Text = "Time & Scales";
			((System.ComponentModel.ISupportInitialize)(this.nudTime)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.NumericUpDown nudTime;
		private System.Windows.Forms.VScrollBar vs1;
		private System.Windows.Forms.VScrollBar vs2;
		private System.Windows.Forms.TextBox txtScale1;
		private System.Windows.Forms.TextBox txtScale2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox txtExcludeR;
	}
}