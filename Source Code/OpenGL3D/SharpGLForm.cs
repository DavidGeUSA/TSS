/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SharpGL;
using SharpGL.SceneGraph;
using System.Globalization;
using System.IO;
using Draw3D;

namespace Field3D
{
	/// <summary>
	/// sharp GL drawing form
	/// 3D drawing is done by function drawContents()
	/// use a context menu to load data
	/// </summary>
	public partial class SharpGLForm : Form
	{
		#region fields and constructors
		//////////////////////////////////////////////////////////////////////
		//3D drawing tool
		private OpenGLUtility _openGL = null;
		//view settings
		private double viewSize = 100.0;
		private double gridSize = 1.5;
		//
		//only vectors with lengths larger than this percent of the maximum length will be displayed.
		private double vectorLengthPercent = 0.05;
		//the maximum vector size of the vectors to be displayed
		private double maximumVectorLength = 0;
		//
		private double vectorLengthFitFactor = 0.3;
		private double vectorLocationFitFactor = 0.2;
		///////////////////////////////////////////////////////////////////////
		//
		//UI for setting drawing parameters, called via context menu
		private FormDrawParameters _parameterForm = null;
		//UI for controlling data files
		private FormDataFilesControl _controlForm = null;
		//UI for rotating and shifting
		FormRotate frmRotate = null;
		//
		//enable partial display of all vectors if the vectors are in radius indexing
		private UInt32 _skipRadius = 0;       //number of radiuses to be ignored when drawing vectors in radius indexing
		private UInt32 _skipSpaceStep = 0;    //number of space points to be ignored when drawing vectors in radius indexing
		private bool _oneRadiusOnly = false;  //indicate whether only space points on one radius are to be displayed
		private UInt32 _radius = 0;           //the radius to be displayed when _oneRadiusOnly is true
		private UInt32 _maxRadius = 0;        //when vectors is in radius indexing, it is the maximum radius
		//
		//field space and digitization
		UInt32 N = 0;            //number of double space intervals on one side of an axis, counting from the first interval
		double range = 0.0;      //space range at one side of an axis
		double ds = 0.0;         //space interval. ds = range / (2N+1) because there are 2N+1 space intervals in one side of an axis
		UInt32 maxN = 0;         //number of space points in one axis, it is 4*N+3 because in one side of an axis there are 2N+1 points, plus one point at origin
		UInt64 maxN2 = 0;        //number of space points in one plane, it is maxN^2
		UInt64 maxN3 = 0;        //number of space points in one cubic, it is maxN^3
		double minSpace = 0.0;   //the minimum space location in an axis, it is -range, or 2ds*N+ds
		//
		//vectors collected for display
		private List<Vector3D> _vectorCollector = null;
		private Vector3D[] _vectorCache = null;
		//
		//indexing method for array "vectors"
		private bool _isInSphere = false; //true: vectors is in radius indexing; false: vectors is in row-major 3D array indexing
		//choose to show which fields
		EMShowSelect emShow = EMShowSelect.ShowElectric;
		//
		//vectors to be displayed; only one of them will be used
		//see drawContents()
		private Point3D[, ,] field = null;
		private Vector3D[] vectors = null;
		private EMFieldPoints emField = null;
		//
		private Line3D[] lines = null; //draw some lines to help view the drawing
		//
		/// <summary>
		/// Initializes a new instance of the <see cref="SharpGLForm"/> class.
		/// </summary>
		public SharpGLForm()
		{
			InitializeComponent();
			progressBar1.Visible = false;
			if (_openGL == null)
			{
				_openGL = new OpenGLUtility(openGLControl.OpenGL, openGLControl.ClientSize.Width, openGLControl.ClientSize.Height, viewSize, gridSize);
			}
		}
		#endregion

