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
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Globalization;
using Draw3D;

namespace Draw2D
{
	/// <summary>
	/// 2D drawing form
	/// </summary>
	public partial class FormDraw2D : Form
	{
		string filename = null;
		long N = 0;
		int dataType = 0;
		double Ymax = 0;
		double Xmax = 0;
		double Ymin = 0;
		double[] Y = null;
		double[] X = null;
		double[] Z = null;
		double[] W = null;
		double[] V = null;
		double[] S = null;
		string[] D = null;
		int marginX = 80; //pixels
		int marginTop = 60;
		int marginBottom = 30;
		int xLabel = 10;
		float dotSize = 0.5f;
		ChartType chartType = ChartType.Line;
		FormEMperform _frmEMP = null;
		//
		FormDrawingData _frmDrawingData = null;
		//------------------------------------------------------
		FormEMcontrol _frmEMcontrol = null;
		UInt32 _dataNum = 0;
		private ErrorsOverTime _divergDiffs = null;
		//-----------------------------------------------------
		int linesLoaded = 0;
		//
		public FormDraw2D()
		{
			InitializeComponent();
		}
		public void SetEMPscale(int index, double value)
		{
			if (Y != null && index >= 0 && index < Y.Length)
			{
				Y[index] = value;
				Ymax = 0;
				for (int i = 0; i < Y.Length; i++)
				{
					if (Y[i] > Ymax) Ymax = Y[i];
				}
				this.Refresh();
			}
		}
		#region Interface with FormEMcontrol
		public int GetMaxTimeIndex()
		{
			if (_divergDiffs != null)
			{
				return _divergDiffs.TimeCount;
			}
			return 0;
		}
		public int GetCurrentTimeIndex()
		{
			if (_divergDiffs == null)
				return 0;
			return _divergDiffs.GetCurrentTime();
		}
		public int GetExcludeRadiuses()
		{
			if (_divergDiffs == null)
				return 0;
			return _divergDiffs.GetExcluderadiuses();
		}
		public int SetExcludeRadiuses(int r)
		{
			if (_divergDiffs == null)
				return 0;
			_divergDiffs.SetExcludeRadius(r);
			ShowEMErrorPage(_divergDiffs.GetCurrentTime());
			return _divergDiffs.GetExcluderadiuses();
		}
		public void ShowEMErrorPage(int t)
		{
			if (_divergDiffs != null && t >= 0 && t < _divergDiffs.TimeCount)
			{
				ErrorsOnRadiuses eor = _divergDiffs[t];
				int rs = _divergDiffs.DisplayRadiusCount;
				if (X == null || X.Length != rs)
				{
					X = new double[rs];
				}
				if (Y == null || Y.Length != rs)
				{
					Y = new double[rs];
				}
				if (Z == null || Z.Length != rs)
				{
					Z = new double[rs];
				}
				W = null;
				V = null;
				S = null;
				for (int r = 0; r < rs; r++)
				{
					X[r] = r;
					Y[r] = eor.Diverg1(r);
					Z[r] = eor.Diverg2(r);
				}
				Ymin = 0.0;
				Ymax = eor.GetMaxValue();
				Xmax = rs;
				N = rs;
				this.Refresh();
			}
		}
		public void SetEMScale1(double r)
		{
			if (_divergDiffs != null && r > 0.0)
			{
				if (r != _divergDiffs.Scale1)
				{
					_divergDiffs.SetScale1(r);
					ShowEMErrorPage(_divergDiffs.GetCurrentTime());
				}
			}
		}
		public void SetEMScale2(double r)
		{
			if (_divergDiffs != null && r > 0.0)
			{
				if (r != _divergDiffs.Scale2)
				{
					_divergDiffs.SetScale2(r);
					ShowEMErrorPage(_divergDiffs.GetCurrentTime());
				}
			}
		}
		public double Scale1
		{
			get
			{
				if (_divergDiffs != null)
					return _divergDiffs.Scale1;
				return 1.0;
			}
		}
		public double Scale2
		{
			get
			{
				if (_divergDiffs != null)
					return _divergDiffs.Scale2;
				return 1.0;
			}
		}
		#endregion
		#region Interface with FormDrawingData
		public void UpdateDataFilename()
		{
			if (_frmDrawingData != null && !_frmDrawingData.IsDisposed && !_frmDrawingData.Disposing)
			{
				_frmDrawingData.ShowFileName(filename);
			}
		}

		public UInt32 DataNum
		{
			get { return _dataNum; }
		}

