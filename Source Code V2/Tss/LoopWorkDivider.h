#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 07/15/2020
Allrights reserved by David Ge

divide a for(...) loop into multiple parts to be executed by multiple threads

Modifications
Date            Author      Description
---------------------------------------------
2021-03-06      David Ge    Linux compatibility
********************************************************************/

#ifdef __linux__ 
#include <cstddef>
#endif
#ifdef _WIN32
#define NULL 0
#endif

typedef struct LoopWorkStruct{
	unsigned int StartIndex;
	unsigned int EndIndex;
	size_t StartOneDimIndex;
}LoopWorkStruct;

/*
	divide a loop into N parts for multi-threads.
	loop from 0 to nx
	dnx = int(nx/N)
	1st loop work: 0,1,...,dnx-1
	2nd loop work: dnx+0,dnx+1,...,2dnx-1
	3rd loop work: 2dnx+0,2dnx+1,...,3dnx-1
	...
	Nth loop work: (N-1)dnx+0,(N-1)dnx+1,...,nx

	n-th loop work, n=1,2,...,N:
		starting index = (n-1)*ndx
		ending index   = if(n<N) n*ndx-1; if(n==N) nx
		starting one-Dimension index = (nz+1)*(ny+1)*(starting index)
*/
class LoopWorkDivider
{
private:
	unsigned int nx;
	unsigned int ny;
	unsigned int nz;
	unsigned int N;
	LoopWorkStruct *works;
public:
	LoopWorkDivider();
	~LoopWorkDivider();
	void initializeDivider(unsigned int nx0,unsigned int ny0, unsigned int nz0, unsigned int n);
	unsigned int Count(){ return N; }
	unsigned int StartIndex(unsigned int i){ return works[i].StartIndex; }
	unsigned int EndIndex(unsigned int i){ return works[i].EndIndex; }
	size_t StartIndexOneDim(unsigned int i){ return works[i].StartOneDimIndex; }
};