		#region interface for interacting with FormDataFileControl
		public bool LoadEMDataFromFile(string file)
		{
			try
			{
				field = null;
				vectors = null;
				emField = new EMFieldPoints(this);
				emField.LoadFromFile(file);
				return true;
			}
			catch (Exception err)
			{
				MessageBox.Show(this, string.Format(CultureInfo.InvariantCulture, "{0}\r\nStack trace:\r\n{1}", err.Message, err.StackTrace), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			return false;
		}
		#endregion

		#region interface for interacting with parameter setting UI
		public void SetLengthPercent(double percent)
		{
			if (percent > 0.0 && percent < 1.0)
			{
				vectorLengthPercent = percent;
				_vectorCache = null;
				this.Refresh();
			}
		}
		public void SetGridSize(double g)
		{
			if (g > 0.1)
			{
				gridSize = g;
				_openGL.SetGridSize(g);
				this.Refresh();
			}
		}
		public void SetViewSize(double v)
		{
			if (v > 0.9)
			{
				viewSize = v;
				_openGL.SetViewSize(v);
				this.Refresh();
			}
		}
		public bool IsInSphere()
		{
			return _isInSphere;
		}
		public void SetSkipRadiusSpace(UInt32 r, UInt32 s)
		{
			_skipRadius = r;
			_skipSpaceStep = s;
			_vectorCache = null;
			this.Refresh();
		}
		public void SetRadiusOnly(bool radiusOnly, UInt32 r)
		{
			_oneRadiusOnly = radiusOnly;
			if (vectors != null)
			{
				UInt32 maxR = SphereUtility.GetSphereRadius((UInt32)vectors.Length);
				if (r <= maxR)
				{
					_radius = r;
					if (_isInSphere)
					{
						_vectorCache = null;
						this.Refresh();
					}
				}
			}
		}
		public void SetEMSelect(EMShowSelect em)
		{
			if (emShow != em)
			{
				emShow = em;
				if (emField != null)
				{
					if (vectors == null)
					{
						//force redraw emField
						_vectorCache = null;
						this.Refresh();
					}
				}
			}
		}
		public void ClearVectors()
		{
			vectors = null;
			_vectorCache = null;
			this.Refresh();
		}
		public void SetLengthFitFactor(double factor)
		{
			if (vectorLengthFitFactor != factor)
			{
				vectorLengthFitFactor = factor;
				if (field == null)
				{
					if (_vectorCache != null)
					{
						scaleVectorsFitView();
						this.Refresh();
					}
				}
			}
		}
		public void SetLocationFitFactor(double factor)
		{
			if (vectorLocationFitFactor != factor)
			{
				vectorLocationFitFactor = factor;
				if (field == null)
				{
					if (_vectorCache != null)
					{
						scaleVectorsFitView();
						this.Refresh();
					}
				}
			}
		}
		#endregion

		#region interface for interacting with class RadiusIndexingDemo
		public bool ShowOnlyOneRadius
		{
			get { return _oneRadiusOnly; }
		}
		public UInt32 RadiusToShow
		{
			get { return _radius; }
		}
		public UInt32 SkipRadius
		{
			get { return _skipRadius; }
		}
		public UInt32 SkipSpacePoints
		{
			get { return _skipSpaceStep; }
		}
		public void SetMaxRadius(UInt32 r)
		{
			_maxRadius = r;
		}
		public void CollectEMFieldPoint(double x, double y, double z, EMField3D em)
		{
			Vector3D v = new Vector3D();
			v.x = x;
			v.y = y;
			v.z = z;
			if (emShow == EMShowSelect.ShowElectric)
			{
				v.vx = em.Ex;
				v.vy = em.Ey;
				v.vz = em.Ez;
			}
			else if (emShow == EMShowSelect.ShowMagnetic)
			{
				v.vx = em.Hx;
				v.vy = em.Hy;
				v.vz = em.Hz;
			}
			double len = Math.Sqrt(v.vx * v.vx + v.vy * v.vy + v.vz * v.vz);
			if (len > maximumVectorLength)
			{
				maximumVectorLength = len;
			}
			_vectorCollector.Add(v);
		}
		public void StartCollectingVectors(UInt32 maxR)
		{
			_maxRadius = maxR;
			maximumVectorLength = 0.0;
			_vectorCollector = new List<Vector3D>();
			_vectorCache = null;
		}
		public void FinishCollectingVectors()
		{
			_openGL.SetMinimumVectorLength(maximumVectorLength * vectorLengthPercent);
			List<Vector3D> vs = new List<Vector3D>();
			for (int i = 0; i < _vectorCollector.Count; i++)
			{
				if (_openGL.DrawVector(_vectorCollector[i]))
				{
					vs.Add(_vectorCollector[i]);
				}
			}
			_vectorCache = vs.ToArray();
			_vectorCollector = null;
			scaleVectorsFitView();
		}
		#endregion

		#region interface for interacting with FormRotate
		public double GetViewSize()
		{
			return viewSize;
		}
		public void RotateOnX(double angle)
		{
			_openGL.RotateOnX(angle);
		}
		public void RotateOnY(double angle)
		{
			_openGL.RotateOnY(angle);
		}
		public void RotateOnZ(double angle)
		{
			_openGL.RotateOnZ(angle);
		}
		public void ResetRotation()
		{
			_openGL.ResetRotation();
		}
		public void SetShiftX(double sx)
		{
			_openGL.SetShiftX(sx);
		}
		public void SetShiftY(double sy)
		{
			_openGL.SetShiftY( sy);
		}
		public void SetShiftZ(double sz)
		{
			_openGL.SetShiftZ( sz);
		}
		public double GetShiftX()
		{
			return _openGL.GetShiftX();
		}
		public double GetShiftY()
		{
			return _openGL.GetShiftY();
		}
		public double GetShiftZ()
		{
			return _openGL.GetShiftZ();
		}
		#endregion

		#region load data from files
		private void showProgressBar(long maxItem, string info)
		{
			this.Text = info;
			progressBar1.Minimum = 0;
			progressBar1.Maximum = (int)maxItem;
			progressBar1.Visible = true;
			progressBar1.BringToFront();
		}
		private void load3Dscalarfile(string dataFile)
		{
			UInt32 maxR;
			double max;
			string infoFile = string.Format(CultureInfo.InvariantCulture, "{0}.info", dataFile);
			BinaryReader br = new BinaryReader(new FileStream(infoFile, FileMode.Open));
			//objs = null;
			vectors = null;
			N = br.ReadUInt32();
			range = br.ReadDouble();
			max = br.ReadDouble();
			br.Close();
			_isInSphere = false;
			if (max == 0.0) max = 1.0;
			SpaceDigitizer.SpaceDigitize(range, N, out ds, out  maxN, out  maxN2, out  maxN3, out  minSpace, out  maxR);
			br = new BinaryReader(new FileStream(dataFile, FileMode.Open));
			field = new Point3D[maxN, maxN, maxN];
			for (uint i = 0; i < maxN; i++)
			{
				for (uint j = 0; j < maxN; j++)
				{
					for (uint k = 0; k < maxN; k++)
					{
						field[i, j, k].x = (br.ReadDouble()) / max;
					}
				}
			}
			br.Close();
			this.Refresh();
		}
		private void load3Dvectorfile(string dataFile)
		{
			UInt32 maxR;
			double max;
			string infoFile = string.Format(CultureInfo.InvariantCulture, "{0}.info", dataFile);
			BinaryReader br = new BinaryReader(new FileStream(infoFile, FileMode.Open));
			//objs = null;
			vectors = null;
			N = br.ReadUInt32();
			range = br.ReadDouble();
			max = br.ReadDouble();
			br.Close();
			_isInSphere = false;
			if (max == 0.0) max = 1.0;
			SpaceDigitizer.SpaceDigitize(range, N, out ds, out  maxN, out  maxN2, out  maxN3, out  minSpace, out  maxR);
			br = new BinaryReader(new FileStream(dataFile, FileMode.Open));
			field = new Point3D[maxN, maxN, maxN];
			for (uint i = 0; i < maxN; i++)
			{
				for (uint j = 0; j < maxN; j++)
				{
					for (uint k = 0; k < maxN; k++)
					{
						field[i, j, k].x = (br.ReadDouble()) / max;
						field[i, j, k].y = (br.ReadDouble()) / max;
						field[i, j, k].z = (br.ReadDouble()) / max;
					}
				}
			}
			br.Close();
			this.Refresh();
		}

		/// <summary>
		/// load 3D vectors from a data file into varible "vectors"
		/// </summary>
		/// <param name="file">full file path of the data file</param>
		private void load3DVectors(string file)
		{
			int itemsize = 6 * sizeof(double);
			FileInfo fi = new FileInfo(file);
			if ((fi.Length % itemsize) != 0)
			{
				MessageBox.Show(this, "Invalid file size", file, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			else
			{
				double len;
				long items = fi.Length / itemsize;
				showProgressBar(items, "reading vectors from file, please wait...");
				int k = 0;
				int n = (int)items / 100;
				if (n <= 0) n = 1;
				N = 0;
				//objs = null;
				field = null;
				maximumVectorLength = 0.0;
				vectors = new Vector3D[items];
				_isInSphere = false;
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				for (long i = 0; i < items; i++)
				{
					vectors[i].x = br.ReadDouble();
					vectors[i].y = br.ReadDouble();
					vectors[i].z = br.ReadDouble();
					vectors[i].vx = br.ReadDouble();
					vectors[i].vy = br.ReadDouble();
					vectors[i].vz = br.ReadDouble();
					len = Math.Sqrt(vectors[i].vx * vectors[i].vx + vectors[i].vy * vectors[i].vy + vectors[i].vz * vectors[i].vz);
					if (len > maximumVectorLength) maximumVectorLength = len;
					k++;
					if (k > n)
					{
						k = 0;
						progressBar1.Value = (int)i;
					}
				}
				br.Close();
				_vectorCache = null;
				progressBar1.Hide();
				this.Text = Path.GetFileName(file);
				this.Refresh();
			}
		}
		public bool LoadEMFieldFromFile(string file)
		{
			bool bRet = false;
			int itemsize = 9 * sizeof(double);
			FileInfo fi = new FileInfo(file);
			if ((fi.Length % itemsize) != 0)
			{
				MessageBox.Show(this, "Invalid file size", file, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			else
			{
				double len;
				double vx, vy, vz;
				long items = fi.Length / itemsize;
				showProgressBar(items, "reading vectors from file, please wait...");
				int k = 0;
				int n = (int)items / 100;
				if (n <= 0) n = 1;
				N = 0;
				field = null;
				maximumVectorLength = 0.0;
				_isInSphere = false;
				vectors = new Vector3D[items];
				BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
				for (long i = 0; i < items; i++)
				{
					vectors[i].x = br.ReadDouble();
					vectors[i].y = br.ReadDouble();
					vectors[i].z = br.ReadDouble();
					vectors[i].vx = br.ReadDouble();
					vectors[i].vy = br.ReadDouble();
					vectors[i].vz = br.ReadDouble();
					vx = br.ReadDouble();
					vy = br.ReadDouble();
					vz = br.ReadDouble();
					if (emShow == EMShowSelect.ShowMagnetic)
					{
						vectors[i].vx = vx;
						vectors[i].vy = vy;
						vectors[i].vz = vz;
					}
					len = Math.Sqrt(vectors[i].vx * vectors[i].vx + vectors[i].vy * vectors[i].vy + vectors[i].vz * vectors[i].vz);
					if (len > maximumVectorLength) maximumVectorLength = len;
					k++;
					if (k > n)
					{
						k = 0;
						progressBar1.Value = (int)i;
					}
				}
				br.Close();
				bRet = true;
				_vectorCache = null;
				progressBar1.Hide();
				this.Text = Path.GetFileName(file);
				this.Refresh();
			}
			return bRet;
		}
		void loadDivergenceReport(string file)
		{
			FileInfo fi = new FileInfo(file);
			BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
			UInt32 maxR = br.ReadUInt32();
			UInt32 itemsize = (maxR+1)*sizeof(double);
			if (((fi.Length - sizeof(UInt32)) % itemsize) != 0)
			{
				br.Close();
				MessageBox.Show(this, "Invalid file size", file, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			else
			{
				List<Vector3D> vs = new List<Vector3D>();
				long items = (fi.Length-sizeof(UInt32)) / itemsize;
				double rScale = 5.0 * viewSize / (double)maxR;
				double tScale =5.0* viewSize / (double)items;
				maximumVectorLength = 0.0;
				for (long t = 0; t < items; t++)
				{
					double ts = (double)(t * tScale);
					for (int r = 0; r <= maxR; r++)
					{
						Vector3D v = new Vector3D();
						//x-axis is radius
						v.x = (double)r * rScale;
						//y-axis is time
						v.y = ts;
						//z-axis is divergence
						v.z = 0.0;
						v.vx = 0.0;
						v.vy = 0.0;
						v.vz = br.ReadDouble();
						if (v.vz > maximumVectorLength) maximumVectorLength = v.vz;
						vs.Add(v);
					}
				}
				br.Close();
				_isInSphere = false;
				//
				Vector3D v1 = new Vector3D();
				v1.x = (double)maxR * rScale;
				v1.y = 0.0;
				v1.z = 0.0;
				v1.vx = 0.0;
				v1.vy = maximumVectorLength;
				v1.vz = 0.0;
				vs.Add(v1);
				//
				vectors = vs.ToArray();
				_maxRadius = maxR;
				_vectorCache = null;
				
				this.Text = Path.GetFileName(file);
				this.Refresh();
			}
		}
		#endregion

		#region draw contents

		///////////////////////////////////////////////////////////////////
		/// <summary>
		/// assume that vectors in array variable "vectors" are arranged by radius,
		/// vectors at the smaller radius are at front positions of the array.
		/// this function draws vectors beloging to a same radius.
		/// </summary>
		/// <param name="vs">vectors drawed by this function</param>
		/// <param name="n">interval for updating progress bar</param>
		/// <param name="r">the radius</param>
		/// <returns>true: r is within the range of vectors; false: r is too large</returns>
		private bool drawAtRadius(List<Vector3D> vs, int n, UInt32 r)
		{
			bool ok = false;
			if (r == 0)
			{
				if (vectors != null && vectors.Length > 0)
				{
					if (_openGL.DrawVector(vectors[0]))
					{
						vs.Add(vectors[0]);
					}
					ok = true;
				}
			}
			else
			{
				UInt32 maxR = SphereUtility.GetSphereRadius((UInt32)vectors.Length);
				if (r <= maxR)
				{
					//index of the first vector in the array for the radius
					UInt32 startIdx = SphereUtility.PointsInSphere(r - 1);
					//index of the last vector in the array for the radius
					UInt32 endIdx = startIdx + SphereUtility.PointsAtRadius(r) - 1;
					if (endIdx < vectors.Length) //radius not too large 
					{
						int k = 0; //number of collected vectors before last progress-bar update
						if (_skipSpaceStep == 0) //draw all vectors
						{
							for (UInt32 i = startIdx; i < endIdx; i++)
							{
								if (_openGL.DrawVector(vectors[i]))
								{
									vs.Add(vectors[i]);
								}
								k++;
								if (k > n)
								{
									k = 0;
									progressBar1.Value = (int)i;
								}
							}
						}
						else //draw part of vectors so that the screen is not too dense
						{
							bool[] usages = SphereUtility.SkipSpacePoints(r, _skipSpaceStep);
							for (UInt32 i = startIdx; i < endIdx; i++)
							{
								if (usages[i - startIdx])
								{
									if (_openGL.DrawVector(vectors[i]))
									{
										vs.Add(vectors[i]);
									}
								}
								k++;
								if (k > n)
								{
									k = 0;
									progressBar1.Value = (int)i;
								}
							}
						}
						ok = true;
					}
					else
					{
						//a programming error
						throw new Exception("Error calling drawAtRadius. Radius too big for available vectors.");
					}
				}
			}
			return ok;
		}


		/// <summary>
		/// draw using data in 3D array variable field
		/// </summary>
		private void drawField()
		{
			/*
				 * field is in cubic indexing: integer index [0, maxN-1] corresponds to space locatio [-R, R]
				 * field[i,j,k] holds an end point of a vector starting from the space location 
				 * maxN should be 4*N+3
			*/
			OpenGL gl = openGLControl.OpenGL;
			gl.Begin(SharpGL.Enumerations.BeginMode.Lines);
			gl.Color(1, 0, 0);
			double x = minSpace, y, z;
			//
			for (uint i = 0; i < maxN; i++)
			{
				y = minSpace;
				for (uint j = 0; j < maxN; j++)
				{
					z = minSpace;
					for (uint k = 0; k < maxN; k++)
					{
						//start point
						gl.Vertex(x, y, z);
						//end point
						gl.Vertex(x + field[i, j, k].x, y + field[i, j, k].y, z + field[i, j, k].z);
						z += ds;
					}
					y += ds;
				}
				x += ds;
			}
			gl.End();
		}

		/// <summary>
		/// Visible space locations and vectors should be in the range of OpenGL view.
		/// This function scale space locations and vector sizes in _vectorCache to make a good view on the screen.
		/// 
		/// Space scaling:
		///		scale (x, y, z) to fit into view
		///	Length scaling:
		///		scale (vx, vy, vz) to fit into view
		/// </summary>
		private void scaleVectorsFitView()
		{
			if (_vectorCache != null && _vectorCache.Length > 0)
			{
				double fitLocation = vectorLocationFitFactor * viewSize;
				double fitLength = vectorLengthFitFactor * viewSize;
				double spaceScale = 1.0;
				double lengthScale = 1.0;
				double maxSpace = 0.0;
				double maxLength = 0.0;
				for (int i = 0; i < _vectorCache.Length; i++)
				{
					if (Math.Abs(_vectorCache[i].x) > maxSpace) maxSpace = Math.Abs(_vectorCache[i].x);
					if (Math.Abs(_vectorCache[i].y) > maxSpace) maxSpace = Math.Abs(_vectorCache[i].y);
					if (Math.Abs(_vectorCache[i].z) > maxSpace) maxSpace = Math.Abs(_vectorCache[i].z);
					//
					if (Math.Abs(_vectorCache[i].vx) > maxLength) maxLength = Math.Abs(_vectorCache[i].vx);
					if (Math.Abs(_vectorCache[i].vy) > maxLength) maxLength = Math.Abs(_vectorCache[i].vy);
					if (Math.Abs(_vectorCache[i].vz) > maxLength) maxLength = Math.Abs(_vectorCache[i].vz);
				}
				if (maxSpace > 0.0)
				{
					spaceScale = fitLocation / maxSpace;
				}
				if (maxLength > 0.0)
				{
					lengthScale = fitLength / maxLength;
				}
				for (int i = 0; i < _vectorCache.Length; i++)
				{
					_vectorCache[i].x *= spaceScale;
					_vectorCache[i].y *= spaceScale;
					_vectorCache[i].z *= spaceScale;
					//
					_vectorCache[i].vx *= lengthScale;
					_vectorCache[i].vy *= lengthScale;
					_vectorCache[i].vz *= lengthScale;
				}
				_openGL.SetMinimumVectorLength(viewSize * vectorLengthPercent);
			}
		}

		/// <summary>
		/// draw using data in array variable vectors
		/// </summary>
		private void drawVectors()
		{
			if (!refreshDrawings())
			{
				/*
					 each vector holds space location and a vector at the location
				*/
				_openGL.SetColor(1, 0, 0);
				//collect and draw 3D vectors
				string title = this.Text;
				int n = vectors.Length / 100; //number of collected vectors when the progress-bar should be updated
				if (n <= 0) n = 1;
				showProgressBar(vectors.Length, "drawing vectors, please wait ...");
				List<Vector3D> vs = new List<Vector3D>(); //collected vectors
				_openGL.SetMinimumVectorLength(maximumVectorLength * vectorLengthPercent);
				if (_isInSphere)
				{
					//array vectors is in radius indexing: vectors at smaller radiuses are at front positions of the array
					if (_oneRadiusOnly)  //only show vectors on one radius specified by _radius
					{
						drawAtRadius(vs, n, _radius);
					}
					else //draw all vectors
					{
						UInt32 r = 0;        //radius
						bool ok = true;
						UInt32 maxR = SphereUtility.GetSphereRadius((UInt32)vectors.Length);
						while (r <= maxR && ok)
						{
							ok = drawAtRadius(vs, n, r);
							if (ok)
							{
								if (_skipRadius > 0) //number of radius to be ignored
								{
									//skip some radius so that the drawing is not too dense
									r += _skipRadius;
								}
								else
								{
									r++;
								}
							}
						}
					}
				}
				else //simply draw all vectors
				{
					int k = 0; //number of collected vectors before last progress-bar update
					for (int i = 0; i < vectors.Length; i++)
					{
						if (_openGL.DrawVector(vectors[i]))
						{
							vs.Add(vectors[i]);
						}
						k++;
						if (k > n)
						{
							k = 0;
							progressBar1.Value = i;
						}
					}
				}
				_vectorCache = vs.ToArray();
				scaleVectorsFitView();
				progressBar1.Hide();
				this.Text = title;
			}
		}

		///////////////////////////////////////
		/// <summary>
		/// draw vectors in _vectorCache
		/// </summary>
		/// <returns>true: _vectorCache is not null; false: _vectorCache is null</returns>
		private bool refreshDrawings()
		{
			if (_vectorCache != null) //3D vector objects already collected, just draw them 
			{
				_openGL.SetColor(1, 0, 0);
				for (int i = 0; i < _vectorCache.Length; i++)
				{
					_openGL.DrawVector(_vectorCache[i]);
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		/*
			draw vector data in 3D space
		 * one of following variables holds the data to be drawn, listed below in an order of priority:
		 * field   - it is a 3D array of Point3D in row-major format, array position represent space location;
		 *			Point3D represents a vector at the space location
		 * vectors - it is a one-dimentional array of Vector3D which specifies both a space location and a vector
		 *			at the space location;
		 */
		private void drawContents()
		{
			if (field != null) //draw data in field which is a 3D array of Point3D in row-major format, not in radius index
			{
				drawField();
			}
			else if (vectors != null) //draw vectors
			{
				drawVectors();
			}
			else if (emField != null) //draw EM fields
			{
				refreshDrawings();
			}
			else
			{
				//for testing
				//vectors = new Vector3D[2];
				//Vector3D v1 = new Vector3D(0, 0, 0, 30, 30, 30);
				//Vector3D v2 = new Vector3D(10, 5, -10, 0, 50, -5);
				//vectors[0] = v1;
				//vectors[1] = v2;
			}
			if (lines != null && lines.Length > 0)
			{
				for (int i = 0; i < lines.Length; i++)
				{
					Line3D l = lines[i];
					_openGL.DrawLine(ref l);
				}
			}
		}
		#endregion

		#region handle control events

		/// <summary>
		/// Handles the OpenGLDraw event of the openGLControl control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="RenderEventArgs"/> instance containing the event data.</param>
		private void openGLControl_OpenGLDraw(object sender, RenderEventArgs e)
		{
			_openGL.PrepareDrawingArea();
			//
			drawContents();
		}

		/// <summary>
		/// Handles the OpenGLInitialized event of the openGLControl control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
		private void openGLControl_OpenGLInitialized(object sender, EventArgs e)
		{
			if (_openGL == null)
			{
				_openGL = new OpenGLUtility(openGLControl.OpenGL, openGLControl.ClientSize.Width, openGLControl.ClientSize.Height, viewSize, gridSize);
			}
			_openGL.OnInitialized();
		}

		/// <summary>
		/// Handles the Resized event of the openGLControl control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
		private void openGLControl_Resized(object sender, EventArgs e)
		{
			_openGL.OnResize(openGLControl.ClientSize.Width, openGLControl.ClientSize.Height);
		}

		private void openGLControl_MouseDown(object sender, MouseEventArgs e)
		{
			_openGL.OnMouseDown(e);
		}
		private void openGLControl_MouseUp(object sender, MouseEventArgs e)
		{
			_openGL.OnMouseUp(e);
		}

		private void openGLControl_MouseMove(object sender, MouseEventArgs e)
		{
			_openGL.OnMouseMove(e);
		}
		private void resetRotationToolStripMenuItem_Click(object sender, EventArgs e)
		{
			_openGL.ResetRotation();
		}
		#endregion

		#region context menu handlers
		private void showEMSImDataFilesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load EM Simulation Data Files";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				if (LoadEMDataFromFile(dlg.FileName))
				{
					if (_controlForm == null)
					{
						_controlForm = new FormDataFilesControl();
					}
					if (_controlForm.SetDataFile(this, dlg.FileName))
					{
						_controlForm.SetFitFactors(vectorLocationFitFactor * 100.0, vectorLengthFitFactor * 100.0);
						_controlForm.Hide();
						_controlForm.Show(this);
					}
					else
					{
						_controlForm.Hide();
					}
				}
			}
		}
		private void showEMFieldFilesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load EM Simulation Field Files";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				if (LoadEMFieldFromFile(dlg.FileName))
				{
					if (_controlForm == null)
					{
						_controlForm = new FormDataFilesControl();
					}
					if (_controlForm.SetDataFile(this, dlg.FileName))
					{
						_controlForm.SetFitFactors(vectorLocationFitFactor * 100.0, vectorLengthFitFactor * 100.0);
						_controlForm.Hide();
						_controlForm.Show(this);
					}
					else
					{
						_controlForm.Hide();
					}
				}
			}
		}
		private void loadField3DToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load 3D scalar field";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				string infoFile = string.Format(CultureInfo.InvariantCulture, "{0}.info", dlg.FileName);
				if (!File.Exists(infoFile))
				{
					MessageBox.Show(this, string.Format(CultureInfo.InvariantCulture, "File not found:{0}", infoFile), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
				else
				{
					load3Dscalarfile(dlg.FileName);
				}
			}
		}


		private void loadField3DVectorToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load 3D vector field";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				string infoFile = string.Format(CultureInfo.InvariantCulture, "{0}.info", dlg.FileName);
				if (!File.Exists(infoFile))
				{
					MessageBox.Show(this, string.Format(CultureInfo.InvariantCulture, "File not found:{0}", infoFile), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
				else
				{
					load3Dvectorfile(dlg.FileName);
				}
			}
		}

		private void loadField3DVectorsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load 3D Vectors";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				_isInSphere = false;
				load3DVectors(dlg.FileName);
			}
		}

		private void setDrawingPercentToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (_parameterForm != null)
			{
				if (_parameterForm.Disposing || _parameterForm.IsDisposed)
				{
					_parameterForm = null;
				}
			}
			if (_parameterForm == null)
			{
				_parameterForm = new FormDrawParameters();
			}
			if (_isInSphere)
			{
				if (_maxRadius == 0)
				{
					_maxRadius = SphereUtility.GetSphereRadius((UInt32)vectors.Length);
				}
			}
			_parameterForm.SetParameters(vectorLengthPercent * 100, gridSize, viewSize, _skipRadius, _skipSpaceStep, _oneRadiusOnly, _radius, _maxRadius, emShow);
			_parameterForm.SetFitFactors(vectorLocationFitFactor * 100.0, vectorLengthFitFactor * 100.0);
			_parameterForm.SetDrawingForm(this);
			_parameterForm.Show(this);
		}

		private void load3DFieldSphereToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load 3D Field Sphere";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				_isInSphere = true;
				_maxRadius = 0;
				load3DVectors(dlg.FileName);
				_maxRadius = SphereUtility.GetSphereRadius((UInt32)vectors.Length);
				if (_parameterForm != null && !_parameterForm.IsDisposed && !_parameterForm.Disposing)
				{
					_parameterForm.SetRadiusOnly(_maxRadius);
				}
			}
		}
		private void showRadiusIndexToolStripMenuItem_Click(object sender, EventArgs e)
		{
			UInt32 r = 3;
			N = 0;
			field = null;
			vectors = null;
			emField = null;
			Vector3D[] vs = new Vector3D[SphereUtility.PointsInSphere(r)];
			RadiusIndexingDemo vr = new RadiusIndexingDemo(r, vs);
			if (vr.GetLastHandlerError() == SphereUtility.ERR_OK)
			{
				maximumVectorLength = vr.MaxVectorLength;
				vectors = vs;
				_isInSphere = true;
				_vectorCache = null;
				this.Refresh();
			}
		}

		private void showDivergenceReportToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Title = "Load Divergence Summary Report";
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
			{
				_isInSphere = true;
				_maxRadius = 0;
				loadDivergenceReport(dlg.FileName);
				if (_parameterForm != null && !_parameterForm.IsDisposed && !_parameterForm.Disposing)
				{
					_parameterForm.SetRadiusOnly(_maxRadius);
				}
				showRotateAndShiftToolStripMenuItem_Click(this, EventArgs.Empty);
			}
		}

		private void showRotateAndShiftToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (frmRotate == null || frmRotate.IsDisposed || frmRotate.Disposing)
			{
				frmRotate = new FormRotate();
			}
			frmRotate.Show(this);
			frmRotate.SetDrawingOwner(this);
		}

