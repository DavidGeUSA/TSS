/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	/// <summary>
	/// UI for controlling display of data files.
	/// 1. each data file holds EM field data for one time step, go through data files to see fields evolve over time
	/// 2. adjust space location and vector size zooming for better viewes
	/// </summary>
	public partial class FormDataFilesControl : Form
	{
		#region fields and constructors
		public SharpGLForm drawingForm;
		private bool _synching = false;
		private string filenamebase;
		private string fileext;
		private EMFileFormat emFormat = EMFileFormat.EMinSohere;
		public FormDataFilesControl()
		{
			InitializeComponent();
		}
		#endregion

		#region interface
		public bool SetDataFile(SharpGLForm form, string filename)
		{
			bool bRet = false;
			drawingForm = form;
			if (!string.IsNullOrEmpty(filename))
			{
				int n;
				string dir = Path.GetDirectoryName(filename);
				fileext = Path.GetExtension(filename);
				filenamebase = Path.GetFileNameWithoutExtension(filename);
				if (!string.IsNullOrEmpty(filenamebase))
				{
					n = filenamebase.LastIndexOf('_');
					if (n > 0)
					{
						int nl = filenamebase.Length - n - 1;
						string ts = filenamebase.Substring(n + 1, nl);
						try
						{
							int tIdx = Convert.ToInt32(ts);
							if (tIdx >= 0)
							{
								nupTimeStepIndex.Value = tIdx;
								filenamebase = Path.Combine(dir, filenamebase.Substring(0, n + 1));
								bRet = true;
								if (string.CompareOrdinal(fileext, ".em") == 0)
								{
									emFormat = EMFileFormat.EMinSohere;
								}
								else if (string.CompareOrdinal(fileext, ".field") == 0)
								{
									emFormat = EMFileFormat.EMfieldPoints;
								}
								else
								{
									bRet = false;
									MessageBox.Show(this, "Unsupported file format", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
								}
							}
						}
						catch
						{
						}
					}
				}
			}
			return bRet;
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
		#endregion

		#region event handlers
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

		private void nupTimeStepIndex_ValueChanged(object sender, EventArgs e)
		{
			try
			{
				int n = Convert.ToInt32(nupTimeStepIndex.Value);
				string file = string.Format(CultureInfo.InvariantCulture, "{0}{1}{2}", filenamebase, n, fileext);
				if (File.Exists(file))
				{
					if (emFormat == EMFileFormat.EMinSohere)
						drawingForm.LoadEMDataFromFile(file);
					else if (emFormat == EMFileFormat.EMfieldPoints)
						drawingForm.LoadEMFieldFromFile(file);
				}
			}
			catch
			{
			}
		}
		#endregion
	}
}
