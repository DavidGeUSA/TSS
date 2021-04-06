
# Electromagnetic Fields Analysis. Version 2.0.0. 
# Created: 2017-02-08 for Microsoft Windows; updated 2021-03-11 to support Linux
# Author: David Ge (dge893@gmail.com)
#
# modification history
# Date          Author         Description
#-------------------------------------------------------------------------------------------
# 2021-03-11    David Ge       created this makefile to add Linux support.
#                              Ubuntu 20.04.2 LTS, release 20.04, Codename: focal
#-------------------------------------------------------------------------------------------

CC = g++

CFLAGS = -Wall

EMDIRS = -IFileUtil -IProcessMonitor -IOutputUtil -IMemoryMan -IMathTools -IEMField -ITssInSphere -IboostLib -ITss -IFieldDataComparer -IFieldProvider0 -IBC0 -ISRC0 -Iradius0 -ITFSF0 -ITssTest -IEMconsole

EMLibs =  -lTssTest -lTFSF0 -lradius0 -lSRC0 -lBC0 -lFieldProvider0 -lFieldDataComparer -lTss -lboostLib -lTssInSphere -lEMField -lMathTools -lMemoryMan -lOutputUtil -lProcessMonitor -lFileUtil

all : FileUtillib ProcessMonitorlib OutputUtillib MemoryManlib MathToolslib EMFieldlib TssInSpherelib boostLiblib Tsslib FieldDataComparerlib FieldProvider0lib BC0lib SRC0lib radius0lib TFSF0lib TssTestlib EMexe

FileUtillib:  
	$(CC) $(CFLAGS) -c -o lib/fileutil.o FileUtil/fileutil.cpp
	$(CC) $(CFLAGS) -c -o lib/taskFile.o FileUtil/taskFile.cpp
	ar cr lib/libFileUtil.a lib/fileutil.o lib/taskFile.o
	ranlib lib/libFileUtil.a

ProcessMonitorlib:
	$(CC) $(CFLAGS) -c -o lib/ProcessMonitor.o ProcessMonitor/ProcessMonitor.cpp
	ar cr lib/libProcessMonitor.a lib/ProcessMonitor.o
	ranlib lib/libProcessMonitor.a 

OutputUtillib:
	$(CC) $(CFLAGS) -c -o lib/OutputUtil.o OutputUtil/OutputUtility.cpp
	ar cr lib/libOutputUtil.a lib/OutputUtil.o
	ranlib lib/libOutputUtil.a

MemoryManlib:
	$(CC) $(CFLAGS) -c -o lib/MemoryMan.o MemoryMan/MemoryManager.cpp
	ar cr lib/libMemoryMan.a lib/MemoryMan.o
	ranlib lib/libMemoryMan.a

MathToolslib:
	$(CC) $(CFLAGS) -c -o lib/Matrix.o MathTools/Matrix.cpp
	$(CC) $(CFLAGS) -c -o lib/Triangle.o MathTools/Triangle.cpp
	$(CC) $(CFLAGS) -c -o lib/randomTools.o MathTools/randomTools.cpp
	ar cr lib/libMathTools.a lib/Matrix.o lib/Triangle.o lib/randomTools.o
	ranlib lib/libMathTools.a

EMFieldlib:
	$(CC) $(CFLAGS) -c -o lib/BoundaryCondition.o EMField/BoundaryCondition.cpp
	$(CC) $(CFLAGS) -c -o lib/FDTD.o EMField/FDTD.cpp
	$(CC) $(CFLAGS) -c -o lib/FdtdMemory.o EMField/FdtdMemory.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldSource.o EMField/FieldSource.cpp
	$(CC) $(CFLAGS) -c -o lib/Plugin.o EMField/Plugin.cpp
	$(CC) $(CFLAGS) -c -o lib/RadiusIndex.o EMField/RadiusIndex.cpp
	$(CC) $(CFLAGS) -c -o lib/TotalFieldScatteredFieldBoundary.o EMField/TotalFieldScatteredFieldBoundary.cpp
	ar cr lib/libEMField.a lib/BoundaryCondition.o lib/FDTD.o lib/FdtdMemory.o lib/FieldSource.o lib/Plugin.o lib/RadiusIndex.o lib/TotalFieldScatteredFieldBoundary.o
	ranlib lib/libEMField.a

