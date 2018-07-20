namespace Field3D
{
	partial class FormRotate
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btRotateZPositive = new System.Windows.Forms.Button();
			this.btRotateYPositive = new System.Windows.Forms.Button();
			this.btRotateXPositive = new System.Windows.Forms.Button();
			this.btResetRotate = new System.Windows.Forms.Button();
			this.label9 = new System.Windows.Forms.Label();
			this.btRotateZ = new System.Windows.Forms.Button();
			this.label8 = new System.Windows.Forms.Label();
			this.btRotateY = new System.Windows.Forms.Button();
			this.label7 = new System.Windows.Forms.Label();
			this.btRotateX = new System.Windows.Forms.Button();
			this.txtRotateZ = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.txtRotateY = new System.Windows.Forms.TextBox();
			this.txtRotateX = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.txtShiftZ = new System.Windows.Forms.TextBox();
			this.hsbShiftY = new System.Windows.Forms.HScrollBar();
			this.label4 = new System.Windows.Forms.Label();
			this.hsbShiftX = new System.Windows.Forms.HScrollBar();
			this.hsbShiftZ = new System.Windows.Forms.HScrollBar();
			this.label5 = new System.Windows.Forms.Label();
			this.txtShiftY = new System.Windows.Forms.TextBox();
			this.txtShiftX = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btRotateZPositive);
			this.groupBox1.Controls.Add(this.btRotateYPositive);
			this.groupBox1.Controls.Add(this.btRotateXPositive);
			this.groupBox1.Controls.Add(this.btResetRotate);
			this.groupBox1.Controls.Add(this.label9);
			this.groupBox1.Controls.Add(this.btRotateZ);
			this.groupBox1.Controls.Add(this.label8);
			this.groupBox1.Controls.Add(this.btRotateY);
			this.groupBox1.Controls.Add(this.label7);
			this.groupBox1.Controls.Add(this.btRotateX);
			this.groupBox1.Controls.Add(this.txtRotateZ);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.txtRotateY);
			this.groupBox1.Controls.Add(this.txtRotateX);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(359, 131);
			this.groupBox1.TabIndex = 26;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Rotate by axis";
			// 
			// btRotateZPositive
			// 
			this.btRotateZPositive.Location = new System.Drawing.Point(196, 69);
			this.btRotateZPositive.Name = "btRotateZPositive";
			this.btRotateZPositive.Size = new System.Drawing.Size(72, 23);
			this.btRotateZPositive.TabIndex = 44;
			this.btRotateZPositive.Text = "Apply +";
			this.btRotateZPositive.UseVisualStyleBackColor = true;
			this.btRotateZPositive.Click += new System.EventHandler(this.btRotateZPositive_Click);
			// 
			// btRotateYPositive
			// 
			this.btRotateYPositive.Location = new System.Drawing.Point(196, 42);
			this.btRotateYPositive.Name = "btRotateYPositive";
			this.btRotateYPositive.Size = new System.Drawing.Size(72, 23);
			this.btRotateYPositive.TabIndex = 43;
			this.btRotateYPositive.Text = "Apply +";
			this.btRotateYPositive.UseVisualStyleBackColor = true;
			this.btRotateYPositive.Click += new System.EventHandler(this.btRotateYPositive_Click);
			// 
			// btRotateXPositive
			// 
			this.btRotateXPositive.Location = new System.Drawing.Point(196, 17);
			this.btRotateXPositive.Name = "btRotateXPositive";
			this.btRotateXPositive.Size = new System.Drawing.Size(72, 23);
			this.btRotateXPositive.TabIndex = 42;
			this.btRotateXPositive.Text = "Apply +";
			this.btRotateXPositive.UseVisualStyleBackColor = true;
			this.btRotateXPositive.Click += new System.EventHandler(this.btRotateXPositive_Click);
			// 
			// btResetRotate
			// 
			this.btResetRotate.Location = new System.Drawing.Point(196, 102);
			this.btResetRotate.Name = "btResetRotate";
			this.btResetRotate.Size = new System.Drawing.Size(148, 23);
			this.btResetRotate.TabIndex = 41;
			this.btResetRotate.Text = "Reset rotation";
			this.btResetRotate.UseVisualStyleBackColor = true;
			this.btResetRotate.Click += new System.EventHandler(this.btResetRotate_Click);
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(133, 74);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(40, 13);
			this.label9.TabIndex = 40;
			this.label9.Text = "degree";
			// 
			// btRotateZ
			// 
			this.btRotateZ.Location = new System.Drawing.Point(272, 69);
			this.btRotateZ.Name = "btRotateZ";
			this.btRotateZ.Size = new System.Drawing.Size(72, 23);
			this.btRotateZ.TabIndex = 39;
			this.btRotateZ.Text = "Apply -";
			this.btRotateZ.UseVisualStyleBackColor = true;
			this.btRotateZ.Click += new System.EventHandler(this.btRotateZ_Click);
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(133, 47);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(40, 13);
			this.label8.TabIndex = 38;
			this.label8.Text = "degree";
			// 
			// btRotateY
			// 
			this.btRotateY.Location = new System.Drawing.Point(272, 42);
			this.btRotateY.Name = "btRotateY";
			this.btRotateY.Size = new System.Drawing.Size(72, 23);
			this.btRotateY.TabIndex = 37;
			this.btRotateY.Text = "Apply -";
			this.btRotateY.UseVisualStyleBackColor = true;
			this.btRotateY.Click += new System.EventHandler(this.btRotateY_Click);
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(133, 22);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(40, 13);
			this.label7.TabIndex = 36;
			this.label7.Text = "degree";
			// 
			// btRotateX
			// 
			this.btRotateX.Location = new System.Drawing.Point(272, 17);
			this.btRotateX.Name = "btRotateX";
			this.btRotateX.Size = new System.Drawing.Size(72, 23);
			this.btRotateX.TabIndex = 35;
			this.btRotateX.Text = "Apply -";
			this.btRotateX.UseVisualStyleBackColor = true;
			this.btRotateX.Click += new System.EventHandler(this.btRotateX_Click);
			// 
			// txtRotateZ
			// 
			this.txtRotateZ.Location = new System.Drawing.Point(76, 71);
			this.txtRotateZ.Name = "txtRotateZ";
			this.txtRotateZ.Size = new System.Drawing.Size(51, 20);
			this.txtRotateZ.TabIndex = 34;
			this.txtRotateZ.Text = "2";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(15, 74);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(36, 13);
			this.label3.TabIndex = 33;
			this.label3.Text = "z-axis:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(15, 22);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(36, 13);
			this.label1.TabIndex = 27;
			this.label1.Text = "x-axis:";
			// 
			// txtRotateY
			// 
			this.txtRotateY.Location = new System.Drawing.Point(76, 45);
			this.txtRotateY.Name = "txtRotateY";
			this.txtRotateY.Size = new System.Drawing.Size(51, 20);
			this.txtRotateY.TabIndex = 31;
			this.txtRotateY.Text = "2";
			// 
			// txtRotateX
			// 
			this.txtRotateX.Location = new System.Drawing.Point(76, 19);
			this.txtRotateX.Name = "txtRotateX";
			this.txtRotateX.Size = new System.Drawing.Size(51, 20);
			this.txtRotateX.TabIndex = 28;
			this.txtRotateX.Text = "2";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(15, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(36, 13);
			this.label2.TabIndex = 30;
			this.label2.Text = "y-axis:";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.txtShiftZ);
			this.groupBox2.Controls.Add(this.hsbShiftY);
			this.groupBox2.Controls.Add(this.label4);
			this.groupBox2.Controls.Add(this.hsbShiftX);
			this.groupBox2.Controls.Add(this.hsbShiftZ);
			this.groupBox2.Controls.Add(this.label5);
			this.groupBox2.Controls.Add(this.txtShiftY);
			this.groupBox2.Controls.Add(this.txtShiftX);
			this.groupBox2.Controls.Add(this.label6);
			this.groupBox2.Location = new System.Drawing.Point(12, 149);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(359, 100);
			this.groupBox2.TabIndex = 27;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Shift by axis";
			// 
			// txtShiftZ
			// 
			this.txtShiftZ.Location = new System.Drawing.Point(297, 71);
			this.txtShiftZ.Name = "txtShiftZ";
			this.txtShiftZ.Size = new System.Drawing.Size(47, 20);
			this.txtShiftZ.TabIndex = 34;
			this.txtShiftZ.Text = "0";
			this.txtShiftZ.TextChanged += new System.EventHandler(this.txtShiftZ_TextChanged);
			// 
			// hsbShiftY
			// 
			this.hsbShiftY.Location = new System.Drawing.Point(64, 51);
			this.hsbShiftY.Minimum = -100;
			this.hsbShiftY.Name = "hsbShiftY";
			this.hsbShiftY.Size = new System.Drawing.Size(230, 10);
			this.hsbShiftY.TabIndex = 29;
			this.hsbShiftY.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbShiftY_Scroll);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(15, 74);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(36, 13);
			this.label4.TabIndex = 33;
			this.label4.Text = "z-axis:";
			// 
			// hsbShiftX
			// 
			this.hsbShiftX.Location = new System.Drawing.Point(64, 25);
			this.hsbShiftX.Minimum = -100;
			this.hsbShiftX.Name = "hsbShiftX";
			this.hsbShiftX.Size = new System.Drawing.Size(230, 10);
			this.hsbShiftX.TabIndex = 25;
			this.hsbShiftX.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbShiftX_Scroll);
			// 
			// hsbShiftZ
			// 
			this.hsbShiftZ.Location = new System.Drawing.Point(64, 77);
			this.hsbShiftZ.Minimum = -100;
			this.hsbShiftZ.Name = "hsbShiftZ";
			this.hsbShiftZ.Size = new System.Drawing.Size(230, 10);
			this.hsbShiftZ.TabIndex = 32;
			this.hsbShiftZ.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbShiftZ_Scroll);
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(15, 22);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(36, 13);
			this.label5.TabIndex = 27;
			this.label5.Text = "x-axis:";
			// 
			// txtShiftY
			// 
			this.txtShiftY.Location = new System.Drawing.Point(297, 45);
			this.txtShiftY.Name = "txtShiftY";
			this.txtShiftY.Size = new System.Drawing.Size(47, 20);
			this.txtShiftY.TabIndex = 31;
			this.txtShiftY.Text = "0";
			this.txtShiftY.TextChanged += new System.EventHandler(this.txtShiftY_TextChanged);
			// 
			// txtShiftX
			// 
			this.txtShiftX.Location = new System.Drawing.Point(297, 19);
			this.txtShiftX.Name = "txtShiftX";
			this.txtShiftX.Size = new System.Drawing.Size(47, 20);
			this.txtShiftX.TabIndex = 28;
			this.txtShiftX.Text = "0";
			this.txtShiftX.TextChanged += new System.EventHandler(this.txtShiftX_TextChanged);
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(15, 48);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(36, 13);
			this.label6.TabIndex = 30;
			this.label6.Text = "y-axis:";
			// 
			// FormRotate
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(377, 252);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormRotate";
			this.Opacity = 0.5D;
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "Rotate and Shift";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtRotateX;
		private System.Windows.Forms.TextBox txtRotateZ;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtRotateY;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.TextBox txtShiftZ;
		private System.Windows.Forms.HScrollBar hsbShiftY;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.HScrollBar hsbShiftX;
		private System.Windows.Forms.HScrollBar hsbShiftZ;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox txtShiftY;
		private System.Windows.Forms.TextBox txtShiftX;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Button btRotateZ;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Button btRotateY;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Button btRotateX;
		private System.Windows.Forms.Button btResetRotate;
		private System.Windows.Forms.Button btRotateZPositive;
		private System.Windows.Forms.Button btRotateYPositive;
		private System.Windows.Forms.Button btRotateXPositive;

	}
}