/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 06/12/2017
	Allrights reserved by David Ge

********************************************************************/
using System;
using System.Collections.Generic;

namespace Draw2D
{
	/// <summary>
	/// avaerage magnitude of divergence on a radius
	/// </summary>
	class ErrorsOnRadiuses
	{
		private int _radiusArraySize;
		private int _excludeRadiuses;
		private double[] _diverg1;
		private double[] _diverg2;
		private double _scale1;
		private double _scale2;
		private double _max1;
		private double _max2;
		public ErrorsOnRadiuses(int size)
		{
			_radiusArraySize = size;
			_excludeRadiuses = 0;
			_diverg1 = new double[size];
			_diverg2 = new double[size];
			_max1 = -1.0;
			_max2 = -1.0;
			_scale1 = 1.0;
			_scale2 = 1.0;
		}
		public int maxRadius
		{
			get
			{
				return _radiusArraySize - _excludeRadiuses;
			}
		}
		public int RadiusCount
		{
			get
			{
				return _radiusArraySize;
			}
		}
		public int ExcludeRadiuses
		{
			get
			{
				return _excludeRadiuses;
			}
			set
			{
				if (value >= 0 && value < _radiusArraySize)
				{
					if (value != _excludeRadiuses)
					{
						_excludeRadiuses = value;
						_max1 = -1.0;
						_max2 = -1.0;
					}
				}
			}
		}
		public double GetMaxValue()
		{
			if (_max1 < 0.0 || _max2 < 0.0)
			{
				int MR = maxRadius;
				for (int r = 0; r < MR; r++)
				{
					if (_diverg1[r] > _max1) _max1 = _diverg1[r];
					if (_diverg2[r] > _max2) _max2 = _diverg2[r];
				}
			}
			return Math.Max(_scale1 * _max1, _scale2 * _max2);
		}
		public void SetDiverg1(int i, double v)
		{
			_diverg1[i] = v;
		}
		public void SetDiverg2(int i, double v)
		{
			_diverg2[i] = v;
		}
		public double Diverg1(int i)
		{
			return _diverg1[i] * _scale1;
		}
		public double Diverg2(int i)
		{
			return _diverg2[i] * _scale2;
		}
		public void SetScale1(double s)
		{
			if (s > 0.0)
			{
				_scale1 = s;
			}
		}
		public void SetScale2(double s)
		{
			if (s > 0.0)
			{
				_scale2 = s;
			}
		}
		public double Scale1
		{
			get
			{
				return _scale1;
			}
		}
		public double Scale2
		{
			get
			{
				return _scale2;
			}
		}
	}

	/// <summary>
	/// a list of ErrorsOnRadiuses, each one represent errors at one time step
	/// </summary>
	class ErrorsOverTime
	{
		private List<ErrorsOnRadiuses> _divergs;
		private int _currentTime = 0;
		private double _scale1 = 1.0;
		private double _scale2 = 1.0;
		private int _excludeRadiuses;
		public ErrorsOverTime()
		{
			_divergs = new List<ErrorsOnRadiuses>();
		}
		public void AddTimeItem(ErrorsOnRadiuses e)
		{
			_divergs.Add(e);
		}
		public ErrorsOnRadiuses this[int t]
		{
			get
			{
				if (t >= 0 && t < _divergs.Count)
				{
					_currentTime = t;
					if (_scale1 != 1.0 && _divergs[t].Scale1 == 1.0)
					{
						_divergs[t].SetScale1(_scale1);
					}
					if (_scale2 != 1.0 && _divergs[t].Scale2 == 1.0)
					{
						_divergs[t].SetScale2(_scale2);
					}
					return _divergs[t];
				}
				return null;
			}
		}
		public ErrorsOnRadiuses GetCurrentItem()
		{
			return _divergs[_currentTime];
		}
		public int GetCurrentTime()
		{
			return _currentTime;
		}
		public int GetExcluderadiuses()
		{
			return _excludeRadiuses;
		}
		public void SetExcludeRadius(int r)
		{
			if (r >= 0)
			{
				if (r != _excludeRadiuses)
				{
					for (int t = 0; t < _divergs.Count; t++)
					{
						_divergs[t].ExcludeRadiuses = r;
					}
					if (_divergs.Count > 0)
					{
						_excludeRadiuses = _divergs[0].ExcludeRadiuses;
					}
				}
			}
		}
		public void SetScale1(double s)
		{
			if (s > 0.0)
			{
				_scale1 = s;
				_divergs[_currentTime].SetScale1(s);
			}
		}
		public void SetScale2(double s)
		{
			if (s > 0.0)
			{
				_scale2 = s;
				_divergs[_currentTime].SetScale2(s);
			}
		}
		public double Scale1
		{
			get
			{
				return _scale1;
			}
		}
		public double Scale2
		{
			get
			{
				return _scale2;
			}
		}
		public int TimeCount
		{
			get
			{
				return _divergs.Count;
			}
		}
		public int RadiusCount
		{
			get
			{
				if (_divergs.Count > 0)
				{
					return _divergs[0].RadiusCount;
				}
				return 0;
			}
		}
		public int DisplayRadiusCount
		{
			get
			{
				if (_divergs.Count > 0)
				{
					return _divergs[0].maxRadius;
				}
				return 0;
			}
		}
	}
}
