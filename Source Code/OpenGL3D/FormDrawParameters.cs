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
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	/// <summary>
	/// UI for adjusting drawing parameters
	/// </summary>
	public partial class FormDrawParameters : Form
	{
		#region fields and constructors
		private SharpGLForm drawingForm;
		private bool _synching = false;
		public FormDrawParameters()
		{
			InitializeComponent();
		}
		#endregion

		#region update parameters
		public void SetDrawingForm(SharpGLForm form)
		{
			drawingForm = form;
			btApplySkipRadius.Enabled = form.IsInSphere();
		}
		public void SetParameters(double p, double g, double v, UInt32 skipRadius, UInt32 skipSpace, bool radiusOnly, UInt32 radius, UInt32 maxRadius, EMShowSelect em)
		{
			_synching = true;
			if (p >= 0 && p <= 100)
			{
				txtPercent.Text = p.ToString(CultureInfo.InvariantCulture);
				numericUpDown1.Value = Convert.ToDecimal(p);
				hScrollBar1.Value = Convert.ToInt32(p);
			}
			if (g > 0)
			{
				txtGridSize.Text = g.ToString(CultureInfo.InvariantCulture);
				numericUpDown2.Value = Convert.ToDecimal(g);
			}
			if (v > 0)
			{
				txtViewSize.Text = v.ToString(CultureInfo.InvariantCulture);
				numericUpDown3.Value = Convert.ToDecimal(v);
			}
			txtSkipRadius.Text = skipRadius.ToString(CultureInfo.InvariantCulture);
			txtSkipSpace.Text = skipSpace.ToString(CultureInfo.InvariantCulture);
			txtRadius.Text = radius.ToString(CultureInfo.InvariantCulture);
			chkRadiusOnly.Checked = radiusOnly;
			hScrollBarRadius.Minimum = 0;
			hScrollBarRadius.Maximum = (int)maxRadius;
			hScrollBarRadius.Value = (int)radius;
			cbEMShow.SelectedIndex = (int)em;
			_synching = false;
		}
		public void SetFitFactors(double locationFactor, double lengthFactor)
		{
			_synching = true;
			txtLengthZoom.Text = lengthFactor.ToString(CultureInfo.InvariantCulture);
			txtSpaceZoom.Text = locationFactor.ToString(CultureInfo.InvariantCulture);
			hsbLengthZoom.Value = Convert.ToInt32(lengthFactor);
			hsbLocationZoom.Value = Convert.ToInt32(locationFactor);
			_synching = false;
		}
		public void SetRadiusOnly(UInt32 r)
		{
			hScrollBarRadius.Minimum = 0;
			hScrollBarRadius.Maximum = (int)r;
		}
		private void updateDrawingForm()
		{
			if (drawingForm != null)
			{
				drawingForm.SetLengthPercent(Convert.ToDouble(numericUpDown1.Value)/100.0);
			}
		}
		private void updateDrawingFormGridSize()
		{
			if (drawingForm != null)
			{
				drawingForm.SetGridSize(Convert.ToDouble(numericUpDown2.Value));
			}
		}
		private void updateDrawingFormViewSize()
		{
			if (drawingForm != null)
			{
				drawingForm.SetViewSize(Convert.ToDouble(numericUpDown3.Value));
			}
		}
		private void updateDrawingFormSkipRadius(UInt32 r, UInt32 s, UInt32 o, bool b)
		{
			if (drawingForm != null)
			{
				drawingForm.SetSkipRadiusSpace(r, s);
				drawingForm.SetRadiusOnly(b, o);
				if (drawingForm.RadiusToShow != o)
				{
					bool sync = _synching;
					_synching = true;
					txtRadius.Text = drawingForm.RadiusToShow.ToString(CultureInfo.InvariantCulture);
					_synching = sync;
				}
			}
		}
		public void updateDrawingFormRadiusOnly(bool b, UInt32 r)
		{
			if (drawingForm != null)
			{
				drawingForm.SetRadiusOnly(b, r);
			}
		}
		public void updateDrawingFormEMSelect(EMShowSelect em)
		{
			if (drawingForm != null)
			{
				drawingForm.SetEMSelect(em);
			}
		}
		#endregion

		#region event handlers
		private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				txtPercent.Text = hScrollBar1.Value.ToString(CultureInfo.InvariantCulture);
				numericUpDown1.Value = hScrollBar1.Value;
				_synching = false;
				updateDrawingForm();
			}
		}

		private void numericUpDown1_ValueChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				hScrollBar1.Value = Convert.ToInt32(numericUpDown1.Value);
				txtPercent.Text = numericUpDown1.Value.ToString(CultureInfo.InvariantCulture);
				_synching = false;
				updateDrawingForm();
			}
		}
		private void txtPercent_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				if (!_synching)
				{
					_synching = true;
					try
					{
						decimal d = Convert.ToDecimal(txtPercent.Text);
						if (d >= 0 && d <= 100)
						{
							numericUpDown1.Value = d;
							hScrollBar1.Value = Convert.ToInt32(d);
							updateDrawingForm();
						}
					}
					catch
					{
					}
					_synching = false;
				}
			}
		}

		private void btClode_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void txtGridSize_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					decimal d = Convert.ToDecimal(txtGridSize.Text);
					if (d > 0)
					{
						numericUpDown2.Value = d;
						updateDrawingFormGridSize();
					}
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void numericUpDown2_ValueChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					txtGridSize.Text = numericUpDown2.Value.ToString(CultureInfo.InvariantCulture);
					updateDrawingFormGridSize();
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void txtViewSize_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					decimal d = Convert.ToDecimal(txtViewSize.Text);
					if (d > 0)
					{
						numericUpDown3.Value = d;
						updateDrawingFormViewSize();
					}
				}
				catch
				{
				}
				_synching = false;
			}

		}

		private void numericUpDown3_ValueChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					txtViewSize.Text = numericUpDown3.Value.ToString(CultureInfo.InvariantCulture);
					updateDrawingFormViewSize();
				}
				catch
				{
				}
				_synching = false;
			}
		}

		private void btClear_Click(object sender, EventArgs e)
		{
			if (drawingForm != null)
			{
				drawingForm.ClearVectors();
			}
		}

		private void btApplySkipRadius_Click(object sender, EventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				try
				{
					UInt32 r = Convert.ToUInt32(txtSkipRadius.Text);
					UInt32 s = Convert.ToUInt32(txtSkipSpace.Text);
					UInt32 o = Convert.ToUInt32(txtRadius.Text);
					bool b = chkRadiusOnly.Checked;
					txtSkipSpace.Text = s.ToString(CultureInfo.InvariantCulture);
					txtSkipRadius.Text = r.ToString(CultureInfo.InvariantCulture);
					updateDrawingFormSkipRadius(r, s, o, b);
				}
				catch
				{
				}
				_synching = false;
			}
		}
		private void txtRadius_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				if (!_synching)
				{
					_synching = true;
					try
					{
						UInt32 r = Convert.ToUInt32(txtRadius.Text);
						if (r <= hScrollBarRadius.Maximum)
						{
							hScrollBarRadius.Value = (int)r;
							updateDrawingFormRadiusOnly(chkRadiusOnly.Checked, r);
						}
					}
					catch
					{
					}
					_synching = false;
				}
			}
		}

		private void hScrollBarRadius_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				_synching = true;
				txtRadius.Text = hScrollBarRadius.Value.ToString(CultureInfo.InvariantCulture);
				_synching = false;
				updateDrawingFormRadiusOnly(chkRadiusOnly.Checked, (UInt32)hScrollBarRadius.Value);
			}
		}

		private void cbEMShow_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				updateDrawingFormEMSelect((EMShowSelect)(cbEMShow.SelectedIndex));
			}
		}

		private void txtSpaceZoom_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				try
				{
					double f = Convert.ToDouble(txtSpaceZoom.Text);
					if (f > 0.0 && f <= 100.0)
					{
						hsbLocationZoom.Value = Convert.ToInt32(txtSpaceZoom.Text);
						drawingForm.SetLocationFitFactor(f / 100.0);
					}
				}
				catch
				{
				}
			}
		}

		private void txtLengthZoom_TextChanged(object sender, EventArgs e)
		{
			if (!_synching)
			{
				try
				{
					double f = Convert.ToDouble(txtLengthZoom.Text);
					if (f > 0.0 && f <= 100.0)
					{
						hsbLengthZoom.Value = Convert.ToInt32(txtLengthZoom.Text);
						drawingForm.SetLengthFitFactor(f / 100.0);
					}
				}
				catch
				{
				}
			}
		}

		private void hsbLocationZoom_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				try
				{
					double f = Convert.ToDouble(hsbLocationZoom.Value);
					if (f > 0.0 && f <= 100.0)
					{
						txtSpaceZoom.Text = hsbLocationZoom.Value.ToString(CultureInfo.InvariantCulture);
						drawingForm.SetLocationFitFactor(f / 100.0);
					}
				}
				catch
				{
				}
			}
		}

		private void hsbLengthZoom_Scroll(object sender, ScrollEventArgs e)
		{
			if (!_synching)
			{
				try
				{
					double f = Convert.ToDouble(hsbLengthZoom.Value);
					if (f > 0.0 && f <= 100.0)
					{
						txtLengthZoom.Text = hsbLengthZoom.Value.ToString(CultureInfo.InvariantCulture);
						drawingForm.SetLengthFitFactor(f / 100.0);
					}
				}
				catch
				{
				}
			}
		}


		#endregion

		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);
			int w = 1;
			int h = btClode.Top - 10;
			e.Graphics.DrawLine(Pens.Black, w,     h,     this.ClientSize.Width - 2, h);
			e.Graphics.DrawLine(Pens.White, w + 2, h + 1, this.ClientSize.Width, h + 1);
			e.Graphics.DrawLine(Pens.White, w + 2, h + 2, this.ClientSize.Width, h + 2);
		}

	}
}