TssInSpherelib:
	$(CC) $(CFLAGS) -c -o lib/ApplyCurls.o TssInSphere/ApplyCurls.cpp
	$(CC) $(CFLAGS) -c -o lib/ApplyCurlsEvenInhomogeneous.o TssInSphere/ApplyCurlsEvenInhomogeneous.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlEstimatorAsymmetric.o TssInSphere/CurlEstimatorAsymmetric.cpp
	$(CC) $(CFLAGS) -c -o lib/DerivativeEstimator.o TssInSphere/DerivativeEstimator.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldAnalysor.o TssInSphere/FieldAnalysor.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldSourceSphereCurrent.o TssInSphere/FieldSourceSphereCurrent.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldStatistics.o TssInSphere/FieldStatistics.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldStatisticsByDivergence.o TssInSphere/FieldStatisticsByDivergence.cpp
	$(CC) $(CFLAGS) -c -o lib/FourthSpaceOrderTss.o TssInSphere/FourthSpaceOrderTss.cpp
	$(CC) $(CFLAGS) -c -o lib/TssInhomogeneous.o TssInSphere/TssInhomogeneous.cpp
	$(CC) $(CFLAGS) -c -o lib/TssInSphere.o TssInSphere/TssInSphere.cpp
	ar cr lib/libTssInSphere.a lib/ApplyCurls.o lib/ApplyCurlsEvenInhomogeneous.o lib/CurlEstimatorAsymmetric.o lib/DerivativeEstimator.o lib/FieldAnalysor.o lib/FieldSourceSphereCurrent.o lib/FieldStatistics.o lib/FieldStatisticsByDivergence.o lib/FourthSpaceOrderTss.o lib/TssInhomogeneous.o lib/TssInSphere.o
	ranlib lib/libTssInSphere.a

boostLiblib:
	$(CC) $(CFLAGS) -c -o lib/HiMatrix.o boostLib/HiMatrix.cpp
	ar cr lib/libboostLib.a lib/HiMatrix.o
	ranlib lib/libboostLib.a

Tsslib:
	$(CC) $(CFLAGS) -c -o lib/BoundaryTss.o Tss/BoundaryTss.cpp
	$(CC) $(CFLAGS) -c -o lib/Curl.o Tss/Curl.cpp
	$(CC) $(CFLAGS) -c -o lib/Curl_x.o Tss/Curl_x.cpp
	$(CC) $(CFLAGS) -c -o lib/Curl_y.o Tss/Curl_y.cpp
	$(CC) $(CFLAGS) -c -o lib/Curl_z.o Tss/Curl_z.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlOne.o Tss/CurlOne.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlThreeA.o Tss/CurlThreeA.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlThreeB.o Tss/CurlThreeB.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlTwo.o Tss/CurlTwo.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlTwo_0.o Tss/CurlTwo_0.cpp
	$(CC) $(CFLAGS) -c -o lib/DivergenceStatistic.o Tss/DivergenceStatistic.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldSourceTss.o Tss/FieldSourceTss.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldsSetter.o Tss/FieldsSetter.cpp
	$(CC) $(CFLAGS) -c -o lib/LoopWorkDivider.o Tss/LoopWorkDivider.cpp
	$(CC) $(CFLAGS) -c -o lib/PmlRange.o Tss/PmlRange.cpp
	$(CC) $(CFLAGS) -c -o lib/PmlTss.o Tss/PmlTss.cpp
	$(CC) $(CFLAGS) -c -o lib/Space.o Tss/Space.cpp
	$(CC) $(CFLAGS) -c -o lib/ThreadPool.o Tss/ThreadPool.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTss.o Tss/TimeTss.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTss2Threads.o Tss/TimeTss2Threads.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTss4th.o Tss/TimeTss4th.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTssBase.o Tss/TimeTssBase.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTssMultiThreads.o Tss/TimeTssMultiThreads.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeYee.o Tss/TimeYee.cpp
	$(CC) $(CFLAGS) -c -o lib/Tss.o Tss/Tss.cpp
	$(CC) $(CFLAGS) -c -o lib/VirtualField.o Tss/VirtualField.cpp
	$(CC) $(CFLAGS) -c -o lib/TimeTssRotateSymmetryZ.o Tss/TimeTssRotateSymmetryZ.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlOneSymmetryZ.o Tss/CurlOneSymmetryZ.cpp
	$(CC) $(CFLAGS) -c -o lib/RotateSymmetryField.o Tss/RotateSymmetryField.cpp
	$(CC) $(CFLAGS) -c -o lib/Simulator.o Tss/Simulator.cpp
	ar cr lib/libTss.a lib/BoundaryTss.o lib/Curl.o lib/Curl_x.o  lib/Curl_y.o lib/Curl_z.o lib/CurlOne.o lib/CurlThreeA.o lib/CurlThreeB.o lib/CurlTwo.o lib/CurlTwo_0.o lib/DivergenceStatistic.o lib/FieldSourceTss.o lib/FieldsSetter.o lib/LoopWorkDivider.o lib/PmlRange.o lib/PmlTss.o lib/Space.o lib/ThreadPool.o lib/TimeTss.o lib/TimeTss2Threads.o lib/TimeTss4th.o lib/TimeTssBase.o lib/TimeTssMultiThreads.o lib/TimeYee.o lib/Tss.o lib/VirtualField.o lib/CurlOneSymmetryZ.o lib/RotateSymmetryField.o lib/TimeTssRotateSymmetryZ.o lib/Simulator.o
	ranlib lib/libTss.a

