/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by Bob Limnor

********************************************************************/
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Field3D
{
	/*
	 * rotation utility
	 */

	// code from
	// https://bitbucket.org/sinbad/ogre/src/3cbd67467fab3fef44d1b32bc42ccf4fb1ccfdd0/OgreMain/src/OgreQuaternion.cpp?at=default&fileviewer=file-view-default
	class Quaternion
	{
		private double x, y, z, w;
		private UInt32[] s_iNext = new UInt32[3];
		private static double msEpsilon = 1e-05;
		public Quaternion()
		{
			x = y = z = w = 0.0;
			s_iNext[0] = 1; s_iNext[1] = 2; s_iNext[2] = 0;
		}
		public Quaternion(double w0, double x0, double y0, double z0)
		{
			w = w0; x = x0; y = y0; z = z0;
			s_iNext[0] = 1; s_iNext[1] = 2; s_iNext[2] = 0;
		}
		public Quaternion(Quaternion q)
		{
			x = q.w; y = q.y; z = q.z; w = q.w;
			s_iNext[0] = 1; s_iNext[1] = 2; s_iNext[2] = 0;
		}
		public void FromRotationMatrix(double[,] kRot)
		{
			// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
			// article "Quaternion Calculus and Fast Animation".

			double fTrace = kRot[0, 0] + kRot[1, 1] + kRot[2, 2];
			double fRoot;

			if (fTrace > 0.0)
			{
				// |w| > 1/2, may as well choose w > 1/2
				fRoot = Math.Sqrt(fTrace + 1.0);  // 2w
				w = 0.5f * fRoot;
				fRoot = 0.5 / fRoot;  // 1/(4w)
				x = (kRot[2, 1] - kRot[1, 2]) * fRoot;
				y = (kRot[0, 2] - kRot[2, 0]) * fRoot;
				z = (kRot[1, 0] - kRot[0, 1]) * fRoot;
			}
			else
			{
				// |w| <= 1/2
				//UInt32[] s_iNext = new UInt32[3];// = { 1, 2, 0 };
				//s_iNext[0] = 1; s_iNext[1] = 2; s_iNext[2] = 0;
				//
				UInt32 i = 0;
				if (kRot[1, 1] > kRot[0, 0])
					i = 1;
				if (kRot[2, 2] > kRot[i, i])
					i = 2;
				UInt32 j = s_iNext[i];
				UInt32 k = s_iNext[j];

				fRoot = Math.Sqrt(kRot[i, i] - kRot[j, j] - kRot[k, k] + 1.0);
				double[] apkQuat = new double[3];// = { &x, &y, &z };

				//*apkQuat[i] = 0.5f * fRoot;
				if (i == 0) x = 0.5 * fRoot;
				else if (i == 1) y = 0.5 * fRoot;
				else z = 0.5 * fRoot;
				//
				fRoot = 0.5 / fRoot;
				w = (kRot[k, j] - kRot[j, k]) * fRoot;
				//
				//*apkQuat[j] = (kRot[j,i] + kRot[i,j]) * fRoot;
				if (j == 0) x = (kRot[j, i] + kRot[i, j]) * fRoot;
				else if (j == 1) y = (kRot[j, i] + kRot[i, j]) * fRoot;
				else z = (kRot[j, i] + kRot[i, j]) * fRoot;
				//
				//*apkQuat[k] = (kRot[k,i] + kRot[i,k]) * fRoot;
				if (k == 0) x = (kRot[k, i] + kRot[i, k]) * fRoot;
				else if (k == 1) y = (kRot[k, i] + kRot[i, k]) * fRoot;
				else z = (kRot[k, i] + kRot[i, k]) * fRoot;
			}
		}
		public static double[,] GetRotationMatrix(double angle, double[] u)
		{
			double[,] W = new double[3, 3];
			double cosValue = Math.Cos(angle);
			double sinValue = Math.Sin(angle);
			double cos1 = 1.0 - cosValue;
			W[0, 0] = cosValue + u[0] * u[0] * cos1;
			W[0, 1] = u[0] * u[1] * cos1 - u[2] * sinValue;
			W[0, 2] = u[0] * u[2] * cos1 + u[1] * sinValue;
			W[1, 0] = u[1] * u[0] * cos1 + u[2] * sinValue;
			W[1, 1] = cosValue + u[1] * u[1] * cos1;
			W[1, 2] = u[1] * u[2] * cos1 - u[0] * sinValue;
			W[2, 0] = u[2] * u[0] * cos1 - u[1] * sinValue;
			W[2, 1] = u[2] * u[1] * cos1 + u[0] * sinValue;
			W[2, 2] = cosValue + u[2] * u[2] * cos1;
			return W;
		}
		public void ToRotationMatrix(double[,] kRot)
		{
			double fTx = x + x;
			double fTy = y + y;
			double fTz = z + z;
			double fTwx = fTx * w;
			double fTwy = fTy * w;
			double fTwz = fTz * w;
			double fTxx = fTx * x;
			double fTxy = fTy * x;
			double fTxz = fTz * x;
			double fTyy = fTy * y;
			double fTyz = fTz * y;
			double fTzz = fTz * z;

			kRot[0, 0] = 1.0 - (fTyy + fTzz);
			kRot[0, 1] = fTxy - fTwz;
			kRot[0, 2] = fTxz + fTwy;
			kRot[1, 0] = fTxy + fTwz;
			kRot[1, 1] = 1.0f - (fTxx + fTzz);
			kRot[1, 2] = fTyz - fTwx;
			kRot[2, 0] = fTxz - fTwy;
			kRot[2, 1] = fTyz + fTwx;
			kRot[2, 2] = 1.0f - (fTxx + fTyy);
		}
		public void FromAngleAxis(double rfAngle, double[] rkAxis)
		{
			// assert:  axis[] is unit length
			//
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

			double fHalfAngle = (0.5 * rfAngle);
			double fSin = Math.Sin(fHalfAngle);
			w = Math.Cos(fHalfAngle);
			x = fSin * rkAxis[0];//.x;
			y = fSin * rkAxis[1];//.y;
			z = fSin * rkAxis[2];//.z;
		}
		public static Quaternion angleAxis(double rfAngle, double[] rkAxis)
		{
			Quaternion q = new Quaternion();
			q.FromAngleAxis(rfAngle, rkAxis);
			return q;
		}
		public void ToAngleAxis(out double rfAngle, double[] rkAxis)
		{
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
			// angle is in Radian

			double fSqrLength = x * x + y * y + z * z;
			if (fSqrLength > 0.0)
			{
				rfAngle = 2.0 * Math.Acos(w);
				double fInvLength = 1.0 / Math.Sqrt(fSqrLength);// Math.InvSqrt(fSqrLength);
				rkAxis[0] = x * fInvLength;
				rkAxis[1] = y * fInvLength;
				rkAxis[2] = z * fInvLength;
			}
			else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				rfAngle = (0.0);
				rkAxis[0] = 1.0;
				rkAxis[1] = 0.0;
				rkAxis[2] = 0.0;
			}
		}
		public void ToAngleAxisInDegree(out double rfAngle, double[] rkAxis)
		{
			ToAngleAxis(out rfAngle, rkAxis);
			rfAngle = rfAngle * 180.0 / Math.PI;
		}
		//akAxis: each column is one axis
		public void FromAxes(double[,] akAxis)
		{
			double[,] kRot = new double[3, 3];

			for (int iCol = 0; iCol < 3; iCol++)
			{
				kRot[0, iCol] = akAxis[iCol, 0];//.x;
				kRot[1, iCol] = akAxis[iCol, 1];//.y;
				kRot[2, iCol] = akAxis[iCol, 2];//.z;
			}
			FromRotationMatrix(kRot);
		}
		public void FromAxes(double[] xaxis, double[] yaxis, double[] zaxis)
		{
			double[,] kRot = new double[3, 3];

			kRot[0, 0] = xaxis[0];
			kRot[1, 0] = xaxis[1];
			kRot[2, 0] = xaxis[2];

			kRot[0, 1] = yaxis[0];
			kRot[1, 1] = yaxis[1];
			kRot[2, 1] = yaxis[2];

			kRot[0, 2] = zaxis[0];
			kRot[1, 2] = zaxis[1];
			kRot[2, 2] = zaxis[2];

			FromRotationMatrix(kRot);

		}
		public void ToAxes(double[][] akAxis)
		{
			double[,] kRot = new double[3, 3];

			ToRotationMatrix(kRot);

			for (int iCol = 0; iCol < 3; iCol++)
			{
				akAxis[iCol][0] = kRot[0, iCol];
				akAxis[iCol][1] = kRot[1, iCol];
				akAxis[iCol][2] = kRot[2, iCol];
			}
		}
		public double[] xAxis
		{
			get
			{
				//Real fTx  = 2.0*x;
				double fTy = 2.0 * y;
				double fTz = 2.0 * z;
				double fTwy = fTy * w;
				double fTwz = fTz * w;
				double fTxy = fTy * x;
				double fTxz = fTz * x;
				double fTyy = fTy * y;
				double fTzz = fTz * z;
				double[] v = new double[3];
				v[0] = 1.0 - (fTyy + fTzz);
				v[1] = fTxy + fTwz;
				v[2] = fTxz - fTwy;
				return v;
				//return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
			}
		}
		//-----------------------------------------------------------------------
		public double[] yAxis
		{
			get
			{
				double fTx = 2.0 * x;
				double fTy = 2.0 * y;
				double fTz = 2.0 * z;
				double fTwx = fTx * w;
				double fTwz = fTz * w;
				double fTxx = fTx * x;
				double fTxy = fTy * x;
				double fTyz = fTz * y;
				double fTzz = fTz * z;
				double[] v = new double[3];
				v[0] = fTxy - fTwz;
				v[1] = 1.0 - (fTxx + fTzz);
				v[2] = fTyz + fTwx;
				return v;
				//return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
			}
		}
		//-----------------------------------------------------------------------
		public double[] zAxis
		{
			get
			{
				double fTx = 2.0 * x;
				double fTy = 2.0 * y;
				double fTz = 2.0 * z;
				double fTwx = fTx * w;
				double fTwy = fTy * w;
				double fTxx = fTx * x;
				double fTxz = fTz * x;
				double fTyy = fTy * y;
				double fTyz = fTz * y;
				double[] v = new double[3];
				v[0] = fTxz + fTwy;
				v[1] = fTyz - fTwx;
				v[2] = 1.0 - (fTxx + fTyy);
				return v;
				//return Vector3(fTxz + fTwy, fTyz - fTwx, 1.0f - (fTxx + fTyy));
			}
		}
		public void ToAxes(double[] xaxis, double[] yaxis, double[] zaxis)
		{
			double[,] kRot = new double[3, 3];

			ToRotationMatrix(kRot);

			xaxis[0] = kRot[0, 0];
			xaxis[1] = kRot[1, 0];
			xaxis[2] = kRot[2, 0];

			yaxis[0] = kRot[0, 1];
			yaxis[1] = kRot[1, 1];
			yaxis[2] = kRot[2, 1];

			zaxis[0] = kRot[0, 2];
			zaxis[1] = kRot[1, 2];
			zaxis[2] = kRot[2, 2];
		}
		public static Quaternion Plus(Quaternion q1, Quaternion q2)
		{
			return new Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
		}
		//public static Quaternion operator +(Quaternion rkQ)
		//{
		//	return new Quaternion(w + rkQ.w, x + rkQ.x, y + rkQ.y, z + rkQ.z);
		//}
		public static Quaternion Minus(Quaternion q1, Quaternion q2)
		{
			return new Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
		}
		//public static Quaternion operator -(Quaternion rkQ)
		//{
		//	return new Quaternion(w - rkQ.w, x - rkQ.x, y - rkQ.y, z - rkQ.z);
		//}
		public Quaternion multiply(Quaternion rkQ)
		{
			// NOTE:  Multiplication is not generally commutative, so in most
			// cases p*q != q*p.

			Quaternion q = new Quaternion
			(
				w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
				w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
				w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
				w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
			);
			if (q.w > 1.0 || q.w < -1.0)
			{
				q.normalise();
			}
			return q;
		}
		public void Scale(double s)
		{
			w *= s; x *= s; y *= s; z *= s;
		}
		public Quaternion multiply(double fScalar)
		{
			return new Quaternion(fScalar * w, fScalar * x, fScalar * y, fScalar * z);
		}
		public Quaternion multiply(double fScalar, Quaternion rkQ)
		{
			return new Quaternion(fScalar * rkQ.w, fScalar * rkQ.x, fScalar * rkQ.y,
				fScalar * rkQ.z);
		}
		public Quaternion negate()
		{
			return new Quaternion(-w, -x, -y, -z);
		}
		public static double[] sumVectors(double[] v, params double[][] vs)
		{
			double[] sum = new double[3];
			for (int i = 0; i < 3; i++)
			{
				sum[i] = v[i];
			}
			if (vs.Length > 0)
			{
				for (int i = 0; i < 3; i++)
				{
					for (int k = 0; k < vs.Length; k++)
					{
						sum[i] += vs[k][i];
					}
				}
			}
			return sum;
		}
		public static void ScaleVector(double scale, double[] v)
		{
			for (int i = 0; i < 3; i++)
			{
				v[i] *= scale;
			}
		}
		public static double[] transVector(double[,] T, double[] vin)
		{
			double[] vout = new double[3];
			for (int r = 0; r < 3; r++)
			{
				vout[r] = 0.0;
				for (int c = 0; c < 3; c++)
				{
					vout[r] += T[r, c] * vin[c];
				}
			}
			return vout;
		}
		public static double dot(double[] v1, double[] v2)
		{
			double s = 0.0;
			for (int i = 0; i < 3; i++)
			{
				s += v1[i] * v2[i];
			}
			return s;
		}
		public static double[] cross(double[] v1, double[] v2)
		{
			double[] vout = new double[3];
			vout[0] = v1[1] * v2[2] - v1[2] * v2[1];
			vout[1] = v1[2] * v2[0] - v1[0] * v2[2];
			vout[2] = v1[0] * v2[1] - v1[1] * v2[0];
			return vout;
		}
		public double Dot(Quaternion rkQ)
		{
			return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z;
		}
		public double Norm()
		{
			return w * w + x * x + y * y + z * z;
		}
		public Quaternion Inverse()
		{
			double fNorm = w * w + x * x + y * y + z * z;
			if (fNorm > 0.0)
			{
				double fInvNorm = 1.0f / fNorm;
				return new Quaternion(w * fInvNorm, -x * fInvNorm, -y * fInvNorm, -z * fInvNorm);
			}
			else
			{
				// return an invalid result to flag the error
				//return ZERO;
				return new Quaternion();
			}
		}
		public Quaternion UnitInverse()
		{
			// assert:  'this' is unit length
			return new Quaternion(w, -x, -y, -z);
		}
		public Quaternion Exp()
		{
			// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
			// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

			double fAngle = (Math.Sqrt(x * x + y * y + z * z));
			double fSin = Math.Sin(fAngle);

			Quaternion kResult = new Quaternion();
			kResult.w = Math.Cos(fAngle);

			if (Math.Abs(fSin) >= msEpsilon)
			{
				double fCoeff = fSin / fAngle;// (fAngle.valueRadians());
				kResult.x = fCoeff * x;
				kResult.y = fCoeff * y;
				kResult.z = fCoeff * z;
			}
			else
			{
				kResult.x = x;
				kResult.y = y;
				kResult.z = z;
			}

			return kResult;
		}
		public Quaternion Log()
		{
			// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
			// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

			Quaternion kResult = new Quaternion();
			kResult.w = 0.0;

			if (Math.Abs(w) < 1.0)
			{
				//Radian fAngle ( Math::ACos(w) );
				double fAngle = Math.Acos(w);
				double fSin = Math.Sin(fAngle);
				if (Math.Abs(fSin) >= msEpsilon)
				{
					double fCoeff = fAngle / fSin; //fAngle.valueRadians()/fSin
					kResult.x = fCoeff * x;
					kResult.y = fCoeff * y;
					kResult.z = fCoeff * z;
					return kResult;
				}
			}

			kResult.x = x;
			kResult.y = y;
			kResult.z = z;

			return kResult;
		}
		public double[] multiply(double[] v)
		{
			// nVidia SDK implementation
			double[] uv, uuv;
			double[] qvec = { x, y, z };
			//uv = qvec.crossProduct(v);
			uv = cross(qvec, v);
			//uuv = qvec.crossProduct(uv);
			uuv = cross(qvec, uv);
			//uv *= (2.0f * w);
			ScaleVector(2.0 * w, uv);
			//uuv *= 2.0f;
			ScaleVector(2.0, uuv);
			//return v + uv + uuv;
			return sumVectors(v, uv, uuv);
		}
		public bool equals(Quaternion rhs, double tolerance)
		{
			double fCos = Dot(rhs);
			//Radian angle = Math::ACos(fCos);
			double angle = Math.Acos(fCos);
			/*
			return (Math::Abs(angle.valueRadians()) <= tolerance.valueRadians())
				|| Math::RealEqual(angle.valueRadians(), Math::PI, tolerance.valueRadians());
			*/
			return (Math.Abs(angle) <= tolerance)
				|| (Math.Abs(angle - Math.PI) <= tolerance);
		}
		public double normalise()
		{
			double len = Norm();
			double factor = 1.0 / Math.Sqrt(len);
			// *this = *this * factor;
			Scale(factor);
			return len;
		}
		public Quaternion Slerp(double fT, Quaternion rkP, Quaternion rkQ, bool shortestPath)
		{
			double fCos = rkP.Dot(rkQ);
			Quaternion rkT;

			// Do we need to invert rotation?
			if (fCos < 0.0 && shortestPath)
			{
				fCos = -fCos;
				//rkT = -rkQ;
				rkT = rkQ.multiply(-0.1);
			}
			else
			{
				rkT = rkQ;
			}

			if (Math.Abs(fCos) < 1 - msEpsilon)
			{
				// Standard case (slerp)
				double fSin = Math.Sqrt(1.0 - (fCos * fCos));
				//Radian fAngle = Math::ATan2(fSin, fCos);
				double fAngle = Math.Atan2(fSin, fCos);
				double fInvSin = 1.0 / fSin;
				double fCoeff0 = Math.Sin((1.0f - fT) * fAngle) * fInvSin;
				double fCoeff1 = Math.Sin(fT * fAngle) * fInvSin;
				//return fCoeff0 * rkP + fCoeff1 * rkT;
				Quaternion q1 = rkP.multiply(fCoeff0);
				Quaternion q2 = rkT.multiply(fCoeff1);
				return Quaternion.Plus(q1, q2);
			}
			else
			{
				// There are two situations:
				// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
				//    interpolation safely.
				// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
				//    are an infinite number of possibilities interpolation. but we haven't
				//    have method to fix this case, so just use linear interpolation here.
				Quaternion q1 = rkP.multiply(1.0 - fT);
				Quaternion q2 = rkT.multiply(fT);
				//Quaternion t = (1.0f - fT) * rkP + fT * rkT;
				Quaternion t = Quaternion.Plus(q1, q2);
				// taking the complement requires renormalisation
				t.normalise();
				return t;
			}
		}
		public Quaternion SlerpExtraSpins(double fT, Quaternion rkP, Quaternion rkQ, int iExtraSpins)
		{
			double fCos = rkP.Dot(rkQ);
			//Radian fAngle ( Math::ACos(fCos) );
			double fAngle = (Math.Acos(fCos));

			//if ( Math::Abs(fAngle.valueRadians()) < msEpsilon )
			if (Math.Abs(fAngle) < msEpsilon)
				return rkP;

			double fSin = Math.Sin(fAngle);
			//Radian fPhase ( Math::PI*iExtraSpins*fT );
			double fPhase = (Math.PI * iExtraSpins * fT);
			double fInvSin = 1.0 / fSin;
			double fCoeff0 = Math.Sin((1.0 - fT) * fAngle - fPhase) * fInvSin;
			double fCoeff1 = Math.Sin(fT * fAngle + fPhase) * fInvSin;
			Quaternion q1 = rkP.multiply(fCoeff0);
			Quaternion q2 = rkQ.multiply(fCoeff1);
			//return fCoeff0 * rkP + fCoeff1 * rkQ;
			return Quaternion.Plus(q1, q2);
		}
		public void Intermediate(Quaternion rkQ0, Quaternion rkQ1, Quaternion rkQ2, out Quaternion rkA, out Quaternion rkB)
		{
			// assert:  q0, q1, q2 are unit quaternions

			Quaternion kQ0inv = rkQ0.UnitInverse();
			Quaternion kQ1inv = rkQ1.UnitInverse();
			Quaternion rkP0 = rkQ1.multiply(kQ0inv);
			Quaternion rkP1 = rkQ2.multiply(kQ1inv);
			//Quaternion kArg = 0.25*(rkP0.Log()-rkP1.Log());
			Quaternion q1 = rkP0.Log();
			Quaternion q2 = rkP1.Log();
			Quaternion kArg = Quaternion.Minus(q1, q2);
			kArg.Scale(0.25);
			//Quaternion kMinusArg = -kArg;
			Quaternion kMinusArg = kArg.multiply(-0.1);

			//rkA = rkQ1*kArg.Exp();
			rkA = rkQ1.multiply(kArg.Exp());
			rkB = rkQ1.multiply(kMinusArg.Exp());
		}
		public Quaternion Squad(double fT, Quaternion rkP, Quaternion rkA, Quaternion rkB, Quaternion rkQ, bool shortestPath)
		{
			double fSlerpT = 2.0 * fT * (1.0 - fT);
			Quaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
			//Quaternion kSlerpQ = Slerp(fT, rkA, rkB);
			Quaternion kSlerpQ = Slerp(fT, rkA, rkB, shortestPath);
			//return Slerp(fSlerpT, kSlerpP, kSlerpQ);
			return Slerp(fSlerpT, kSlerpP, kSlerpQ, shortestPath);
		}
		//return Radian
		public double getRoll(bool reprojectAxis)
		{
			if (reprojectAxis)
			{
				// roll = atan2(localx.y, localx.x)
				// pick parts of xAxis() implementation that we need
				//			Real fTx  = 2.0*x;
				double fTy = 2.0 * y;
				double fTz = 2.0 * z;
				double fTwz = fTz * w;
				double fTxy = fTy * x;
				double fTyy = fTy * y;
				double fTzz = fTz * z;

				// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

				//return Radian(Math::ATan2(fTxy+fTwz, 1.0f-(fTyy+fTzz)));
				return Math.Atan2(fTxy + fTwz, 1.0 - (fTyy + fTzz));

			}
			else
			{
				//return Radian(Math::ATan2(2*(x*y + w*z), w*w + x*x - y*y - z*z));
				return Math.Atan2(2.0 * (x * y + w * z), w * w + x * x - y * y - z * z);
			}
		}
		//returns Radian
		public double getPitch(bool reprojectAxis)
		{
			if (reprojectAxis)
			{
				// pitch = atan2(localy.z, localy.y)
				// pick parts of yAxis() implementation that we need
				double fTx = 2.0 * x;
				//			Real fTy  = 2.0f*y;
				double fTz = 2.0 * z;
				double fTwx = fTx * w;
				double fTxx = fTx * x;
				double fTyz = fTz * y;
				double fTzz = fTz * z;

				// Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
				//return Radian(Math::ATan2(fTyz+fTwx, 1.0f-(fTxx+fTzz)));
				return Math.Atan2(fTyz + fTwx, 1.0 - (fTxx + fTzz));
			}
			else
			{
				// internal version
				//return Radian(Math::ATan2(2*(y*z + w*x), w*w - x*x - y*y + z*z));
				return Math.Atan2(2.0 * (y * z + w * x), w * w - x * x - y * y + z * z);
			}
		}
		//returns Radian
		public double getYaw(bool reprojectAxis)
		{
			if (reprojectAxis)
			{
				// yaw = atan2(localz.x, localz.z)
				// pick parts of zAxis() implementation that we need
				double fTx = 2.0 * x;
				double fTy = 2.0 * y;
				double fTz = 2.0 * z;
				double fTwy = fTy * w;
				double fTxx = fTx * x;
				double fTxz = fTz * x;
				double fTyy = fTy * y;

				// Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

				//return Radian(Math::ATan2(fTxz+fTwy, 1.0f-(fTxx+fTyy)));
				return Math.Atan2(fTxz + fTwy, 1.0 - (fTxx + fTyy));

			}
			else
			{
				// internal version
				//return Radian(Math::ASin(-2*(x*z - w*y)));
				return Math.Asin(-2.0 * (x * z - w * y));
			}
		}
		public Quaternion nlerp(double fT, Quaternion rkP, Quaternion rkQ, bool shortestPath)
		{
			Quaternion result;
			Quaternion q1, q2;
			double fCos = rkP.Dot(rkQ);
			if (fCos < 0.0 && shortestPath)
			{
				//q1 = -rkQ;
				q1 = rkQ.multiply(-0.1);
				q2 = Quaternion.Minus(q1, rkP);
				q2.Scale(fT);
				result = Quaternion.Plus(rkP, q2);
				//result = rkP + fT * ((-rkQ) - rkP);
			}
			else
			{
				q2 = Quaternion.Minus(rkQ, rkP);
				q2.Scale(fT);
				//result = rkP + fT * (rkQ - rkP);
				result = Quaternion.Plus(rkP, q2);
			}
			result.normalise();
			return result;
		}
		public static double length2(double[] v)
		{
			double s = 0.0;
			for (int i = 0; i < 3; i++)
				s += v[i] * v[i];
			return Math.Sqrt(s);
		}
		public static double[] normalize(double[] v)
		{
			double[] p = new double[3];
			double s = length2(v);
			if (s < 1e-10)
			{
				for (int i = 0; i < 3; i++)
				{
					p[i] = 0.0;
				}
				if (Math.Abs(v[0]) > Math.Abs(v[1]))
				{
					if (Math.Abs(v[0]) > Math.Abs(v[2]))
					{
						p[0] = (double)Math.Sign(v[0]);
					}
					else
					{
						p[2] = (double)Math.Sign(v[2]);
					}
				}
				else
				{
					if (Math.Abs(v[1]) > Math.Abs(v[2]))
					{
						p[1] = (double)Math.Sign(v[1]);
					}
					else
					{
						p[2] = (double)Math.Sign(v[2]);
					}
				}
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					p[i] = v[i] / s;
				}
			}
			return p;
		}
		public static bool IsOpposite(double[] start, double[] dest)
		{
			double s = length2(start);
			if (s > msEpsilon)
			{
				start[0] = start[0] / s; start[1] = start[1] / s; start[2] = start[2] / s;
				s = length2(dest);
				if (s > msEpsilon)
				{
					dest[0] = dest[0] / s; dest[1] = dest[1] / s; dest[2] = dest[2] / s;

					double cosTheta = Quaternion.dot(start, dest);
					if (Math.Abs(cosTheta) >= 0.01)
					{
						double[] c = cross(start, dest);
						s = length2(c);
						if (s > msEpsilon)
						{
							return false;
						}
					}
				}
			}
			return true;
		}
		// Returns a quaternion such that q*start = dest
		public static Quaternion RotationBetweenVectors(double[] start, double[] dest)
		{
			start = normalize(start);
			dest = normalize(dest);

			double cosTheta = Quaternion.dot(start, dest);
			double[] rotationAxis = new double[3];

			if (cosTheta < -0.99)
			{
				// special case when vectors in opposite directions :
				// there is no "ideal" rotation axis
				// So guess one; any will do as long as it's perpendicular to start
				// This implementation favors a rotation around the Up axis,
				// since it's often what you want to do.
				//rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
				rotationAxis = cross(new double[] { 0.0, 0.0, 1.0 }, start);
				if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
					rotationAxis = cross(new double[] { 1.0, 0.0, 0.0 }, start);
				//rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

				rotationAxis = normalize(rotationAxis);
				//return angleAxis(180.0, rotationAxis);
				return angleAxis(Math.PI, rotationAxis);
			}

			// Implementation from Stan Melax's Game Programming Gems 1 article
			rotationAxis = cross(start, dest);

			
			double s = Math.Sqrt((1.0 + cosTheta) * 2.0);
			double invs = 1.0 / s;

			Quaternion q = new Quaternion(
				s * 0.5,
				rotationAxis[0] * invs,
				rotationAxis[1] * invs,
				rotationAxis[2] * invs
			);
			if (q.w > 1.0 || q.w < -1.0)
			{
				q.normalise();
			}
			return q;
		}
		// Returns a quaternion that will make your object looking towards 'direction'.
		// Similar to RotationBetweenVectors, but also controls the vertical orientation.
		// This assumes that at rest, the object faces +Z.
		// Beware, the first parameter is a direction, not the target point !
		public static Quaternion LookAt(double[] direction, double[] desiredUp)
		{

			if (length2(direction) < 0.0001)
				return new Quaternion();

			// Recompute desiredUp so that it's perpendicular to the direction
			// You can skip that part if you really want to force desiredUp
			double[] right = cross(direction, desiredUp);
			desiredUp = cross(right, direction);

			// Find the rotation between the front of the object (that we assume towards +Z,
			// but this depends on your model) and the desired direction
			Quaternion rot1 = RotationBetweenVectors(new double[] { 0.0, 0.0, 1.0 }, direction);
			// Because of the 1rst rotation, the up is probably completely screwed up. 
			// Find the rotation between the "up" of the rotated object, and the desired up
			//double[] newUp = rot1 * (new double[]{0.0, 1.0, 0.0});
			double[] newUp = rot1.multiply(new double[] { 0.0, 1.0, 0.0 });
			Quaternion rot2 = RotationBetweenVectors(newUp, desiredUp);

			// Apply them
			//return rot2 * rot1; // remember, in reverse order.
			return rot2.multiply(rot1);
		}
		// Like SLERP, but forbids rotation greater than maxAngle (in radians)
		// In conjunction to LookAt, can make your characters 
		public static Quaternion RotateTowards(Quaternion q1, Quaternion q2, double maxAngle)
		{
			if (maxAngle < 0.001)
			{
				// No rotation allowed. Prevent dividing by 0 later.
				return q1;
			}

			//float cosTheta = dot(q1, q2);
			double cosTheta = q1.Dot(q2);

			// q1 and q2 are already equal.
			// Force q2 just to be sure
			if (cosTheta > 0.9999)
			{
				return q2;
			}

			// Avoid taking the long path around the sphere
			if (cosTheta < 0)
			{
				//q1 = q1 * -1.0;
				q1.Scale(-1.0);
				cosTheta *= -1.0;
			}

			double angle = Math.Acos(cosTheta);

			// If there is only a 2° difference, and we are allowed 5°,
			// then we arrived.
			if (angle < maxAngle)
			{
				return q2;
			}

			// This is just like slerp(), but with a custom t
			double t = maxAngle / angle;
			angle = maxAngle;

			Quaternion Q1 = q1.multiply(Math.Sin((1.0 - t) * angle));
			Quaternion Q2 = q2.multiply(Math.Sin(t * angle));
			Quaternion res = Quaternion.Plus(Q1, Q2);
			res.Scale(1.0 / Math.Sin(angle));
			//quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
			//res = normalize(res);
			res.normalise();
			return res;

		}

	}
	
	/// <summary>
	/// use mouse to rorate 3D view
	/// </summary>
	class RotatorMouseController
	{
		private double _cameraDistance;
		private double _viewWidth;
		private double _viewHeight;
		//
		private double _rateX;
		private double _rateY;
		private double _cameraDistance2;
		//
		private Quaternion _quat;
		//
		private double arate = 1.0;
		//
		private double x0;
		private double y0;
		private double _x;
		private double _y;
		//
		private double _angleInDegree = 0.0;
		private double[] _rotateAxis;
		public RotatorMouseController(double cameraDistance, double viewWidth, double viewHeight)
		{
			_rotateAxis = new double[3];
			_quat = new Quaternion(1.0, 0.0, 0.0, 0.0);
			_quat.ToAngleAxisInDegree(out _angleInDegree, _rotateAxis);
			_cameraDistance = cameraDistance;
			_cameraDistance2 = _cameraDistance * _cameraDistance;
			OnViewSizeChange(viewWidth, viewHeight);
		}
		public void OnViewSizeChange(double viewWidth, double viewHeight)
		{
			_viewWidth = viewWidth;
			_viewHeight = viewHeight;
			_rateX = 2.0 * _cameraDistance / _viewWidth;
			_rateY = 2.0 * _cameraDistance / _viewHeight;
			x0 = _viewWidth / 2.0;
			y0 = _viewHeight / 2.0;
		}
		public void OnMouseDown(MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				_x = e.Location.X;
				_y = e.Location.Y;
			}
		}
		public void OnMouseMove(MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				rotateByMouse(e);
			}
		}
		public void OnMouseUp(MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				rotateByMouse(e);
			}
		}
		public void Reset()
		{
			_quat.FromAngleAxis(0.0, new double[] { 1.0, 0.0, 0.0 });
			_quat.ToAngleAxisInDegree(out _angleInDegree, _rotateAxis);
		}
		public double Angle { get { return _angleInDegree; } }
		public double Ux { get { return _rotateAxis[0]; } }
		public double Uy { get { return _rotateAxis[1]; } }
		public double Uz { get { return _rotateAxis[2]; } }
		private void rotateByMouse(MouseEventArgs e)
		{
			double sx = (((double)_x - x0) * _rateX) * arate, sy = (((double)_y - y0) * _rateY) * arate;
			double ex = (((double)e.Location.X - x0) * _rateX) * arate, ey = (((double)e.Location.Y - y0) * _rateY) * arate;
			Rotate(sx, sy, ex, ey);
			_x = e.Location.X;
			_y = e.Location.Y;
		}
		public void Rotate(double sx, double sy, double ex, double ey)
		{
			if (sx > _cameraDistance) sx = _cameraDistance - sx;
			if (sy > _cameraDistance) sy = _cameraDistance - sy;
			if (ex > _cameraDistance) ex = _cameraDistance - ex;
			if (ey > _cameraDistance) ey = _cameraDistance - ey;
			//use sphere x^2+y^2+z^2=r^2 to get z
			double xy2 = sx * sx + sy * sy;
			double sz;
			if (xy2 > _cameraDistance2)
				sz = 0.0;
			else
				sz = Math.Sqrt(_cameraDistance2 - xy2);
			double[] start = new double[] { sx, sy, sz };
			double ez;
			xy2 = ex * ex + ey * ey;
			if (xy2 > _cameraDistance2)
				ez = 0.0;
			else
				ez = Math.Sqrt(_cameraDistance2 - xy2);
			double[] end = new double[] { ex, ey, ez };
			RotateBetweenAxises(start, end);
		}
		public void RotateBetweenAxises(double[] start, double[] end)
		{
			if (!Quaternion.IsOpposite(start, end))
			{
				double[,] R = new double[3, 3];
				double angle;
				double[] axis = new double[3];
				_quat.ToAngleAxis(out angle, axis);
				R = Quaternion.GetRotationMatrix(angle, axis);
				start = Quaternion.transVector(R, start);
				end = Quaternion.transVector(R, end);
				Quaternion q = Quaternion.RotationBetweenVectors(start, end);
				q.ToAngleAxis(out angle, end);
				AddRotation(angle, end);
			}
		}
		public void RotateOnX(double angle)
		{
			double a = angle * Math.PI / 180.0;
			double[] start = new double[] { 0.0, 10.0, 0.0 };
			double[] end = new double[] { 0.0, 10.0 * Math.Cos(a), 10.0 * Math.Sin(a) };
			RotateBetweenAxises(start, end);
		}
		public void RotateOnY(double angle)
		{
			double a = angle * Math.PI / 180.0;
			double[] start = new double[] { 0.0, 0.0, 10.0 };
			double[] end = new double[] { 10.0 * Math.Sin(a),0.0, 10.0 * Math.Cos(a) };
			RotateBetweenAxises(start, end);
		}
		public void RotateOnZ(double angle)
		{
			double a = angle * Math.PI / 180.0;
			double[] start = new double[] { 10.0, 0.0, 0.0 };
			double[] end = new double[] { 10.0 * Math.Cos(a), 10.0 * Math.Sin(a), 0.0 };
			RotateBetweenAxises(start, end);
		}
		public void AddRotation(double angle, double[] axis)
		{
			Quaternion q = Quaternion.angleAxis(angle, axis);
			double[] end = new double[3];
			_quat = q.multiply(_quat);
			_quat.ToAngleAxisInDegree(out _angleInDegree, _rotateAxis);
		}
	}
}
