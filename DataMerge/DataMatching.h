#pragma once
#include "DataHeader.h"


class DataMatching
{
public:
	void setDataSet(stShotInfo inputData[SENSOR_TOTALCNT]);
	void setMaxDiffTime(int val);
	void excute();


private:
	void matchingWithTime();
	void writeCSV(stShotInfo inputData[SENSOR_TOTALCNT]);
	vector<string> split(string inputStr, string delimiter);


private:
	int m_maxDiffSec;
	stShotInfo m_dataSet[SENSOR_TOTALCNT];


};