FieldDataComparerlib:
	$(CC) $(CFLAGS) -c -o lib/DivergenceComparer.o FieldDataComparer/DivergenceComparer.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldDataComparer.o FieldDataComparer/FieldDataComparer.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldDivergenceComparer.o FieldDataComparer/FieldDivergenceComparer.cpp
	ar cr lib/libFieldDataComparer.a lib/DivergenceComparer.o lib/FieldDataComparer.o lib/FieldDivergenceComparer.o
	ranlib lib/libFieldDataComparer.a

FieldProvider0lib:
	$(CC) $(CFLAGS) -c -o lib/ExportField0.o FieldProvider0/ExportField0.cpp
	$(CC) $(CFLAGS) -c -o lib/Gaussian.o FieldProvider0/Gaussian.cpp
	$(CC) $(CFLAGS) -c -o lib/ZeroFields.o FieldProvider0/ZeroFields.cpp
	ar cr lib/libFieldProvider0.a lib/ExportField0.o lib/Gaussian.o lib/ZeroFields.o
	ranlib lib/libFieldProvider0.a

BC0lib:
	$(CC) $(CFLAGS) -c -o lib/AbcFirstOrder.o BC0/AbcFirstOrder.cpp
	$(CC) $(CFLAGS) -c -o lib/ExportABC.o BC0/ExportABC.cpp
	$(CC) $(CFLAGS) -c -o lib/PemcTss.o BC0/PemcTss.cpp
	$(CC) $(CFLAGS) -c -o lib/VoidCondition.o BC0/VoidCondition.cpp
	ar cr lib/libBC0.a lib/AbcFirstOrder.o lib/ExportABC.o lib/PemcTss.o lib/VoidCondition.o
	ranlib lib/libBC0.a

SRC0lib:
	$(CC) $(CFLAGS) -c -o lib/DipoleSource.o SRC0/DipoleSource.cpp
	$(CC) $(CFLAGS) -c -o lib/ExportFieldSource.o SRC0/ExportFieldSource.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldSourceEz.o SRC0/FieldSourceEz.cpp
	$(CC) $(CFLAGS) -c -o lib/SourceX.o SRC0/SourceX.cpp
	ar cr lib/libSRC0.a lib/DipoleSource.o lib/ExportFieldSource.o lib/FieldSourceEz.o lib/SourceX.o
	ranlib lib/libSRC0.a

