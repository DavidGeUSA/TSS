#include "TimeYee.h"


TimeYee::TimeYee()
{
}


TimeYee::~TimeYee()
{
}

int TimeYee::initFields(FieldsSetter* f0)
{
	int ret = TimeTssBase::initFields(f0);
	if (ret == ERR_OK)
	{
		//use this opportunity to calculate update-coefficients
		double dm = pams->sim * pams->dt / (2.0*pams->mu);
		double de = pams->sie * pams->dt / (2.0*pams->eps);
		ny1 = pams->ny + 1;
		nz1 = pams->nz + 1;
		GhhP[0] = (1.0 - dm) / (1.0 + dm);
		GehP[0] = (pams->dt / (pams->mu*pams->ds)) / (1.0 + dm);
		GeeP[0] = (1.0 - de) / (1.0 + de);
		GheP[0] = (pams->dt / (pams->eps*pams->ds)) / (1.0 + de);
		printf("\r\nYee coefficients: H = %g * H + %g * CurlE; E = %g * E + %g * CurlH\r\n", GhhP[0], GehP[0], GeeP[0], GheP[0]);
	}
	return ret;
}

int TimeYee::AdvanceOneStep()
{
	unsigned i, j, k;
	size_t w = 0;
	unsigned int h = 0;
	//
	for (i = 0; i <= pams->nx; i++)
	{
		for (j = 0; j <= pams->ny; j++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				//Hx[i,    j+1/2,k+1/2]
				//Hy[i+1/2,j,    k+1/2]
				//Hz[i+1/2,j+1/2,k    ]
				//Ex[i+1/2,j,    k    ]
				//Ey[i,    j+1/2,k    ]
				//Ez[i,    j,   ,k+1/2]
				if (k == pams->nz)
				{
					if (j == pams->ny)
					{
						H[w].x = GhhP[h] * H[w].x + GehP[h] * (0.0 - E[w].y - 0.0 + E[w].z);
					}
					else
					{
						H[w].x = GhhP[h] * H[w].x + GehP[h] * (0.0 - E[w].y - E[I(i, j + 1, k)].z + E[w].z);
					}
				}
				else
				{
					if (j == pams->ny)
					{
						H[w].x = GhhP[h] * H[w].x + GehP[h] * (E[I(i, j, k + 1)].y - E[w].y - 0.0 + E[w].z);
					}
					else
					{
						H[w].x = GhhP[h] * H[w].x + GehP[h] * (E[I(i, j, k + 1)].y - E[w].y - E[I(i, j + 1, k)].z + E[w].z);
					}
				}
				if (i == pams->nx)
				{
					if (k == pams->nz)
					{
						H[w].y = GhhP[h] * H[w].y + GehP[h] * (0.0 - E[w].z - 0.0 + E[w].x);
					}
					else
					{
						H[w].y = GhhP[h] * H[w].y + GehP[h] * (0.0 - E[w].z - E[I(i, j, k + 1)].x + E[w].x);
					}
				}
				else
				{
					if (k == pams->nz)
					{
						H[w].y = GhhP[h] * H[w].y + GehP[h] * (E[I(i + 1, j, k)].z - E[w].z - 0.0 + E[w].x);
					}
					else
					{
						H[w].y = GhhP[h] * H[w].y + GehP[h] * (E[I(i + 1, j, k)].z - E[w].z - E[I(i, j, k + 1)].x + E[w].x);
					}
				}
				if (j == pams->ny)
				{
					if (i == pams->nx)
					{
						H[w].z = GhhP[h] * H[w].z + GehP[h] * (0.0 - E[w].x - 0.0                + E[w].y);
					}
					else
					{
						H[w].z = GhhP[h] * H[w].z + GehP[h] * (0.0 - E[w].x - E[I(i + 1, j, k)].y + E[w].y);
					}
				}
				else
				{
					if (i == pams->nx)
					{
						H[w].z = GhhP[h] * H[w].z + GehP[h] * (E[I(i, j + 1, k)].x - E[w].x - 0.0                + E[w].y);
					}
					else
					{
						H[w].z = GhhP[h] * H[w].z + GehP[h] * (E[I(i, j + 1, k)].x - E[w].x - E[I(i + 1, j, k)].y + E[w].y);
					}
				}
				//
				w++;
			}
		}
	}
	//
	w = 0;
	for (i = 0; i <= pams->nx; i++)
	{
		for (j = 0; j <= pams->ny; j++)
		{
			for (k = 0; k <= pams->nz; k++)
			{
				//Hx[i,    j+1/2,k+1/2]
				//Hy[i+1/2,j,    k+1/2]
				//Hz[i+1/2,j+1/2,k    ]
				//Ex[i+1/2,j,    k    ]
				//Ey[i,    j+1/2,k    ]
				//Ez[i,    j,   ,k+1/2]
				if (j == 0)
				{
					if (k == 0)
					{
						E[w].x = GeeP[h] * E[w].x + GheP[h] * (H[w].z - 0.0 - H[w].y + 0.0);
					}
					else
					{
						E[w].x = GeeP[h] * E[w].x + GheP[h] * (H[w].z - 0.0 - H[w].y + H[I(i, j, k - 1)].y);
					}
				}
				else
				{
					if (k == 0)
					{
						E[w].x = GeeP[h] * E[w].x + GheP[h] * (H[w].z - H[I(i, j - 1, k)].z - H[w].y + 0.0);
					}
					else
					{
						E[w].x = GeeP[h] * E[w].x + GheP[h] * (H[w].z - H[I(i, j - 1, k)].z - H[w].y + H[I(i, j, k - 1)].y);
					}
				}
				if (k == 0)
				{
					if (i == 0)
					{
						E[w].y = GeeP[h] * E[w].y + GheP[h] * (H[w].x - 0.0 - H[w].z + 0.0);
					}
					else
					{
						E[w].y = GeeP[h] * E[w].y + GheP[h] * (H[w].x - 0.0 - H[w].z + H[I(i - 1, j, k)].z);
					}
				}
				else
				{
					if (i == 0)
					{
						E[w].y = GeeP[h] * E[w].y + GheP[h] * (H[w].x - H[I(i, j, k - 1)].x - H[w].z + 0.0);
					}
					else
					{
						E[w].y = GeeP[h] * E[w].y + GheP[h] * (H[w].x - H[I(i, j, k - 1)].x - H[w].z + H[I(i - 1, j, k)].z);
					}
				}
				if (i == 0)
				{
					if (j == 0)
					{
						E[w].z = GeeP[h] * E[w].z + GheP[h] * (H[w].y - 0.0 - H[w].x + 0.0);
					}
					else
					{
						E[w].z = GeeP[h] * E[w].z + GheP[h] * (H[w].y - 0.0 - H[w].x + H[I(i, j - 1, k)].x);
					}
				}
				else
				{
					if (j == 0)
					{
						E[w].z = GeeP[h] * E[w].z + GheP[h] * (H[w].y - H[I(i - 1, j, k)].y - H[w].x + 0.0);
					}
					else
					{
						E[w].z = GeeP[h] * E[w].z + GheP[h] * (H[w].y - H[I(i - 1, j, k)].y - H[w].x + H[I(i, j - 1, k)].x);
					}
				}
				//
				w++;
			}
		}
	}
	//
	return ERR_OK;
}
//average E field to (i*ds,j*ds,k*ds)
void TimeYee::modifyFieldForStatistics_E(Point3Dstruct *src, Point3Dstruct *dest)
{
	size_t w = 0;
	for (unsigned int i = 0; i <= pams->nx; i++)
	{
		for (unsigned int j = 0; j <= pams->ny; j++)
		{
			for (unsigned int k = 0; k <= pams->nz; k++)
			{
				if (i == 0)
				{
					dest[w].x = src[w].x;
				}
				else
				{
					dest[w].x = (src[w].x + src[I(i - 1, j, k)].x) / 2.0;
				}
				if (j == 0)
				{
					dest[w].y = src[w].y;
				}
				else
				{
					dest[w].y = (src[w].y + src[I(i, j - 1, k)].y) / 2.0;
				}
				if (k == 0)
				{
					dest[w].z = src[w].z;
				}
				else
				{
					dest[w].z = (src[w].z + src[I(i, j, k - 1)].z) / 2.0;
				}
				w++;
			}
		}
	}
}
//average H field to (i*ds,j*ds,k*ds)
void TimeYee::modifyFieldForStatistics_H(Point3Dstruct *src, Point3Dstruct *dest)
{
	double dsq = 0.5;
	size_t w = 0;
	for (unsigned int i = 0; i <= pams->nx; i++)
	{
		for (unsigned int j = 0; j <= pams->ny; j++)
		{
			for (unsigned int k = 0; k <= pams->nz; k++)
			{
				if (j == 0 || k == 0)
				{
					dest[w].x = src[w].x;
				}
				else
				{
					dest[w].x = (src[w].x + src[I(i, j - 1, k - 1)].x) * dsq;
				}
				if (i == 0 || k == 0)
				{
					dest[w].y = src[w].y;
				}
				else
				{
					dest[w].y = (src[w].y + src[I(i - 1, j, k - 1)].y) * dsq;
				}
				if (i == 0 || j == 0)
				{
					dest[w].z = src[w].z;
				}
				else
				{
					dest[w].z = (src[w].z + src[I(i - 1, j - 1, k)].z) * dsq;
				}
				w++;
			}
		}
	}
}