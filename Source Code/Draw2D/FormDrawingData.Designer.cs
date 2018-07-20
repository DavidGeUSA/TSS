namespace Draw2D
{
	partial class FormDrawingData
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
			this.lblFile = new System.Windows.Forms.Label();
			this.lblN = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.lblXvalue = new System.Windows.Forms.Label();
			this.lblPageSize = new System.Windows.Forms.Label();
			this.btPrev = new System.Windows.Forms.Button();
			this.btNext = new System.Windows.Forms.Button();
			this.btLoadData = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.btSetN = new System.Windows.Forms.Button();
			this.txtN = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// lblFile
			// 
			this.lblFile.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblFile.Location = new System.Drawing.Point(44, 4);
			this.lblFile.Name = "lblFile";
			this.lblFile.Size = new System.Drawing.Size(248, 23);
			this.lblFile.TabIndex = 17;
			this.lblFile.Text = "***";
			this.lblFile.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// lblN
			// 
			this.lblN.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblN.Location = new System.Drawing.Point(201, 79);
			this.lblN.Name = "lblN";
			this.lblN.Size = new System.Drawing.Size(36, 23);
			this.lblN.TabIndex = 18;
			this.lblN.Text = "*";
			this.lblN.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(26, 13);
			this.label1.TabIndex = 19;
			this.label1.Text = "File:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(310, 9);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(0, 13);
			this.label2.TabIndex = 20;
			// 
			// lblXvalue
			// 
			this.lblXvalue.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblXvalue.Location = new System.Drawing.Point(313, 39);
			this.lblXvalue.Name = "lblXvalue";
			this.lblXvalue.Size = new System.Drawing.Size(73, 23);
			this.lblXvalue.TabIndex = 25;
			this.lblXvalue.Text = "0";
			this.lblXvalue.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// lblPageSize
			// 
			this.lblPageSize.AutoSize = true;
			this.lblPageSize.Location = new System.Drawing.Point(79, 44);
			this.lblPageSize.Name = "lblPageSize";
			this.lblPageSize.Size = new System.Drawing.Size(13, 13);
			this.lblPageSize.TabIndex = 24;
			this.lblPageSize.Text = "0";
			// 
			// btPrev
			// 
			this.btPrev.Location = new System.Drawing.Point(163, 39);
			this.btPrev.Name = "btPrev";
			this.btPrev.Size = new System.Drawing.Size(34, 23);
			this.btPrev.TabIndex = 23;
			this.btPrev.Text = "<";
			this.btPrev.UseVisualStyleBackColor = true;
			this.btPrev.Click += new System.EventHandler(this.btPrev_Click);
			// 
			// btNext
			// 
			this.btNext.Location = new System.Drawing.Point(203, 39);
			this.btNext.Name = "btNext";
			this.btNext.Size = new System.Drawing.Size(34, 23);
			this.btNext.TabIndex = 22;
			this.btNext.Text = ">";
			this.btNext.UseVisualStyleBackColor = true;
			this.btNext.Click += new System.EventHandler(this.btNext_Click);
			// 
			// btLoadData
			// 
			this.btLoadData.Location = new System.Drawing.Point(15, 118);
			this.btLoadData.Name = "btLoadData";
			this.btLoadData.Size = new System.Drawing.Size(112, 23);
			this.btLoadData.TabIndex = 21;
			this.btLoadData.Text = "Show chart data";
			this.btLoadData.UseVisualStyleBackColor = true;
			this.btLoadData.Click += new System.EventHandler(this.btLoadData_Click);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(118, 44);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(35, 13);
			this.label3.TabIndex = 26;
			this.label3.Text = "Page:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 44);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(56, 13);
			this.label4.TabIndex = 27;
			this.label4.Text = "Page size:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(271, 44);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(36, 13);
			this.label5.TabIndex = 28;
			this.label5.Text = "x-axis:";
			// 
			// btSetN
			// 
			this.btSetN.Location = new System.Drawing.Point(313, 79);
			this.btSetN.Name = "btSetN";
			this.btSetN.Size = new System.Drawing.Size(75, 23);
			this.btSetN.TabIndex = 30;
			this.btSetN.Text = "Set Items";
			this.btSetN.UseVisualStyleBackColor = true;
			this.btSetN.Click += new System.EventHandler(this.btSetN_Click);
			// 
			// txtN
			// 
			this.txtN.Location = new System.Drawing.Point(251, 81);
			this.txtN.Name = "txtN";
			this.txtN.Size = new System.Drawing.Size(56, 20);
			this.txtN.TabIndex = 29;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(16, 84);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(181, 13);
			this.label6.TabIndex = 31;
			this.label6.Text = "Number of points on one side of axis:";
			// 
			// FormDrawingData
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(400, 160);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.btSetN);
			this.Controls.Add(this.txtN);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.lblXvalue);
			this.Controls.Add(this.lblPageSize);
			this.Controls.Add(this.btPrev);
			this.Controls.Add(this.btNext);
			this.Controls.Add(this.btLoadData);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.lblN);
			this.Controls.Add(this.lblFile);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormDrawingData";
			this.Opacity = 0.5D;
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "Drawing Data";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label lblFile;
		private System.Windows.Forms.Label lblN;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label lblXvalue;
		private System.Windows.Forms.Label lblPageSize;
		private System.Windows.Forms.Button btPrev;
		private System.Windows.Forms.Button btNext;
		private System.Windows.Forms.Button btLoadData;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button btSetN;
		private System.Windows.Forms.TextBox txtN;
		private System.Windows.Forms.Label label6;
	}
}