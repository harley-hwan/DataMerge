#pragma once
#include "DataHeader.h"
#include <stdexcept>

// 예외 클래스 정의
class DataLoadException : public std::runtime_error {
public:
    DataLoadException(const std::string& msg) : std::runtime_error(msg) {}
};

class DataLoader
{
public:
    void loadIncludePath(string path);
    void loadIncludePath_v1(string path);
    void excute(stShotInfo* outputData[(int)SENSOR_TOTALCNT]);
    
    // 에러 로그 접근
    std::vector<std::string> getErrorLog() const { return m_errorLog; }
    std::vector<std::string> getWarningLog() const { return m_warningLog; }

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
    
    // 헬퍼 함수들
    bool findColumnIndex(const vector<string>& items, const string& columnName, int& outIndex);
    void logError(const string& message);
    void logWarning(const string& message);
    string sanitizeString(const string& str);
    bool validateDataRow(const vector<string>& row, size_t expectedSize, const string& sensorName, int rowIndex);

private:
    stShotInfo m_dataSet[SENSOR_TOTALCNT];
    std::vector<std::string> m_errorLog;
    std::vector<std::string> m_warningLog;
};