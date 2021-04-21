#!/bin/bash

echo "run short simulations and compare results with files in ./Regression"

./EM /W./tmp /D./_data /T./taskFiles/UnitTest.task /K301

echo "no boundary"
./EM /W./tmp /D./_data /K300 /T./taskFiles/ricker21_nx120_NB.task

echo "PEMC boundary"
./EM /W./tmp /D./_data /K300 /T./taskFiles/ricker21_nx120_PEMC.task

echo "PML absorbing layer Ln=4, mag=0.01"
./EM /W./tmp /D./_data /K300 /T./taskFiles/r21_84_NB_PML.task

echo "PML absorbing layer Ln=8, mag=0.01"
./EM /W./tmp /D./_data /K300 /T./taskFiles/r21_84_NB_PML8.task

echo "PML absorbing layer Ln=12, mag=0.01"
./EM /W./tmp /D./_data /K300 /T./taskFiles/r21_84_NB_PML12.task

echo "Yee algorithm"
./EM /W./tmp /D./_data /K300 /T./taskFiles/task_gaussianYEE240.task

echo "single thread TSS"
./EM /W./tmp /D./_data /K300 /T./taskFiles/task_gaussianTSS120.task


