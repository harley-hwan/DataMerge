#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

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

// 시간 처리를 위한 구조체
struct TimeInfo {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    std::string toString() const;
    time_t toTimeT() const;
    static TimeInfo fromString(const std::string& dateTime);
};

// 전역 헬퍼 함수
namespace DataHelper {
    // YYYYMMDD_HHMMSS 형식의 문자열을 time_t로 변환
    time_t parseDateTime(const std::string& dateTime);

    // 문자열 앞뒤 공백 제거
    std::string trim(const std::string& str);
}