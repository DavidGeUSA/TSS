namespace Field3D
{
	partial class FormDrawParameters
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
			this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
			this.txtPercent = new System.Windows.Forms.TextBox();
			this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
			this.btClode = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.numericUpDown3 = new System.Windows.Forms.NumericUpDown();
			this.numericUpDown2 = new System.Windows.Forms.NumericUpDown();
			this.txtViewSize = new System.Windows.Forms.TextBox();
			this.txtGridSize = new System.Windows.Forms.TextBox();
			this.btClear = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.txtSkipRadius = new System.Windows.Forms.TextBox();
			this.btApplySkipRadius = new System.Windows.Forms.Button();
			this.txtSkipSpace = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.chkRadiusOnly = new System.Windows.Forms.CheckBox();
			this.txtRadius = new System.Windows.Forms.TextBox();
			this.hScrollBarRadius = new System.Windows.Forms.HScrollBar();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.label3 = new System.Windows.Forms.Label();
			this.cbEMShow = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.txtSpaceZoom = new System.Windows.Forms.TextBox();
			this.txtLengthZoom = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.hsbLocationZoom = new System.Windows.Forms.HScrollBar();
			this.hsbLengthZoom = new System.Windows.Forms.HScrollBar();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown3)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).BeginInit();
			this.groupBox3.SuspendLayout();
			this.SuspendLayout();
			// 
			// hScrollBar1
			// 
			this.hScrollBar1.Location = new System.Drawing.Point(14, 69);
			this.hScrollBar1.Name = "hScrollBar1";
			this.hScrollBar1.Size = new System.Drawing.Size(230, 21);
			this.hScrollBar1.TabIndex = 0;
			this.hScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBar1_Scroll);
			// 
			// txtPercent
			// 
			this.txtPercent.Location = new System.Drawing.Point(14, 33);
			this.txtPercent.Name = "txtPercent";
			this.txtPercent.Size = new System.Drawing.Size(96, 20);
			this.txtPercent.TabIndex = 1;
			this.txtPercent.Text = "50";
			this.txtPercent.KeyUp += new System.Windows.Forms.KeyEventHandler(this.txtPercent_KeyUp);
			// 
			// numericUpDown1
			// 
			this.numericUpDown1.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.numericUpDown1.DecimalPlaces = 3;
			this.numericUpDown1.Location = new System.Drawing.Point(116, 34);
			this.numericUpDown1.Name = "numericUpDown1";
			this.numericUpDown1.Size = new System.Drawing.Size(128, 16);
			this.numericUpDown1.TabIndex = 3;
			this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
			// 
			// btClode
			// 
			this.btClode.Location = new System.Drawing.Point(426, 283);
			this.btClode.Name = "btClode";
			this.btClode.Size = new System.Drawing.Size(110, 23);
			this.btClode.TabIndex = 4;
			this.btClode.Text = "&Close";
			this.btClode.UseVisualStyleBackColor = true;
			this.btClode.Click += new System.EventHandler(this.btClode_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.numericUpDown1);
			this.groupBox1.Controls.Add(this.hScrollBar1);
			this.groupBox1.Controls.Add(this.txtPercent);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(259, 99);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Visible vectors length percent";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.numericUpDown3);
			this.groupBox2.Controls.Add(this.numericUpDown2);
			this.groupBox2.Controls.Add(this.txtViewSize);
			this.groupBox2.Controls.Add(this.txtGridSize);
			this.groupBox2.Location = new System.Drawing.Point(277, 12);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(259, 99);
			this.groupBox2.TabIndex = 6;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Grid size";
			// 
			// numericUpDown3
			// 
			this.numericUpDown3.CausesValidation = false;
			this.numericUpDown3.DecimalPlaces = 3;
			this.numericUpDown3.Location = new System.Drawing.Point(116, 69);
			this.numericUpDown3.Name = "numericUpDown3";
			this.numericUpDown3.Size = new System.Drawing.Size(128, 20);
			this.numericUpDown3.TabIndex = 7;
			this.numericUpDown3.ValueChanged += new System.EventHandler(this.numericUpDown3_ValueChanged);
			// 
			// numericUpDown2
			// 
			this.numericUpDown2.CausesValidation = false;
			this.numericUpDown2.DecimalPlaces = 3;
			this.numericUpDown2.Location = new System.Drawing.Point(116, 28);
			this.numericUpDown2.Name = "numericUpDown2";
			this.numericUpDown2.Size = new System.Drawing.Size(128, 20);
			this.numericUpDown2.TabIndex = 5;
			this.numericUpDown2.ValueChanged += new System.EventHandler(this.numericUpDown2_ValueChanged);
			// 
			// txtViewSize
			// 
			this.txtViewSize.Location = new System.Drawing.Point(14, 69);
			this.txtViewSize.Name = "txtViewSize";
			this.txtViewSize.Size = new System.Drawing.Size(96, 20);
			this.txtViewSize.TabIndex = 6;
			this.txtViewSize.Text = "50";
			this.txtViewSize.TextChanged += new System.EventHandler(this.txtViewSize_TextChanged);
			// 
			// txtGridSize
			// 
			this.txtGridSize.Location = new System.Drawing.Point(14, 27);
			this.txtGridSize.Name = "txtGridSize";
			this.txtGridSize.Size = new System.Drawing.Size(96, 20);
			this.txtGridSize.TabIndex = 4;
			this.txtGridSize.Text = "50";
			this.txtGridSize.TextChanged += new System.EventHandler(this.txtGridSize_TextChanged);
			// 
			// btClear
			// 
			this.btClear.Location = new System.Drawing.Point(277, 283);
			this.btClear.Name = "btClear";
			this.btClear.Size = new System.Drawing.Size(128, 23);
			this.btClear.TabIndex = 8;
			this.btClear.Text = "Clear vectors";
			this.btClear.UseVisualStyleBackColor = true;
			this.btClear.Click += new System.EventHandler(this.btClear_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(11, 26);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(62, 13);
			this.label1.TabIndex = 9;
			this.label1.Text = "Skip radius:";
			// 
			// txtSkipRadius
			// 
			this.txtSkipRadius.Location = new System.Drawing.Point(79, 23);
			this.txtSkipRadius.Name = "txtSkipRadius";
			this.txtSkipRadius.Size = new System.Drawing.Size(31, 20);
			this.txtSkipRadius.TabIndex = 10;
			this.txtSkipRadius.Text = "0";
			// 
			// btApplySkipRadius
			// 
			this.btApplySkipRadius.Location = new System.Drawing.Point(6, 113);
			this.btApplySkipRadius.Name = "btApplySkipRadius";
			this.btApplySkipRadius.Size = new System.Drawing.Size(95, 23);
			this.btApplySkipRadius.TabIndex = 11;
			this.btApplySkipRadius.Text = "Apply";
			this.btApplySkipRadius.UseVisualStyleBackColor = true;
			this.btApplySkipRadius.Click += new System.EventHandler(this.btApplySkipRadius_Click);
			// 
			// txtSkipSpace
			// 
			this.txtSkipSpace.Location = new System.Drawing.Point(210, 23);
			this.txtSkipSpace.Name = "txtSkipSpace";
			this.txtSkipSpace.Size = new System.Drawing.Size(31, 20);
			this.txtSkipSpace.TabIndex = 13;
			this.txtSkipSpace.Text = "0";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(142, 26);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(63, 13);
			this.label2.TabIndex = 12;
			this.label2.Text = "Skip space:";
			// 
			// chkRadiusOnly
			// 
			this.chkRadiusOnly.AutoSize = true;
			this.chkRadiusOnly.Location = new System.Drawing.Point(14, 64);
			this.chkRadiusOnly.Name = "chkRadiusOnly";
			this.chkRadiusOnly.Size = new System.Drawing.Size(116, 17);
			this.chkRadiusOnly.TabIndex = 14;
			this.chkRadiusOnly.Text = "Draw only at radius";
			this.chkRadiusOnly.UseVisualStyleBackColor = true;
			// 
			// txtRadius
			// 
			this.txtRadius.Location = new System.Drawing.Point(145, 62);
			this.txtRadius.Name = "txtRadius";
			this.txtRadius.Size = new System.Drawing.Size(37, 20);
			this.txtRadius.TabIndex = 15;
			this.txtRadius.Text = "0";
			this.txtRadius.KeyUp += new System.Windows.Forms.KeyEventHandler(this.txtRadius_KeyUp);
			// 
			// hScrollBarRadius
			// 
			this.hScrollBarRadius.Location = new System.Drawing.Point(14, 84);
			this.hScrollBarRadius.Name = "hScrollBarRadius";
			this.hScrollBarRadius.Size = new System.Drawing.Size(230, 17);
			this.hScrollBarRadius.TabIndex = 16;
			this.hScrollBarRadius.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBarRadius_Scroll);
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.label2);
			this.groupBox3.Controls.Add(this.hScrollBarRadius);
			this.groupBox3.Controls.Add(this.label1);
			this.groupBox3.Controls.Add(this.txtRadius);
			this.groupBox3.Controls.Add(this.txtSkipRadius);
			this.groupBox3.Controls.Add(this.chkRadiusOnly);
			this.groupBox3.Controls.Add(this.btApplySkipRadius);
			this.groupBox3.Controls.Add(this.txtSkipSpace);
			this.groupBox3.Location = new System.Drawing.Point(12, 117);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(259, 144);
			this.groupBox3.TabIndex = 17;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Radius index";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(288, 219);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(71, 13);
			this.label3.TabIndex = 18;
			this.label3.Text = "EM selection:";
			// 
			// cbEMShow
			// 
			this.cbEMShow.FormattingEnabled = true;
			this.cbEMShow.Items.AddRange(new object[] {
            "Show electric field",
            "Show magnetic field"});
			this.cbEMShow.Location = new System.Drawing.Point(393, 214);
			this.cbEMShow.Name = "cbEMShow";
			this.cbEMShow.Size = new System.Drawing.Size(128, 21);
			this.cbEMShow.TabIndex = 19;
			this.cbEMShow.SelectedIndexChanged += new System.EventHandler(this.cbEMShow_SelectedIndexChanged);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(288, 117);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(177, 13);
			this.label4.TabIndex = 20;
			this.label4.Text = "Vector location view-size factor (%): ";
			// 
			// txtSpaceZoom
			// 
			this.txtSpaceZoom.Location = new System.Drawing.Point(481, 114);
			this.txtSpaceZoom.Name = "txtSpaceZoom";
			this.txtSpaceZoom.Size = new System.Drawing.Size(40, 20);
			this.txtSpaceZoom.TabIndex = 21;
			this.txtSpaceZoom.Text = "50";
			this.txtSpaceZoom.TextChanged += new System.EventHandler(this.txtSpaceZoom_TextChanged);
			// 
			// txtLengthZoom
			// 
			this.txtLengthZoom.Location = new System.Drawing.Point(481, 167);
			this.txtLengthZoom.Name = "txtLengthZoom";
			this.txtLengthZoom.Size = new System.Drawing.Size(40, 20);
			this.txtLengthZoom.TabIndex = 23;
			this.txtLengthZoom.Text = "50";
			this.txtLengthZoom.TextChanged += new System.EventHandler(this.txtLengthZoom_TextChanged);
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(288, 170);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(166, 13);
			this.label5.TabIndex = 22;
			this.label5.Text = "Vector length view-size factor (%):";
			// 
			// hsbLocationZoom
			// 
			this.hsbLocationZoom.Location = new System.Drawing.Point(291, 137);
			this.hsbLocationZoom.Name = "hsbLocationZoom";
			this.hsbLocationZoom.Size = new System.Drawing.Size(230, 10);
			this.hsbLocationZoom.TabIndex = 24;
			this.hsbLocationZoom.Value = 50;
			this.hsbLocationZoom.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbLocationZoom_Scroll);
			// 
			// hsbLengthZoom
			// 
			this.hsbLengthZoom.Location = new System.Drawing.Point(291, 190);
			this.hsbLengthZoom.Name = "hsbLengthZoom";
			this.hsbLengthZoom.Size = new System.Drawing.Size(230, 10);
			this.hsbLengthZoom.TabIndex = 25;
			this.hsbLengthZoom.Value = 50;
			this.hsbLengthZoom.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsbLengthZoom_Scroll);
			// 
			// FormDrawParameters
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(549, 318);
			this.Controls.Add(this.hsbLengthZoom);
			this.Controls.Add(this.hsbLocationZoom);
			this.Controls.Add(this.txtLengthZoom);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.txtSpaceZoom);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.cbEMShow);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.btClear);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.btClode);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormDrawParameters";
			this.Opacity = 0.7D;
			this.Text = "drawing parameters";
			this.TopMost = true;
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown3)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).EndInit();
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.HScrollBar hScrollBar1;
		private System.Windows.Forms.TextBox txtPercent;
		private System.Windows.Forms.NumericUpDown numericUpDown1;
		private System.Windows.Forms.Button btClode;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.NumericUpDown numericUpDown2;
		private System.Windows.Forms.TextBox txtGridSize;
		private System.Windows.Forms.NumericUpDown numericUpDown3;
		private System.Windows.Forms.TextBox txtViewSize;
		private System.Windows.Forms.Button btClear;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtSkipRadius;
		private System.Windows.Forms.Button btApplySkipRadius;
		private System.Windows.Forms.TextBox txtSkipSpace;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.CheckBox chkRadiusOnly;
		private System.Windows.Forms.TextBox txtRadius;
		private System.Windows.Forms.HScrollBar hScrollBarRadius;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox cbEMShow;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox txtSpaceZoom;
		private System.Windows.Forms.TextBox txtLengthZoom;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.HScrollBar hsbLocationZoom;
		private System.Windows.Forms.HScrollBar hsbLengthZoom;
	}
}