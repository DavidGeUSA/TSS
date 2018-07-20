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
using System.IO;
using System.Globalization;
using Draw3D;

namespace Draw2D
{
	/// <summary>
	/// show data in a table
	/// </summary>
	public partial class FormData : Form
	{
		public FormData()
		{
			InitializeComponent();
		}
		public void LoadData(double[] y, double[] z, double[] x, double[] w, double[] v, double[] s)
		{
			DataTable tbl = new DataTable("Data");
			tbl.Columns.Add("X", typeof(double));
			tbl.Columns.Add("Data A", typeof(double));
			tbl.Columns.Add("Data B", typeof(double));
			tbl.Columns.Add("Data C", typeof(double));
			tbl.Columns.Add("Data D", typeof(double));
			tbl.Columns.Add("Data E", typeof(double));
			for (int i = 0; i < y.Length; i++)
			{
				tbl.Rows.Add(Math.Round(x[i], 3), y[i], z == null ? 0 : z[i], w == null ? 0 : w[i], v == null ? 0 : v[i], s == null ? 0 : s[i]);
			}
			dataGridView1.DataSource = tbl;
			//dataGridView1.Columns[2].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
		}
		public bool Load1DComplex(string file, UInt32 dataNum)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			int itemSize = 2 * sizeof(double);
			UInt32 rNum = SpaceDigitizer.SPACE_POINTS(dataNum);
			//int cNum = dataNum + 1;
			long itemCount = rNum;
			if (itemCount * itemSize != size)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				double vr, vi;
				DataTable tbl = new DataTable("Data");
				tbl.Columns.Add("k", typeof(double));
				tbl.Columns.Add("Real", typeof(double));
				tbl.Columns.Add("Imag", typeof(double));
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				for (int i = 0; i < itemCount; i++)
				{
					vr = br.ReadDouble();
					vi = br.ReadDouble();
					tbl.Rows.Add(i, vr, vi);
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				dataGridView1.Columns[2].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
				return true;
			}
			return false;
		}
		public bool Load1DReal(string file, UInt32 dataNum)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			int itemSize = 2 * sizeof(double);
			UInt32 cNum = SpaceDigitizer.FFTW_HALF_N(dataNum);
			long itemCount = cNum;
			if (itemCount * itemSize != size)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				double vr, vi;
				DataTable tbl = new DataTable("Data");
				tbl.Columns.Add("k", typeof(double));
				tbl.Columns.Add("Real", typeof(double));
				tbl.Columns.Add("Imag", typeof(double));
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				for (int i = 0; i < itemCount; i++)
				{
					vr = br.ReadDouble();
					vi = br.ReadDouble();
					tbl.Rows.Add(i, Math.Round(vr, 10), Math.Round(vi, 10));
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				dataGridView1.Columns[2].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
				return true;
			}
			return false;
		}
		public bool Load2DComplex(string file, UInt32 dataNum)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			int itemSize = 2 * sizeof(double);
			UInt32 rNum = SpaceDigitizer.SPACE_POINTS(dataNum);
			UInt32 rNum2 = rNum * rNum;
			long itemCount = rNum2;
			if (itemCount * itemSize != size)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				object[] vs;
				int i, j;
				double vr, vi;
				DataTable tbl = new DataTable("Data");
				tbl.Columns.Add("k", typeof(int));
				//vs = new object[rNum + 1];
				//vs[0] = 0;
				for (j = 0; j < rNum; j++)
				{
					tbl.Columns.Add(string.Format(CultureInfo.InvariantCulture,"Col{0}",j), typeof(string));
					tbl.Columns[j + 1].Caption = j.ToString(CultureInfo.InvariantCulture);
					//vs[j + 1] = j.ToString(CultureInfo.InvariantCulture);
				}
				//tbl.Rows.Add(vs);
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				for ( i = 0; i < rNum; i++)
				{
					vs = new object[rNum + 1];
					vs[0] = i;
					for (j = 0; j < rNum; j++)
					{
						vr = br.ReadDouble();
						vi = br.ReadDouble();
						vs[j + 1] = string.Format(CultureInfo.InvariantCulture, "{0}[{1}]",Math.Round(vr,6), vi);
					}
					tbl.Rows.Add(vs);
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				dataGridView1.Columns[0].Frozen = true;
				return true;
			}
			return false;
		}
		public bool Load6doubles(string file)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			int itemSize = 6 * sizeof(double) + 4 * sizeof(UInt32);
			if ((size % itemSize) != 0)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				double threshold = 1e-9;
				int d = 8;
				UInt32 i1, j1, i2, j2;
				double r1, c1, r2, c2, m1, m2;
				long N = size / itemSize;
				DataTable tbl = new DataTable("Data");
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				tbl.Columns.Add("i1", typeof(UInt32));
				tbl.Columns.Add("j1", typeof(UInt32));
				tbl.Columns.Add("i2", typeof(UInt32));
				tbl.Columns.Add("j2", typeof(UInt32));
				tbl.Columns.Add("R1", typeof(double));
				tbl.Columns.Add("C1", typeof(double));
				tbl.Columns.Add("R2", typeof(double));
				tbl.Columns.Add("C2", typeof(double));
				tbl.Columns.Add("M1", typeof(double));
				tbl.Columns.Add("M2", typeof(double));
				tbl.Columns.Add("Diff", typeof(double));
				tbl.Columns.Add("Phase1", typeof(double));
				tbl.Columns.Add("Phase2", typeof(double));
				for (int i = 0; i < N; i++)
				{
					i1 = br.ReadUInt32();
					j1 = br.ReadUInt32();
					r1 = br.ReadDouble();
					c1 = br.ReadDouble();

					i2 = br.ReadUInt32();
					j2 = br.ReadUInt32();
					r2 = br.ReadDouble();
					c2 = br.ReadDouble();

					m1 = br.ReadDouble();
					m2 = br.ReadDouble();
					if (Math.Abs(r1) > threshold || Math.Abs(c1) > threshold || Math.Abs(r2) > threshold || Math.Abs(c2) > threshold)
					{
						double sin1 = c1 / Math.Sqrt(m1);
						double sin2 = c2 / Math.Sqrt(m2);
						double a1 = Math.Asin(sin1);
						double a2 = Math.Asin(sin2);
						//Eula's formula:  exp(ia)=cos(a)+i sin(a)
						//thus, r = cos(a)  c = sin(a)
						//if (m1 > 8)
						//{
						//	if (m1 == m2)
						//	{
						//	}
						//}
						//if (a1 < 0) a1 = -a1; if (a2 < 0) a2 = -a2;
						//if (c1 >= 0)
						//{
						//	if (r1 < 0)
						//	{
						//		a1 = Math.PI - a1;
						//	}
						//}
						//else //c1 < 0
						//{
						//	if (r1 < 0)
						//	{
						//		a1 = Math.PI + a1;
						//	}
						//	else
						//	{
						//		a1 = 2.0 * Math.PI - a1;
						//	}
						//}
						//if (c2 >= 0)
						//{
						//	if (r2 < 0)
						//	{
						//		a2 = Math.PI - a2;
						//	}
						//}
						//else //c2 < 0
						//{
						//	if (r2 < 0)
						//	{
						//		a2 = Math.PI + a2;
						//	}
						//	else
						//	{
						//		a2 = 2.0 * Math.PI - a2;
						//	}
						//}
						//double a12 = (a1 - a2) * 180.0 / Math.PI;
						a1 = a1 * 180.0 / Math.PI;
						a2 = a2 * 180.0 / Math.PI;
						tbl.Rows.Add(i1, j1, i2, j2, Math.Round(r1, d), Math.Round(c1, d), Math.Round(r2, d), Math.Round(c2, d), Math.Round(m1, d), Math.Round(m2, d), Math.Round(m1 - m2, d), Math.Round(a1, d), Math.Round(a2, d));
					}
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				return true;
			}
			return false;
		}
		public bool LoadHighLowRows(string file)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			//file format: low row #, high row #, column #, Low R, Low C, High R, High C, Low Magnitude, High Magnitude
			//UInt32, UInt32, UInt32, double, double, double, double, double, double
			int itemSize = 6 * sizeof(double) + 3 * sizeof(UInt32);
			if ((size % itemSize) != 0)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				double threshold = 1e-9;
				int d = 8;
				UInt32 i1, j1, i2;
				double r1, c1, r2, c2, m1, m2;
				long N = size / itemSize;
				DataTable tbl = new DataTable("Data");
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				tbl.Columns.Add("Low_Row", typeof(UInt32));
				tbl.Columns.Add("High_Row", typeof(UInt32));
				tbl.Columns.Add("Column", typeof(UInt32));
				tbl.Columns.Add("R1", typeof(double));
				tbl.Columns.Add("C1", typeof(double));
				tbl.Columns.Add("R2", typeof(double));
				tbl.Columns.Add("C2", typeof(double));
				tbl.Columns.Add("M1", typeof(double));
				tbl.Columns.Add("M2", typeof(double));
				tbl.Columns.Add("Diff", typeof(double));
				tbl.Columns.Add("Phase1", typeof(double));
				tbl.Columns.Add("Phase2", typeof(double));
				for (int i = 0; i < N; i++)
				{
					i1 = br.ReadUInt32();
					i2 = br.ReadUInt32();
					j1 = br.ReadUInt32();
					r1 = br.ReadDouble();
					c1 = br.ReadDouble();
					r2 = br.ReadDouble();
					c2 = br.ReadDouble();
					m1 = br.ReadDouble();
					m2 = br.ReadDouble();
					if (Math.Abs(r1) > threshold || Math.Abs(c1) > threshold || Math.Abs(r2) > threshold || Math.Abs(c2) > threshold)
					{
						double sin1 = c1 / Math.Sqrt(m1);
						double sin2 = c2 / Math.Sqrt(m2);
						double a1 = Math.Asin(sin1);
						double a2 = Math.Asin(sin2);
						a1 = a1 * 180.0 / Math.PI;
						a2 = a2 * 180.0 / Math.PI;
						tbl.Rows.Add(i1, i2, j1, Math.Round(r1, d), Math.Round(c1, d), Math.Round(r2, d), Math.Round(c2, d), Math.Round(m1, d), Math.Round(m2, d), Math.Round(m1 - m2, d), Math.Round(a1, d), Math.Round(a2, d));
					}
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				return true;
			}
			return false;
		}
		public bool Load2DReal(string file, UInt32 dataNum)
		{
			this.Text = file;
			FileInfo fi = new FileInfo(file);
			long size = fi.Length;
			int itemSize = 2 * sizeof(double);
			UInt32 rNum = SpaceDigitizer.SPACE_POINTS(dataNum);
			UInt32 rNum2 = rNum * rNum;
			UInt32 cNum = SpaceDigitizer.FFTW_HALF_N(dataNum);
			long itemCount = rNum * cNum;
			if (itemCount * itemSize != size)
			{
				MessageBox.Show("File size mismatch");
			}
			else
			{
				object[] vs;
				int i, j;
				double vr, vi;
				DataTable tbl = new DataTable("Data");
				//i-frequency column
				tbl.Columns.Add("k", typeof(int));
				//j-data columns
				for (j = 0; j < cNum; j++)
				{
					tbl.Columns.Add(string.Format(CultureInfo.InvariantCulture, "Col{0}", j), typeof(string));
					tbl.Columns[j + 1].Caption = j.ToString(CultureInfo.InvariantCulture);
				}
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				//row-major indexing
				for (i = 0; i < rNum; i++)
				{
					vs = new object[cNum + 1];
					vs[0] = i; //the first column is i-frequency
					for (j = 0; j < cNum; j++)
					{
						vr = br.ReadDouble();
						vi = br.ReadDouble();
						vs[j + 1] = string.Format(CultureInfo.InvariantCulture, "{0}[{1}]", Math.Round(vr, 6), Math.Round(vi, 6));
					}
					tbl.Rows.Add(vs);
				}
				br.Close();
				dataGridView1.DataSource = tbl;
				dataGridView1.Columns[0].Frozen = true;
				return true;
			}
			return false;
		}

	}
}