		#endregion

		/// <summary>
		/// Load EM fields from a data file generated by a EM simulation,
		/// draw the fields in the 3D drawing form
		/// </summary>
		class EMFieldPoints : IProcessFieldBySpace
		{
			private int ret;               //error code
			private UInt32 index;          //item index of the data being read
			private UInt32 maxRadius;      //the maximum radius
			private uint _r;               //radius of data being read
			private bool _needDraw;        //indicate whether points on a radius need to be drawn
			private bool[] _usages;        //space points to be ignored or used
			private UInt32 _countAtRadius; //number of space points on a radius
			BinaryReader br;               //file reader
			private SharpGLForm _form;     //3D drawing form
			//
			/// <summary>
			/// constructor
			/// </summary>
			/// <param name="drawingForm">3D drawing form</param>
			public EMFieldPoints(SharpGLForm drawingForm)
			{
				ret = 0;
				maxRadius = 0;
				_form = drawingForm;
				br = null;
				_usages = null;
				index = 0;
			}
			/// <summary>
			/// load data file and draw on the 3D drawing form
			/// </summary>
			/// <param name="file">data file</param>
			public void LoadFromFile(string file)
			{
				if (file == null || file.Length == 0)
				{
					throw new Exception("Error calling LoadFromFile, file not specified");
				}
				if (!File.Exists(file))
				{
					throw new Exception(string.Format(CultureInfo.InvariantCulture, "Error calling LoadFromFile, file does not exist ({0})", file));
				}
				//
				int itemsize = 6 * sizeof(double); //there are 3 electric field components and 3 magnetic field components
				FileInfo fi = new FileInfo(file);
				if ((fi.Length % itemsize) != 0)
				{
					throw new Exception(string.Format(CultureInfo.InvariantCulture, "Error loading EM data file {0}; invalid file size", file));
				}
				UInt32 items = (UInt32)(fi.Length / itemsize);  //total number of items
				maxRadius = SphereUtility.GetSphereRadius(items); //the maximum radius
				index = 0;
				_usages = null;
				_form.StartCollectingVectors(maxRadius); //prepare the 3D drawing form 
				br = new BinaryReader(new FileStream(file, FileMode.Open)); //open file reader
				_form.Text = file;
				ret = SphereUtility.GoThroughSpherePoints(maxRadius, 0.01, this); //start reading the file
			}
			#region IProcessFieldBySpace
			/// <summary>
			/// it is called when radius changes
			/// </summary>
			/// <param name="r">the radius</param>
			/// <returns>indicates how to continue</returns>
			public RadiusHandleType setRadius(uint r)
			{
				_r = r;
				if (_form.ShowOnlyOneRadius) //only draw points on one radius
				{
					_needDraw = (_form.RadiusToShow == _r); //only draw points on this radius
				}
				else
				{
					_needDraw = true;
				}
				if (_needDraw)
				{
					if (_form.SkipSpacePoints > 0)
					{
						//find out which points to draw
						_usages = SphereUtility.SkipSpacePoints(r, _form.SkipSpacePoints);
						_countAtRadius = 0; //start counting
					}
				}
				if (_form.ShowOnlyOneRadius && _needDraw)
					return RadiusHandleType.ProcessAndFinish; //finish after reading this radius
				else
					return RadiusHandleType.NeedProcess; //must sequentially read through the file
			}
			/// <summary>
			/// read and draw the point
			/// </summary>
			/// <param name="x">x-axis location</param>
			/// <param name="y">y-axis location</param>
			/// <param name="z">z-axis location</param>
			public void handleData(double x, double y, double z)
			{
				bool drawIt = false;
				//read fields from file
				EMField3D em = new EMField3D();
				em.Ex = br.ReadDouble();
				em.Ey = br.ReadDouble();
				em.Ez = br.ReadDouble();
				em.Hx = br.ReadDouble();
				em.Hy = br.ReadDouble();
				em.Hz = br.ReadDouble();
				if (_needDraw) //need to draw on this radius
				{
					if (_form.SkipSpacePoints == 0) //draw all points
					{
						drawIt = true;
					}
					else
					{
						if (_usages == null) //this should not happen
						{
							throw new Exception("_usages is null when reading data file");
						}
						else
						{
							drawIt = _usages[_countAtRadius];
							_countAtRadius++;
						}
					}
				}
				if (drawIt)
				{
					//draw field at this space point
					_form.CollectEMFieldPoint(x, y, z, em);
				}
				index++;
			}

