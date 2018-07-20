/*******************************************************************
	Author: Bob Limnor (bob@limnor.com, aka Wei Ge)
	Last modified: 03/31/2018
	Allrights reserved by Bob Limnor

********************************************************************/

#include "PopulateFields.h"

PopulateYeeFieldsTime0::PopulateYeeFieldsTime0(FieldsInitializer* source, FieldPoint3D *field, double yeeCellSize)
{
	datasource = source;
	_field = field;
	ds2 = yeeCellSize / 2.0; //space shift amount
	dx = dy = dz = 0.0;
	index = 0;
	ret = ERR_OK;
}
void PopulateYeeFieldsTime0::setShifted(double shiftX, double shiftY, double shiftZ)
{
	dx = shiftX;
	dy = shiftY;
	dz = shiftZ;
}
void PopulateYeeFieldsTime0::handleData(double x, double y, double z)
{
	if(datasource != NULL)
	{
		//apply shifts
		x += dx; y += dy; z += dz;
		//get E field
		_field[index].E.x = datasource->funcE0x(x + ds2, y,       z      );
		_field[index].E.y = datasource->funcE0y(x,       y + ds2, z      );
		_field[index].E.z = datasource->funcE0z(x,       y,       z + ds2);
		//get H field
		_field[index].H.x = datasource->funcB0x(x,       y + ds2, z + ds2);
		_field[index].H.y = datasource->funcB0y(x + ds2, y      , z + ds2);
		_field[index].H.z = datasource->funcB0z(x + ds2, y + ds2, z      );
	}
	else
	{
		_field[index].E.x = _field[index].E.y = _field[index].E.z = _field[index].H.x = _field[index].H.y = _field[index].H.z = 0.0;
	}
	index++;
}

