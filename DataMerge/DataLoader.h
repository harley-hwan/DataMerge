#pragma once
#include "DataHeader.h"



class DataLoader
{
public:
	void loadIncludePath(string path);			// �� �������� ���� ���� �����ϰ� �巡�׾� ����� �� ����
	void loadIncludePath_v1(string path);		// �� ������ ���� �ѱ��� ��Ƴ��� ���� ���� �ε��� �� ����
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

