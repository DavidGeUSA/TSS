using System;

namespace Draw2D
{
	static class TssGeometry
	{
		public static UInt32 MaxN(int N)
		{
			return 4 * (UInt32)N + 3;
		}
		public static int CNum(int N)
		{
			return 2 * N + 2;
		}
	}
}
