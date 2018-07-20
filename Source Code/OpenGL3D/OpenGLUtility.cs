/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/
using SharpGL;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	/// <summary>
	/// OpenGL utility
	/// </summary>
	class OpenGLUtility
	{
		private OpenGL gl; //OpenGL library
		private RotatorMouseController _rotatorController; //use mouse to rotate the 3D view
		private int displayWidth;
		private int displayHeight;
		private double viewSize;
		private double gridSize;
		private double minimumLineLength = 0.1;
		private double maximumArrowLength = 0.5;
		private double shiftX = 0.0;
		private double shiftY = 0.0;
		private double shiftZ = 0.0;
		public OpenGLUtility(OpenGL ogl, int width, int height, double view_size, double grid_size)
		{
			gl = ogl;
			gridSize = grid_size;
			displayHeight = height;
			displayWidth = width;
			SetViewSize(view_size);
			_rotatorController = new RotatorMouseController(viewSize, width, height);
		}
		public void SetViewSize(double view_size)
		{
			viewSize = view_size;
			if (viewSize > 100)
			{
				maximumArrowLength = 0.08 + (viewSize - 5.0) / 200.0;
			}
			else if (viewSize > 50)
			{
				maximumArrowLength = 0.08 + (viewSize - 5.0) / 150.0;
			}
			else if (viewSize > 20)
			{
				maximumArrowLength = 0.08 + (viewSize - 5.0) / 50.0;
			}
			else
			{
				maximumArrowLength = 0.08 + (viewSize - 5.0) / 10.0;
			}
			
		}
		public void SetGridSize(double grid_size)
		{
			gridSize = grid_size;
		}
		public void SetMinimumVectorLength(double length)
		{
			minimumLineLength = length;
		}
		public void SetColor(int red, int green, int blue)
		{
			gl.Color(red, green, blue);
		}
		public void PrepareDrawingArea()
		{
			//  Clear the color and depth buffer.
			gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);

			//  Load the identity matrix.
			gl.LoadIdentity();

			gl.Rotate(_rotatorController.Angle, _rotatorController.Ux, _rotatorController.Uy, _rotatorController.Uz);
			//
			DrawAxisGrids();
		}
		public void OnResize(int width, int height)
		{
			displayWidth = width;
			displayHeight = height;
			//  TODO: Set the projection matrix here.

			_rotatorController.OnViewSizeChange(width, height);

			//  Set the projection matrix.
			gl.MatrixMode(OpenGL.GL_PROJECTION);

			//  Load the identity.
			gl.LoadIdentity();

			//  Create a perspective transformation.
			gl.Perspective(60.0f, (double)width / (double)height, 0.01, 100.0);

			//  Use the 'look at' helper function to position and aim the camera.
			//gl.LookAt(-5, 5, -5, 0, 0, 0, 0, 1, 0);
			gl.LookAt(0.0, 0.0, viewSize, 0, 0, 0, 0, 1, 0);

			//  Set the modelview matrix.
			gl.MatrixMode(OpenGL.GL_MODELVIEW);

		}
		public void OnMouseDown(MouseEventArgs e)
		{
			_rotatorController.OnMouseDown(e);
		}
		public void OnMouseUp(MouseEventArgs e)
		{
			_rotatorController.OnMouseUp(e);
		}
		public void OnMouseMove(MouseEventArgs e)
		{
			_rotatorController.OnMouseMove(e);
		}
		public void ResetRotation()
		{
			_rotatorController.Reset();
		}
		public void RotateOnX(double angle)
		{
			_rotatorController.RotateOnX(angle);
		}
		public void RotateOnY(double angle)
		{
			_rotatorController.RotateOnY(angle);
		}
		public void RotateOnZ(double angle)
		{
			_rotatorController.RotateOnZ(angle);
		}
		public void OnInitialized()
		{
			gl.Enable(OpenGL.GL_BLEND);
			gl.BlendFunc(OpenGL.GL_SRC_ALPHA, OpenGL.GL_ONE_MINUS_SRC_ALPHA);

			//  Set the clear color.
			gl.ClearColor(0.9f, 0.9f, 0.9f, 0f);
		}
		public void SetShiftX(double sx)
		{
			shiftX = sx;
		}
		public void SetShiftY(double sy)
		{
			shiftY = sy;
		}
		public void SetShiftZ(double sz)
		{
			shiftZ = sz;
		}
		public double GetShiftX()
		{
			if (shiftX < -viewSize)
				shiftX = -viewSize;
			else if (shiftX > viewSize)
				shiftX = viewSize;
			return shiftX;
		}
		public double GetShiftY()
		{
			if (shiftY < -viewSize)
				shiftY = -viewSize;
			else if (shiftY > viewSize)
				shiftY = viewSize;
			return shiftY;
		}
		public double GetShiftZ()
		{
			if (shiftZ < -viewSize)
				shiftZ = -viewSize;
			else if (shiftZ > viewSize)
				shiftZ = viewSize;
			return shiftZ;
		}
		/// <summary>
		/// C++ source code from
		/// http://www.freemancw.com/2012/06/opengl-cone-function/#code
		/// </summary>
		/// <param name="v"></param>
		/// <returns></returns>
		private static Point3D perp(ref Point3D v)
		{
			double min = Math.Abs(v.x);
			Point3D cardinalAxis = new Point3D(1, 0, 0);

			if (Math.Abs(v.y) < min)
			{
				min = Math.Abs(v.y);
				cardinalAxis = new Point3D(0, 1, 0);
			}

			if (Math.Abs(v.z) < min)
			{
				cardinalAxis = new Point3D(0, 0, 1);
			}

			return Point3D.crossProduct(ref v, ref cardinalAxis);
		}

		/// <summary>
		/// draw a cone
		/// C++ code from http://www.freemancw.com/2012/06/opengl-cone-function/#code
		/// </summary>
		/// <param name="d">Axis defined as a normalized vector from base to apex</param>
		/// <param name="a">Position of apex</param>
		/// <param name="h">Height of cone</param>
		/// <param name="rd">Radius of directrix</param>
		/// <param name="n">Number of radial "slices"</param>
		public void DrawCone(ref Point3D d, ref Point3D a, double h, double rd, int n)
		{
			Point3D c = new Point3D(
				a.x - d.x * h,
				a.y - d.y * h,
				a.z - d.z * h
				);
			Point3D e0 = perp(ref d);
			Point3D e1 = Point3D.crossProduct(ref e0, ref d);
			double angInc = (360.0 / (double)n) * (Math.PI / 180.0);

			// calculate points of the cone
			List<Point3D> pts = new List<Point3D>();
			for (int i = 0; i <= n; ++i)
			{
				double rad = angInc * i;
				Point3D p = new Point3D(
					c.x + (((e0.x * Math.Cos(rad)) + (e1.x * Math.Sin(rad))) * rd),
					c.y + (((e0.y * Math.Cos(rad)) + (e1.y * Math.Sin(rad))) * rd),
					c.z + (((e0.z * Math.Cos(rad)) + (e1.z * Math.Sin(rad))) * rd)
					);
				pts.Add(p);
			}

			// draw cone top
			gl.Begin(SharpGL.Enumerations.BeginMode.TriangleFan);
			gl.Vertex(a.x, a.y, a.z);
			for (int i = 0; i <= n; ++i)
			{
				gl.Vertex(pts[i].x, pts[i].y, pts[i].z);
			}

			// draw cone bottom
			gl.Vertex(c.x, c.y, c.z);
			for (int i = n; i >= 0; --i)
			{
				gl.Vertex(pts[i].x, pts[i].y, pts[i].z);
			}
			gl.End();
		}

		/// <summary>
		/// draw cone at the end of the vector
		/// </summary>
		/// <param name="gl"></param>
		/// <param name="v"></param>
		public void DrawVectorArrow(ref Vector3D v)
		{
			double l = Math.Sqrt(v.vx * v.vx + v.vy * v.vy + v.vz * v.vz);
			if (l > minimumLineLength)
			{
				Point3D a = new Point3D(v.x + v.vx, v.y + v.vy, v.z + v.vz);
				Point3D d = new Point3D(v.vx / l, v.vy / l, v.vz / l);
				double h = l / 10.0;
				if (h > maximumArrowLength) h = maximumArrowLength;
				DrawCone(ref d, ref a, h, h / 2.0, 8);
			}
		}

		/*
			draw a vector if its length is longer than minimumLineLength
		 */
		public bool DrawVector(Vector3D v)
		{
			double l = Math.Sqrt(v.vx * v.vx + v.vy * v.vy + v.vz * v.vz);
			if (l > minimumLineLength)
			{
				v.x += shiftX;
				v.y += shiftY;
				v.z += shiftZ;
				Point3D a = new Point3D(v.x + v.vx, v.y + v.vy, v.z + v.vz); //vector end point
				Point3D d = new Point3D(v.vx / l, v.vy / l, v.vz / l); //normalized vector for drawing arrow
				double h = l / 10.0;
				//draw a line to represent the vector length and direction
				gl.Begin(SharpGL.Enumerations.BeginMode.Lines);
				gl.Vertex(v.x, v.y, v.z);
				gl.Vertex(a.x, a.y, a.z);
				gl.End();
				//draw an arrow to show vector orientation
				if (h > maximumArrowLength) h = maximumArrowLength;
				DrawCone(ref d, ref a, h, h / 2.0, 8);
				return true;
			}
			return false;
		}

		/// <summary>
		/// draw a line
		/// </summary>
		/// <param name="l"></param>
		public void DrawLine(ref Line3D l)
		{
			gl.Begin(SharpGL.Enumerations.BeginMode.Lines);
			gl.Vertex(l.p1.x, l.p1.y, l.p1.z);
			gl.Vertex(l.p2.x, l.p2.y, l.p2.z);
			gl.End();
		}

		/// <summary>
		/// draw x, y, z axis, draw grids on planes of x-y, y-z, and z-x
		/// </summary>
		/// <param name="viewSize"></param>
		public void DrawAxisGrids()
		{
			double gridPos;
			//  Draw axis lines
			gl.Begin(SharpGL.Enumerations.BeginMode.Lines);
			//x-axe
			gl.Color(1.0f, 0.0f, 0.0f);
			gl.Vertex(-viewSize, 0.0, 0.0);
			gl.Vertex(viewSize, 0.0, 0.0);
			//y-axe
			gl.Color(0.0f, 1.0f, 0.0f);
			gl.Vertex(0.0, -viewSize, 0.0);
			gl.Vertex(0.0, viewSize, 0.0);
			//z-axe
			gl.Color(0.0f, 0.0f, 1.0f);
			gl.Vertex(0.0, 0.0, -viewSize);
			gl.Vertex(0.0, 0.0, viewSize);
			//x-y plane
			gl.Color((byte)255, (byte)255, (byte)0, (byte)100);
			for (gridPos = gridSize; gridPos < viewSize; gridPos += gridSize)
			{
				gl.Vertex(gridPos, -viewSize, 0.0);
				gl.Vertex(gridPos, viewSize, 0.0);
				gl.Vertex(-gridPos, -viewSize, 0.0);
				gl.Vertex(-gridPos, viewSize, 0.0);
				gl.Vertex(-viewSize, gridPos, 0.0);
				gl.Vertex(viewSize, gridPos, 0.0);
				gl.Vertex(-viewSize, -gridPos, 0.0);
				gl.Vertex(viewSize, -gridPos, 0.0);
			}
			//y-z plane
			gl.Color((byte)0, (byte)255, (byte)255, (byte)100);
			for (gridPos = gridSize; gridPos < viewSize; gridPos += gridSize)
			{
				gl.Vertex(0.0, -viewSize, gridPos);
				gl.Vertex(0.0, viewSize, gridPos);
				gl.Vertex(0.0, -viewSize, -gridPos);
				gl.Vertex(0.0, viewSize, -gridPos);
				gl.Vertex(0.0, gridPos, -viewSize);
				gl.Vertex(0.0, gridPos, viewSize);
				gl.Vertex(0.0, -gridPos, -viewSize);
				gl.Vertex(0.0, -gridPos, viewSize);
			}
			//z-x plane
			gl.Color((byte)255, (byte)0, (byte)255, (byte)100);
			for (gridPos = gridSize; gridPos < viewSize; gridPos += gridSize)
			{
				gl.Vertex(-viewSize, 0.0, gridPos);
				gl.Vertex(viewSize, 0.0, gridPos);
				gl.Vertex(-viewSize, 0.0, -gridPos);
				gl.Vertex(viewSize, 0.0, -gridPos);
				gl.Vertex(gridPos, 0.0, -viewSize);
				gl.Vertex(gridPos, 0.0, viewSize);
				gl.Vertex(-gridPos, 0.0, -viewSize);
				gl.Vertex(-gridPos, 0.0, viewSize);
			}
			gl.End();
			//draw axis arrows
			Vector3D axe = new Vector3D(0, 0, 0, viewSize, 0, 0);
			gl.Color(1.0f, 0.0f, 0.0f);
			DrawVectorArrow(ref axe);
			axe = new Vector3D(0, 0, 0, 0, viewSize, 0);
			gl.Color(0.0f, 1.0f, 0.0f);
			DrawVectorArrow(ref axe);
			axe = new Vector3D(0, 0, 0, 0, 0, viewSize);
			gl.Color(0.0f, 0.0f, 1.0f);
			DrawVectorArrow(ref axe);
		}
	}
}