		public bool SetN(UInt32 n)
		{
			try
			{
				if (X != null)
				{
					if (n > 0 && n < X.Length)
					{
						N = n;
						this.Refresh();
						return true;
					}
				}
			}
			catch
			{
			}
			return false;
		}
		public int GetXlength()
		{
			if (X == null)
				return 0;
			return X.Length;
		}
		public void LoadData()
		{
			FormData f = new FormData();
			if (Y != null && Z != null && Y.Length == Z.Length && W == null && V == null && S == null && dataType == 1)
			{
				double vr, vi;
				W = new double[Y.Length];
				V = new double[Y.Length];
				S = new double[Y.Length];
				for (int i = 0; i < Y.Length; i++)
				{
					vr = Y[i] * Y[i];
					vi = Z[i] * Z[i];
					S[i] = vr + vi;
					if (S[i] > 1e-9)
					{
						W[i] = Math.Round(vr / S[i], 4);
						V[i] = Math.Round(vi / S[i], 4);
					}
					else
					{
						W[i] = 0;
						V[i] = 0;
					}
				}
			}
			f.LoadData(Y, Z, X, W, V, S);
			f.Text = Path.GetFileName(filename);
			f.Show(this);
		}
		public void ShowNextPage()
		{
			if (nextEndIndex < N)
			{
				lastEndIndex = nextEndIndex;
				this.Refresh();
			}
		}

		public void ShowPreviousPage()
		{
			if (lastEndIndex > 0)
			{
				lastEndIndex -= (this.ClientSize.Width - 2 * marginX);
				if (lastEndIndex < 0)
					lastEndIndex = 0;
				this.Refresh();
			}
		}

