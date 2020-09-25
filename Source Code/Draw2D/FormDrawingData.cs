/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved David Ge

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

namespace Draw2D
{
	/// <summary>
	/// UI for showing data parameters
	/// </summary>
	public partial class FormDrawingData : Form
	{
		private FormDraw2D _owner;
		public FormDrawingData()
		{
			InitializeComponent();
		}
		public void InitOwner(FormDraw2D o)
		{
			_owner = o;
		}
		public void ShowFileName(string name)
		{
			lblFile.Text = Path.GetFileName(name);
		}
		public void ShowXvalue(double xv)
		{
			lblXvalue.Text = xv.ToString(CultureInfo.InvariantCulture);
		}
		public void ShowPageSize(int pageSize)
		{
			lblPageSize.Text = pageSize.ToString(CultureInfo.InvariantCulture);
		}

		private void btLoadData_Click(object sender, EventArgs e)
		{
			btLoadData.Enabled = false;
			_owner.LoadData();
			btLoadData.Enabled = true;
		}

		private void btNext_Click(object sender, EventArgs e)
		{
			_owner.ShowNextPage();
		}

		private void btPrev_Click(object sender, EventArgs e)
		{
			_owner.ShowPreviousPage();
		}

		private void btSetN_Click(object sender, EventArgs e)
		{
			btSetN.Enabled = false;
			if (_owner.GetXlength() > 0)
			{
				try
				{
					UInt32 n = Convert.ToUInt32(txtN.Text);
					if (_owner.SetN(n))
					{
						txtN.Text = string.Format(CultureInfo.InvariantCulture, "{0} / {1}", n, _owner.GetXlength());
					}
				}
				catch
				{
				}
			}
			btSetN.Enabled = true;
		}
	}
}
