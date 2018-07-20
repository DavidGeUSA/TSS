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
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Draw2D
{
	/// <summary>
	/// UI for FFT parameters
	/// </summary>
	public partial class FormParam : Form
	{
		public FormParam()
		{
			InitializeComponent();
		}
		public void DisableLh()
		{
			txtLh.Enabled = false;
		}
		public UInt32 N
		{
			get
			{
				return Convert.ToUInt32(txtN.Text);
			}
			set
			{
				txtN.Text = value.ToString(CultureInfo.InvariantCulture);
			}
		}
		public double Lh
		{
			get
			{
				return Convert.ToDouble(txtLh.Text);
			}
			set
			{
				txtLh.Text = value.ToString(CultureInfo.InvariantCulture);
			}
		}
		public static bool AskForHalfYeeCells(Form owner, ref UInt32 HalfYeeCells)
		{
			FormParam frm = new FormParam();
			frm.N = HalfYeeCells;
			frm.DisableLh();
			if (frm.ShowDialog(owner) == System.Windows.Forms.DialogResult.OK)
			{
				HalfYeeCells = frm.N;
				return true;
			}
			return false;
		}
		public static bool AskForGeometry(Form owner, ref UInt32 HalfYeeCells, ref double HalfFFTrange)
		{
			FormParam frm = new FormParam();
			frm.N = HalfYeeCells;
			frm.Lh = HalfFFTrange;
			if (frm.ShowDialog(owner) == System.Windows.Forms.DialogResult.OK)
			{
				HalfYeeCells = frm.N;
				HalfFFTrange = frm.Lh;
				return true;
			}
			return false;
		}
	}
}
