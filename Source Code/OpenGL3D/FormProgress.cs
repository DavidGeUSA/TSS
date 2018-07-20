using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	public partial class FormProgress : Form
	{
		public FormProgress()
		{
			InitializeComponent();
		}
		public void initProgressBar(int maxItems, string info)
		{
			progressBar1.Minimum = 0;
			progressBar1.Maximum = maxItems;
			label1.Text = info;
			label1.Refresh();
			this.Size = new Size(progressBar1.Width, progressBar1.Height + label1.Height);
		}
		public void SetProgressValue(int n)
		{
			if (n >= 0 && n <= progressBar1.Maximum)
			{
				progressBar1.Value = n;
			}
		}
		private void FormProgress_Load(object sender, EventArgs e)
		{

		}

		private void progressBar1_Click(object sender, EventArgs e)
		{

		}
	}
}
