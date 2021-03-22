#include "GaussianCurlYee.h"


GaussianCurlYee::GaussianCurlYee()
{
}


GaussianCurlYee::~GaussianCurlYee()
{
}

int GaussianCurlYee::initializeByConfig(TaskFile *configs)
{
	int ret = GaussianCurl::initializeByConfig(configs);
	if (ret == ERR_OK)
	{
		ds = configs->getDouble(TP_TSS_DS, false);
		ret = configs->getErrorCode();
	}
	return ret;
}

/*
	set field value according to Yee cell arrangement
*/
void GaussianCurlYee::SetFields(double x, double y, double z, Point3Dstruct *pE, Point3Dstruct *pH)
{
	double eR;
	double ds2 = ds / 2.0;
	//Yee cell arrangement
	//Hx[i,    j+1/2,k+1/2]
	//Hy[i+1/2,j,    k+1/2]
	//Hz[i+1/2,j+1/2,k    ]
	//
	//Ex[i+1/2,j,    k    ]
	//Ey[i,    j+1/2,k    ]
	//Ez[i,    j,   ,k+1/2]
	eR = pow(M_E, -k * ((x+ds2)*(x+ds2) + y*y + z*z));
	pE->x = b*y*z*eR / 2.0;
	eR = pow(M_E, -k * (x*x + (y+ds2)*(y+ds2) + z*z));
	pE->y = -b*x*z*eR;
	eR = pow(M_E, -k * (x*x + y*y + (z+ds2)*(z+ds2)));
	pE->z = b*y*x*eR / 2.0;
	//
	pH->x = 0.0;
	pH->y = 0.0;
	pH->z = 0.0;
}
