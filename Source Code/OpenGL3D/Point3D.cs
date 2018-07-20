/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/
using System;
using System.Collections.Generic;

namespace Field3D
{
	struct Point3D
	{
		public double x;
		public double y;
		public double z;
		public Point3D(double x0, double y0, double z0)
		{
			x = x0; y = y0; z = z0;
		}
		public static Point3D crossProduct(ref Point3D a, ref Point3D b)
		{
			return new Point3D(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
				);
		}
	}

	class Line3D
	{
		public Point3D p1;
		public Point3D p2;
		public Line3D()
		{
			p1.x = 0; p1.y = 0; p1.z = 0;
			p2.x = 0; p2.y = 0; p2.z = 0;
		}
	}

	struct Vector3D
	{
		public double x;
		public double y;
		public double z;
		public double vx;
		public double vy;
		public double vz;
		public Vector3D(double x0, double y0, double z0, double vx0, double vy0, double vz0)
		{
			x = x0; y = y0; z = z0; vx = vx0; vy = vy0; vz = vz0;
		}
	}

	public struct EMField3D
	{
		public double Ex;
		public double Ey;
		public double Ez;
		public double Hx;
		public double Hy;
		public double Hz;
		public EMField3D(double Ex0, double Ey0, double Ez0, double Hx0, double Hy0, double Hz0)
		{
			Ex = Ex0; Ey = Ey0; Ez = Ez0; Hx = Hx0; Hy = Hy0; Hz = Hz0;
		}
	}
}
