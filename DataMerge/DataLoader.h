#pragma once
#include "DataHeader.h"



class DataLoader
{
public:
	void loadIncludePath(string path);			// 각 센서마다 폴더 각각 존재하고 드래그앤 드롭할 때 유용
	void loadIncludePath_v1(string path);		// 각 센서의 파일 한군데 모아놓고 파일 각각 로드할 때 유용
	void excute(stShotInfo* outputData[(int)SENSOR_TOTALCNT]);

private:
	void readGCQFile(string csvFilePath, string logFilePath);
	void readTrackManFile(string csvFilePath);
	void readWaveFile(string csvFilePath);
	void readMaxFile(string csvFilePath);
	void readNXFile(string csvFilePath);
	void readGRadarFile(string csvFilePath);
	void addGCQTime(stShotInfo inputData);
	void addTrackManTime();
	void addMaxTime();
	void addWaveTime();
	void addNXTime();
	void addGRadarTime();
	vector<string> split(string inputStr, string delimiter);




private:
	stShotInfo m_dataSet[SENSOR_TOTALCNT];
	stShotInfo test[SENSOR_TOTALCNT];

};

