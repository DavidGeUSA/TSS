#pragma once
/*******************************************************************
Author: David Ge (dge893@gmail.com, aka Wei Ge)
Last modified: 11/21/2020
Allrights reserved by David Ge

create plugin instance by class name
********************************************************************/
#include "../Tss/Simulator.h"
#include "../Tss/FieldsSetter.h"
#include "../Tss/FieldSourceTss.h"
#include "../Tss/BoundaryTss.h"

Simulator* CreateSimulatorInstance(const char *className);
FieldsSetter* CreateFieldsSetterInstance(const char *className);
FieldSourceTss* CreateSourceInstance(const char *className);
BoundaryTss* CreateBoundaryInstance(const char *className);
DivergenceStatistic* CreateDivgStatistics(const char *className);