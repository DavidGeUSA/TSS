#pragma once

#include "../EMField/FDTD.h"
#include "TssFDTD.h"
#include "YeeFDTD.h"
#include "YeeFDTDSpaceSynched.h"

FDTD *CreateRadiusFDTDInstance(char *name);