			public int GetLastHandlerError()
			{
				return ret;
			}

			public void onFinish()
			{
				if (br != null)
				{
					br.Close();
				}
				_form.FinishCollectingVectors();
			}
			#endregion
		}

		/// <summary>
		/// use vectors to demonstrate radius indexing
		/// </summary>
		class RadiusIndexingDemo : IProcessFieldBySpace
		{
			private int ret;
			private UInt32 index;
			private UInt32 maxRadius;
			private Vector3D[] _objs;
			private uint _r;
			public RadiusIndexingDemo(UInt32 maxR, Vector3D[] vs)
			{
				ret = SphereUtility.ERR_OK;
				maxRadius = maxR;
				_objs = vs;
				index = 0;
				ret = SphereUtility.GoThroughSpherePoints(maxRadius, 10.0, this);
			}
			public double MaxVectorLength
			{
				get { return (double)maxRadius; }
			}
			#region IProcessFieldBySpace
			public RadiusHandleType setRadius(uint r)
			{
				_r = r;
				return RadiusHandleType.NeedProcess;
			}

			public void handleData(double x, double y, double z)
			{
				//create a vector at space point (x,y,z), 
				//vector direction is (0,0,0)-(x,y,z),
				//vector length is the radius of the space point
				if (_r > 0)
				{
					_objs[index] = new Vector3D(x, y, z, x / (double)_r, y / (double)_r, z / (double)_r);
				}
				else
				{
					_objs[index] = new Vector3D(x, y, z, 0, 0, 0);
				}
				index++;
			}

			public int GetLastHandlerError()
			{
				return ret;
			}

			public void onFinish()
			{

			}
			#endregion
		}

	}
}

/*
	specify which field to show.
 * do not change the enum values, they are used as hard-coded integers in UI
 */
public enum EMShowSelect
{
	ShowElectric = 0,
	ShowMagnetic = 1,
	Both         = 2
}

/*
 data file formats for recording EM fields
 */
public enum EMFileFormat
{
	EMinSohere    = 0, //no space points are recorded; field items assume space points in radius indexing
	EMfieldPoints = 1  //a space point is recorded with a field item.
}
