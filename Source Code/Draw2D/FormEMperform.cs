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
using System.Text;
using System.Windows.Forms;

namespace Draw2D
{
	/// <summary>
	/// UI for showing performance of EM simulations
	/// setting scales
	/// </summary>
	public partial class FormEMperform : Form
	{
		private FormDraw2D _owner;
		private DataTable _data;
		private bool _synch = false;
		private IList<EMperformance> _list;
		public FormEMperform()
		{
			InitializeComponent();
		}
		public void LoadData(FormDraw2D owner, IList<EMperformance> list)
		{
			_owner = owner;
			_synch = true;
			_list = list;
			_data = new DataTable("EMPS");
			_data.Columns.Add("FDTD", typeof(string));
			_data.Columns.Add("ErrorMagnitude", typeof(double));
			_data.Columns.Add("TimeUsed", typeof(double));
			_data.Columns.Add("DisplayScale", typeof(double));
			for (int i = 0; i < list.Count; i++)
			{
				_data.Rows.Add(list[i].Description, list[i].Precision, list[i].TimeUsed, 1.0);
			}
			dataGridView1.DataSource = _data;
			dataGridView1.Columns[0].ReadOnly = true;
			dataGridView1.Columns[1].ReadOnly = true;
			dataGridView1.Columns[2].ReadOnly = true;
			dataGridView1.Columns[3].ReadOnly = false;
			dataGridView1.Columns[3].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
			dataGridView1.AllowUserToAddRows = false;
			dataGridView1.AllowUserToDeleteRows = false;
			dataGridView1.AllowUserToOrderColumns = false;
			dataGridView1.AllowUserToResizeRows = false;
			_synch = false;
		}

		private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (!_synch)
			{
				_synch = true;
				if (e.ColumnIndex == 3)
				{
					try
					{
						double s = Convert.ToDouble(dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Value);
						if (s > 0.0)
						{
							double v = _list[e.RowIndex].Precision * s;
							_owner.SetEMPscale(e.RowIndex, v);
						}
					}
					catch
					{
					}
				}
				_synch = false;
			}
		}
	}
}
