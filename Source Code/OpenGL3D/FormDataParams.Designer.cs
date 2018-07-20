namespace Field3D
{
	partial class FormDataParams
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
			this.txtFile = new System.Windows.Forms.TextBox();
			this.btFile = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.txtN = new System.Windows.Forms.TextBox();
			this.btOK = new System.Windows.Forms.Button();
			this.btCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 19);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(49, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Data file:";
			// 
			// txtFile
			// 
			this.txtFile.Location = new System.Drawing.Point(67, 12);
			this.txtFile.Name = "txtFile";
			this.txtFile.Size = new System.Drawing.Size(491, 20);
			this.txtFile.TabIndex = 1;
			// 
			// btFile
			// 
			this.btFile.Location = new System.Drawing.Point(558, 9);
			this.btFile.Name = "btFile";
			this.btFile.Size = new System.Drawing.Size(32, 23);
			this.btFile.TabIndex = 2;
			this.btFile.Text = "...";
			this.btFile.UseVisualStyleBackColor = true;
			this.btFile.Click += new System.EventHandler(this.btFile_Click);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(43, 52);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(18, 13);
			this.label2.TabIndex = 3;
			this.label2.Text = "N:";
			// 
			// txtN
			// 
			this.txtN.Location = new System.Drawing.Point(67, 45);
			this.txtN.Name = "txtN";
			this.txtN.Size = new System.Drawing.Size(50, 20);
			this.txtN.TabIndex = 4;
			this.txtN.Text = "24";
			// 
			// btOK
			// 
			this.btOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btOK.Location = new System.Drawing.Point(67, 90);
			this.btOK.Name = "btOK";
			this.btOK.Size = new System.Drawing.Size(75, 23);
			this.btOK.TabIndex = 5;
			this.btOK.Text = "OK";
			this.btOK.UseVisualStyleBackColor = true;
			// 
			// btCancel
			// 
			this.btCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btCancel.Location = new System.Drawing.Point(148, 90);
			this.btCancel.Name = "btCancel";
			this.btCancel.Size = new System.Drawing.Size(75, 23);
			this.btCancel.TabIndex = 6;
			this.btCancel.Text = "&Cancel";
			this.btCancel.UseVisualStyleBackColor = true;
			// 
			// FormDataParams
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(602, 139);
			this.Controls.Add(this.btCancel);
			this.Controls.Add(this.btOK);
			this.Controls.Add(this.txtN);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.btFile);
			this.Controls.Add(this.txtFile);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormDataParams";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Data Parameters";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtFile;
		private System.Windows.Forms.Button btFile;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtN;
		private System.Windows.Forms.Button btOK;
		private System.Windows.Forms.Button btCancel;
	}
}