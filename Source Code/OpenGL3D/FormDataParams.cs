/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	/*
	  Dialogue for browsing data file and specifying N
	 */
	public partial class FormDataParams : Form
	{
		public FormDataParams()
		{
			InitializeComponent();
		}
		public string Filename
		{
			get
			{
				return txtFile.Text;
			}
		}
		public UInt32 N
		{
			get
			{
				return Convert.ToUInt32(txtN.Text);
			}
		}

		private void btFile_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Select 3D field file";
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				txtFile.Text = dlg.FileName;
			}
		}
	}
}
