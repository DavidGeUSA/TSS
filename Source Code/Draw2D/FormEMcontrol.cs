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
	/// UI for setting time step index and scales
	/// </summary>
	public partial class FormEMcontrol : Form
	{
		private FormDraw2D _owner;
		private bool _synching = false;
		public FormEMcontrol()
		{
			InitializeComponent();
		}
		public void SetDrawingOwner(FormDraw2D o)
		{
			_synching = true;
			double v;
			_owner = o;
			nudTime.Minimum = 0;
			nudTime.Maximum = _owner.GetMaxTimeIndex();
			nudTime.Value = 0;
			v = _owner.Scale1;
			txtScale1.Text = v.ToString(CultureInfo.InvariantCulture);
			vs1.Value = scaleToVs(v);
			v = _owner.Scale2;
			txtScale2.Text = v.ToString(CultureInfo.InvariantCulture);
			vs2.Value = scaleToVs(v);
			_synching = false;

		}
		private void nudTime_ValueChanged(object sender, EventArgs e)
		{
			try
			{
				if (!_synching)
				{
					_synching = true;
					_owner.ShowEMErrorPage((int)nudTime.Value);
					nudTime.Value = _owner.GetCurrentTimeIndex();
					_synching = false;
				}
			}
			catch
			{
			}
		}

		private void txtScale1_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double r = Convert.ToDouble(txtScale1.Text);
					if (r > 0)
					{
						int vs = scaleToVs(r);
						vs1.Value = vs;
						_owner.SetEMScale1(r);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void txtScale2_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double r = Convert.ToDouble(txtScale2.Text);
					if (r > 0)
					{
						int vs = scaleToVs(r);
						vs2.Value = vs;
						_owner.SetEMScale2(r);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}
		double fscale = 0.1;
		/*
		 * minimum = 1; maximum = 1000
		 * mean = 500 coresponds to scale = 1
		 * value > 500 : scale > 1 =>  1+10^((value-500)*0.1)
		 * value < 500 : scale < 1 =>  10^((value-500)*0.1)
		 * 
		 */
		private double vsToScale(int vs)
		{
			if (vs == 500)
				return 1.0;
			//make value upside down:
			vs = 1000 - vs;
			if (vs > 500)
			{
				return 1.0 + Math.Pow(10.0, ((double)vs - 500.0) * fscale);
			}
			return Math.Pow(10.0, ((double)vs - 500.0) * fscale);
		}
		private int scaleToVs(double s)
		{
			if (s == 1.0)
				return 500;
			if (s > 1.0)
			{
				//make value upside down
				int vs = -(int)(Math.Log10((s - 1.0) / fscale))+500;
				return vs;
			}
			else
			{
				//make value upside down
				int vs = -(int)(Math.Log10((s) / fscale)) + 500;
				return vs;
			}
		}
		private void vs1_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double sc = vsToScale(vs1.Value);
					txtScale1.Text = sc.ToString(CultureInfo.InvariantCulture);
					_owner.SetEMScale1(sc);
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void vs2_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					double sc = vsToScale(vs2.Value);
					txtScale2.Text = sc.ToString(CultureInfo.InvariantCulture);
					_owner.SetEMScale2(sc);
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void txtExcludeR_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					int r = Convert.ToInt32(txtExcludeR.Text);
					int r0 = _owner.SetExcludeRadiuses(r);
					if (r0 != r)
					{
						txtExcludeR.Text = r0.ToString(CultureInfo.InvariantCulture);
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

	}
}
