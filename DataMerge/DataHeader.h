#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <io.h>

#define MPH2MPS			0.44704

#define GCQLogFile		"FSXLogFile"
#define GCQCsvFile		"Report"
#define TM4File			"TM"
#define TM4File1		"Trackman"
#define WaveFile		"ShotRes"
#define MaxFile			"MAX"
#define NXFile			"NXShotData"
#define GRadarFile		"ShotRes"

using namespace std;

typedef enum
{
	SENSOR_WAVE,
	SENSOR_GCQ,
	SENSOR_TM,
	SENSOR_MAX,
	SENSOR_GRADAR,
	SENSOR_NX,
	SENSOR_TOTALCNT
} enSensors;


struct stShotInfo
{
	string snName;
	vector<string> item;
	vector<vector<string>> dataVec;
};