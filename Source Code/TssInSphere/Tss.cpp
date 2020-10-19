#include "Tss.h"

SpaceOrderTss::SpaceOrderTss()
{
}


SpaceOrderTss::~SpaceOrderTss()
{
}

/*
fill the estimaton matrix in row-major format
*/
int SpaceOrderTss::fillMatrix()
{
	int ret = ERR_OK;
	if (hs == 0 || a == NULL || A == NULL)
	{
		ret = ERR_INVALID_CALL;
	}
	else
	{
		//YEE:
		//2[ 1/2           (1/2)^3 / 3!          ...  (1/2)^(2(hs-1) + 1) / (2(hs-1) + 1)! 
		//   3/2           (3/2)^3 / 3!          ...  (3/2)^(2(hs-1) + 1) / (2(hs-1) + 1)!
		//                                       ...
		//  (2(hs-1)+1)/2 ((2(hs-1)+1)/2)^3 / 3! ... ((2(hs-1) + 1) / 2)^(2(hs-1) + 1) / (2(hs-1) + 1)!))]

		//TSS:
		//2[ 1    1/3!        ...  1/(2(hs-1) + 1)!
		//   2    2^3/3!      ...  2^(2(hs-1) + 1)/(2(hs-1) + 1)!
		//                    ...
		//   hs   (hs)^3/3!   ...  (hs)^(2(hs-1) + 1)/(2(hs-1) + 1)! ]
		int ij = 0;
		double c = 0.0;
		double d;
		double s, g;
		for (unsigned int i = 0; i < hs; i++)
		{
			c += 1.0;
			d = c;
			s = 1.0;
			g = 1.0;
			for (unsigned int j = 0; j < hs; j++)
			{
				A[ij + j] = 2.0 * d / s;
				d = d*c*c;
				g = g + 1.0;
				s = s * g;
				g = g + 1.0;
				s = s * g;
			}
			ij += hs;
		}
	}
	return ret;
}

Tss::Tss()
{
}


Tss::~Tss()
{
}
