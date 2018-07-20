/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace Draw3D
{
	/*
	* return value of setRadius
	*/
	public enum RadiusHandleType
	{
		NeedProcess      = 0, //I want to process the radius
		DoNotProcess     = 1, //I do not want to process the radius
		Finish           = 2, //I do not want to process the radius and all other radius
		ProcessAndFinish = 3, //I want to process the radius, but I will not process all other radius
	}

	/*
	 * space digitization for EM simulation
	 */
	static class SpaceDigitizer
	{
		/// <summary>
		/// N is a number of double space intervals at one side of an axis, starting at one space interval,
		/// the index for the far end of the positive axis is 2N+1
		/// </summary>
		/// <param name="N">double space intervals at one side of an axis, starting at one space interval</param>
		/// <returns>max radius</returns>
		public static UInt32 GRID_RADIUS(UInt32 N)
		{
			return 2 * N + 1;
		}

		/// <summary>
		/// for a given half grid number get total space points.
		/// at one side of axis, there are 2*i_N + 1 data points,
		/// for both sides, there are 2*(2*i_N+1)=4*i_N+2 data points,
		/// adding the data point at 0, totally there are 4*i_N+3 data points
		/// </summary>
		/// <param name="N">double space intervals at one side of an axis, starting at one space interval</param>
		/// <returns>total space points</returns>
		public static UInt32 SPACE_POINTS(UInt32 N)
		{
			return 4 * N + 3;
		}

		/// <summary>
		/// calculate space interval size.
		/// range is the (2*i_N+1)-th data point. because it starts at one space interval, not at 0, there are 2*i_N+1 space intervals at
		/// one side of axis, therefore, the size of space interval is the range divided by 2*i_N+1
		/// </summary>
		/// <param name="range">a positive value, it is the space location corresponding to the data point at the far end of positive side of axis</param>
		/// <param name="N">double space intervals at one side of an axis, starting at one space interval</param>
		/// <returns>space interval size</returns>
		public static double SPACE_INTERVAL(double range, UInt32 N)
		{
			return range / (double)GRID_RADIUS(N);
		}

		/// <summary>
		/// FFT frequency resolution
		/// </summary>
		/// <param name="halfFFTrange">FFT half range</param>
		/// <returns></returns>
		public static double FFT_FREQSTEP(double halfFFTrange)
		{
			return Math.PI / halfFFTrange;
		}

		/// <summary>
		/// due to FFT symmetry, only half sample points are non-redundant.
		/// number of half sample points = Int{(4*N+3)/2}=Int{2*N + 1.5}=2*N+2=2(N+1)
		/// </summary>
		/// <param name="N"></param>
		/// <returns></returns>
		public static UInt32 FFTW_HALF_N(UInt32 N)
		{
			return 2 * (N + 1);
		}

		/// <summary>
		/// give space range and number of space intervals, calculate related space digitizing values
		/// </summary>
		/// <param name="range">space range at one side of the axis</param>
		/// <param name="N">double space intervals at one side of an axis, starting at one space interval</param>
		/// <param name="ds">space interval size</param>
		/// <param name="maxN">space points on one axis</param>
		/// <param name="maxN2">space points on a plane</param>
		/// <param name="maxN3">space points in a cubic</param>
		/// <param name="minSpace">far end of negative side of an axis</param>
		/// <param name="maxR">maximum radius</param>
		public static void SpaceDigitize(double range, UInt32 N, out double ds, out UInt32 maxN, out UInt64 maxN2, out UInt64 maxN3, out double minSpace, out UInt32 maxR)
		{
			ds = SPACE_INTERVAL(range, N);
			maxN = SPACE_POINTS(N);
			maxN2 = maxN * maxN;
			maxN3 = maxN * maxN2;
			minSpace = - (double)GRID_RADIUS(N) * ds;
			maxR = GRID_RADIUS(N);
		}
	}

	/// <summary>
	/// interface for going through space point by point
	/// </summary>
	public interface IProcessField
	{
		void processAtPoint(double x, double y, double z);
	}

	/// <summary>
	/// interface for going through space by radius indexing,
	/// radius by radius,
	/// each space point is reprsented by radius index m,n,p
	/// </summary>
	public interface IProcessFieldByIndex
	{
		RadiusHandleType setRadius(int r);
		void handleData(int m, int n, int p);
		void onFinish();
	}

	/// <summary>
	/// interface for going through space by radius indexing,
	/// radius by radius,
	/// each point is represented by space location x,y,z
	/// </summary>
	public interface IProcessFieldBySpace
	{
		RadiusHandleType setRadius(UInt32 r);
		void handleData(double x, double y, double z);
		int GetLastHandlerError();
		void onFinish();
	}

	/// <summary>
	/// sphere radius indexing utility
	/// </summary>
	public sealed class SphereUtility
	{
		public const int ERR_OK = 0;
		/// <summary>
		/// going through space in radius indexing, radius by radius,
		/// each space point is reprsented by a 3D space location (x,y,z)
		/// </summary>
		/// <param name="maxRadius">the maximum radius</param>
		/// <param name="ds">space step size</param>
		/// <param name="proc">object to handle each space point</param>
		/// <returns>error code</returns>
		public static int GoThroughSpherePoints(UInt32 maxRadius, double ds, IProcessFieldBySpace proc)
		{
			int ret = ERR_OK;
			double x1, x2, y1, y2, z1, z2;
			double d, dd;
			double radius = 0; //radius space location
			UInt32 j, k;
			UInt32 r; //radius index number
			RadiusHandleType rht;
			//
			//special handling for origin point
			//r = 0
			//0: (0,0,0)
			rht = proc.setRadius(0);
			if (rht == RadiusHandleType.Finish)
			{
				proc.onFinish();
				return ret;
			}
			if (rht != RadiusHandleType.DoNotProcess)
			{
				proc.handleData(0, 0, 0);
			}
			if (rht == RadiusHandleType.ProcessAndFinish)
			{
				proc.onFinish();
				return ret;
			}
			//go through space radius by radius
			for (r = 1; r <= maxRadius; r++)
			{
				ret = proc.GetLastHandlerError();
				if (ret != ERR_OK)
				{
					break;
				}
				//notify radius change
				rht = proc.setRadius(r);
				if (rht == RadiusHandleType.DoNotProcess)
				{
					continue;
				}
				else if (rht == RadiusHandleType.Finish)
				{
					break;
				}
				//initialize
				radius += ds;
				//pointCount = 0;
				//
				//1. 8 Corners. Three edges: 8 points: 0 - 7
				x1 = radius; x2 = -x1;
				y1 = radius; y2 = -y1;
				z1 = radius; z2 = -z1;
				//
				//source->getField(x1, y1, z1, &p);     // 0: (r, r, r)
				proc.handleData(x1, y1, z1);
				//source->getField(x1, y1, z2, &p);     // 1: (r, r, -r)
				proc.handleData(x1, y1, z2);
				//source->getField(x1, y2, z1, &p);     // 2: (r, -r, r)
				proc.handleData(x1, y2, z1);
				//source->getField(x1, y2, z2, &p);     // 3: (r, -r, -r)
				proc.handleData(x1, y2, z2);
				//source->getField(x2, y1, z1, &p);     // 4: (-r, r, r)
				proc.handleData(x2, y1, z1);
				//source->getField(x2, y1, z2, &p);     // 5: (-r, r, -r)
				proc.handleData(x2, y1, z2);
				//source->getField(x2, y2, z1, &p);     // 6: (-r, -r, r)
				proc.handleData(x2, y2, z1);
				//source->getField(x2, y2, z2, &p);     // 7: (-r, -r, -r)
				proc.handleData(x2, y2, z2);
				//
				//pointCount += 8;
				//last index: 7
				//
				//2. 12 Edge Line Centers. Two axes at edges, one axis at 0: 12 points: 8 - 19
				//source->getField(x1, y1, 0, &p);     // 8: (r, r, 0)
				proc.handleData(x1, y1, 0);
				//source->getField(x1, y2, 0, &p);     // 9: (r, -r, 0)
				proc.handleData(x1, y2, 0);
				//source->getField(x2, y1, 0, &p);     // 10: (-r, r, 0)
				proc.handleData(x2, y1, 0);
				//source->getField(x2, y2, 0, &p);     // 11: (-r, -r, 0)
				proc.handleData(x2, y2, 0);
				//
				//source->getField(0, y1, z1, &p);    // 12: (0, r, r)
				proc.handleData(0, y1, z1);
				//source->getField(0, y1, z2, &p);    // 13: (0, r, -r)
				proc.handleData(0, y1, z2);
				//source->getField(0, y2, z1, &p);    // 14: (0, -r, r)
				proc.handleData(0, y2, z1);
				//source->getField(0, y2, z2, &p);    // 15: (0, -r, -r)
				proc.handleData(0, y2, z2);
				//
				//source->getField(x1, 0, z1, &p);   // 16: (r, 0, r)
				proc.handleData(x1, 0, z1);
				//source->getField(x1, 0, z2, &p);   // 17: (r, 0, -r)
				proc.handleData(x1, 0, z2);
				//source->getField(x2, 0, z1, &p);   // 18: (-r, 0, r)
				proc.handleData(x2, 0, z1);
				//source->getField(x2, 0, z2, &p);   // 19: (-r, 0, -r)
				proc.handleData(x2, 0, z2);
				//pointCount += 12;
				//last index : 19
				//
				//3.  plane centers. two axes at 0: 6 points from 24r-4 to 24r+1
				//source->getField(x1, 0, 0, &p);   // 20: (r, 0, 0)
				proc.handleData(x1, 0, 0);
				//source->getField(x2, 0, 0, &p);   // 21: (-r, 0, 0)
				proc.handleData(x2, 0, 0);
				//source->getField(0, y1, 0, &p);   // 22: (0, r, 0)
				proc.handleData(0, y1, 0);
				//source->getField(0, y2, 0, &p);   // 23: (0, -r, 0)
				proc.handleData(0, y2, 0);
				//source->getField(0, 0, z1, &p);   // 24: (0, 0, r)
				proc.handleData(0, 0, z1);
				//source->getField(0, 0, z2, &p);   // 25: (0, 0, -r)
				proc.handleData(0, 0, z2);
				//last index: 25
				d = 0;
				//3. (r>1) 12 Edge Lines exclude center. Each line has 2(r-1)  points. Two axes at edge, one axis not at 0: 24(r-1)  points: 26 to 24r+1
				for (k = 1; k < r; k++)
				{
					ret = proc.GetLastHandlerError();
					if (ret != ERR_OK)
					{
						break;
					}
					d += ds;
					//source->getField(x1, y1, d, &p);   // 26+24*(k-1): (r, r, k) --------------
					proc.handleData(x1, y1, d);
					//source->getField(x1, y1, -d, &p);   // 27+24*(k-1): (r, r, -k) -------------
					proc.handleData(x1, y1, -d);
					//source->getField(x1, y2, d, &p);   // 28+24*(k-1): (r, -r, k) -------------
					proc.handleData(x1, y2, d);
					//source->getField(x1, y2, -d, &p);   // 29+24*(k-1): (r, -r, -k) -------------
					proc.handleData(x1, y2, -d);
					//
					//source->getField(x2, y1, d, &p);   // 30+24*(k-1): (-r, r, k) -----------------
					proc.handleData(x2, y1, d);
					//source->getField(x2, y1, -d, &p);   // 31+24*(k-1): (-r, r, -k) ------------------
					proc.handleData(x2, y1, -d);
					//source->getField(x2, y2, d, &p);   // 32+24*(k-1): (-r, -r, k) ------------------
					proc.handleData(x2, y2, d);
					//source->getField(x2, y2, -d, &p);   // 33+24*(k-1): (-r, -r, -k) -----------------
					proc.handleData(x2, y2, -d);
					//
					//source->getField(d, y1, z1, &p);   // 34+24*(k-1): (k, r, r) ---------------------
					proc.handleData(d, y1, z1);
					//source->getField(-d, y1, z1, &p);   // 35+24*(k-1): (-k, r, r) ----------------------------
					proc.handleData(-d, y1, z1);
					//source->getField(d, y1, z2, &p);   // 36+24*(k-1): (k, r, -r) --------------------
					proc.handleData(d, y1, z2);
					//source->getField(-d, y1, z2, &p);   // 37+24*(k-1): (-k, r, -r) ---------------------------
					proc.handleData(-d, y1, z2);
					//
					//source->getField(d, y2, z1, &p);   // 38+24*(k-1): (k, -r, r) ---------------------
					proc.handleData(d, y2, z1);
					//source->getField(-d, y2, z1, &p);   // 39+24*(k-1): (-k, -r, r) ---------------------------
					proc.handleData(-d, y2, z1);
					//source->getField(d, y2, z2, &p);   // 40+24*(k-1): (k, -r, -r) ----------------------
					proc.handleData(d, y2, z2);
					//source->getField(-d, y2, z2, &p);   // 41+24*(k-1): (-k, -r, -r) ---------------------------
					proc.handleData(-d, y2, z2);
					//
					//source->getField(x1, d, z1, &p);   // 42+24*(k-1): (r, k, r) ------------------------
					proc.handleData(x1, d, z1);
					//source->getField(x1, -d, z1, &p);   // 43+24*(k-1): (r, -k, r) ----------------------
					proc.handleData(x1, -d, z1);
					//source->getField(x2, d, z1, &p);   // 44+24*(k-1): (-r, k, r) -----------------------
					proc.handleData(x2, d, z1);
					//source->getField(x2, -d, z1, &p);   // 45+24*(k-1): (-r, -k, r) ---------------------
					proc.handleData(x2, -d, z1);
					//
					//source->getField(x1, d, z2, &p);   // 46+24*(k-1): (r, k, -r) ------------------------
					proc.handleData(x1, d, z2);
					//source->getField(x1, -d, z2, &p);   // 47+24*(k-1): (r, -k, -r) ----------------------
					proc.handleData(x1, -d, z2);
					//source->getField(x2, d, z2, &p);   // 48+24*(k-1): (-r, k, -r)  ------------------------
					proc.handleData(x2, d, z2);
					//source->getField(x2, -d, z2, &p);   // 49+24*(k-1): (-r, -k, -r) ------------------------
					proc.handleData(x2, -d, z2);
				}
				//last index : 49+24*(r-2) = 24r + 1
				//4 Inner parts of cross-lines on m-plane. One axis at 0 and m=±r:24r+2 to 32r-7,8(r-1) points
				d = 0;
				for (k = 1; k < r; k++)
				{
					ret = proc.GetLastHandlerError();
					if (ret != ERR_OK)
					{
						break;
					}
					d += ds;
					//m=+-r, n=0 //////////////////////////////////////////////
					//source->getField(x1, 0, d, &p);   // 24r + 2 + 24 * (k-1): (r, 0, k)    ---------------------
					proc.handleData(x1, 0, d);
					//source->getField(x1, 0, -d, &p);   // 24r + 3 + 24 * (k-1): (r, 0, -k)  ---------------------
					proc.handleData(x1, 0, -d);
					//source->getField(x2, 0, d, &p);   // 24r + 4 + 24 * (k-1): (-r, 0, k)  ---------------------------
					proc.handleData(x2, 0, d);
					//source->getField(x2, 0, -d, &p);   // 24r + 5 + 24 * (k-1): (-r, 0, -k) ------------------------
					proc.handleData(x2, 0, -d);
					//m=+-r, p=0
					//source->getField(x1, d, 0, &p);   // 24r + 6 + 24 * (k-1): (r, k, 0)    ----------------------
					proc.handleData(x1, d, 0);
					//source->getField(x1, -d, 0, &p);   // 24r + 7 + 24 * (k-1): (r, -k, 0)  ---------------------
					proc.handleData(x1, -d, 0);
					//source->getField(x2, d, 0, &p);   // 24r + 8 + 24 * (k-1): (-r, k, 0)   -------------------------
					proc.handleData(x2, d, 0);
					//source->getField(x2, -d, 0, &p);   // 24r + 9 + 24 * (k-1): (-r, -k, 0)  ---------------------------
					proc.handleData(x2, -d, 0);
					///////////////////////////////////////
					//n=+-r, m=0 ///////////////////////////////////////////////
					//source->getField(0, y1, d, &p);   // 24r + 10 + 24 * (k-1): (0, r, k)    ---------------------------
					proc.handleData(0, y1, d);
					//source->getField(0, y1, -d, &p);   // 24r + 11 + 24 * (k-1): (0, r, -k)   -------------------------
					proc.handleData(0, y1, -d);
					//source->getField(0, y2, d, &p);   // 24r + 12 + 24 * (k-1): (0, -r, k)     -----------------------------
					proc.handleData(0, y2, d);
					//source->getField(0, y2, -d, &p);   // 24r + 13 + 24 * (k-1): (0, -r, -k)   ----------------------------
					proc.handleData(0, y2, -d);
					//n=+-r, p=0 
					//source->getField(d, y1, 0, &p);   // 24r + 14 + 24 * (k-1): (k, r, 0)      --------------------------------
					proc.handleData(d, y1, 0);
					//source->getField(-d, y1, 0, &p);   // 24r + 15 + 24 * (k-1): (-k, r, 0)     -------------------------------
					proc.handleData(-d, y1, 0);
					//source->getField(d, y2, 0, &p);   // 24r + 16 + 24 * (k-1): (k, -r, 0)      --------------------------------
					proc.handleData(d, y2, 0);
					//source->getField(-d, y2, 0, &p);   // 24r + 17 + 24 * (k-1): (-k, -r, 0)    -----------------------------------
					proc.handleData(-d, y2, 0);
					/////////////////////////////////////////////
					//p=+-r, n=0 ///////////////////////////////////////////////
					//source->getField(d, 0, z1, &p);   // 24r + 18 + 24 * (k-1): (k, 0, r)       -----------------------------------
					proc.handleData(d, 0, z1);
					//source->getField(-d, 0, z1, &p);   // 24r + 19 + 24 * (k-1): (-k, 0, r)     ------------------------------------
					proc.handleData(-d, 0, z1);
					//source->getField(d, 0, z2, &p);   // 24r + 20 + 24 * (k-1): (k, 0, -r)      ------------------------------------
					proc.handleData(d, 0, z2);
					//source->getField(-d, 0, z2, &p);   // 24r + 21 + 24 * (k-1): (-k, 0, -r)     -----------------------------------
					proc.handleData(-d, 0, z2);
					//p=+-r, m=0
					//source->getField(0, d, z1, &p);   // 24r + 22 + 24 * (k-1): (0, k, r)        ----------------------------
					proc.handleData(0, d, z1);
					//source->getField(0, -d, z1, &p);   // 24r + 23 + 24 * (k-1): (0, -k, r)      ---------------------------------------
					proc.handleData(0, -d, z1);
					//source->getField(0, d, z2, &p);   // 24r + 24 + 24 * (k-1): (0, k, -r)         ----------------------------------
					proc.handleData(0, d, z2);
					//source->getField(0, -d, z2, &p);   // 24r + 25 + 24 * (k-1): (0, -k, -r)       ---------------------------------
					proc.handleData(0, -d, z2);
					//
				}
				//Last index: 24r + 25 + 24 * (k-1) = 24r + 25 + 24 * (r-2) = 48r - 23
				//5. one at edge, the other two not at 0
				d = 0;
				for (j = 1; j < r; j++)
				{
					ret = proc.GetLastHandlerError();
					if (ret != ERR_OK)
					{
						break;
					}
					d += ds;
					dd = 0;
					for (k = 1; k < r; k++)
					{
						ret = proc.GetLastHandlerError();
						if (ret != ERR_OK)
						{
							break;
						}
						dd += ds;
						//24 points
						//5.1 m=r
						//source->getField(x1, d, dd, &p);   // 48r - 22 + 24*[(k-1) + (r-1)*(j-1)]: (r, j, k)          -----------------------------
						proc.handleData(x1, d, dd);
						//source->getField(x1, d, -dd, &p);   // 48r - 21 + 24*[(k-1) + (r-1) * (j-1)]: (r, j, -k)      ------------------------------
						proc.handleData(x1, d, -dd);
						//source->getField(x1, -d, dd, &p);   // 48r - 20 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, k)      -------------------------------
						proc.handleData(x1, -d, dd);
						//source->getField(x1, -d, -dd, &p);   // 48r - 19 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, -k)   ---------------------------------
						proc.handleData(x1, -d, -dd);
						//m=-r
						//source->getField(x2, d, dd, &p);   // 48r - 18 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, k)       ----------------------------------
						proc.handleData(x2, d, dd);
						//source->getField(x2, d, -dd, &p);   // 48r - 17 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, -k)      ------------------------------------
						proc.handleData(x2, d, -dd);
						//source->getField(x2, -d, dd, &p);   // 48r - 16 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, k)       ----------------------------
						proc.handleData(x2, -d, dd);
						//source->getField(x2, -d, -dd, &p);   // 48r - 15 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, -k)    ------------------------------
						proc.handleData(x2, -d, -dd);
						//5.2 n=r
						//source->getField(d, y1, dd, &p);   // 48r - 14 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, k)     -----------------------------------
						proc.handleData(d, y1, dd);
						//source->getField(d, y1, -dd, &p);   // 48r - 13 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, -k)    -------------------------------------
						proc.handleData(d, y1, -dd);
						//source->getField(-d, y1, dd, &p);   // 48r - 12 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, k)     --------------------------------
						proc.handleData(-d, y1, dd);
						//source->getField(-d, y1, -dd, &p);   // 48r - 11 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, -k)  ----------------------------------
						proc.handleData(-d, y1, -dd);
						//n=-r
						//source->getField(d, y2, dd, &p);   // 48r - 10 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, k)    ----------------------------------
						proc.handleData(d, y2, dd);
						//source->getField(d, y2, -dd, &p);   // 48r - 9 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, -k)     ----------------------------------
						proc.handleData(d, y2, -dd);
						//source->getField(-d, y2, dd, &p);   // 48r - 8 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, k)       ------------------------------
						proc.handleData(-d, y2, dd);
						//source->getField(-d, y2, -dd, &p);   // 48r - 7 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, -k)     --------------------
						proc.handleData(-d, y2, -dd);
						//5.3 p=r
						//source->getField(d, dd, z1, &p);   // 48r - 6 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, r)       ---------------------------
						proc.handleData(d, dd, z1);
						//source->getField(-d, dd, z1, &p);   // 48r - 5 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, r)        ----------------------------
						proc.handleData(-d, dd, z1);
						//source->getField(d, -dd, z1, &p);   // 48r - 4 + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, r)       ------------------------
						proc.handleData(d, -dd, z1);
						//source->getField(-d, -dd, z1, &p);   // 48r - 3 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, r)   ----------------------------
						proc.handleData(-d, -dd, z1);
						//p=-r
						//source->getField(d, dd, z2, &p);   // 48r - 2 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, -r)       -----------------------------------
						proc.handleData(d, dd, z2);
						//source->getField(-d, dd, z2, &p);   // 48r - 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, -r)     ---------------------------------------
						proc.handleData(-d, dd, z2);
						//source->getField(d, -dd, z2, &p);   // 48r + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, -r)          -----------------------------------
						proc.handleData(d, -dd, z2);
						//source->getField(-d, -dd, z2, &p);   // 48r + 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, -r)  -------------------------------
						proc.handleData(-d, -dd, z2);
					}
				}
				//last index: 48r + 1 + 24*[(r-2) + (r-2) * (r-1)] = 48r + 1 + 24r - 48 + 24 (r^2 - 3r + 2) = 24 r^2 + 1
				if (rht == RadiusHandleType.ProcessAndFinish)
				{
					break;
				}
			}//r>0
			if (ret == ERR_OK)
			{
				proc.onFinish();
			}

			return ret;
		}

		/// <summary>
		/// going through space in radius indexing, radius by radius,
		/// each space point is reprsented by a radius indexing (m,n,p)
		/// </summary>
		/// <param name="maxRadius">the maximum radius</param>
		/// <param name="proc">the object to handle each space point</param>
		/// <returns>error code</returns>
		public static int GoThroughSpherePoints(UInt32 maxRadius, IProcessFieldByIndex proc)
		{
			int ret = ERR_OK;
			int j, k;
			int r;
			RadiusHandleType rht;

			//start from the origin point
			//r = 0
			//0: (0,0,0)
			rht = proc.setRadius(0);
			if (rht == RadiusHandleType.Finish)
			{
				proc.onFinish();
				return ret;
			}
			if (rht != RadiusHandleType.DoNotProcess)
			{
				//source->getField(0, 0, 0, &p);
				proc.handleData(0, 0, 0);
			}
			if (rht == RadiusHandleType.ProcessAndFinish)
			{
				proc.onFinish();
				return ret;
			}
			//go through space radius by radius
			for (r = 1; r <= maxRadius; r++)
			{
				if (ret != ERR_OK)
				{
					break;
				}
				//notify radius change
				rht = proc.setRadius(r);
				if (rht == RadiusHandleType.DoNotProcess)
				{
					continue;
				}
				else if (rht == RadiusHandleType.Finish)
				{
					break;
				}
				//initialize
				//radius += ds2;
				//pointCount = 0;
				//
				//1. 8 Corners. Three edges: 8 points: 0 - 7
				//x1 = radius; x2 = -x1;
				//y1 = radius; y2 = -y1;
				//z1 = radius; z2 = -z1;
				//
				//source->getField(x1, y1, z1, &p);     // 0: (r, r, r)
				proc.handleData(r, r, r);
				//source->getField(x1, y1, z2, &p);     // 1: (r, r, -r)
				proc.handleData(r, r, -r);
				//source->getField(x1, y2, z1, &p);     // 2: (r, -r, r)
				proc.handleData(r, -r, r);
				//source->getField(x1, y2, z2, &p);     // 3: (r, -r, -r)
				proc.handleData(r, -r, -r);
				//source->getField(x2, y1, z1, &p);     // 4: (-r, r, r)
				proc.handleData(-r, r, r);
				//source->getField(x2, y1, z2, &p);     // 5: (-r, r, -r)
				proc.handleData(-r, r, -r);
				//source->getField(x2, y2, z1, &p);     // 6: (-r, -r, r)
				proc.handleData(-r, -r, r);
				//source->getField(x2, y2, z2, &p);     // 7: (-r, -r, -r)
				proc.handleData(-r, -r, -r);
				//
				//pointCount += 8;
				//last index: 7
				//
				//2. 12 Edge Line Centers. Two axes at edges, one axis at 0: 12 points: 8 - 19
				//source->getField(x1, y1, 0, &p);     // 8: (r, r, 0)
				proc.handleData(r, r, 0);
				//source->getField(x1, y2, 0, &p);     // 9: (r, -r, 0)
				proc.handleData(r, -r, 0);
				//source->getField(x2, y1, 0, &p);     // 10: (-r, r, 0)
				proc.handleData(-r, r, 0);
				//source->getField(x2, y2, 0, &p);     // 11: (-r, -r, 0)
				proc.handleData(-r, -r, 0);
				//
				//source->getField(0, y1, z1, &p);    // 12: (0, r, r)
				proc.handleData(0, r, r);
				//source->getField(0, y1, z2, &p);    // 13: (0, r, -r)
				proc.handleData(0, r, -r);
				//source->getField(0, y2, z1, &p);    // 14: (0, -r, r)
				proc.handleData(0, -r, r);
				//source->getField(0, y2, z2, &p);    // 15: (0, -r, -r)
				proc.handleData(0, -r, -r);
				//
				//source->getField(x1, 0, z1, &p);   // 16: (r, 0, r)
				proc.handleData(r, 0, r);
				//source->getField(x1, 0, z2, &p);   // 17: (r, 0, -r)
				proc.handleData(r, 0, -r);
				//source->getField(x2, 0, z1, &p);   // 18: (-r, 0, r)
				proc.handleData(-r, 0, r);
				//source->getField(x2, 0, z2, &p);   // 19: (-r, 0, -r)
				proc.handleData(-r, 0, -r);
				//pointCount += 12;
				//last index : 19
				//
				//3.  plane centers. two axes at 0: 6 points from 24r-4 to 24r+1
				//source->getField(x1, 0, 0, &p);   // 20: (r, 0, 0)
				proc.handleData(r, 0, 0);
				//source->getField(x2, 0, 0, &p);   // 21: (-r, 0, 0)
				proc.handleData(-r, 0, 0);
				//source->getField(0, y1, 0, &p);   // 22: (0, r, 0)
				proc.handleData(0, r, 0);
				//source->getField(0, y2, 0, &p);   // 23: (0, -r, 0)
				proc.handleData(0, -r, 0);
				//source->getField(0, 0, z1, &p);   // 24: (0, 0, r)
				proc.handleData(0, 0, r);
				//source->getField(0, 0, z2, &p);   // 25: (0, 0, -r)
				proc.handleData(0, 0, -r);
				//last index: 25
				//d = 0;
				//3. (r>1) 12 Edge Lines exclude center. Each line has 2(r-1)  points. Two axes at edge, one axis not at 0: 24(r-1)  points: 26 to 24r+1
				for (k = 1; k < r; k++)
				{
					if (ret != ERR_OK)
					{
						break;
					}
					//d += ds2;
					//source->getField(x1, y1, d, &p);   // 26+24*(k-1): (r, r, k) --------------
					proc.handleData(r, r, k);
					//source->getField(x1, y1, -d, &p);   // 27+24*(k-1): (r, r, -k) -------------
					proc.handleData(r, r, -k);
					//source->getField(x1, y2, d, &p);   // 28+24*(k-1): (r, -r, k) -------------
					proc.handleData(r, -r, k);
					//source->getField(x1, y2, -d, &p);   // 29+24*(k-1): (r, -r, -k) -------------
					proc.handleData(r, -r, -k);
					//
					//source->getField(x2, y1, d, &p);   // 30+24*(k-1): (-r, r, k) -----------------
					proc.handleData(-r, r, k);
					//source->getField(x2, y1, -d, &p);   // 31+24*(k-1): (-r, r, -k) ------------------
					proc.handleData(-r, r, -k);
					//source->getField(x2, y2, d, &p);   // 32+24*(k-1): (-r, -r, k) ------------------
					proc.handleData(-r, -r, k);
					//source->getField(x2, y2, -d, &p);   // 33+24*(k-1): (-r, -r, -k) -----------------
					proc.handleData(-r, -r, -k);
					//
					//source->getField(d, y1, z1, &p);   // 34+24*(k-1): (k, r, r) ---------------------
					proc.handleData(k, r, r);
					//source->getField(-d, y1, z1, &p);   // 35+24*(k-1): (-k, r, r) ----------------------------
					proc.handleData(-k, r, r);
					//source->getField(d, y1, z2, &p);   // 36+24*(k-1): (k, r, -r) --------------------
					proc.handleData(k, r, -r);
					//source->getField(-d, y1, z2, &p);   // 37+24*(k-1): (-k, r, -r) ---------------------------
					proc.handleData(-k, r, -r);
					//
					//source->getField(d, y2, z1, &p);   // 38+24*(k-1): (k, -r, r) ---------------------
					proc.handleData(k, -r, r);
					//source->getField(-d, y2, z1, &p);   // 39+24*(k-1): (-k, -r, r) ---------------------------
					proc.handleData(-k, -r, r);
					//source->getField(d, y2, z2, &p);   // 40+24*(k-1): (k, -r, -r) ----------------------
					proc.handleData(k, -r, -r);
					//source->getField(-d, y2, z2, &p);   // 41+24*(k-1): (-k, -r, -r) ---------------------------
					proc.handleData(-k, -r, -r);
					//
					//source->getField(x1, d, z1, &p);   // 42+24*(k-1): (r, k, r) ------------------------
					proc.handleData(r, k, r);
					//source->getField(x1, -d, z1, &p);   // 43+24*(k-1): (r, -k, r) ----------------------
					proc.handleData(r, -k, r);
					//source->getField(x2, d, z1, &p);   // 44+24*(k-1): (-r, k, r) -----------------------
					proc.handleData(-r, k, r);
					//source->getField(x2, -d, z1, &p);   // 45+24*(k-1): (-r, -k, r) ---------------------
					proc.handleData(-r, -k, r);
					//
					//source->getField(x1, d, z2, &p);   // 46+24*(k-1): (r, k, -r) ------------------------
					proc.handleData(r, k, -r);
					//source->getField(x1, -d, z2, &p);   // 47+24*(k-1): (r, -k, -r) ----------------------
					proc.handleData(r, -k, -r);
					//source->getField(x2, d, z2, &p);   // 48+24*(k-1): (-r, k, -r)  ------------------------
					proc.handleData(-r, k, -r);
					//source->getField(x2, -d, z2, &p);   // 49+24*(k-1): (-r, -k, -r) ------------------------
					proc.handleData(-r, -k, -r);
				}
				//last index : 49+24*(r-2) = 24r + 1
				//4 Inner parts of cross-lines on m-plane. One axis at 0 and m=±r:24r+2 to 32r-7,8(r-1) points
				//d = 0;
				for (k = 1; k < r; k++)
				{
					if (ret != ERR_OK)
					{
						break;
					}
					//d += ds2;
					//m=+-r, n=0 //////////////////////////////////////////////
					//source->getField(x1, 0, d, &p);   // 24r + 2 + 24 * (k-1): (r, 0, k)    ---------------------
					proc.handleData(r, 0, k);
					//source->getField(x1, 0, -d, &p);   // 24r + 3 + 24 * (k-1): (r, 0, -k)  ---------------------
					proc.handleData(r, 0, -k);
					//source->getField(x2, 0, d, &p);   // 24r + 4 + 24 * (k-1): (-r, 0, k)  ---------------------------
					proc.handleData(-r, 0, k);
					//source->getField(x2, 0, -d, &p);   // 24r + 5 + 24 * (k-1): (-r, 0, -k) ------------------------
					proc.handleData(-r, 0, -k);
					//m=+-r, p=0
					//source->getField(x1, d, 0, &p);   // 24r + 6 + 24 * (k-1): (r, k, 0)    ----------------------
					proc.handleData(r, k, 0);
					//source->getField(x1, -d, 0, &p);   // 24r + 7 + 24 * (k-1): (r, -k, 0)  ---------------------
					proc.handleData(r, -k, 0);
					//source->getField(x2, d, 0, &p);   // 24r + 8 + 24 * (k-1): (-r, k, 0)   -------------------------
					proc.handleData(-r, k, 0);
					//source->getField(x2, -d, 0, &p);   // 24r + 9 + 24 * (k-1): (-r, -k, 0)  ---------------------------
					proc.handleData(-r, -k, 0);
					///////////////////////////////////////
					//n=+-r, m=0 ///////////////////////////////////////////////
					//source->getField(0, y1, d, &p);   // 24r + 10 + 24 * (k-1): (0, r, k)    ---------------------------
					proc.handleData(0, r, k);
					//source->getField(0, y1, -d, &p);   // 24r + 11 + 24 * (k-1): (0, r, -k)   -------------------------
					proc.handleData(0, r, -k);
					//source->getField(0, y2, d, &p);   // 24r + 12 + 24 * (k-1): (0, -r, k)     -----------------------------
					proc.handleData(0, -r, k);
					//source->getField(0, y2, -d, &p);   // 24r + 13 + 24 * (k-1): (0, -r, -k)   ----------------------------
					proc.handleData(0, -r, -k);
					//n=+-r, p=0 
					//source->getField(d, y1, 0, &p);   // 24r + 14 + 24 * (k-1): (k, r, 0)      --------------------------------
					proc.handleData(k, r, 0);
					//source->getField(-d, y1, 0, &p);   // 24r + 15 + 24 * (k-1): (-k, r, 0)     -------------------------------
					proc.handleData(-k, r, 0);
					//source->getField(d, y2, 0, &p);   // 24r + 16 + 24 * (k-1): (k, -r, 0)      --------------------------------
					proc.handleData(k, -r, 0);
					//source->getField(-d, y2, 0, &p);   // 24r + 17 + 24 * (k-1): (-k, -r, 0)    -----------------------------------
					proc.handleData(-k, -r, 0);
					/////////////////////////////////////////////
					//p=+-r, n=0 ///////////////////////////////////////////////
					//source->getField(d, 0, z1, &p);   // 24r + 18 + 24 * (k-1): (k, 0, r)       -----------------------------------
					proc.handleData(k, 0, r);
					//source->getField(-d, 0, z1, &p);   // 24r + 19 + 24 * (k-1): (-k, 0, r)     ------------------------------------
					proc.handleData(-k, 0, r);
					//source->getField(d, 0, z2, &p);   // 24r + 20 + 24 * (k-1): (k, 0, -r)      ------------------------------------
					proc.handleData(k, 0, -r);
					//source->getField(-d, 0, z2, &p);   // 24r + 21 + 24 * (k-1): (-k, 0, -r)     -----------------------------------
					proc.handleData(-k, 0, -r);
					//p=+-r, m=0
					//source->getField(0, d, z1, &p);   // 24r + 22 + 24 * (k-1): (0, k, r)        ----------------------------
					proc.handleData(0, k, r);
					//source->getField(0, -d, z1, &p);   // 24r + 23 + 24 * (k-1): (0, -k, r)      ---------------------------------------
					proc.handleData(0, -k, r);
					//source->getField(0, d, z2, &p);   // 24r + 24 + 24 * (k-1): (0, k, -r)         ----------------------------------
					proc.handleData(0, k, -r);
					//source->getField(0, -d, z2, &p);   // 24r + 25 + 24 * (k-1): (0, -k, -r)       ---------------------------------
					proc.handleData(0, -k, -r);
					//
				}
				//Last index: 24r + 25 + 24 * (k-1) = 24r + 25 + 24 * (r-2) = 48r - 23
				//5. one at edge, the other two not at 0
				//d = 0;
				for (j = 1; j < r; j++)
				{
					if (ret != ERR_OK)
					{
						break;
					}
					//d += ds2;
					//dd = 0;
					for (k = 1; k < r; k++)
					{
						if (ret != ERR_OK)
						{
							break;
						}
						//dd += ds2;
						//24 points
						//5.1 m=r
						//source->getField(x1, d, dd, &p);   // 48r - 22 + 24*[(k-1) + (r-1)*(j-1)]: (r, j, k)          -----------------------------
						proc.handleData(r, j, k);
						//source->getField(x1, d, -dd, &p);   // 48r - 21 + 24*[(k-1) + (r-1) * (j-1)]: (r, j, -k)      ------------------------------
						proc.handleData(r, j, -k);
						//source->getField(x1, -d, dd, &p);   // 48r - 20 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, k)      -------------------------------
						proc.handleData(r, -j, k);
						//source->getField(x1, -d, -dd, &p);   // 48r - 19 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, -k)   ---------------------------------
						proc.handleData(r, -j, -k);
						//m=-r
						//source->getField(x2, d, dd, &p);   // 48r - 18 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, k)       ----------------------------------
						proc.handleData(-r, j, k);
						//source->getField(x2, d, -dd, &p);   // 48r - 17 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, -k)      ------------------------------------
						proc.handleData(-r, j, -k);
						//source->getField(x2, -d, dd, &p);   // 48r - 16 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, k)       ----------------------------
						proc.handleData(-r, -j, k);
						//source->getField(x2, -d, -dd, &p);   // 48r - 15 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, -k)    ------------------------------
						proc.handleData(-r, -j, -k);
						//5.2 n=r
						//source->getField(d, y1, dd, &p);   // 48r - 14 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, k)     -----------------------------------
						proc.handleData(j, r, k);
						//source->getField(d, y1, -dd, &p);   // 48r - 13 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, -k)    -------------------------------------
						proc.handleData(j, r, -k);
						//source->getField(-d, y1, dd, &p);   // 48r - 12 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, k)     --------------------------------
						proc.handleData(-j, r, k);
						//source->getField(-d, y1, -dd, &p);   // 48r - 11 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, -k)  ----------------------------------
						proc.handleData(-j, r, -k);
						//n=-r
						//source->getField(d, y2, dd, &p);   // 48r - 10 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, k)    ----------------------------------
						proc.handleData(j, -r, k);
						//source->getField(d, y2, -dd, &p);   // 48r - 9 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, -k)     ----------------------------------
						proc.handleData(j, -r, -k);
						//source->getField(-d, y2, dd, &p);   // 48r - 8 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, k)       ------------------------------
						proc.handleData(-j, -r, k);
						//source->getField(-d, y2, -dd, &p);   // 48r - 7 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, -k)     --------------------
						proc.handleData(-j, -r, -k);
						//5.3 p=r
						//source->getField(d, dd, z1, &p);   // 48r - 6 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, r)       ---------------------------
						proc.handleData(j, k, r);
						//source->getField(-d, dd, z1, &p);   // 48r - 5 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, r)        ----------------------------
						proc.handleData(-j, k, r);
						//source->getField(d, -dd, z1, &p);   // 48r - 4 + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, r)       ------------------------
						proc.handleData(j, -k, r);
						//source->getField(-d, -dd, z1, &p);   // 48r - 3 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, r)   ----------------------------
						proc.handleData(-j, -k, r);
						//p=-r
						//source->getField(d, dd, z2, &p);   // 48r - 2 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, -r)       -----------------------------------
						proc.handleData(j, k, -r);
						//source->getField(-d, dd, z2, &p);   // 48r - 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, -r)     ---------------------------------------
						proc.handleData(-j, k, -r);
						//source->getField(d, -dd, z2, &p);   // 48r + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, -r)          -----------------------------------
						proc.handleData(j, -k, -r);
						//source->getField(-d, -dd, z2, &p);   // 48r + 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, -r)  -------------------------------
						proc.handleData(-j, -k, -r);
					}
				}
				//last index: 48r + 1 + 24*[(r-2) + (r-2) * (r-1)] = 48r + 1 + 24r - 48 + 24 (r^2 - 3r + 2) = 24 r^2 + 1
				if (rht == RadiusHandleType.ProcessAndFinish)
				{
					break;
				}
			}//r>0
			if (ret == ERR_OK)
			{
				proc.onFinish();
			}
			return ret;
		}

		/// <summary>
		/// generate a boolean array indicating which points of a radius are included for drawing.
		/// </summary>
		/// <param name="r">the radius</param>
		/// <param name="skip">number of points to be skipped</param>
		/// <returns>boolean array</returns>
		public static bool[] SkipSpacePoints(UInt32 r, UInt32 skip)
		{
			bool[] usages = new bool[PointsAtRadius(r)];
			if (skip == 0) //all points are included
			{
				for (int i = 0; i < usages.Length; i++)
				{
					usages[i] = true;
				}
				return usages;
			}
			if (r == 0)
			{
				usages[0] = true;
				return usages;
			}
			setPointUsages pu = new setPointUsages(usages, r, skip);
			int ret = SphereUtility.GoThroughSpherePoints(r, pu);
			if (ret != SphereUtility.ERR_OK)
			{
				throw new Exception(string.Format(CultureInfo.InvariantCulture, "Error going through space by index. Error code={0}", ret));
			}
			return usages;
		}
		/// <summary>
		/// going through space on one radius and skip a number of points
		/// </summary>
		class setPointUsages:IProcessFieldByIndex
		{
			private UInt32 index;
			private UInt32 _skipCount;
			private bool[] _usages;
			private UInt32 _r;
			private UInt32 _skips;
			/// <summary>
			/// constructor
			/// </summary>
			/// <param name="usages">a boolean array recording skipped and not-skipped points</param>
			/// <param name="r">a radius. each boolean array item corresponds to one space point on the radius</param>
			/// <param name="skips">number of space points to be skipped</param>
			public setPointUsages(bool[] usages, UInt32 r, UInt32 skips)
			{
				_usages = usages;
				_r = r;
				if (r <= 0 || skips == 0)
				{
					throw new Exception("Error setting point usages. Radius and Skips must be larger than 0");
				}
				index = 0;
				_skips = skips;
				_skipCount = skips; //do not skip the first space point
			}
			public RadiusHandleType setRadius(int r)
			{
				if (r == _r)
				{
					index = 0; //counting starts
					return RadiusHandleType.ProcessAndFinish; //only process this radius and finish
				}
				else
				{
					return RadiusHandleType.DoNotProcess; //do not process this radius and go to the next radius
				}
			}
			/// <summary>
			/// handle the space point
			/// </summary>
			/// <param name="m">the radius index for x-axis</param>
			/// <param name="n">the radius index for y-axis</param>
			/// <param name="p">the radius index for z-axis</param>
			public void handleData(int m, int n, int p)
			{
				_skipCount++; //count the number of space points 
				if (_skipCount >= _skips) //enough points have been skipped
				{
					_usages[index] = true; //use this point
					_skipCount = 0;
				}
				else
				{
					_usages[index] = false; //skip this point
				}
				index++;
			}

			public void onFinish()
			{
				
			}
		}
		/// <summary>
		/// number of space points on a radius
		/// </summary>
		/// <param name="radius"></param>
		/// <returns></returns>
		public static UInt32 PointsAtRadius(UInt32 radius)
		{
			if (radius == 0)
				return 1;
			//return 6 * (2 * (radius - 1) + 1) * (2 * radius + 1) + 8;
			return 24 * radius * radius + 2;
		}

		/// <summary>
		/// number of space points in a sphere
		/// </summary>
		/// <param name="radius"></param>
		/// <returns></returns>
		public static UInt32 PointsInSphere(UInt32 radius)
		{
			UInt32 r;
			UInt32 c = 0;
			for (r = 0; r <= radius; r++)
			{
				c += PointsAtRadius(r);
			}
			return c;
		}

		/// <summary>
		/// get radius for a given number of space points
		/// </summary>
		/// <param name="pointCount"></param>
		/// <returns></returns>
		public static UInt32 GetSphereRadius(UInt32 pointCount)
		{
			UInt32 r = 0;
			UInt32 c = 0;
			while (true)
			{
				c += PointsAtRadius(r);
				if (c >= pointCount)
					return r;
				r++;
			}
		}
	}
}