		#endregion
		protected override void OnMouseMove(MouseEventArgs e)
		{
			base.OnMouseMove(e);
			if (_frmDrawingData != null && !_frmDrawingData.IsDisposed && !_frmDrawingData.Disposing)
			{
				if (N > 0)
				{
					int nX = e.X - marginX;
					if (nX >= 0)
					{
						int nPageSize = this.ClientSize.Width - 2 * marginX;
						int xIndex = lastEndIndex + nX / dx;
						if (xIndex >= 0 && xIndex < N)
						{
							_frmDrawingData.ShowXvalue(X[xIndex]);
						}
					}
				}
			}
		}
		protected override void OnResize(EventArgs e)
		{
			base.OnResize(e);
			this.Refresh();
		}
		//these two values are for page navigations; next page and previous page
		private int lastEndIndex = 0; //the last item index of displayed items; it should start from 0. it is the starting index of the current page
		private int nextEndIndex = 0; //= lastEndIndex + items currently displayed
		private int pageSize = 0; //width in pixels for the drawing area
		private int dx = 1; //pixels of x-interval
		/// <summary>
		/// make 2D drawing
		/// </summary>
		/// <param name="e"></param>
		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);
			if (N > 0) //data loaded
			{
				//make sure Ymin < Ymax if not both Ymax=0 and Ymin=0
				if (Ymin >= Ymax)
				{
					if (Ymax != 0)
					{
						//if Ymax > 0 then Ymin = 0
						//if Ymax < 0 then Ymin = 2 * Ymax
						Ymin = Ymax - Math.Abs(Ymax);
					}
					else if (Ymin != 0)
					{
						Ymax = Ymin;
						Ymin = 0;
					}
					else
					{
						Ymax = 1.0;
					}
				}
				int h = this.ClientSize.Height - marginTop - marginBottom; //display height
				//page size in pixels
				pageSize = this.ClientSize.Width - 2 * marginX; //display width
				if (_frmDrawingData != null && !_frmDrawingData.IsDisposed && !_frmDrawingData.Disposing)
				{
					_frmDrawingData.ShowPageSize(pageSize);
				}
				if (pageSize > 10 && h > 0) //drawing area is big enough
				{
					//draw axises and labels ========================================================================
					float dotHalf = dotSize / 2f;
					float x, ya, za, wa, va, sa, x0 = 0, y0 = 0, z0 = 0, w0 = 0, v0 = 0, s0 = 0;
					int yMinpos = this.ClientSize.Height - marginBottom; //minimum Y display position
					int yMaxPos = marginTop; //maximum Y display position
					int yMiddlePos = yMinpos; //horizontal line position
					double r = (Ymax - Ymin) / h; //ratio of data magnitude vs display height
					SizeF sF = e.Graphics.MeasureString("8", this.Font);
					int yLabelInterval = (int)(sF.Height + 3); //vertical label display interval
					int numY = (int)(h / yLabelInterval); //number of vertical labels
					//horizontal line position
					if (Ymin < 0 && Ymax > 0)
					{
						//X-axis at 0
						yMiddlePos = yMinpos + (int)(Ymin / r);
					}
					else
					{
						//X-axis at half position
						yMiddlePos = yMinpos + (int)((2 * Ymin - Ymax) / r);
					}
					//draw Y-axis line
					e.Graphics.DrawLine(Pens.Black, marginX, marginTop, marginX, yMinpos);
					////draw Y-labels
					int i = 0;
					double y = Ymin;
					int yPos = yMinpos; //y-position is calculated by yMinpos + (int)((Ymin - y) / r);
					while (true)
					{
						e.Graphics.DrawString(doubleToStr(y), this.Font, Brushes.Black, (float)xLabel, (float)yPos);
						yPos -= yLabelInterval;
						if (yPos < marginTop) break;
						y = Ymin + (yMinpos - yPos) * r; //calculate y value from y-position 
						if (y > Ymax) break;
						i++;
						if (i > numY) break;
					}
					//draw Bottom X-axis line
					e.Graphics.DrawLine(Pens.Black, marginX, yMinpos, this.ClientSize.Width - marginX, yMinpos);
					//draw middle X-axis line
					if (Ymin < 0 && Ymax > 0)
					{
						e.Graphics.DrawLine(Pens.Black, marginX, yMiddlePos, this.ClientSize.Width - marginX, yMiddlePos);
					}
					//===================================================================================================
					//calculate items to be displayed----------------------------------------------
					dx = 1; //assume one pixel for one item, x-interval is 1
					int endIndex = pageSize; //end index is the page size in pixels
					int items = (int)N - lastEndIndex; //items to be displayed
					if (items > 0) //there are items to be displayed
					{
						if (items < pageSize) //less items to be displayed in a page
						{
							dx = pageSize / (int)items; //x-interval can be more than one pixel 
							endIndex = (int)items;
						}
					}
					else //not items to be displayed
					{
						endIndex = 0;
					}
					//------------------------------------------------------------------------------
					int dxLabel = 40; //x-label interval
					double lastLabelX = 0;
					if (Xmax <= 0) Xmax = 1;
					double rX = pageSize / Xmax;
					//draw data
					for (i = 0; i < endIndex; i++)
					{
						x = (float)(marginX + i * dx); //x-axis location
						ya = yMinpos + (int)((Ymin - Y[i + lastEndIndex]) / r); //y-location for y data
						if (chartType == ChartType.Line)
						{
							//draw a dot to represent y data
							e.Graphics.DrawEllipse(Pens.Red, x - dotHalf, ya - dotHalf, dotSize, dotSize); //draw a dot
							if (i == 0)
							{
							}
							else
							{
								//Red line is for Y
								//x0, y0 are only available for i > 0 
								e.Graphics.DrawLine(Pens.Red, x0, y0, x, ya); //draw a line from the previous dot
							}
						}
						else if (chartType == ChartType.Text)
						{
							if (D != null)
							{
								x = (float)( X[i + lastEndIndex] * rX);
								e.Graphics.DrawString(D[i + lastEndIndex],this.Font, Brushes.Blue, x, ya); 
							}
						}
						y0 = ya;
						if (Z != null) //draw the second data if available
						{
							za = yMinpos + (int)((Ymin - Z[i + lastEndIndex]) / r); //y-location for z data
							e.Graphics.DrawEllipse(Pens.Blue, x - dotHalf, za - dotHalf, dotSize, dotSize); //draw a dot
							if (i == 0)
							{
							}
							else
							{
								//Blue line is for Z
								//x0 and z0 are only available for i > 0
								e.Graphics.DrawLine(Pens.Blue, x0, z0, x, za); //draw a line from the previous dot
							}
							z0 = za;
						}
						if (W != null) //draw the third data if available
						{
							wa = yMinpos + (int)((Ymin - W[i + lastEndIndex]) / r); //y-location for w data
							e.Graphics.DrawEllipse(Pens.Green, x - dotHalf, wa - dotHalf, dotSize, dotSize); //draw a dot
							if (i == 0)
							{
							}
							else
							{
								//Green line is for W
								//x0 and w0 are only available for i > 0
								e.Graphics.DrawLine(Pens.Green, x0, w0, x, wa); //draw a line from the previous dot
							}
							w0 = wa;
						}
						if (V != null) //draw the fourth data if available
						{
							va = yMinpos + (int)((Ymin - V[i + lastEndIndex]) / r); //y-location for v data
							e.Graphics.DrawEllipse(Pens.Violet, x - dotHalf, va - dotHalf, dotSize, dotSize); //draw a dot
							if (i == 0)
							{
							}
							else
							{
								//Violet line is for V
								//x0 and v0 are only available for i > 0
								e.Graphics.DrawLine(Pens.Violet, x0, v0, x, va); //draw a line from the previous dot
							}
							v0 = va;
						}
						if (S != null) //draw the fifth data if available
						{
							sa = yMinpos + (int)((Ymin - S[i + lastEndIndex]) / r); //y-location for s data
							e.Graphics.DrawEllipse(Pens.Yellow, x - dotHalf, sa - dotHalf, dotSize, dotSize); //draw a dot
							if (i == 0)
							{
							}
							else
							{
								//Yellow line is for S
								//x0 and v0 are only available for i > 0
								e.Graphics.DrawLine(Pens.Yellow, x0, s0, x, sa); //draw a line from the previous dot
							}
							s0 = sa;
						}
						x0 = x;
						if (chartType == ChartType.Line)
						{
							if ((x - lastLabelX) >= dxLabel) //need to draw a x-label
							{
								e.Graphics.DrawString(X[i + lastEndIndex].ToString("##.##", CultureInfo.InvariantCulture), this.Font, Brushes.Black, x, (float)(yMinpos));
								lastLabelX = x;
							}
						}
						else if (chartType == ChartType.Text)
						{
							e.Graphics.DrawString(X[i + lastEndIndex].ToString("##.##", CultureInfo.InvariantCulture), this.Font, Brushes.Black, x, (float)(yMinpos));
						}
					}
					nextEndIndex = lastEndIndex + endIndex;
				}
			}
			else
			{
				//data not loaded
				string s = "Right-click here to load data";
				e.Graphics.DrawString(s, this.Font, Brushes.Blue, ((float)this.ClientSize.Width - e.Graphics.MeasureString(s,this.Font).Width) / (float)2.0, (float)(this.ClientSize.Height / 2));
			}
		}

		private bool askForFilename(string title)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.CheckFileExists = true;
			dlg.Title = title;
			try
			{
				dlg.FileName = filename;
			}
			catch
			{
			}
			if (dlg.ShowDialog(this) == DialogResult.OK)
			{
				filename = dlg.FileName;
				UpdateDataFilename();
				W = null;
				V = null;
				S = null;
				dataType = 0;
				return true;
			}
			return false;
		}

		private string doubleToStr(double v)
		{
			double av = Math.Abs(v);
			int n = 0;
			string s;
			if (v != 0)
			{
				if (v > 99)
				{
					// v = (v * 10^(-n)) * 10^n
					// (v * 10^(-n))= v / 10^n < 90
					// v / 90 < 10^n
					// n = log10(v/90)
					n = (int)(Math.Log10(av / 90.0));
					v = v / Math.Pow(10.0, (double)n);
				}
				else if (v < 0.01)
				{
					// v = (v * 10^n) * 10^(-n)
					// v * 10^n > 90
					// v / 90 > 10^(-n)
					// -n = log10(v/90)
					n = (int)(Math.Log10(av / 90.0));
					v = v * Math.Pow(10.0, (double)(-n));
				}
				s = v.ToString("##.##", CultureInfo.InvariantCulture);
				if (n > 0)
				{
					s = string.Format(CultureInfo.InvariantCulture, "{0}e{1}", s, n);
				}
				else if (n < 0)
				{
					s = string.Format(CultureInfo.InvariantCulture, "{0}e-{1}", s, -n);
				}
			}
			else
			{
				s = "0.0";
			}
			return s;
		}

		private void dComplexToComplexToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if(askForFilename("File of 1-D Complex to Complex FFT"))
			{
				UInt32 dataNum = _dataNum;
				if (FormParam.AskForHalfYeeCells(this, ref dataNum))
				{
					FormData f = new FormData();
					if (f.Load1DComplex(filename, dataNum))
					{
						f.Show(this);
					}
				}
			}
		}

		private void dRealToComplexToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if(askForFilename("File of Real to Complex FFT"))
			{
				UInt32 dataNum = _dataNum;
				if (FormParam.AskForHalfYeeCells(this, ref dataNum))
				{
					FormData f = new FormData();
					if (f.Load1DReal(filename, dataNum))
					{
						f.Show(this);
					}
				}
			}
		}

		private void dComplexToComplexToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if(askForFilename("File of 2-D Complex to Complex FFT"))
			{
				UInt32 dataNum = _dataNum;
				if (FormParam.AskForHalfYeeCells(this, ref dataNum))
				{
					FormData f = new FormData();
					if (f.Load2DComplex(filename, dataNum))
					{
						f.Show(this);
					}
				}
			}
		}

		private void dRealToComplexToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if (askForFilename("File of 2-D Real to Complex FFT"))
			{
				UInt32 dataNum = _dataNum;
				if (FormParam.AskForHalfYeeCells(this, ref dataNum))
				{
					FormData f = new FormData();
					if (f.Load2DReal(filename, dataNum))
					{
						f.Show(this);
					}
				}
			}
		}

		private void dComplexFileToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename("1-D Frequency : Complex strength"))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = sizeof(UInt32) + 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					long itemCount = size / itemSize;
					UInt32[] fqs = new UInt32[itemCount];
					double[] fRs = new double[itemCount];
					double[] fIs = new double[itemCount];
					Ymax = double.MinValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (long i = 0; i < itemCount; i++)
					{
						fqs[i] = br.ReadUInt32();
						fRs[i] = br.ReadDouble();
						fIs[i] = br.ReadDouble();
						Xmax = fqs[i];
						if (Math.Abs(fRs[i]) > Ymax) Ymax = Math.Abs(fRs[i]);
						if (Math.Abs(fIs[i]) > Ymax) Ymax = Math.Abs(fIs[i]);
					}
					br.Close();
					Ymin = 0;
					//use the fis[0] as the x interval
					N = (UInt32)(Xmax / fqs[0]);
					UInt32[] F = new UInt32[N];
					X = new double[N];
					Y = new double[N];
					Z = new double[N];
					W = null;
					V = null;
					S = null;
					F[0] = fqs[0];
					X[0] = F[0];
					Y[0] = fRs[0];
					Z[0] = fIs[0];
					double dx1, dx2;
					long h = -1;
					for (long i = 1; i < N; i++)
					{
						F[i] = F[i - 1] + F[0];
						dx1 = double.MaxValue;
						dx2 = double.MaxValue;
						h = -1;
						for (long k = 1; k < itemCount; k++)
						{
							if (F[i] == fqs[k])
							{
								h = k;
								break;
							}
							else
							{
								if (fqs[k] < F[i])
								{
									dx1 = F[i] - fqs[k];
									if (dx2 < double.MaxValue)
									{
										if (dx2 < dx1)
										{
											break;
										}
										else
										{
											h = k;
											break;
										}
									}
									else
									{
										h = k;
									}
								}
								else
								{
									dx2 = fqs[k] - F[i];
									if (dx2 < dx1)
									{
										h = k;
										break;
									}
									else
									{
										if (h > 0)
										{
											break;
										}
										else
										{
											h = k;
										}
									}
								}
							}
						}
						if (h == -1)
						{
							throw new Exception("the nearest frequency not found");
						}
						else
						{
							X[i] = F[i];
							if (F[i] == fqs[h])
							{
								Y[i] = fRs[h];
								Z[i] = fIs[h];
							}
							else if (F[i - 1] == fqs[h])
							{
								Y[i] = Y[i - 1];
								Z[i] = Z[i - 1];
							}
							else
							{
								//use linear estimation
								//(F[i],?)(F[i-1], Y[i-1]) -- (fqs[h], fRs[h])
								//(?-Y[i-1])/(F[i]-F[i-1]) = (fRs[h]-Y[i-1])/(fqs[h]-F[i-1])
								//? = Y[i-1] + F[0]*(fRs[h]-Y[i-1])/(fqs[h]-F[i-1])
								Y[i] = Y[i - 1] + F[0] * (fRs[h] - Y[i - 1]) / (fqs[h] - F[i - 1]);
								Z[i] = Z[i - 1] + F[0] * (fIs[h] - Z[i - 1]) / (fqs[h] - F[i - 1]);
							}
						}
					}
					//
					int ntype = 1;
					if (ntype == 1)
					{
						//show change rate instead
						double[] rateY = new double[N];
						double[] rateZ = new double[N];
						rateY[0] = Y[0] / F[0];
						rateZ[0] = Z[0] / F[0];
						for (long i = 1; i < N; i++)
						{
							rateY[i] = (Y[i] - Y[i - 1]) / F[0];
							rateZ[i] = (Z[i] - Z[i - 1]) / F[0];
						}
						Ymax = double.MinValue;
						for (long i = 0; i < N; i++)
						{
							Y[i] = rateY[i];
							Z[i] = rateZ[i];
							if (Math.Abs(Y[i]) > Ymax) Ymax = Math.Abs(Y[i]);
							if (Math.Abs(Z[i]) > Ymax) Ymax = Math.Abs(Z[i]);
						}
					}
					//
					this.Refresh();
				}
			}
		}
		private void d1RealSourceToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename("1-D FFT Real to Complex"))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = new double[N];
					Ymax = double.MinValue; Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = i;
						Y[i] = br.ReadDouble();
						Z[i] = br.ReadDouble();
						if (Y[i] > Ymax) Ymax = Y[i];
						if (Y[i] < Ymin) Ymin = Y[i];
						if (Z[i] > Ymax) Ymax = Z[i];
						if (Z[i] < Ymin) Ymin = Z[i];
						Y[i] = Math.Round(Y[i], 10);
						Z[i] = Math.Round(Z[i], 10);
					}
					br.Close();
					Xmax = N;
					dataType = 1;
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}
		private void dRealSourceInFrequencyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename("1D Real to Complex FFT"))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					UInt32 halfYC = 0;
					double halfFttRange = 0;
					if (FormParam.AskForGeometry(this, ref halfYC, ref halfFttRange))
					{
						double dw = SpaceDigitizer.FFT_FREQSTEP(halfFttRange);
						N = size / itemSize;
						Y = new double[N];
						X = new double[N];
						Z = new double[N];
						Ymax = double.MinValue; Ymin = double.MaxValue;
						BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
						for (UInt32 i = 0; i < N; i++)
						{
							X[i] = (double)i * dw;
							Y[i] = br.ReadDouble();
							Z[i] = br.ReadDouble();
							if (Y[i] > Ymax) Ymax = Y[i];
							if (Y[i] < Ymin) Ymin = Y[i];
							if (Z[i] > Ymax) Ymax = Z[i];
							if (Z[i] < Ymin) Ymin = Z[i];
							Y[i] = Math.Round(Y[i], 10);
							Z[i] = Math.Round(Z[i], 10);
						}
						br.Close();
						Xmax = N;
						this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
						this.Refresh();
					}
				}
			}
		}
		private void dStrengthToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if (askForFilename(dStrengthToolStripMenuItem1.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					double vr, vi;
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = new double[N];
					W = new double[N];
					Ymax = double.MinValue; Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = i;
						vr = br.ReadDouble();
						vi = br.ReadDouble();
						Y[i] = vr * vr + vi * vi;
						Z[i] = 0.0;
						W[i] = 0.0;
						if (Y[i] > Ymax) Ymax = Y[i];
						if (Y[i] < Ymin) Ymin = Y[i];
						if (Y[i] > 1e-9)
						{
							Z[i] = vr * vr / Y[i];
							W[i] = vi * vi / Y[i];
						}
						Y[i] = Math.Round(Y[i], 10);
					}
					br.Close();
					Xmax = N;
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void dStrengthNormalizedToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(dStrengthNormalizedToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					double v;
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = null;
					Ymax = double.MinValue; Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = i;
						Y[i] = br.ReadDouble();
						v = br.ReadDouble();
						Y[i] = v * v + Y[i] * Y[i];
						if (Y[i] > Ymax) Ymax = Y[i];
						if (Y[i] < Ymin) Ymin = Y[i];
					}
					br.Close();
					Xmax = N;
					v = Math.Max(Math.Abs(Ymax), Math.Abs(Ymin));
					if (v > 0)
					{
						for (UInt32 i = 0; i < N; i++)
						{
							Y[i] = Y[i] / v;
						}
						Ymax = Ymax / v;
						Ymin = Ymin / v;
					}
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void dStrengthNormalizedByDimToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(dStrengthNormalizedByDimToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size == 0) || (size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					double v;
					double vr;
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = null;
					vr = N;
					Ymax = double.MinValue; Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = i;
						Y[i] = br.ReadDouble();
						v = br.ReadDouble();
						Y[i] = (v * v + Y[i] * Y[i]) / vr;
						if (Y[i] > Ymax) Ymax = Y[i];
						if (Y[i] < Ymin) Ymin = Y[i];
					}
					br.Close();
					Xmax = N;
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void dRelativeStrengthDecreaseToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(dRelativeStrengthDecreaseToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					double vsum, v, v0;
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = null;
					vsum = 0;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = i;
						Y[i] = br.ReadDouble();
						v = br.ReadDouble();
						Y[i] = v * v + Y[i] * Y[i];
						vsum += Y[i];
					}
					br.Close();
					Xmax = N;
					v = vsum - Y[0];
					Y[0] = 1;
					for (UInt32 i = 1; i < N; i++)
					{
						v0 = Y[i];
						Y[i] = v / vsum;
						v -= v0;
					}
					Ymax = 1;
					Ymin = 0;
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void dSpaceFrequencyIndexToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(dSpaceFrequencyIndexToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 2 * sizeof(UInt32);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = null;
					Ymax = double.MinValue;
					Xmax = double.MinValue;
					Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = br.ReadUInt32();
						Y[i] = br.ReadUInt32();
						if (Y[i] > Ymax) Ymax = Y[i];
						if (X[i] > Xmax) Xmax = X[i];
						if (Y[i] < Ymin) Ymin = Y[i];
					}
					br.Close();
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void showDataFileInChartToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(showDataFileInChartToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				N = fi.Length / (2 * sizeof(double));
				Y = new double[N];
				X = new double[N];
				Z = null;
				BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
				Xmax = 0;
				Ymax = double.MinValue;
				Ymin = double.MaxValue;
				for (long i = 0; i < N; i++)
				{
					X[i] = br.ReadDouble();
					Y[i] = br.ReadDouble();
					if (X[i] > Xmax) Xmax = X[i];
					if (Y[i] > Ymax) Ymax = Y[i];
					if (Y[i] < Ymin) Ymin = Y[i];
				}
				br.Close();
				this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
				this.Refresh();
			}
		}

		private void showCoseToolStripMenuItem_Click(object sender, EventArgs e)
		{
			N = 100;
			double dx = 2.0 / N;
			Y = new double[N];
			X = new double[N];
			Z = null;
			X[0] = -1.0;
			Y[0] = Math.Acos(X[0]) * 180.0 / Math.PI;
			for (int i = 1; i < N; i++)
			{
				X[i] = X[i - 1] + dx;
				Y[i] = Math.Acos(X[i]) * 180.0 / Math.PI;
			}
			Xmax = N;
			Ymax = 360;
			Ymin = -360;
			this.Refresh();
		}

		private void dMaxcenterFrequenciesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(dMaxcenterFrequenciesToolStripMenuItem.Text))
			{
				FileInfo fi = new FileInfo(filename);
				long size = fi.Length;
				int itemSize = 3 * sizeof(double);
				if ((size % itemSize) != 0)
				{
					MessageBox.Show("File size mismatch");
				}
				else
				{
					N = size / itemSize;
					Y = new double[N];
					X = new double[N];
					Z = new double[N];
					Ymax = double.MinValue;
					Xmax = double.MinValue;
					Ymin = double.MaxValue;
					BinaryReader br = new BinaryReader(new FileStream(filename, FileMode.Open));
					for (UInt32 i = 0; i < N; i++)
					{
						X[i] = br.ReadDouble();
						Y[i] = br.ReadDouble();
						Z[i] = br.ReadDouble();
						if (X[i] > Xmax) Xmax = X[i];
						if (Y[i] > Ymax) Ymax = Y[i];
						if (Y[i] < Ymin) Ymin = Y[i];
						if (Z[i] > Ymax) Ymax = Z[i];
						if (Z[i] < Ymin) Ymin = Z[i];
					}
					br.Close();
					this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
					this.Refresh();
				}
			}
		}

		private void doublesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(doublesToolStripMenuItem.Text))
			{
				FormData fm = new FormData();
				bool bOK = fm.Load6doubles(filename);
				if (bOK)
				{
					fm.Show(this);
				}
			}
		}

		private void highLowRowsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(highLowRowsToolStripMenuItem.Text))
			{
				FormData fm = new FormData();
				bool bOK = fm.LoadHighLowRows(filename);
				if (bOK)
				{
					fm.Show(this);
				}
			}
		}

		private void showDataControlToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (_frmDrawingData != null)
			{
				if (_frmDrawingData.IsDisposed || _frmDrawingData.Disposing)
				{
					_frmDrawingData = null;
				}
			}
			if (_frmDrawingData == null)
			{
				_frmDrawingData = new FormDrawingData();
				_frmDrawingData.InitOwner(this);
				_frmDrawingData.Show(this);
			}
		}

		private void eMErrorDifferencesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(eMErrorDifferencesToolStripMenuItem.Text))
			{
				BinaryReader br = null;
				try
				{
					FileInfo fi = new FileInfo(filename);
					br = new BinaryReader(new FileStream(filename, FileMode.Open));
					UInt32 u = br.ReadUInt32(); //columns = 2 (u+1)
					int rs = (int)u + 1;
					long size = fi.Length - sizeof(UInt32);
					long usize = 2 * (long)rs * sizeof(double);
					if ((size % usize) != 0)
					{
						MessageBox.Show(this, "File size mismatch", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
					}
					else
					{
						long ts = size / (2 * rs * sizeof(double));
						_divergDiffs = new ErrorsOverTime();
						N = 0;
						for (long t = 0; t < ts; t++)
						{
							ErrorsOnRadiuses er = new ErrorsOnRadiuses(rs);
							for (int r = 0; r < rs; r++)
							{
								er.SetDiverg1(r, br.ReadDouble());
							}
							for (int r = 0; r < rs; r++)
							{
								er.SetDiverg2(r, br.ReadDouble());
							}
							_divergDiffs.AddTimeItem(er);
						}
						if (_frmEMcontrol != null && _frmEMcontrol.IsDisposed && _frmEMcontrol.Disposing)
						{
							_frmEMcontrol = null;
						}
						if (_frmEMcontrol == null)
						{
							_frmEMcontrol = new FormEMcontrol();
							_frmEMcontrol.SetDrawingOwner(this);
							_frmEMcontrol.Show(this);
						}
						ShowEMErrorPage(0);
						this.Text = string.Format(CultureInfo.InvariantCulture, "Data file - {0}", Path.GetFileName(filename));
						this.Refresh();
					}
				}
				catch (Exception err)
				{
					MessageBox.Show(this, err.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
				if (br != null)
				{
					br.Close();
				}
			}
		}

		private void addATimefileToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//linesLoaded determines which array to use
			if (askForFilename(addATimefileToolStripMenuItem.Text))
			{
				BinaryReader br = null;
				try
				{
					long size = sizeof(UInt32);
					FileInfo fi = new FileInfo(filename);
					if ((fi.Length % size) != 0)
					{
						MessageBox.Show("File size mismatch");
					}
					else
					{
						long ns = (fi.Length / size);
						br = new BinaryReader(new FileStream(filename, FileMode.Open));
						switch (linesLoaded)
						{
							case 0:
								N = ns;
								X = new double[N];
								Y = new double[N];
								Z = null;
								W = null;
								V = null;
								S = null;
								Ymin = 0;
								Ymax = 0;
								Xmax = N;
								for (long i = 0; i < N; i++)
								{
									X[i] = i;
									Y[i] = br.ReadUInt32();
									if(Math.Abs(Y[i]) > Ymax) Ymax = Math.Abs(Y[i]);
								}
								linesLoaded = 1;
								break;
							case 1:
								if (N != ns)
								{
									MessageBox.Show("Invalid file size");
								}
								else
								{
									Z = new double[N];
									for (long i = 0; i < N; i++)
									{
										Z[i] = br.ReadUInt32();
										if (Math.Abs(Z[i]) > Ymax) Ymax = Math.Abs(Z[i]);
									}
									linesLoaded = 2;
								}
								break;
							case 2:
								if (N != ns)
								{
									MessageBox.Show("Invalid file size");
								}
								else
								{
									W = new double[N];
									for (long i = 0; i < N; i++)
									{
										W[i] = br.ReadUInt32();
										if (Math.Abs(W[i]) > Ymax) Ymax = Math.Abs(W[i]);
									}
									linesLoaded = 3;
								}
								break;
							case 3:
								if (N != ns)
								{
									MessageBox.Show("Invalid file size");
								}
								else
								{
									V = new double[N];
									for (long i = 0; i < N; i++)
									{
										V[i] = br.ReadUInt32();
										if (Math.Abs(V[i]) > Ymax) Ymax = Math.Abs(V[i]);
									}
									linesLoaded = 4;
								}
								break;
							case 4:
								if (N != ns)
								{
									MessageBox.Show("Invalid file size");
								}
								else
								{
									S = new double[N];
									for (long i = 0; i < N; i++)
									{
										S[i] = br.ReadUInt32();
										if (Math.Abs(S[i]) > Ymax) Ymax = Math.Abs(S[i]);
									}
									linesLoaded = 5;
								}
								break;
							case 5:
								if (N != ns)
								{
									MessageBox.Show("Invalid file size");
								}
								else
								{
									Y = new double[N];
									for (long i = 0; i < N; i++)
									{
										Y[i] = br.ReadUInt32();
										if (Math.Abs(Y[i]) > Ymax) Ymax = Math.Abs(Y[i]);
									}
									linesLoaded = 1;
								}
								break;
						}
					}
				}
				catch
				{
				}
				if (br != null)
				{
					br.Close();
				}
				this.Refresh();
			}
		}

		private void eMPerformancesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (askForFilename(eMPerformancesToolStripMenuItem.Text))
			{
				try
				{
					List<EMperformance> emps = new List<EMperformance>();
					StreamReader sr = new StreamReader(filename);
					string line;
					while (!sr.EndOfStream)
					{
						line = sr.ReadLine();
						EMperformance emp = EMperformance.Parse(line);
						if(emp != null)
						{
							emps.Add(emp);
						}
					}
					sr.Close();
					N = emps.Count;
					Ymin = 0;
					Ymax = 0;
					Xmax = 0;
					X = new double[N]; //time used to finish the simulation
					Y = new double[N]; //average divergence magnitude
					D = new string[N]; //text description of the simulation
					Z = null;
					W = null;
					V = null;
					S = null;
					for (int i = 0; i < N; i++)
					{
						EMperformance em = emps[i];
						X[i] = em.TimeUsed;
						if (X[i] > Xmax) Xmax = X[i];
						Y[i] = em.Precision;
						if (Y[i] > Ymax) Ymax = Y[i];
						D[i] = em.Description;
					}
					chartType = ChartType.Text;

					if (_frmEMP != null)
					{
						if (_frmEMP.IsDisposed || _frmEMP.Disposing)
							_frmEMP = null;
					}
					if (_frmEMP == null)
					{
						_frmEMP = new FormEMperform();
					}
					_frmEMP.LoadData(this, emps);
					_frmEMP.Hide();
					_frmEMP.Show(this);
					this.Refresh();
				}
				catch (Exception err)
				{
					MessageBox.Show(this, err.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}
	}
	public class EMperformance
	{
		public EMperformance()
		{
		}
		public double Precision { get; set; }
		public double TimeUsed { get; set; }
		public string Description { get; set; }
		public static EMperformance Parse(string s)
		{
			if (!string.IsNullOrEmpty(s))
			{
				s = s.Trim();
				if (s.Length > 0)
				{
					int n = s.IndexOf(',');
					if (n > 0)
					{
						string sn = s.Substring(0, n);
						try
						{
							double p = Convert.ToDouble(sn);
							s = s.Substring(n + 1);
							s = s.Trim();
							n = s.IndexOf(',');
							if (n > 0)
							{
								sn = s.Substring(0, n);
								double t = Convert.ToDouble(sn);
								s = s.Substring(n + 1);
								s = s.Trim();
								EMperformance emp = new EMperformance();
								emp.Precision = p;
								emp.TimeUsed = t;
								emp.Description = s;
								return emp;
							}
						}
						catch
						{
						}
					}
				}
			}
			return null;
		}
	}
	class ComplexData
	{
		public double Real;
		public double Image;
		public ComplexData(double rl, double img)
		{
			Real = rl;
			Image = img;
		}
		public double Size2
		{
			get
			{
				return Real * Real + Image * Image;
			}
		}

	}
	enum ChartType
	{
		Line = 0,
		Text = 1
	}
}
