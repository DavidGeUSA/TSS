/*******************************************************************
	Author: David Ge (dge893@gmail.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by David Ge

********************************************************************/

#include "EMField.h"
#include "RadiusIndex.h"

#define NULL 0

#define MAX_NUMBER_POINTS_1D 1290

/*
	Allocate memory for a 3D integer array
	it can be used for storing mapping between 1D series indexes and 3D radius index
	After using the array, it should be freed by Free3DIntegerArray()
*/
size_t ***Allocate3DIntegerArray(int n)
{
	size_t ***a = new size_t **[n]();
	for(int i=0;i<n;i++)
	{
		a[i] = new size_t *[n]();
		for(int j=0;j<n;j++)
		{
			a[i][j] = new size_t[n]();
		}
	}
	return a;
}

/*
	free a 3D integer array which was allocated by Allocate3DIntegerArray
*/
void Free3DIntegerArray(size_t ***a, int n)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			delete[] a[i][j];
		}
		delete[] a[i];
	}
	delete[] a;
}

/*
	p3 = *p * *p * *p
	given p3, find *p
	returns ERR_OK if succeed
*/
int cubicRoot(size_t p3, size_t *p)
{
	int ret = ERR_OK;
	if(p3 == 0)
	{
		*p = 0;
	}
	else if(p3 == 1)
	{
		*p = 1;
	}
	else
	{
		size_t p2 = p3 / 3; //initially it is larger than the solution
		size_t p1 = p2 / 2;
		size_t p2_3 = p2 * p2 * p2;
		size_t p1_3 = p1 * p1 * p1;
		if(p2_3 == p3)
		{
			*p = p2;
			return ERR_OK;
		}
		if(p1_3 == p3)
		{
			*p = p1;
			return ERR_OK;
		}
		//assuming p2 is always larger than the solution
		while(ret == ERR_OK)
		{
			if(p1_3 > p3)
			{
				//p1 is also larger than the solution
				//go downwards
				p2 = p1;
				p1 = p1 / 2;
			}
			else
			{
				//p1 is less than the solution
				p1 = (p1 + p2)/2;
			}
			if(p1 == p2)
			{
				ret = ERR_INVALID_SIZE;
				break;
			}
			p1_3 = p1 * p1 * p1;
			if(p1_3 == p3)
			{
				*p = p1;
				return ERR_OK;
			}
		}
	}
	return ret;
}

