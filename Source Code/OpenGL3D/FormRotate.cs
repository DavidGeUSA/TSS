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

namespace Field3D
{
	/*
	  UI for rotating and shifting 3D view
	 */
	public partial class FormRotate : Form
	{
		SharpGLForm fOwner = null;
		double viewSize = 0;
		int viewSizeN = 0;
		bool _synching = false;
		public FormRotate()
		{
			InitializeComponent();
		}
		public void SetDrawingOwner(SharpGLForm owner)
		{
			double s;
			fOwner = owner;
			//
			_synching = true;
			//
			viewSize = fOwner.GetViewSize();
			viewSizeN = (int)viewSize;
			//
			hsbShiftX.Minimum = -viewSizeN;
			hsbShiftX.Maximum = viewSizeN;
			s = fOwner.GetShiftX();
			txtShiftX.Text = s.ToString(CultureInfo.InvariantCulture);
			hsbShiftX.Value = (int)s;
			//
			hsbShiftY.Minimum = -viewSizeN;
			hsbShiftY.Maximum = viewSizeN;
			s = fOwner.GetShiftY();
			txtShiftY.Text = s.ToString(CultureInfo.InvariantCulture);
			hsbShiftY.Value = (int)s;
			//
			hsbShiftZ.Minimum = -viewSizeN;
			hsbShiftZ.Maximum = viewSizeN;
			s = fOwner.GetShiftZ();
			txtShiftZ.Text = s.ToString(CultureInfo.InvariantCulture);
			hsbShiftZ.Value = (int)s;
			//
			_synching = false;
		}
		private void btRotateX_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateX.Text);
				fOwner.RotateOnX(-a);
			}
			catch
			{
			}
		}

		private void btRotateY_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateY.Text);
				fOwner.RotateOnY(-a);
			}
			catch
			{
			}
		}

		private void btRotateZ_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateZ.Text);
				fOwner.RotateOnZ(-a);
			}
			catch
			{
			}
		}
		private void btRotateXPositive_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateX.Text);
				fOwner.RotateOnX(a);
			}
			catch
			{
			}
		}

		private void btRotateYPositive_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateY.Text);
				fOwner.RotateOnY(a);
			}
			catch
			{
			}
		}

		private void btRotateZPositive_Click(object sender, EventArgs e)
		{
			try
			{
				double a = Convert.ToDouble(txtRotateZ.Text);
				fOwner.RotateOnZ(a);
			}
			catch
			{
			}
		}

		private void btResetRotate_Click(object sender, EventArgs e)
		{
			try
			{
				fOwner.ResetRotation();
			}
			catch
			{
			}
		}

		private void txtShiftX_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double s = Convert.ToDouble(txtShiftX.Text);
					if (s > -viewSizeN && s < viewSizeN)
					{
						hsbShiftX.Value = (int)s;
						fOwner.SetShiftX(s);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void txtShiftY_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double s = Convert.ToDouble(txtShiftY.Text);
					if (s > -viewSizeN && s < viewSizeN)
					{
						hsbShiftY.Value = (int)s;
						fOwner.SetShiftY(s);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void txtShiftZ_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double s = Convert.ToDouble(txtShiftZ.Text);
					if (s > -viewSizeN && s < viewSizeN)
					{
						hsbShiftZ.Value = (int)s;
						fOwner.SetShiftZ(s);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void hsbShiftX_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					int n = hsbShiftX.Value;
					txtShiftX.Text = n.ToString(CultureInfo.InvariantCulture);
					fOwner.SetShiftX((double)n);
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void hsbShiftY_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					int n = hsbShiftY.Value;
					txtShiftY.Text = n.ToString(CultureInfo.InvariantCulture);
					fOwner.SetShiftY((double)n);
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void hsbShiftZ_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					int n = hsbShiftZ.Value;
					txtShiftZ.Text = n.ToString(CultureInfo.InvariantCulture);
					fOwner.SetShiftZ((double)n);
				}
				catch
				{
				}
				_synching = false;
			}
		}

	}
}
