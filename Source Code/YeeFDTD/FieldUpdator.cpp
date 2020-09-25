/*******************************************************************
	Author:  David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "FieldUpdator.h"

UpdateField::UpdateField()
{
	_field = NULL;
	ret = ERR_OK;
	index = 0;
}
void UpdateField::setMaxRadius(RadiusIndexToSeriesIndex *cache, int maxR, double ch_i, double ce_i)
{
	seriesIndex = cache;
	maxRadius = maxR; 
	maxRadiusNeg = -maxR; 
	ch = ch_i;
	ce = ce_i;
}
void UpdateField::reset(FieldPoint3D *field)
{
	_field = field; 
	index=0;
}
void UpdateHField::handleData(int m, int n, int p)
{
	size_t i;
	double hx,hy,hz;
	//H = H - (dt/mu) Curl(E)
	//    Curl(E)x = dEz/dy - dEy/dz
	//    Curl(E)y = dEx/dz - dEz/dx
	//    Curl(E)z = dEy/dx - dEx/dy
	//               dEz/dy = (Ez(m,n+1,p)-Ez(m,n-1,p))/ds, for n
	//Yee algorithm
	//ch = dt/(ds*mu)
	//Hx = Hx + ch * { (Ey(m,n,p+1) - Ey(m,n,p) - Ez(m,n+1,p) + Ez(m,n,p) }
	//Hy = Hy + ch * { (Ez(m+1,n,p) - Ez(m,n,p) - Ex(m,n,p+1) + Ex(m,n,p) }
	//Hz = Hz + ch * { (Ex(m,n+1,p) - Ex(m,n,p) - Ey(m+1,n,p) + Ey(m,n,p) }
	//
	hx = -_field[index].E.y + _field[index].E.z;
	hy = -_field[index].E.z + _field[index].E.x;
	hz = -_field[index].E.x + _field[index].E.y;
	// handle p+1
	if(p == maxRadius)
	{
		//at positive edge, p+1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m,n,p+1);
		hx += _field[i].E.y;
		hy -= _field[i].E.x;
	}
	//handle n+1
	if(n == maxRadius)
	{
		//at positive edge, n+1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m,n+1,p);
		hx -= _field[i].E.z;
		hz += _field[i].E.x;
	}
	//handle m+1
	if(m == maxRadius)
	{
		//at positive edge, m+1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m+1,n,p);
		hy += _field[i].E.z;
		hz -= _field[i].E.y;
	}
	//
	_field[index].H.x += hx * ch;
	_field[index].H.y += hy * ch;
	_field[index].H.z += hz * ch;
	//
	index++;
}
void UpdateEField::handleData(int m, int n, int p)
{
	size_t i;
	double ex,ey,ez;
	//E = E + (dt/eps) Curl(H)
	//    Curl(H)x = dHz/dy - dHy/dz
	//    Curl(H)y = dHx/dz - dHz/dx
	//    Curl(H)z = dHy/dx - dHx/dy
	//               dHz/dy = (Hz(m,n+1,p)-Hz(m,n-1,p))/ds
	//Yee's algorithm
	//ce = dt / (ds*eps)
	//Ex = Ex + ce {Hz(m,n,p)-Hz(m,n-1,p)-Hy(m,n,p)+Hy(m,n,p-1)}
	//Ey = Ey + ce {Hx(m,n,p)-Hx(m,n,p-1)-Hz(m,n,p)+Hz(m-1,n,p)}
	//Ez = Ez + ce {Hy(m,n,p)-Hy(m-1,n,p)-Hx(m,n,p)+Hx(m,n-1,p)}
	ex = _field[index].H.z - _field[index].H.y;
	ey = _field[index].H.x - _field[index].H.z;
	ez = _field[index].H.y - _field[index].H.x;
	//handle n-1
	if(n == maxRadiusNeg)
	{
		//at negative edge, n-1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m,n-1,p);
		ez += _field[i].H.x;
		ex -= _field[i].H.z;
	}
	//handle p-1
	if(p == maxRadiusNeg)
	{
		//at negative edge, p-1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m,n,p-1);
		ex += _field[i].H.y;
		ey -= _field[i].H.x;
	}
	//handle m-1
	if(m == maxRadiusNeg)
	{
		//at negative edge, m-1 not available, assume 0
	}
	else
	{
		i = seriesIndex->Index(m-1,n,p);
		ey += _field[i].H.z;
		ez -= _field[i].H.y;
	}
	//
	_field[index].E.x += ex * ce;
	_field[index].E.y += ey * ce;
	_field[index].E.z += ez * ce;
	//

	index++;
}