radius0lib:
	$(CC) $(CFLAGS) -c -o lib/ExportTssFDTD.o radius0/ExportTssFDTD.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldUpdator.o radius0/FieldUpdator.cpp
	$(CC) $(CFLAGS) -c -o lib/PopulateFields.o radius0/PopulateFields.cpp
	$(CC) $(CFLAGS) -c -o lib/TssFDTD.o radius0/TssFDTD.cpp
	$(CC) $(CFLAGS) -c -o lib/YeeFDTD.o radius0/YeeFDTD.cpp
	$(CC) $(CFLAGS) -c -o lib/YeeFDTDSpaceSynched.o radius0/YeeFDTDSpaceSynched.cpp
	ar cr lib/libradius0.a lib/ExportTssFDTD.o lib/FieldUpdator.o lib/PopulateFields.o lib/TssFDTD.o lib/YeeFDTD.o lib/YeeFDTDSpaceSynched.o
	ranlib lib/libradius0.a

TFSF0lib:
	$(CC) $(CFLAGS) -c -o lib/ExportTFSF.o TFSF0/ExportTFSF.cpp
	$(CC) $(CFLAGS) -c -o lib/TfsfEz.o TFSF0/TfsfEz.cpp
	ar cr lib/libTFSF0.a lib/ExportTFSF.o lib/TfsfEz.o
	ranlib lib/libTFSF0.a

TssTestlib:
	$(CC) $(CFLAGS) -c -o lib/BoundaryTssVoid.o TssTest/BoundaryTssVoid.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlXYZ.o TssTest/CurlXYZ.cpp
	$(CC) $(CFLAGS) -c -o lib/CurlXYZ2.o TssTest/CurlXYZ2.cpp
	$(CC) $(CFLAGS) -c -o lib/FieldsNull.o TssTest/FieldsNull.cpp
	$(CC) $(CFLAGS) -c -o lib/GaussianCurl.o TssTest/GaussianCurl.cpp
	$(CC) $(CFLAGS) -c -o lib/GaussianCurlYee.o TssTest/GaussianCurlYee.cpp
	$(CC) $(CFLAGS) -c -o lib/GaussianTest.o TssTest/GaussianTest.cpp
	$(CC) $(CFLAGS) -c -o lib/HeidlerCurrent.o TssTest/HeidlerCurrent.cpp
	$(CC) $(CFLAGS) -c -o lib/PluginInstances.o TssTest/PluginInstances.cpp
	$(CC) $(CFLAGS) -c -o lib/RadiusCubic.o TssTest/RadiusCubic.cpp
	$(CC) $(CFLAGS) -c -o lib/RadiusStatistics.o TssTest/RadiusStatistics.cpp
	$(CC) $(CFLAGS) -c -o lib/RickerSource.o TssTest/RickerSource.cpp
	$(CC) $(CFLAGS) -c -o lib/TssTest.o TssTest/TssTest.cpp
	ar cr lib/libTssTest.a lib/BoundaryTssVoid.o lib/CurlXYZ.o lib/CurlXYZ2.o lib/FieldsNull.o lib/GaussianCurl.o lib/GaussianCurlYee.o lib/GaussianTest.o lib/HeidlerCurrent.o lib/PluginInstances.o lib/RadiusCubic.o lib/RadiusStatistics.o lib/RickerSource.o lib/TssTest.o
	ranlib lib/libTssTest.a

EMexe:
	$(CC) $(CFLAGS) -c -o lib/FieldSimulation.o EMconsole/FieldSimulation.cpp
	$(CC) $(CFLAGS) -c -o lib/taskClasses.o EMconsole/taskClasses.cpp
	$(CC) $(CFLAGS) -c -o lib/tasks.o EMconsole/tasks.cpp
	$(CC) $(CFLAGS) -c -o lib/simConsole.o EMconsole/simConsole.cpp
	$(CC) $(EMDIRS) -pthread -L./lib -o EM lib/simConsole.o lib/FieldSimulation.o lib/taskClasses.o lib/tasks.o $(EMLibs)

clean:
	rm -rf lib/*.o lib/*.a lib/*.~ ./EM