/*
	get radius from memory size.

	size: memory size
	radius: pointer to radius

	use following relations to find (*radius):
	size = sizeof(FieldPoint3D) * (item count)
	(2 * (*radius) + 1)^3 = (item count)
*/
int MemorySizeToRadius(size_t size, unsigned *radius)
{
	int ret = ERR_OK;
	if( (size % sizeof(FieldPoint3D)) != 0 )
	{
		ret = ERR_INVALID_SIZE;
	}
	else
	{
		//s3 = (2*r+1)^3
		size_t maxN3;
		size_t s3 = size / sizeof(FieldPoint3D);
		ret = cubicRoot(s3, &maxN3);
		if(ret == ERR_OK)
		{
			if(maxN3 > MAX_NUMBER_POINTS_1D)
			{
				ret = ERR_RADIUS_3DINDEX_TOO_BIG;
			}
			else
			{
				if( (maxN3-1) % 2 != 0)
				{
					ret = ERR_INVALID_SIZE;
				}
				else
				{
					*radius = (unsigned)((maxN3 - 1) / 2);
				}
			}
		} 
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////
/*
	get number of data points at a given radius
	a: radius
	return: number of data points on the radius
*/
size_t pointsAt(unsigned r)
{
	if(r == 0)
		return 1;
	return 24*r*r + 2;
}

/*
	get total data points in a sphere for a given radius.
	the "sphere" is actually a cubic
*/
size_t totalPointsInSphere(unsigned r)
{
	size_t r21 = 2 * r + 1;
	return r21 * r21 * r21;
}

/*
	returns series index from radius indexes m,n,p
	radius = max(|m|,|n|,|p|)
	passing radius into the function for convenience
	the return value is between 0 and pointsAt(radius)-1
	the return value is the index for space points of the memory block for the radius, not for the whole memory 
*/
size_t IndexesToIndex(unsigned radius, int m, int n, int p, int *ret)
{
	int r = (int)radius;
	*ret = ERR_OK;
	if( r < 0)
	{
		*ret = ERR_GEOMETRY_TOO_BIG;
		return 0;
	}
	//
	if(m == r)
	{
		if(n == r)
		{
			if(p == r) //(r, r, r)
			{
				return 0;
			}
			else if( (p+r) == 0) //(r, r, -r)
			{
				return 1;
			}
			else if(p == 0) //(r, r, 0)
			{
				return 8;
			}
			else if(p > 0 && p < r) //(r, r, +)
			{
				return 26 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(r, r, -)
			{
				return 27 + 24 * (-p-1);
			}
		}
		else if( (n+r) == 0)
		{
			if(p == r) //(r, -r, r)
			{
				return 2;
			}
			else if( (p+r) == 0) //(r, -r, -r)
			{
				return 3;
			}
			else if(p == 0) //(r, -r, 0)
			{
				return 9;
			}
			else if(p > 0 && p < r) //(r, -r, +)
			{
				return 28 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(r, -r, -)
			{
				return 29 + 24 * (-p-1);
			}
		}
		else if(n == 0) //m = r
		{
			if(p == r) //(r, 0, r)
			{
				return 16;
			}
			else if( (p+r) == 0) //(r, 0, -r)
			{
				return 17;
			}
			else if(p == 0) //(r, 0, 0)
			{
				return 20;
			}
			else if( p > 0 && p < r) //(r, 0, +)
			{
				return 24 * r + 2 + 24 * (p-1);
			}
			else if( p < 0 && (p+r) > 0) //(r, 0, -)
			{
				return 24 * r + 3 + 24 * (-p-1);
			}
		}
		else if(n > 0 && n < r) //m==r
		{
			if(p == r) //(r, +, r)
			{
				return 42 + 24 * (n-1);
			}
			else if( (p+r) == 0) //(r, +, -r)
			{
				return 46 + 24 * (n-1);
			}
			else if(p == 0) //(r, +, 0)
			{
				return 24*r + 6 + 24 * (n-1);
			}
			else if(p > 0 && p < r) //(r, +, +)
			{
				return 48*r - 22 + 24*((p-1) + (r-1)*(n-1)); 
			}
			else if(p < 0 && (p+r) > 0) //(r, +, -)
			{
				return 48*r - 21 + 24*((-p-1) + (r-1) * (n-1));
			}
		}
		else if(n < 0 && (n+r) > 0) //m == r
		{
			if(p == r) //(r, -, r)
			{
				return 43 + 24 * (-n-1);
			}
			else if( (p+r) == 0) //(r, -, -r)
			{
				return 47 + 24 * (-n-1);
			}
			else if(p == 0) //(r, -, 0)
			{
				return 24*r + 7 + 24 * (-n-1);
			}
			else if(p > 0 && (p+r) > 0) //(r, -, +)
			{
				return 48*r - 20 + 24*((p-1) + (r-1) * (-n-1));
			}
			else if(p < 0 && (p+r) > 0) //(r, -, -)
			{
				return 48*r - 19 + 24*((-p-1) + (r-1) * (-n-1));
			}
		}
	}
	else if( (m+r) == 0) //m == -r
	{
		if(n == r)
		{
			if(p == r) //(-r, r, r)
			{
				return 4;
			}
			else if( (p+r) == 0) //(-r, r, -r)
			{
				return 5;
			}
			else if(p == 0) //(-r, r, 0)
			{
				return 10;
			}
			else if(p > 0 && p < r) //(-r, r, +)
			{
				return 30 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(-r, r, -)
			{
				return 31 + 24 * (-p-1);
			}
		}
		else if( (n+r) == 0)
		{
			if(p == r) //(-r, -r, r)
			{
				return 6;
			}
			else if( (p+r) == 0) //(-r,-r, -r)
			{
				return 7;
			}
			else if(p == 0) //(-r, -r, 0)
			{
				return 11;
			}
			else if(p > 0 && p < r) //(-r, -r, +)
			{
				return 32 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(-r, -r, -)
			{
				return 33 + 24 * (-p-1);
			}
		}
		else if(n == 0)
		{
			if(p == r) //(-r, 0, r);
			{
				return 18;
			}
			else if( (p+r) == 0) //(-r, 0, -r)
			{
				return 19;
			}
			else if(p == 0) //(-r, 0, 0)
			{
				return 21;
			}
			else if(p > 0 && p < r) //(-r, 0, +)
			{
				return 24 * r + 4 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(-r, 0, -)
			{
				return 24*r + 5 + 24 * (-p-1);
			}
		}
		else if(n > 0 && n < r) //m=-r
		{
			if(p == r) // (-r, +, r)
			{
				return 44 + 24 * (n-1); 
			}
			else if ( (p+r) == 0) //(-r, +, -r)
			{
				return 48 + 24 * (n-1);
			}
			else if(p == 0) //(-r, +, 0)
			{
				return 24*r + 8 + 24 * (n-1);
			}
			else if(p > 0 && p < r) //(-r, +, +)
			{
				return 48*r - 18 + 24*((p-1) + (r-1) * (n-1));
			}
			else if(p < 0 && (p+r) > 0) //(-r, +, -)
			{
				return 48*r - 17 + 24*((-p-1) + (r-1) * (n-1));
			}
		}
		else if(n < 0 && (n+r) > 0)  //m = -r
		{
			if(p == r) //(-r, -, r)
			{
				return 45 + 24 * (-n-1);
			}
			else if( (p+r) == 0) //(-r, -, -r)
			{
				return 49 + 24 * (-n-1);
			}
			else if(p == 0) //(-r, -, 0)
			{
				return 24*r + 9 + 24 * (-n-1);
			}
			else if(p > 0 && p < r) //(-r, -, +)
			{
				return 48*r - 16 + 24*((p-1) + (r-1) * (-n-1));
			}
			else if(p < 0 && (p+r) > 0) //(-r, -, -)
			{
				return 48*r - 15 + 24*((-p-1) + (r-1) * (-n-1));
			}
		}
	}
	else if(m == 0)
	{
		if(n == r)
		{
			if(p == r) //(0, r, r)
			{
				return 12;
			}
			else if( (p+r) == 0) //(0, r, -r)
			{
				return 13;
			}
			else if(p == 0) //(0, r, 0)
			{
				return 22;
			}
			else if(p > 0 && p < r) //(0, r, +)
			{
				return 24*r + 10 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(0, r, -)
			{
				return 24*r + 11 + 24 * (-p-1);
			}
		}
		else if( (n+r) == 0) //m==0
		{
			if(p==r) //(0, -r, r)
			{
				return 14;
			}
			else if( (p+r) == 0) //(0, -r, -r)
			{
				return 15;
			}
			else if(p == 0) //(0, -r, 0)
			{
				return 23;
			}
			else if(p > 0 && p < r) //(0, -r, +)
			{
				return 24*r + 12 + 24 * (p-1);
			}
			else if(p < 0 && (p+r) > 0) //(0, -r, -)
			{
				return 24*r + 13 + 24 * (-p-1);
			}
		}
		else if(n == 0)
		{
			if(p == r) //(0, 0, r)
			{
				return 24;
			}
			else if( (p+r) == 0) // (0, 0, =r)
			{
				return 25;
			}
			else if(p == 0) //(0, 0, 0)
			{
				if(r == 0)
				{
					return 0;
				}
			}
		}
		else if(n > 0 && n < r) //m==0
		{
			if(p == r) //(0, +, r)
			{
				return 24*r + 22 + 24 * (n-1);
			}
			else if( (p+r) == 0) //(0, +, -r)
			{
				return 24*r + 24 + 24 * (n-1);
			}
		}
		else if( n < 0 && (n+r) > 0)
		{
			if(p == r) //(0, -, r)
			{
				return 24*r + 23 + 24 * (-n-1);
			}
			else if( (p+r) == 0) //(0, -, -r)
			{
				return 24*r + 25 + 24 * (-n-1);
			}
		}
	}
	else if(m > 0 && m < r)
	{
		if(n == r)
		{
			if(p == r) //(+, r, r)
			{
				return 34 + 24 * (m-1);
			}
			else if( (p+r) == 0) //(+, r, -r)
			{
				return 36 + 24 * (m-1);
			}
			else if(p == 0) //(+, r, 0)
			{
				return 24*r + 14 + 24 * (m-1);
			}
			else if(p > 0 && p < r) //(+, r, +)
			{
				return 48*r - 14 + 24*((p-1) + (r-1) * (m-1));
			}
			else if(p < 0 && (p+r) > 0) //(+, r, -)
			{
				return 48*r - 13 + 24*((-p-1) + (r-1) * (m-1));
			}
		}
		else if( (n+r) == 0)
		{
			if(p == r) //(+, -r, r)
			{
				return 38 + 24 * (m-1);
			}
			else if( (p+r) == 0) //(+, -r, -r)
			{
				return 40 + 24 * (m-1);
			}
			else if(p == 0) //(+, -r, 0)
			{
				return 24*r + 16 + 24 * (m-1);
			}
			else if(p > 0 && p < r) //(+, -r, +)
			{
				return 48*r - 10 + 24*((p-1) + (r-1) * (m-1));
			}
			else if(p < 0 && (p+r) > 0) //(+, -r, -)
			{
				return 48*r - 9 + 24*((-p-1) + (r-1) * (m-1));
			}
		}
		else if(n == 0)  //m > 0
		{
			if(p == r) //(+, 0, r)
			{
				return 24*r + 18 + 24 * (m-1);
			}
			else if( (p+r) == 0) //(+, 0, -r)
			{
				return 24*r + 20 + 24 * (m-1);
			}
		}
		else if(n > 0 && n < r)
		{
			if(p == r) //(+, +, r)
			{
				return 48*r - 6 + 24*((n-1) + (r-1) * (m-1));
			}
			else if( (p+r) == 0) //(+, +, -r)
			{
				return 48*r - 2 + 24*((n-1) + (r-1) * (m-1));
			}
		}
		else if(n < 0 && (n+r) > 0)
		{
			if(p == r) //(+, -, r)
			{
				return 48*r - 4 + 24*((-n-1) + (r-1) * (m-1));
			}
			else if( (p+r) == 0) //(+, -, -r)
			{
				return 48*r + 24*((-n-1) + (r-1) * (m-1)); 
			}
		}
	}
	else if(m < 0 && (m+r) > 0)
	{
		if(n == r)
		{
			if(p == r) //(-, r, r)
			{
				return 35 + 24 * (-m-1);
			}
			else if( (p+r) == 0) //(-, r, -r)
			{
				return 37 + 24 * (-m-1);
			}
			else if(p == 0) //(-, r, 0)
			{
				return 24*r + 15 + 24 * (-m-1);
			}
			else if(p > 0 && p < r) //(-, r, +)
			{
				return 48*r - 12 + 24*((p-1) + (r-1) * (-m-1));
			}
			else if(p < 0 && (p+r) > 0) //(-, r, -)
			{
				return 48*r - 11 + 24*((-p-1) + (r-1) * (-m-1));
			}
		}
		else if( (n+r) == 0)
		{
			if(p == r) //(-, -r, r)
			{
				return 39 + 24 * (-m-1);
			}
			else if( (p+r) == 0) //(-, -r, -r)
			{
				return 41 + 24 * (-m-1);
			}
			else if(p == 0) //(-, -r, 0)
			{
				return 24*r + 17 + 24 * (-m-1);
			}
			else if(p > 0 && p < r) //(-, -r, +)
			{
				return 48*r - 8 + 24*((p-1) + (r-1) * (-m-1));
			}
			else if(p < 0 && (p+r) > 0) //(-, -r, -)
			{
				return 48*r - 7 + 24*((-p-1) + (r-1) * (-m-1));
			}
		}
		else if(n == 0) //m < 0
		{
			if(p == r) //(-, 0, r)
			{
				return 24*r + 19 + 24 * (-m-1);
			}
			else if( (p+r) == 0) //(-, 0, -r)
			{
				return 24*r + 21 + 24 * (-m-1);
			}
		}
		else if(n > 0 && n < r) //n > 0
		{
			if(p == r) //(-, +, r)
			{
				return 48*r - 5 + 24*((n-1) + (r-1) * (-m-1));
			}
			else if( (p+r) == 0) //(-, +, -r)
			{
				return 48*r - 1 + 24*((n-1) + (r-1) * (-m-1));
			}
		}
		else if(n < 0 && (n+r) > 0)
		{
			if(p == r) //(-, -, r)
			{
				return 48*r - 3 + 24*((-n-1) + (r-1) * (-m-1));
			}
			else if( (p+r) == 0) //(-, -, -r)
			{
				return 48*r + 1 + 24*((-n-1) + (r-1) * (-m-1));
			}
		}
	}
	//
	*ret = ERR_INVALID_INDEX_RADIUS;
	return 0;
}


/*
	get radius indexes for a given series index
	it is the opposit of IndexesToIndex.
	the given series index, a, starts from 0, not from the starting index of the memory block for the radius in the whole memory
*/
int IndexToIndexes(unsigned radius, size_t a, int *m, int *n, int *p)
{
	int ret = ERR_OK;
	int r = (int)radius;
	size_t u;
	if( r < 0)
	{
		return ERR_GEOMETRY_TOO_BIG;
	}
	switch (a)
	{
		case 0:
			*m=*n=*p=r;     // 0: (r, r, r)
			break;
		case 1:
			*m=*n=r; *p= -r;     // 1: (r, r, -r)
			break;
		case 2:
			*m=*p=r; *n=-r;     // 2: (r, -r, r)
			break;
		case 3:
			*m=r; *n=*p=-r;     // 3: (r, -r, -r)
			break;
		case 4:
			*m=-r; *n=*p=r;      // 4: (-r, r, r)
			break;
		case 5:
			*m=*p=-r; *n=r;     // 5: (-r, r, -r)
			break;
		case 6:
			*m=*n=-r; *p=r;     // 6: (-r, -r, r)
			break;
		case 7:
			*m=*n=*p=-r;     // 7: (-r, -r, -r)
			break;
		//2. 12 Edge Line Centers. Two axes at edges, one axis at 0: 12 points: 8 - 19
		case 8:
			*m=*n=r; *p=0;    // 8: (r, r, 0)
			break;
		case 9:
			*m=r; *n=-r; *p =0;     // 9: (r, -r, 0)
			break;
		case 10:
			*m=-r; *n=r; *p = 0;     // 10: (-r, r, 0)
			break;
		case 11:
			*m=*n=-r; *p =0;     // 11: (-r, -r, 0)
			break; 
		//
		case 12:
			*m=0; *n=*p = r;    // 12: (0, r, r)
			break;
		case 13:
			*m=0; *n=r; *p = -r;    // 13: (0, r, -r)
			break;
		case 14:
			*m=0; *n=-r; *p=r;    // 14: (0, -r, r)
			break;
		case 15:
			*m=0; *n=*p=-r;    // 15: (0, -r, -r)
			break;
		//
		case 16:
			*m=*p=r; *n = 0;   // 16: (r, 0, r)
			break;
		case 17:
			*m=r; *n = 0; *p = -r;   // 17: (r, 0, -r)
			break;
		case 18:
			*m=-r; *n=0; *p=r;   // 18: (-r, 0, r)
			break;
		case 19:
			*m=*p=-r; *n=0;  // 19: (-r, 0, -r)
			break;
		case 20:
			*m=r; *n=*p=0;   // 20: (r, 0, 0)
			break;
		case 21:
			*m=-r; *n=*p=0;   // 21: (-r, 0, 0)
			break;
		case 22:
			*m=*p=0; *n=r;   // 22: (0, r, 0)
			break;
		case 23:
			*m=*p=0; *n=-r;   // 23: (0, -r, 0)
			break;
		case 24:
			*m=*n=0; *p=r;    // 24: (0, 0, r)
			break;
		case 25:
			*m=*n=0; *p=-r;   // 25: (0, 0, -r)
			break;
		//last index : 25
		default:
			//3. (r>1) 12 Edge Lines exclude center. Each line has 2(r-1)  points. Two axes at edge, one axis not at 0: 24(r-1)  points: 20 to 24r-5
			if(r <= 1)
			{
				ret = ERR_INVALID_RADIUS_INDEX;
			}
			else
			{
				u= 24 * r + 1;
				if(a <= u)
				{
					for(int k=1;k<r;k++)
					{
						u = 24*r + 25 + 24 * (k-1);
						if(a <= u)
						{
							u = a - 24*(k-1);
							switch(u)
							{
								case 26:
									*m=*n=r; *p=k;   // 26+24*(k-1): (r, r, k)
									break;
								case 27:
									*m=*n=r; *p=-k;   // 27+24*(k-1): (r, r, -k)
									break;
								case 28:
									*m=r; *n=-r; *p=k;   // 28+24*(k-1): (r, -r, k)
									break; 
								case 29:
									*m=r; *n=-r; *p=-k;   // 29+24*(k-1): (r, -r, -k)
									break;
							//
								case 30:
									*m=-r; *n=r; *p=k;   // 30+24*(k-1): (-r, r, k)
									break;
								case 31:
									*m=-r; *n=r; *p=-k;   // 31+24*(k-1): (-r, r, -k)
									break;
								case 32:
									*m=*n=-r; *p=k;   // 32+24*(k-1): (-r, -r, k)
									break;
								case 33:
									*m=*n=-r; *p=-k;   // 33+24*(k-1): (-r, -r, -k)
									break;
							//
								case 34:
									*m=k; *n=*p=r;   // 34+24*(k-1): (k, r, r)
									break;
								case 35:
									*m=-k; *n=*p=r;    // 35+24*(k-1): (-k, r, r)
									break;
								case 36:
									*m=k; *n=r; *p=-r;   // 36+24*(k-1): (k, r, -r)
									break;
								case 37:
									*m=-k; *n=r; *p = -r;   // 37+24*(k-1): (-k, r, -r)
									break;
							//
								case 38:
									*m=k; *n=-r; *p=r;   // 38+24*(k-1): (k, -r, r)
									break;
								case 39:
									*m=-k; *n=-r; *p=r;   // 39+24*(k-1): (-k, -r, r)
									break;
								case 40:
									*m=k; *n=*p=-r;    // 40+24*(k-1): (k, -r, -r)
									break;
								case 41:
									*m=-k; *n=*p=-r;   // 41+24*(k-1): (-k, -r, -r)
									break;
							//
								case 42:
									*m=*p=r; *n=k;    // 42+24*(k-1): (r, k, r)
									break;
								case 43:
									*m=*p=r; *n=-k;   // 43+24*(k-1): (r, -k, r)
									break;
								case 44:
									*m=-r; *n=k; *p=r;   // 44+24*(k-1): (-r, k, r)
									break;
								case 45:
									*m=-r; *n=-k; *p=r;    // 45+24*(k-1): (-r, -k, r)
									break;
							//
								case 46:
									*m=r; *n=k; *p=-r;   // 46+24*(k-1): (r, k, -r)
									break;
								case 47:
									*m=r; *n=-k; *p=-r;    // 47+24*(k-1): (r, -k, -r)
									break;
								case 48:
									*m=*p=-r; *n=k;   // 48+24*(k-1): (-r, k, -r)
									break;
								case 49:
									*m=*p=-r; *n=-k;   // 49+24*(k-1): (-r, -k, -r)
									break;
							}
						}//if
					} //for
				}//if a <= 24r + 1
				//last index : 49+24*(r-2) = 24r + 1
				else
				{
					//4 Inner parts of cross-lines on m-plane. One axis at 0 and m=±r:24r+2 to 32r-7,8(r-1) points
					u = 48 * r - 23;
					if(a <= u)
					{
						for(int k=1;k<r;k++)
						{
							u = a - 24*r - 24 * (k-1);
							switch(u)
							{
								//m=+-r, n=0 //////////////////////////////////////////////
								case 2:
									*m=r; *n=0; *p = k;   // 24r + 2 + 24 * (k-1): (r, 0, k)
									break;
								case 3:
									*m=r; *n=0; *p=-k;   // 24r + 3 + 24 * (k-1): (r, 0, -k)
									break;
								case 4:
									*m=-r; *n=0; *p=k;   // 24r + 4 + 24 * (k-1): (-r, 0, k)
									break;
								case 5:
									*m=-r; *n=0; *p=-k;   // 24r + 5 + 24 * (k-1): (-r, 0, -k)
									break;
								//m=+-r, p=0
								case 6:
									*m=r; *n=k; *p=0;   // 24r + 6 + 24 * (k-1): (r, k, 0)
									break;
								case 7:
									*m=r; *n=-k; *p = 0;   // 24r + 7 + 24 * (k-1): (r, -k, 0)
									break;
								case 8:
									*m=-r; *n=k; *p=0;   // 24r + 8 + 24 * (k-1): (-r, k, 0)
									break;
								case 9:
									*m=-r; *n=-k; *p=0;   // 24r + 9 + 24 * (k-1): (-r, -k, 0)
									break;
								///////////////////////////////////////
								//n=+-r, m=0 ///////////////////////////////////////////////
								case 10:
									*m=0; *n=r; *p=k;   // 24r + 10 + 24 * (k-1): (0, r, k)
									break;
								case 11:
									*m=0;*n=r;*p=-k;   // 24r + 11 + 24 * (k-1): (0, r, -k)
									break;
								case 12:
									*m=0;*n=-r;*p=k;   // 24r + 12 + 24 * (k-1): (0, -r, k)
									break;
								case 13:
									*m=0; *n=-r; *p=-k;   // 24r + 13 + 24 * (k-1): (0, -r, -k)
									break;
								//n=+-r, p=0
								case 14:
									*m=k; *n=r; *p=0;   // 24r + 14 + 24 * (k-1): (k, r, 0)
									break;
								case 15:
									*m=-k; *n=r; *p=0;   // 24r + 15 + 24 * (k-1): (-k, r, 0)
									break;
								case 16:
									*m=k; *n=-r; *p = 0;   // 24r + 16 + 24 * (k-1): (k, -r, 0)
									break;
								case 17:
									*m=-k; *n=-r; *p=0;   // 24r + 17 + 24 * (k-1): (-k, -r, 0)
									break;
								/////////////////////////////////////////////
								//p=+-r, n=0 ///////////////////////////////////////////////
								case 18:
									*m=k; *n=0; *p = r;   // 24r + 18 + 24 * (k-1): (k, 0, r)
									break;
								case 19:
									*m=-k; *n=0; *p=r;   // 24r + 19 + 24 * (k-1): (-k, 0, r)
									break;
								case 20:
									*m=k; *n=0; *p=-r;   // 24r + 20 + 24 * (k-1): (k, 0, -r)
									break;
								case 21:
									*m=-k; *n=0; *p=-r;   // 24r + 21 + 24 * (k-1): (-k, 0, -r)
									break;
								//p=+-r, m=0
								case 22:
									*m=0; *n=k; *p =r;   // 24r + 22 + 24 * (k-1): (0, k, r)
									break;
								case 23:
									*m=0; *n=-k; *p=r;   // 24r + 23 + 24 * (k-1): (0, -k, r)
									break;
								case 24:
									*m=0; *n=k; *p = -r;   // 24r + 24 + 24 * (k-1): (0, k, -r)
									break;
								case 25:
									*m = 0; *n =-k; *p = -r;   // 24r + 25 + 24 * (k-1): (0, -k, -r)
									break;
							}//switch
						}//for
					}//if a < 48 * r - 23
					//Last index: 24r + 25 + 24 * (k-1) = 24r + 25 + 24 * (r-2) = 48r - 23
					else
					{
						//5. one at edge, the other two not at 0
						for(int j=1;j<r;j++)
						{
							for(int k=1;k<r;k++)
							{
								//24 points
								u = a - (48*r - 22 + 24*((k-1) + (r-1)*(j-1)));
								if(u < 24)
								{
									switch(u)
									{
									//5.1 m=r
										case 0:
											*m=r; *n=j; *p=k;   // 48r - 22 + 24*[(k-1) + (r-1)*(j-1)]: (r, j, k)
											break;
										case 1:
											*m=r; *n=j; *p = -k;   // 48r - 21 + 24*[(k-1) + (r-1) * (j-1)]: (r, j, -k)
											break;
										case 2:
											*m=r; *n=-j; *p=k;   // 48r - 20 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, k)
											break;
										case 3:
											*m=r; *n=-j; *p = -k;   // 48r - 19 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, -k)
											break;
									//m=-r
										case 4:
											*m=-r; *n=j; *p=k;   // 48r - 18 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, k)
											break;
										case 5:
											*m=-r; *n=j; *p=-k;   // 48r - 17 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, -k)
											break;
										case 6:
											*m=-r; *n=-j; *p= k;   // 48r - 16 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, k)
											break;
										case 7:
											*m=-r; *n=-j; *p=-k;   // 48r - 15 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, -k)
											break;
									//5.2 n=r
										case 8:
											*m=j; *n=r; *p=k;   // 48r - 14 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, k)
											break;
										case 9:
											*m=j; *n=r; *p=-k;   // 48r - 13 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, -k)
											break;
										case 10:
											*m=-j; *n=r; *p=k;   // 48r - 12 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, k)
											break;
										case 11:
											*m=-j; *n=r; *p=-k;   // 48r - 11 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, -k)
											break;
									//n=-r
										case 12:
											*m=j; *n=-r; *p=k;   // 48r - 10 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, k)
											break;
										case 13:
											*m=j; *n=-r; *p=-k;   // 48r - 9 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, -k)
											break;
										case 14:
											*m=-j; *n = -r; *p = k;   // 48r - 8 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, k)
											break;
										case 15:
											*m=-j; *n=-r; *p=-k;   // 48r - 7 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, -k)
											break;
									//5.3 p=r
										case 16:
											*m=j; *n=k; *p=r;   // 48r - 6 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, r)
											break;
										case 17:
											 *m=-j;*n=k;*p=r;   // 48r - 5 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, r)
											 break;
										case 18:
											*m=j;*n=-k;*p=r;   // 48r - 4 + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, r)
											break;
										case 19:
											*m=-j; *n=-k; *p=r;    // 48r - 3 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, r)
											break;
									//p=-r
										case 20:
											*m=j;*n=k;*p=-r;   // 48r - 2 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, -r)
											break;
										case 21:
											*m=-j;*n=k;*p=-r;   // 48r - 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, -r)
											break;
										case 22:
											*m=j;*n=-k;*p=-r;   // 48r + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, -r)
											break;
										case 23:
											*m=-j; *n=-k; *p= -r;   // 48r + 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, -r)
											break;
									}//switch
									return ret;
								}//if
							}//for
						}//for
						//last index: 48r + 1 + 24*[(r-2) + (r-1) * (r-2)] = 48r + 1 + 24r - 48 + 24 * (r^2 - 3r + 2) = 24*r^2 + 1
						if(u >= 24)
						{
							ret = ERR_INVALID_RADIUS_INDEX;
						}
					}
				}
			}//r>1
			break;//default
	}//switch
	//
	return ret;
}

////////GoThroughSphereByIndexes///////////////////////////////////////////////////////////////////////////
/*
	go through space points radius by radius
	radius = 0, 1, 2, ..., maxR
	for each radius, go through radius indexes m, n, p
	for each combination of (m,n,p), call virtual function handleData
*/
int GoThroughSphereByIndexes::gothroughSphere(int maxR)
{
	ret = ERR_OK;
	index = 0;
	maxRadius = maxR;
	int j,k;
		int r;
		RadiusHandleType rht;
		//r = 0
		//0: (0,0,0)
		rht = setRadius(0);
		if(rht == Finish)
		{
			onFinish();
			return ret;
		}
		if(rht != DoNotProcess)
		{
			//source->getField(0, 0, 0, &p);
			handleData(0, 0, 0);
		}
		if(rht == ProcessAndFinish)
		{
			onFinish();
			return ret;
		}
		//
		for(r=1;r<=maxRadius;r++)
		{
			if(ret != ERR_OK)
			{
				break;
			}
			rht = setRadius(r);
			if(rht == DoNotProcess)
			{
				continue;
			}
			else if(rht == Finish)
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
			handleData(r, r, r); 
			//source->getField(x1, y1, z2, &p);     // 1: (r, r, -r)
			handleData(r, r, -r); 
			//source->getField(x1, y2, z1, &p);     // 2: (r, -r, r)
			handleData(r, -r, r); 
			//source->getField(x1, y2, z2, &p);     // 3: (r, -r, -r)
			handleData(r, -r, -r); 
			//source->getField(x2, y1, z1, &p);     // 4: (-r, r, r)
			handleData(-r, r, r); 
			//source->getField(x2, y1, z2, &p);     // 5: (-r, r, -r)
			handleData(-r, r, -r); 
			//source->getField(x2, y2, z1, &p);     // 6: (-r, -r, r)
			handleData(-r, -r, r); 
			//source->getField(x2, y2, z2, &p);     // 7: (-r, -r, -r)
			handleData(-r, -r, -r);
			//
			//pointCount += 8;
			//last index: 7
			//
			//2. 12 Edge Line Centers. Two axes at edges, one axis at 0: 12 points: 8 - 19
			//source->getField(x1, y1, 0, &p);     // 8: (r, r, 0)
			handleData(r, r, 0); 
			//source->getField(x1, y2, 0, &p);     // 9: (r, -r, 0)
			handleData(r, -r, 0); 
			//source->getField(x2, y1, 0, &p);     // 10: (-r, r, 0)
			handleData(-r, r, 0); 
			//source->getField(x2, y2, 0, &p);     // 11: (-r, -r, 0)
			handleData(-r, -r, 0); 
			//
			//source->getField(0, y1, z1, &p);    // 12: (0, r, r)
			handleData(0, r, r);
			//source->getField(0, y1, z2, &p);    // 13: (0, r, -r)
			handleData(0, r, -r);
			//source->getField(0, y2, z1, &p);    // 14: (0, -r, r)
			handleData(0, -r, r);
			//source->getField(0, y2, z2, &p);    // 15: (0, -r, -r)
			handleData(0, -r, -r);
			//
			//source->getField(x1, 0, z1, &p);   // 16: (r, 0, r)
			handleData(r, 0, r); 
			//source->getField(x1, 0, z2, &p);   // 17: (r, 0, -r)
			handleData(r, 0, -r);
			//source->getField(x2, 0, z1, &p);   // 18: (-r, 0, r)
			handleData(-r, 0, r); 
			//source->getField(x2, 0, z2, &p);   // 19: (-r, 0, -r)
			handleData(-r, 0, -r);
			//pointCount += 12;
			//last index : 19
			//
			//3.  plane centers. two axes at 0: 6 points from 24r-4 to 24r+1
			//source->getField(x1, 0, 0, &p);   // 20: (r, 0, 0)
			handleData(r, 0, 0);
			//source->getField(x2, 0, 0, &p);   // 21: (-r, 0, 0)
			handleData(-r, 0, 0);
			//source->getField(0, y1, 0, &p);   // 22: (0, r, 0)
			handleData(0, r, 0);
			//source->getField(0, y2, 0, &p);   // 23: (0, -r, 0)
			handleData(0, -r, 0);
			//source->getField(0, 0, z1, &p);   // 24: (0, 0, r)
			handleData(0, 0, r);
			//source->getField(0, 0, z2, &p);   // 25: (0, 0, -r)
			handleData(0, 0, -r);
			//last index: 25
			//d = 0;
			//3. (r>1) 12 Edge Lines exclude center. Each line has 2(r-1)  points. Two axes at edge, one axis not at 0: 24(r-1)  points: 26 to 24r+1
			for(k=1;k<r;k++)
			{
				if(ret != ERR_OK)
				{
					break;
				}
				//d += ds2;
				//source->getField(x1, y1, d, &p);   // 26+24*(k-1): (r, r, k) --------------
				handleData(r, r, k); 
				//source->getField(x1, y1, -d, &p);   // 27+24*(k-1): (r, r, -k) -------------
				handleData(r, r, -k); 
				//source->getField(x1, y2, d, &p);   // 28+24*(k-1): (r, -r, k) -------------
				handleData(r, -r, k); 
				//source->getField(x1, y2, -d, &p);   // 29+24*(k-1): (r, -r, -k) -------------
				handleData(r, -r, -k);
				//
				//source->getField(x2, y1, d, &p);   // 30+24*(k-1): (-r, r, k) -----------------
				handleData(-r, r, k); 
				//source->getField(x2, y1, -d, &p);   // 31+24*(k-1): (-r, r, -k) ------------------
				handleData(-r, r, -k); 
				//source->getField(x2, y2, d, &p);   // 32+24*(k-1): (-r, -r, k) ------------------
				handleData(-r, -r, k); 
				//source->getField(x2, y2, -d, &p);   // 33+24*(k-1): (-r, -r, -k) -----------------
				handleData(-r, -r, -k);
				//
				//source->getField(d, y1, z1, &p);   // 34+24*(k-1): (k, r, r) ---------------------
				handleData(k, r, r);
				//source->getField(-d, y1, z1, &p);   // 35+24*(k-1): (-k, r, r) ----------------------------
				handleData(-k, r, r);
				//source->getField(d, y1, z2, &p);   // 36+24*(k-1): (k, r, -r) --------------------
				handleData(k, r, -r);
				//source->getField(-d, y1, z2, &p);   // 37+24*(k-1): (-k, r, -r) ---------------------------
				handleData(-k, r, -r);
				//
				//source->getField(d, y2, z1, &p);   // 38+24*(k-1): (k, -r, r) ---------------------
				handleData(k, -r, r);
				//source->getField(-d, y2, z1, &p);   // 39+24*(k-1): (-k, -r, r) ---------------------------
				handleData(-k, -r, r);
				//source->getField(d, y2, z2, &p);   // 40+24*(k-1): (k, -r, -r) ----------------------
				handleData(k, -r, -r);
				//source->getField(-d, y2, z2, &p);   // 41+24*(k-1): (-k, -r, -r) ---------------------------
				handleData(-k, -r, -r);
				//
				//source->getField(x1, d, z1, &p);   // 42+24*(k-1): (r, k, r) ------------------------
				handleData(r, k, r);
				//source->getField(x1, -d, z1, &p);   // 43+24*(k-1): (r, -k, r) ----------------------
				handleData(r, -k, r);
				//source->getField(x2, d, z1, &p);   // 44+24*(k-1): (-r, k, r) -----------------------
				handleData(-r, k, r);
				//source->getField(x2, -d, z1, &p);   // 45+24*(k-1): (-r, -k, r) ---------------------
				handleData(-r, -k, r);
				//
				//source->getField(x1, d, z2, &p);   // 46+24*(k-1): (r, k, -r) ------------------------
				handleData(r, k, -r);
				//source->getField(x1, -d, z2, &p);   // 47+24*(k-1): (r, -k, -r) ----------------------
				handleData(r, -k, -r);
				//source->getField(x2, d, z2, &p);   // 48+24*(k-1): (-r, k, -r)  ------------------------
				handleData(-r, k, -r);
				//source->getField(x2, -d, z2, &p);   // 49+24*(k-1): (-r, -k, -r) ------------------------
				handleData(-r, -k, -r);
			}
			//last index : 49+24*(r-2) = 24r + 1
			//4 Inner parts of cross-lines on m-plane. One axis at 0 and m=±r:24r+2 to 32r-7,8(r-1) points
			//d = 0;
			for(k=1;k<r;k++)
			{
				if(ret != ERR_OK)
				{
					break;
				}
				//d += ds2;
				//m=+-r, n=0 //////////////////////////////////////////////
				//source->getField(x1, 0, d, &p);   // 24r + 2 + 24 * (k-1): (r, 0, k)    ---------------------
				handleData(r, 0, k);
				//source->getField(x1, 0, -d, &p);   // 24r + 3 + 24 * (k-1): (r, 0, -k)  ---------------------
				handleData(r, 0, -k);
				//source->getField(x2, 0, d, &p);   // 24r + 4 + 24 * (k-1): (-r, 0, k)  ---------------------------
				handleData(-r, 0, k);
				//source->getField(x2, 0, -d, &p);   // 24r + 5 + 24 * (k-1): (-r, 0, -k) ------------------------
				handleData(-r, 0, -k);
				//m=+-r, p=0
				//source->getField(x1, d, 0, &p);   // 24r + 6 + 24 * (k-1): (r, k, 0)    ----------------------
				handleData(r, k, 0);
				//source->getField(x1, -d, 0, &p);   // 24r + 7 + 24 * (k-1): (r, -k, 0)  ---------------------
				handleData(r, -k, 0);
				//source->getField(x2, d, 0, &p);   // 24r + 8 + 24 * (k-1): (-r, k, 0)   -------------------------
				handleData(-r, k, 0);
				//source->getField(x2, -d, 0, &p);   // 24r + 9 + 24 * (k-1): (-r, -k, 0)  ---------------------------
				handleData(-r, -k, 0);
				///////////////////////////////////////
				//n=+-r, m=0 ///////////////////////////////////////////////
				//source->getField(0, y1, d, &p);   // 24r + 10 + 24 * (k-1): (0, r, k)    ---------------------------
				handleData(0, r, k);
				//source->getField(0, y1, -d, &p);   // 24r + 11 + 24 * (k-1): (0, r, -k)   -------------------------
				handleData(0, r, -k);
				//source->getField(0, y2, d, &p);   // 24r + 12 + 24 * (k-1): (0, -r, k)     -----------------------------
				handleData(0, -r, k);
				//source->getField(0, y2, -d, &p);   // 24r + 13 + 24 * (k-1): (0, -r, -k)   ----------------------------
				handleData(0, -r, -k);
				//n=+-r, p=0 
				//source->getField(d, y1, 0, &p);   // 24r + 14 + 24 * (k-1): (k, r, 0)      --------------------------------
				handleData(k, r, 0);
				//source->getField(-d, y1, 0, &p);   // 24r + 15 + 24 * (k-1): (-k, r, 0)     -------------------------------
				handleData(-k, r, 0);
				//source->getField(d, y2, 0, &p);   // 24r + 16 + 24 * (k-1): (k, -r, 0)      --------------------------------
				handleData(k, -r, 0);
				//source->getField(-d, y2, 0, &p);   // 24r + 17 + 24 * (k-1): (-k, -r, 0)    -----------------------------------
				handleData(-k, -r, 0);
				/////////////////////////////////////////////
				//p=+-r, n=0 ///////////////////////////////////////////////
				//source->getField(d, 0, z1, &p);   // 24r + 18 + 24 * (k-1): (k, 0, r)       -----------------------------------
				handleData(k, 0, r);
				//source->getField(-d, 0, z1, &p);   // 24r + 19 + 24 * (k-1): (-k, 0, r)     ------------------------------------
				handleData(-k, 0, r);
				//source->getField(d, 0, z2, &p);   // 24r + 20 + 24 * (k-1): (k, 0, -r)      ------------------------------------
				handleData(k, 0, -r);
				//source->getField(-d, 0, z2, &p);   // 24r + 21 + 24 * (k-1): (-k, 0, -r)     -----------------------------------
				handleData(-k, 0, -r);
				//p=+-r, m=0
				//source->getField(0, d, z1, &p);   // 24r + 22 + 24 * (k-1): (0, k, r)        ----------------------------
				handleData(0, k, r);
				//source->getField(0, -d, z1, &p);   // 24r + 23 + 24 * (k-1): (0, -k, r)      ---------------------------------------
				handleData(0, -k, r);
				//source->getField(0, d, z2, &p);   // 24r + 24 + 24 * (k-1): (0, k, -r)         ----------------------------------
				handleData(0, k, -r);
				//source->getField(0, -d, z2, &p);   // 24r + 25 + 24 * (k-1): (0, -k, -r)       ---------------------------------
				handleData(0, -k, -r);
				//
			}
			//Last index: 24r + 25 + 24 * (k-1) = 24r + 25 + 24 * (r-2) = 48r - 23
			//5. one at edge, the other two not at 0
			//d = 0;
			for(j=1;j<r;j++)
			{
				if(ret != ERR_OK)
				{
					break;
				}
				//d += ds2;
				//dd = 0;
				for(k=1;k<r;k++)
				{
					if(ret != ERR_OK)
					{
						break;
					}
					//dd += ds2;
					//24 points
					//5.1 m=r
					//source->getField(x1, d, dd, &p);   // 48r - 22 + 24*[(k-1) + (r-1)*(j-1)]: (r, j, k)          -----------------------------
					handleData(r, j, k);
					//source->getField(x1, d, -dd, &p);   // 48r - 21 + 24*[(k-1) + (r-1) * (j-1)]: (r, j, -k)      ------------------------------
					handleData(r, j, -k);
					//source->getField(x1, -d, dd, &p);   // 48r - 20 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, k)      -------------------------------
					handleData(r, -j, k);
					//source->getField(x1, -d, -dd, &p);   // 48r - 19 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, -k)   ---------------------------------
					handleData(r, -j, -k);
					//m=-r
					//source->getField(x2, d, dd, &p);   // 48r - 18 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, k)       ----------------------------------
					handleData(-r, j, k);
					//source->getField(x2, d, -dd, &p);   // 48r - 17 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, -k)      ------------------------------------
					handleData(-r, j, -k);
					//source->getField(x2, -d, dd, &p);   // 48r - 16 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, k)       ----------------------------
					handleData(-r, -j, k);
					//source->getField(x2, -d, -dd, &p);   // 48r - 15 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, -k)    ------------------------------
					handleData(-r, -j, -k);
					//5.2 n=r
					//source->getField(d, y1, dd, &p);   // 48r - 14 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, k)     -----------------------------------
					handleData(j, r, k);
					//source->getField(d, y1, -dd, &p);   // 48r - 13 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, -k)    -------------------------------------
					handleData(j, r, -k);
					//source->getField(-d, y1, dd, &p);   // 48r - 12 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, k)     --------------------------------
					handleData(-j, r, k);
					//source->getField(-d, y1, -dd, &p);   // 48r - 11 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, -k)  ----------------------------------
					handleData(-j, r, -k);
					//n=-r
					//source->getField(d, y2, dd, &p);   // 48r - 10 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, k)    ----------------------------------
					handleData(j, -r, k);
					//source->getField(d, y2, -dd, &p);   // 48r - 9 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, -k)     ----------------------------------
					handleData(j, -r, -k);
					//source->getField(-d, y2, dd, &p);   // 48r - 8 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, k)       ------------------------------
					handleData(-j, -r, k);
					//source->getField(-d, y2, -dd, &p);   // 48r - 7 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, -k)     --------------------
					handleData(-j, -r, -k);
					//5.3 p=r
					//source->getField(d, dd, z1, &p);   // 48r - 6 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, r)       ---------------------------
					handleData(j, k, r);
					//source->getField(-d, dd, z1, &p);   // 48r - 5 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, r)        ----------------------------
					handleData(-j, k, r);
					//source->getField(d, -dd, z1, &p);   // 48r - 4 + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, r)       ------------------------
					handleData(j, -k, r);
					//source->getField(-d, -dd, z1, &p);   // 48r - 3 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, r)   ----------------------------
					handleData(-j, -k, r);
					//p=-r
					//source->getField(d, dd, z2, &p);   // 48r - 2 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, -r)       -----------------------------------
					handleData(j, k, -r);
					//source->getField(-d, dd, z2, &p);   // 48r - 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, -r)     ---------------------------------------
					handleData(-j, k, -r);
					//source->getField(d, -dd, z2, &p);   // 48r + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, -r)          -----------------------------------
					handleData(j, -k, -r);
					//source->getField(-d, -dd, z2, &p);   // 48r + 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, -r)  -------------------------------
					handleData(-j, -k, -r);
				}
			}
			//last index: 48r + 1 + 24*[(r-2) + (r-2) * (r-1)] = 48r + 1 + 24r - 48 + 24 (r^2 - 3r + 2) = 24 r^2 + 1
			if(rht == ProcessAndFinish)
			{
				break;
			}
		}//r>0
		if(ret == ERR_OK)
		{
			onFinish();
		}
		return ret;
}

/*
	go through space points radius by radius
	radius = 0, 1, 2, ..., maxR
	for each radius, go through space points belonging to the radius
	for each space point call virtual function handleData
*/
int GoThroughSphereBySpaces::gothroughSphere(int maxR, double ds)
{
	ret = ERR_OK;
	index = 0;
	unsigned maxRadius = (unsigned)maxR;
	double x1,x2,y1,y2,z1,z2;
	double d, dd;
	double radius = 0; //radius in space distance, not in index, radius = r * ds
	unsigned j,k;
	unsigned r;        //radius in index, not in space distance
	RadiusHandleType rht;
	//
	//r = 0
	//0: (0,0,0)
	rht = setRadius(0);
	if(rht == Finish)
	{
		onFinish();
		return ret;
	}
	if(rht != DoNotProcess)
	{
		//source->getField(0, 0, 0, &p);
		handleData(0, 0, 0);
	}
	if(rht == ProcessAndFinish)
	{
		onFinish();
		return ret;
	}
	//
	for(r=1;r<=maxRadius;r++)
	{
		ret = GetLastHandlerError();
		if(ret != ERR_OK)
		{
			break;
		}
		rht = setRadius(r);
		if(rht == DoNotProcess)
		{
			continue;
		}
		else if(rht == Finish)
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
		handleData(x1, y1, z1); 
		//source->getField(x1, y1, z2, &p);     // 1: (r, r, -r)
		handleData(x1, y1, z2); 
		//source->getField(x1, y2, z1, &p);     // 2: (r, -r, r)
		handleData(x1, y2, z1); 
		//source->getField(x1, y2, z2, &p);     // 3: (r, -r, -r)
		handleData(x1, y2, z2); 
		//source->getField(x2, y1, z1, &p);     // 4: (-r, r, r)
		handleData(x2, y1, z1); 
		//source->getField(x2, y1, z2, &p);     // 5: (-r, r, -r)
		handleData(x2, y1, z2); 
		//source->getField(x2, y2, z1, &p);     // 6: (-r, -r, r)
		handleData(x2, y2, z1); 
		//source->getField(x2, y2, z2, &p);     // 7: (-r, -r, -r)
		handleData(x2, y2, z2);
		//
		//pointCount += 8;
		//last index: 7
		//
		//2. 12 Edge Line Centers. Two axes at edges, one axis at 0: 12 points: 8 - 19
		//source->getField(x1, y1, 0, &p);     // 8: (r, r, 0)
		handleData(x1, y1, 0); 
		//source->getField(x1, y2, 0, &p);     // 9: (r, -r, 0)
		handleData(x1, y2, 0); 
		//source->getField(x2, y1, 0, &p);     // 10: (-r, r, 0)
		handleData(x2, y1, 0); 
		//source->getField(x2, y2, 0, &p);     // 11: (-r, -r, 0)
		handleData(x2, y2, 0); 
		//
		//source->getField(0, y1, z1, &p);    // 12: (0, r, r)
		handleData(0, y1, z1);
		//source->getField(0, y1, z2, &p);    // 13: (0, r, -r)
		handleData(0, y1, z2);
		//source->getField(0, y2, z1, &p);    // 14: (0, -r, r)
		handleData(0, y2, z1);
		//source->getField(0, y2, z2, &p);    // 15: (0, -r, -r)
		handleData(0, y2, z2);
		//
		//source->getField(x1, 0, z1, &p);   // 16: (r, 0, r)
		handleData(x1, 0, z1); 
		//source->getField(x1, 0, z2, &p);   // 17: (r, 0, -r)
		handleData(x1, 0, z2);
		//source->getField(x2, 0, z1, &p);   // 18: (-r, 0, r)
		handleData(x2, 0, z1); 
		//source->getField(x2, 0, z2, &p);   // 19: (-r, 0, -r)
		handleData(x2, 0, z2);
		//pointCount += 12;
		//last index : 19
		//
		//3.  plane centers. two axes at 0: 6 points from 24r-4 to 24r+1
		//source->getField(x1, 0, 0, &p);   // 20: (r, 0, 0)
		handleData(x1, 0, 0);
		//source->getField(x2, 0, 0, &p);   // 21: (-r, 0, 0)
		handleData(x2, 0, 0);
		//source->getField(0, y1, 0, &p);   // 22: (0, r, 0)
		handleData(0, y1, 0);
		//source->getField(0, y2, 0, &p);   // 23: (0, -r, 0)
		handleData(0, y2, 0);
		//source->getField(0, 0, z1, &p);   // 24: (0, 0, r)
		handleData(0, 0, z1);
		//source->getField(0, 0, z2, &p);   // 25: (0, 0, -r)
		handleData(0, 0, z2);
		//last index: 25
		d = 0;
		//3. (r>1) 12 Edge Lines exclude center. Each line has 2(r-1)  points. Two axes at edge, one axis not at 0: 24(r-1)  points: 26 to 24r+1
		for(k=1;k<r;k++)
		{
			ret = GetLastHandlerError();
			if(ret != ERR_OK)
			{
				break;
			}
			d += ds;
			//source->getField(x1, y1, d, &p);   // 26+24*(k-1): (r, r, k) --------------
			handleData(x1, y1, d); 
			//source->getField(x1, y1, -d, &p);   // 27+24*(k-1): (r, r, -k) -------------
			handleData(x1, y1, -d); 
			//source->getField(x1, y2, d, &p);   // 28+24*(k-1): (r, -r, k) -------------
			handleData(x1, y2, d); 
			//source->getField(x1, y2, -d, &p);   // 29+24*(k-1): (r, -r, -k) -------------
			handleData(x1, y2, -d);
			//
			//source->getField(x2, y1, d, &p);   // 30+24*(k-1): (-r, r, k) -----------------
			handleData(x2, y1, d); 
			//source->getField(x2, y1, -d, &p);   // 31+24*(k-1): (-r, r, -k) ------------------
			handleData(x2, y1, -d); 
			//source->getField(x2, y2, d, &p);   // 32+24*(k-1): (-r, -r, k) ------------------
			handleData(x2, y2, d); 
			//source->getField(x2, y2, -d, &p);   // 33+24*(k-1): (-r, -r, -k) -----------------
			handleData(x2, y2, -d);
			//
			//source->getField(d, y1, z1, &p);   // 34+24*(k-1): (k, r, r) ---------------------
			handleData(d, y1, z1);
			//source->getField(-d, y1, z1, &p);   // 35+24*(k-1): (-k, r, r) ----------------------------
			handleData(-d, y1, z1);
			//source->getField(d, y1, z2, &p);   // 36+24*(k-1): (k, r, -r) --------------------
			handleData(d, y1, z2);
			//source->getField(-d, y1, z2, &p);   // 37+24*(k-1): (-k, r, -r) ---------------------------
			handleData(-d, y1, z2);
			//
			//source->getField(d, y2, z1, &p);   // 38+24*(k-1): (k, -r, r) ---------------------
			handleData(d, y2, z1);
			//source->getField(-d, y2, z1, &p);   // 39+24*(k-1): (-k, -r, r) ---------------------------
			handleData(-d, y2, z1);
			//source->getField(d, y2, z2, &p);   // 40+24*(k-1): (k, -r, -r) ----------------------
			handleData(d, y2, z2);
			//source->getField(-d, y2, z2, &p);   // 41+24*(k-1): (-k, -r, -r) ---------------------------
			handleData(-d, y2, z2);
			//
			//source->getField(x1, d, z1, &p);   // 42+24*(k-1): (r, k, r) ------------------------
			handleData(x1, d, z1);
			//source->getField(x1, -d, z1, &p);   // 43+24*(k-1): (r, -k, r) ----------------------
			handleData(x1, -d, z1);
			//source->getField(x2, d, z1, &p);   // 44+24*(k-1): (-r, k, r) -----------------------
			handleData(x2, d, z1);
			//source->getField(x2, -d, z1, &p);   // 45+24*(k-1): (-r, -k, r) ---------------------
			handleData(x2, -d, z1);
			//
			//source->getField(x1, d, z2, &p);   // 46+24*(k-1): (r, k, -r) ------------------------
			handleData(x1, d, z2);
			//source->getField(x1, -d, z2, &p);   // 47+24*(k-1): (r, -k, -r) ----------------------
			handleData(x1, -d, z2);
			//source->getField(x2, d, z2, &p);   // 48+24*(k-1): (-r, k, -r)  ------------------------
			handleData(x2, d, z2);
			//source->getField(x2, -d, z2, &p);   // 49+24*(k-1): (-r, -k, -r) ------------------------
			handleData(x2, -d, z2);
		}
		//last index : 49+24*(r-2) = 24r + 1
		//4 Inner parts of cross-lines on m-plane. One axis at 0 and m=±r:24r+2 to 32r-7,8(r-1) points
		d = 0;
		for(k=1;k<r;k++)
		{
			ret = GetLastHandlerError();
			if(ret != ERR_OK)
			{
				break;
			}
			d += ds;
			//m=+-r, n=0 //////////////////////////////////////////////
			//source->getField(x1, 0, d, &p);   // 24r + 2 + 24 * (k-1): (r, 0, k)    ---------------------
			handleData(x1, 0, d);
			//source->getField(x1, 0, -d, &p);   // 24r + 3 + 24 * (k-1): (r, 0, -k)  ---------------------
			handleData(x1, 0, -d);
			//source->getField(x2, 0, d, &p);   // 24r + 4 + 24 * (k-1): (-r, 0, k)  ---------------------------
			handleData(x2, 0, d);
			//source->getField(x2, 0, -d, &p);   // 24r + 5 + 24 * (k-1): (-r, 0, -k) ------------------------
			handleData(x2, 0, -d);
			//m=+-r, p=0
			//source->getField(x1, d, 0, &p);   // 24r + 6 + 24 * (k-1): (r, k, 0)    ----------------------
			handleData(x1, d, 0);
			//source->getField(x1, -d, 0, &p);   // 24r + 7 + 24 * (k-1): (r, -k, 0)  ---------------------
			handleData(x1, -d, 0);
			//source->getField(x2, d, 0, &p);   // 24r + 8 + 24 * (k-1): (-r, k, 0)   -------------------------
			handleData(x2, d, 0);
			//source->getField(x2, -d, 0, &p);   // 24r + 9 + 24 * (k-1): (-r, -k, 0)  ---------------------------
			handleData(x2, -d, 0);
			///////////////////////////////////////
			//n=+-r, m=0 ///////////////////////////////////////////////
			//source->getField(0, y1, d, &p);   // 24r + 10 + 24 * (k-1): (0, r, k)    ---------------------------
			handleData(0, y1, d);
			//source->getField(0, y1, -d, &p);   // 24r + 11 + 24 * (k-1): (0, r, -k)   -------------------------
			handleData(0, y1, -d);
			//source->getField(0, y2, d, &p);   // 24r + 12 + 24 * (k-1): (0, -r, k)     -----------------------------
			handleData(0, y2, d);
			//source->getField(0, y2, -d, &p);   // 24r + 13 + 24 * (k-1): (0, -r, -k)   ----------------------------
			handleData(0, y2, -d);
			//n=+-r, p=0 
			//source->getField(d, y1, 0, &p);   // 24r + 14 + 24 * (k-1): (k, r, 0)      --------------------------------
			handleData(d, y1, 0);
			//source->getField(-d, y1, 0, &p);   // 24r + 15 + 24 * (k-1): (-k, r, 0)     -------------------------------
			handleData(-d, y1, 0);
			//source->getField(d, y2, 0, &p);   // 24r + 16 + 24 * (k-1): (k, -r, 0)      --------------------------------
			handleData(d, y2, 0);
			//source->getField(-d, y2, 0, &p);   // 24r + 17 + 24 * (k-1): (-k, -r, 0)    -----------------------------------
			handleData(-d, y2, 0);
			/////////////////////////////////////////////
			//p=+-r, n=0 ///////////////////////////////////////////////
			//source->getField(d, 0, z1, &p);   // 24r + 18 + 24 * (k-1): (k, 0, r)       -----------------------------------
			handleData(d, 0, z1);
			//source->getField(-d, 0, z1, &p);   // 24r + 19 + 24 * (k-1): (-k, 0, r)     ------------------------------------
			handleData(-d, 0, z1);
			//source->getField(d, 0, z2, &p);   // 24r + 20 + 24 * (k-1): (k, 0, -r)      ------------------------------------
			handleData(d, 0, z2);
			//source->getField(-d, 0, z2, &p);   // 24r + 21 + 24 * (k-1): (-k, 0, -r)     -----------------------------------
			handleData(-d, 0, z2);
			//p=+-r, m=0
			//source->getField(0, d, z1, &p);   // 24r + 22 + 24 * (k-1): (0, k, r)        ----------------------------
			handleData(0, d, z1);
			//source->getField(0, -d, z1, &p);   // 24r + 23 + 24 * (k-1): (0, -k, r)      ---------------------------------------
			handleData(0, -d, z1);
			//source->getField(0, d, z2, &p);   // 24r + 24 + 24 * (k-1): (0, k, -r)         ----------------------------------
			handleData(0, d, z2);
			//source->getField(0, -d, z2, &p);   // 24r + 25 + 24 * (k-1): (0, -k, -r)       ---------------------------------
			handleData(0, -d, z2);
			//
		}
		//Last index: 24r + 25 + 24 * (k-1) = 24r + 25 + 24 * (r-2) = 48r - 23
		//5. one at edge, the other two not at 0
		d = 0;
		for(j=1;j<r;j++)
		{
			ret = GetLastHandlerError();
			if(ret != ERR_OK)
			{
				break;
			}
			d += ds;
			dd = 0;
			for(k=1;k<r;k++)
			{
				ret = GetLastHandlerError();
				if(ret != ERR_OK)
				{
					break;
				}
				dd += ds;
				//24 points
				//5.1 m=r
				//source->getField(x1, d, dd, &p);   // 48r - 22 + 24*[(k-1) + (r-1)*(j-1)]: (r, j, k)          -----------------------------
				handleData(x1, d, dd);
				//source->getField(x1, d, -dd, &p);   // 48r - 21 + 24*[(k-1) + (r-1) * (j-1)]: (r, j, -k)      ------------------------------
				handleData(x1, d, -dd);
				//source->getField(x1, -d, dd, &p);   // 48r - 20 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, k)      -------------------------------
				handleData(x1, -d, dd);
				//source->getField(x1, -d, -dd, &p);   // 48r - 19 + 24*[(k-1) + (r-1) * (j-1)]: (r, -j, -k)   ---------------------------------
				handleData(x1, -d, -dd);
				//m=-r
				//source->getField(x2, d, dd, &p);   // 48r - 18 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, k)       ----------------------------------
				handleData(x2, d, dd);
				//source->getField(x2, d, -dd, &p);   // 48r - 17 + 24*[(k-1) + (r-1) * (j-1)]: (-r, j, -k)      ------------------------------------
				handleData(x2, d, -dd);
				//source->getField(x2, -d, dd, &p);   // 48r - 16 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, k)       ----------------------------
				handleData(x2, -d, dd);
				//source->getField(x2, -d, -dd, &p);   // 48r - 15 + 24*[(k-1) + (r-1) * (j-1)]: (-r, -j, -k)    ------------------------------
				handleData(x2, -d, -dd);
				//5.2 n=r
				//source->getField(d, y1, dd, &p);   // 48r - 14 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, k)     -----------------------------------
				handleData(d, y1, dd);
				//source->getField(d, y1, -dd, &p);   // 48r - 13 + 24*[(k-1) + (r-1) * (j-1)]: (j, r, -k)    -------------------------------------
				handleData(d, y1, -dd);
				//source->getField(-d, y1, dd, &p);   // 48r - 12 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, k)     --------------------------------
				handleData(-d, y1, dd);
				//source->getField(-d, y1, -dd, &p);   // 48r - 11 + 24*[(k-1) + (r-1) * (j-1)]: (-j, r, -k)  ----------------------------------
				handleData(-d, y1, -dd);
				//n=-r
				//source->getField(d, y2, dd, &p);   // 48r - 10 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, k)    ----------------------------------
				handleData(d, y2, dd);
				//source->getField(d, y2, -dd, &p);   // 48r - 9 + 24*[(k-1) + (r-1) * (j-1)]: (j, -r, -k)     ----------------------------------
				handleData(d, y2, -dd);
				//source->getField(-d, y2, dd, &p);   // 48r - 8 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, k)       ------------------------------
				handleData(-d, y2, dd);
				//source->getField(-d, y2, -dd, &p);   // 48r - 7 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -r, -k)     --------------------
				handleData(-d, y2, -dd);
				//5.3 p=r
				//source->getField(d, dd, z1, &p);   // 48r - 6 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, r)       ---------------------------
				handleData(d, dd, z1);
				//source->getField(-d, dd, z1, &p);   // 48r - 5 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, r)        ----------------------------
				handleData(-d, dd, z1);
				//source->getField(d, -dd, z1, &p);   // 48r - 4 + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, r)       ------------------------
				handleData(d, -dd, z1);
				//source->getField(-d, -dd, z1, &p);   // 48r - 3 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, r)   ----------------------------
				handleData(-d, -dd, z1);
				//p=-r
				//source->getField(d, dd, z2, &p);   // 48r - 2 + 24*[(k-1) + (r-1) * (j-1)]: (j, k, -r)       -----------------------------------
				handleData(d, dd, z2);
				//source->getField(-d, dd, z2, &p);   // 48r - 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, k, -r)     ---------------------------------------
				handleData(-d, dd, z2);
				//source->getField(d, -dd, z2, &p);   // 48r + 24*[(k-1) + (r-1) * (j-1)]: (j, -k, -r)          -----------------------------------
				handleData(d, -dd, z2);
				//source->getField(-d, -dd, z2, &p);   // 48r + 1 + 24*[(k-1) + (r-1) * (j-1)]: (-j, -k, -r)  -------------------------------
				handleData(-d, -dd, z2);
			}
		}
		//last index: 48r + 1 + 24*[(r-2) + (r-2) * (r-1)] = 48r + 1 + 24r - 48 + 24 (r^2 - 3r + 2) = 24 r^2 + 1
		if(rht == ProcessAndFinish)
		{
			break;
		}
	}//r>0
	if(ret == ERR_OK)
	{
		onFinish();
	}
	return ret;
}

/////////RadiusIndexToSeriesIndex////////////////////////////////////////////////////////////////////////////
/*
	when the maximum radius is known, (m,n,p)->series index can be put into a 3D array for quick access
*/

RadiusIndexToSeriesIndex::RadiusIndexToSeriesIndex(void)
{
	ret = ERR_OK; 
	index = 0; 
	maxRadius = 0; 
	r = 0; 
	seriesIndex = NULL;
}
RadiusIndexToSeriesIndex::~RadiusIndexToSeriesIndex()
{
	cleanup();
}
void RadiusIndexToSeriesIndex::cleanup()
{
	if(seriesIndex != NULL)
	{
		Free3DIntegerArray(seriesIndex, 2*maxRadius + 1);
		seriesIndex = NULL;
	}
}

/*
	allocate memory and hold (m,n,p) to series index mapping
*/
int RadiusIndexToSeriesIndex::initialize(int maxR)
{
	if(maxRadius != maxR)
	{
		if(seriesIndex != NULL)
		{
			Free3DIntegerArray(seriesIndex, 2*maxRadius + 1);
			seriesIndex = NULL;
		}
	}
	if(seriesIndex == NULL)
	{
		maxRadius = maxR;
		seriesIndex = Allocate3DIntegerArray(2*maxR+1);
		if(seriesIndex == NULL)
		{
			ret = ERR_OUTOFMEMORY;
		}
		else
		{
			//handleData(m,n,p) will be called for each point
			index = 0;
			ret = gothroughSphere(maxR);
		}
	}
	return ret;
}
//#define DEBUGRADIUS
/*
	remember series index for each combination of (mn,n,p)
*/
void RadiusIndexToSeriesIndex::handleData(int m, int n, int p)
{
#ifdef DEBUGRADIUS
	size_t idx = IndexesToIndex(r, m, n, p, &ret);
	if (ret == ERR_OK)
	{
		if (index != idx)
		{
			ret = ERR_INVALID_RADIUS_INDEX;
		}
	}
	if (ret != ERR_OK)
	{
		ret = ret; //break here
	}
#endif
	seriesIndex[CUBICINDEX(m)][CUBICINDEX(n)][CUBICINDEX(p)] = index;
	index ++;
}

/*
	convert radius indexing (m,n,p) to series indexing for accessing field data
*/
size_t RadiusIndexToSeriesIndex::Index(int m, int n, int p)
{
	return seriesIndex[CUBICINDEX(m)][CUBICINDEX(n)][CUBICINDEX(p)];
}

/*
	convert cubic indexing (i,j,k) to series indexing for accessing field data
*/
size_t RadiusIndexToSeriesIndex::CubicIndex(int i, int j, int k)
{
	return seriesIndex[i][j][k];
}
/////////////////////////////////////
