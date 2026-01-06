#include "pch.h"
#include "DataLoader.h"
#include <filesystem>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif

namespace fs = std::filesystem;

// ============================================================================
// TimeInfo 구조체 구현
// ============================================================================

std::string TimeInfo::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') 
        << std::setw(4) << year
        << std::setw(2) << month
        << std::setw(2) << day << "_"
        << std::setw(2) << hour
        << std::setw(2) << minute
        << std::setw(2) << second;
    return oss.str();
}

time_t TimeInfo::toTimeT() const {
    struct tm timeinfo = {};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;
    timeinfo.tm_isdst = -1;
    return mktime(&timeinfo);
}

TimeInfo TimeInfo::fromString(const std::string& dateTime) {
    TimeInfo info = {};
    if (dateTime.length() >= 15) {
        try {
            info.year = std::stoi(dateTime.substr(0, 4));
            info.month = std::stoi(dateTime.substr(4, 2));
            info.day = std::stoi(dateTime.substr(6, 2));
            info.hour = std::stoi(dateTime.substr(9, 2));
            info.minute = std::stoi(dateTime.substr(11, 2));
            info.second = std::stoi(dateTime.substr(13, 2));
        } catch (...) {
            // 파싱 실패 시 기본값 유지
        }
    }
    return info;
}

// ============================================================================
// DataHelper 네임스페이스 함수 구현
// ============================================================================

namespace DataHelper {
    time_t parseDateTime(const std::string& dateTime) {
        if (dateTime.empty() || dateTime.length() < 15) {
            return 0;
        }

        try {
            // YYYYMMDD_HHMMSS 형식
            int year = std::stoi(dateTime.substr(0, 4));
            int month = std::stoi(dateTime.substr(4, 2));
            int day = std::stoi(dateTime.substr(6, 2));
            int hour = std::stoi(dateTime.substr(9, 2));
            int minute = std::stoi(dateTime.substr(11, 2));
            int second = std::stoi(dateTime.substr(13, 2));

            struct tm timeinfo = {};
            timeinfo.tm_year = year - 1900;
            timeinfo.tm_mon = month - 1;
            timeinfo.tm_mday = day;
            timeinfo.tm_hour = hour;
            timeinfo.tm_min = minute;
            timeinfo.tm_sec = second;
            timeinfo.tm_isdst = -1;

            return mktime(&timeinfo);
        }
        catch (const std::exception&) {
            return 0;
        }
    }

    std::string trim(const std::string& str) {
        std::string result = str;
        result.erase(0, result.find_first_not_of(" \t\r\n"));
        result.erase(result.find_last_not_of(" \t\r\n") + 1);
        return result;
    }
}

// ============================================================================
// DataLoader 헬퍼 함수들
// ============================================================================

void DataLoader::logError(const string& message) {
    m_errorLog.push_back("[ERROR] " + message);
    std::cout << "[ERROR] " << message << std::endl;
}

void DataLoader::logWarning(const string& message) {
    m_warningLog.push_back("[WARNING] " + message);
    std::cout << "[WARNING] " << message << std::endl;
}

string DataLoader::sanitizeString(const string& str) {
    return DataHelper::trim(str);
}

bool DataLoader::findColumnIndex(const vector<string>& items, const string& columnName, int& outIndex) {
    for (size_t i = 0; i < items.size(); i++) {
        string cleanName = sanitizeString(items[i]);
        if (cleanName == columnName) {
            outIndex = static_cast<int>(i);
            return true;
        }
    }
    return false;
}

bool DataLoader::validateDataRow(const vector<string>& row, size_t expectedSize, 
                                  const string& sensorName, int rowIndex) {
    if (row.size() != expectedSize) {
        logWarning(sensorName + " row " + std::to_string(rowIndex) + 
                  ": Expected " + std::to_string(expectedSize) + 
                  " columns, got " + std::to_string(row.size()));
        return false;
    }
    return true;
}

vector<string> DataLoader::split(string inputStr, string delimiter)
{
    vector<string> rtnstr;
    size_t pos = 0;
    string token;

    while ((pos = inputStr.find(delimiter)) != string::npos)
    {
        token = inputStr.substr(0, pos);
        rtnstr.push_back(token);
        inputStr.erase(0, pos + delimiter.length());
    }
    rtnstr.push_back(inputStr);

    return rtnstr;
}

// ============================================================================
// loadIncludePath - 센서별 폴더가 분리되어 있을 때
// ============================================================================

void DataLoader::loadIncludePath(string path)
{
    std::cout << "\n=== Loading data from path: " << path << " ===" << std::endl;

    // 경로 끝에 백슬래시가 없으면 추가
    if (!path.empty() && path.back() != '\\' && path.back() != '/')
        path += "\\";

    // GCQ 폴더 처리
    string gcqPath = path + "GCQ";
    if (fs::exists(gcqPath))
    {
        std::cout << "Found GCQ folder" << std::endl;
        gcqPath += "\\";
        string tmpPath = gcqPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open GCQ folder");
        }
        else {
            string gcqLogPath;
            string gcqCsvPath;
            
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");
                vector<string> fileStr2 = split(str, " ");

                string fullPath = gcqPath + fd.name;
                
                if (fileStr[0] == GCQLogFile || fileStr1[0] == GCQLogFile)
                    gcqLogPath = fullPath;

                if (fileStr[0] == GCQCsvFile || fileStr1[0] == GCQCsvFile || fileStr2[0] == GCQCsvFile)
                    gcqCsvPath = fullPath;

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
            
            if (!gcqCsvPath.empty() && !gcqLogPath.empty()) {
                readGCQFile(gcqCsvPath, gcqLogPath);
            } else {
                logWarning("GCQ: Required files not found (CSV or LOG)");
            }
        }
    }

    // WAVE 폴더 처리
    string wavePath = path + "WAVE";
    if (fs::exists(wavePath))
    {
        std::cout << "Found WAVE folder" << std::endl;
        wavePath += "\\Result\\";
        string tmpPath = wavePath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open WAVE Result folder");
        }
        else {
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");

                string fullPath = wavePath + fd.name;

                if (fileStr[0] == WaveFile || fileStr1[0] == WaveFile)
                    readWaveFile(fullPath);

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
        }
    }

    // TM 폴더 처리
    string tmPath = path + "TM";
    if (fs::exists(tmPath))
    {
        std::cout << "Found TM folder" << std::endl;
        tmPath += "\\";
        string tmpPath = tmPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open TM folder");
        }
        else {
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");
                vector<string> fileStr2 = split(str, " ");

                string fullPath = tmPath + fd.name;
                
                if (fileStr[0] == TM4File || fileStr[0] == TM4File1 ||
                    fileStr1[0] == TM4File || fileStr1[0] == TM4File1 ||
                    fileStr2[0] == TM4File || fileStr2[0] == TM4File1)
                    readTrackManFile(fullPath);

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
        }
    }

    // MAX 폴더 처리
    string maxPath = path + "MAX";
    if (fs::exists(maxPath))
    {
        std::cout << "Found MAX folder" << std::endl;
        maxPath += "\\";
        string tmpPath = maxPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open MAX folder");
        }
        else {
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");

                string fullPath = maxPath + fd.name;
                
                if (fileStr[0] == MaxFile || fileStr1[0] == MaxFile)
                    readMaxFile(fullPath);

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
        }
    }

    // NX 폴더 처리
    string nxPath = path + "NX";
    if (fs::exists(nxPath))
    {
        std::cout << "Found NX folder" << std::endl;
        nxPath += "\\";
        string tmpPath = nxPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open NX folder");
        }
        else {
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");

                string fullPath = nxPath + fd.name;
                
                if (fileStr[0] == NXFile || fileStr1[0] == NXFile)
                    readNXFile(fullPath);

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
        }
    }

    // GRADAR 폴더 처리
    string gradarPath = path + "GRADAR";
    if (fs::exists(gradarPath))
    {
        std::cout << "Found GRADAR folder" << std::endl;
        gradarPath += "\\Result\\";
        string tmpPath = gradarPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L) {
            logWarning("Cannot open GRADAR Result folder");
        }
        else {
            do
            {
                string str = fd.name;
                vector<string> fileStr = split(str, "_");
                vector<string> fileStr1 = split(str, ".");

                string fullPath = gradarPath + fd.name;
                
                if (fileStr[0] == GRadarFile || fileStr1[0] == GRadarFile)
                    readGRadarFile(fullPath);

            } while (_findnext(handle, &fd) == 0);
            _findclose(handle);
        }
    }

    std::cout << "=== Data loading complete ===" << std::endl;
}

// ============================================================================
// loadIncludePath_v1 - 모든 파일이 한 폴더에 있을 때
// ============================================================================

void DataLoader::loadIncludePath_v1(string path)
{
    std::cout << "\n=== Loading data from single path: " << path << " ===" << std::endl;

    // 경로 끝에 백슬래시가 없으면 추가
    if (!path.empty() && path.back() != '\\' && path.back() != '/')
        path += "\\";

    string inputPath = path + "*.*";
    struct _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(inputPath.c_str(), &fd)) == -1L) {
        logError("Cannot open path: " + path);
        return;
    }

    string gcqLogPath;
    string gcqCsvPath;
    
    do
    {
        string str = fd.name;
        vector<string> fileStr = split(str, "_");
        vector<string> fileStr1 = split(str, ".");

        string fullPath = path + fd.name;
        
        // GCQ 파일들
        if (fileStr[0] == GCQLogFile || fileStr1[0] == GCQLogFile)
            gcqLogPath = fullPath;

        if (fileStr[0] == GCQCsvFile || fileStr1[0] == GCQCsvFile)
            gcqCsvPath = fullPath;

        // TrackMan 파일
        if (fileStr[0] == TM4File || fileStr1[0] == TM4File ||
            fileStr[0] == TM4File1 || fileStr1[0] == TM4File1)
            readTrackManFile(fullPath);

        // Wave 파일
        if (fileStr[0] == WaveFile || fileStr1[0] == WaveFile)
            readWaveFile(fullPath);

        // MAX 파일
        if (fileStr[0] == MaxFile || fileStr1[0] == MaxFile)
            readMaxFile(fullPath);

        // NX 파일
        if (fileStr[0] == NXFile || fileStr1[0] == NXFile)
            readNXFile(fullPath);

        // GRadar 파일
        if (fileStr[0] == GRadarFile || fileStr1[0] == GRadarFile)
            readGRadarFile(fullPath);
        
    } while (_findnext(handle, &fd) == 0);
    _findclose(handle);

    // GCQ 파일 읽기 (CSV와 LOG 모두 필요)
    if (!gcqCsvPath.empty() && !gcqLogPath.empty()) {
        readGCQFile(gcqCsvPath, gcqLogPath);
    } else {
        if (!gcqCsvPath.empty() || !gcqLogPath.empty()) {
            logWarning("GCQ: Both CSV and LOG files are required");
        }
    }

    std::cout << "=== Data loading complete ===" << std::endl;
}

// ============================================================================
// readGCQFile - GCQ CSV 및 LOG 파일 읽기
// ============================================================================

void DataLoader::readGCQFile(string csvFilePath, string logFilePath)
{
    std::cout << "\n--- Reading GCQ files ---" << std::endl;
    std::cout << "CSV: " << csvFilePath << std::endl;
    std::cout << "LOG: " << logFilePath << std::endl;

    string line;
    std::ifstream csvFile(csvFilePath);
    if (csvFile.fail())
    {
        logError("Failed to open GCQ CSV file: " + csvFilePath);
        return;
    }

    std::ifstream logFile(logFilePath);
    if (logFile.fail())
    {
        logError("Failed to open GCQ LOG file: " + logFilePath);
        csvFile.close();
        return;
    }

    m_dataSet[SENSOR_GCQ].snName = "GCQ";

    // CSV 파일 읽기
    bool headerFound = false;
    size_t expectedColumns = 0;

    while (getline(csvFile, line))
    {
        // BOM 제거 (UTF-8)
        if (line.size() >= 3 &&
            (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF)
        {
            line = line.substr(3);
        }

        // 헤더 찾기
        if (!headerFound && line.find("Foresight") != string::npos)
        {
            vector<string> itemVec = split(line, ",");

            // 빈 항목 제거 및 정리
            vector<string> newItemVec;
            for (size_t i = 0; i < itemVec.size(); i++)
            {
                // 첫 번째 항목에서 Foresight 헤더 제거
                if (i == 0)
                {
                    size_t pos = itemVec[i].find("Club");
                    if (pos != string::npos)
                    {
                        itemVec[i] = itemVec[i].substr(pos);
                    }
                }

                string trimmed = sanitizeString(itemVec[i]);

                if (!trimmed.empty())
                {
                    newItemVec.push_back(trimmed);
                }
            }

            m_dataSet[SENSOR_GCQ].item = newItemVec;
            expectedColumns = newItemVec.size();
            headerFound = true;

            std::cout << "GCQ CSV Header found with " << expectedColumns << " columns" << std::endl;
            continue;
        }

        // 단위 행이나 다른 메타데이터 스킵
        if (line.find("M/S") != string::npos ||
            line.find("Average") != string::npos ||
            line.find("Std.") != string::npos)
        {
            continue;
        }

        // 빈 줄 스킵
        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
        {
            continue;
        }

        // 따옴표 제거
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        vector<string> dataVec = split(line, ",");

        // 데이터가 있는 행인지 확인
        bool hasData = false;
        for (const auto& data : dataVec)
        {
            string trimmedData = sanitizeString(data);
            if (!trimmedData.empty())
            {
                hasData = true;
                break;
            }
        }

        if (!hasData)
        {
            continue;
        }

        // 열 개수 정규화
        if (expectedColumns > 0)
        {
            while (dataVec.size() < expectedColumns)
            {
                dataVec.push_back("");
            }

            if (dataVec.size() > expectedColumns)
            {
                dataVec.resize(expectedColumns);
            }
        }

        m_dataSet[SENSOR_GCQ].dataVec.push_back(dataVec);
    }

    csvFile.close();
    std::cout << "GCQ CSV loaded: " << m_dataSet[SENSOR_GCQ].dataVec.size() << " data rows" << std::endl;

    // LOG 파일 처리
    stShotInfo logDataSet;
    logDataSet.snName = "GCQlog";

    vector<string> logItemVec = {"Date", "Time", "BallSpeed", "LaunchAngle", 
                                  "BallDirection", "BackSpin", "SideSpin", "TotalSpin"};
    logDataSet.item = logItemVec;

    bool isSpinInfo = false;
    string date;
    vector<string> dataVec;
    vector<vector<string>> dataVecVec;

    while (getline(logFile, line))
    {
        // 제어 문자 제거
        line.erase(remove(line.begin(), line.end(), '\0'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        // Date 추출
        if (line.find("Date:") == 0)
        {
            string splitDate = "Date: ";
            if (line.length() >= splitDate.length() + 10)
            {
                string tmp = line.substr(splitDate.length(), 10);
                vector<string> dateVec = split(tmp, "/");
                if (dateVec.size() >= 3)
                {
                    date = dateVec[2] + dateVec[1] + dateVec[0];
                }
            }
            continue;
        }

        // Ball Speed
        if (line.find("Ball Speed (MPH)") != string::npos)
        {
            size_t bracketPos = line.find("[");
            if (bracketPos != string::npos && line.length() > bracketPos + 8)
            {
                string tmp = line.substr(bracketPos + 1, 8);
                vector<string> timeVec = split(tmp, ":");
                if (timeVec.size() >= 3)
                {
                    string time = timeVec[0] + timeVec[1] + timeVec[2];

                    dataVec.clear();
                    dataVec.push_back(date);
                    dataVec.push_back(time);

                    vector<string> datastr = split(line, "=");
                    if (datastr.size() >= 2)
                    {
                        string value = sanitizeString(datastr[1]);
                        dataVec.push_back(value);
                    }
                }
            }
            continue;
        }

        // Launch Angle
        if (line.find("Launch Angle (Deg)") != string::npos)
        {
            vector<string> datastr = split(line, "=");
            if (datastr.size() >= 2)
            {
                string value = sanitizeString(datastr[1]);
                dataVec.push_back(value);
            }
            continue;
        }

        // Azimuth (Ball Direction)
        if (line.find("Azimuth (Deg)") != string::npos)
        {
            vector<string> datastr = split(line, "=");
            if (datastr.size() >= 2)
            {
                string value = sanitizeString(datastr[1]);
                dataVec.push_back(value);
            }
            continue;
        }

        // Spin 정보 시작
        if (line.find("Processing SPIN data") != string::npos)
        {
            isSpinInfo = true;
        }

        if (isSpinInfo)
        {
            // BackSpin
            if (line.find("BackSpinRpm") != string::npos)
            {
                vector<string> datastr = split(line, "=");
                if (datastr.size() >= 2)
                {
                    string value = sanitizeString(datastr[1]);
                    dataVec.push_back(value);
                }
                continue;
            }

            // SideSpin
            if (line.find("SideSpinRpm") != string::npos)
            {
                vector<string> datastr = split(line, "=");
                if (datastr.size() >= 2)
                {
                    string value = sanitizeString(datastr[1]);
                    dataVec.push_back(value);
                }
                continue;
            }

            // TotalSpin
            if (line.find("TotalSpinRpm") != string::npos)
            {
                vector<string> dataStr = split(line, "=");
                if (dataStr.size() >= 2)
                {
                    string value = sanitizeString(dataStr[1]);
                    dataVec.push_back(value);

                    if (dataVec.size() >= 8)
                    {
                        dataVecVec.push_back(dataVec);
                    }

                    dataVec.clear();
                    isSpinInfo = false;
                }
                continue;
            }
        }
    }

    logFile.close();
    logDataSet.dataVec = dataVecVec;
    std::cout << "GCQ LOG loaded: " << logDataSet.dataVec.size() << " data entries" << std::endl;

    // log에서 추출한 시간 데이터 추가
    addGCQTime(logDataSet);
}

// ============================================================================
// 나머지 read 함수들 (TrackMan, Wave, MAX, NX, GRadar)
// ============================================================================

void DataLoader::readTrackManFile(string csvFilePath)
{
    std::cout << "\n--- Reading TrackMan file ---" << std::endl;
    std::cout << "File: " << csvFilePath << std::endl;

    string line;
    std::ifstream file(csvFilePath);
    if (file.fail())
    {
        logError("Failed to open TrackMan file: " + csvFilePath);
        return;
    }

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
    {
        logWarning("TrackMan file is not CSV format");
        file.close();
        return;
    }

    m_dataSet[SENSOR_TM].snName = "TM4";

    while (std::getline(file, line))
    {
        if (line.find("sep=,") != string::npos)
            continue;

        // 헤더 행
        if (line.find("Date") != string::npos)
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            m_dataSet[SENSOR_TM].item = split(line, ",");
            continue;
        }

        if (line.find("m/s") != string::npos)
            continue;

        // 따옴표 제거
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        // 빈 줄 스킵
        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
            continue;

        vector<string> dataStr = split(line, ",");
        
        if (!dataStr.empty() && !sanitizeString(dataStr[0]).empty())
        {
            m_dataSet[SENSOR_TM].dataVec.push_back(dataStr);
        }
    }

    file.close();
    std::cout << "TrackMan CSV loaded: " << m_dataSet[SENSOR_TM].dataVec.size() << " data rows" << std::endl;
}

void DataLoader::readWaveFile(string csvFilePath)
{
    std::cout << "\n--- Reading Wave file ---" << std::endl;
    std::cout << "File: " << csvFilePath << std::endl;

    string line;
    std::ifstream file(csvFilePath);
    if (file.fail())
    {
        logError("Failed to open Wave file: " + csvFilePath);
        return;
    }

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
    {
        logWarning("Wave file is not CSV format");
        file.close();
        return;
    }

    m_dataSet[SENSOR_WAVE].snName = "Wave";

    while (std::getline(file, line))
    {
        if (line.find("Name") != string::npos)
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            m_dataSet[SENSOR_WAVE].item = split(line, ",");
            continue;
        }

        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
            continue;

        vector<string> dataStr = split(line, ",");
        
        if (!dataStr.empty() && !sanitizeString(dataStr[0]).empty())
        {
            m_dataSet[SENSOR_WAVE].dataVec.push_back(dataStr);
        }
    }

    file.close();
    std::cout << "Wave CSV loaded: " << m_dataSet[SENSOR_WAVE].dataVec.size() << " data rows" << std::endl;
}

void DataLoader::readMaxFile(string csvFilePath)
{
    std::cout << "\n--- Reading MAX file ---" << std::endl;
    std::cout << "File: " << csvFilePath << std::endl;

    string line;
    std::ifstream file(csvFilePath);
    if (file.fail())
    {
        logError("Failed to open MAX file: " + csvFilePath);
        return;
    }

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
    {
        logWarning("MAX file is not CSV format");
        file.close();
        return;
    }

    m_dataSet[SENSOR_MAX].snName = "Max";

    while (std::getline(file, line))
    {
        if (line.find("Shot ID") != string::npos)
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            m_dataSet[SENSOR_MAX].item = split(line, ",");
            continue;
        }

        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
            continue;

        vector<string> dataStr = split(line, ",");
        
        if (!dataStr.empty() && !sanitizeString(dataStr[0]).empty())
        {
            m_dataSet[SENSOR_MAX].dataVec.push_back(dataStr);
        }
    }

    file.close();
    std::cout << "MAX CSV loaded: " << m_dataSet[SENSOR_MAX].dataVec.size() << " data rows" << std::endl;
}

void DataLoader::readNXFile(string csvFilePath)
{
    std::cout << "\n--- Reading NX file ---" << std::endl;
    std::cout << "File: " << csvFilePath << std::endl;

    string line;
    std::ifstream file(csvFilePath);
    if (file.fail())
    {
        logError("Failed to open NX file: " + csvFilePath);
        return;
    }

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
    {
        logWarning("NX file is not CSV format");
        file.close();
        return;
    }

    m_dataSet[SENSOR_NX].snName = "NX";

    while (std::getline(file, line))
    {
        if (line.find("NXSensor") != string::npos)
            continue;

        if (line.find("ShotDB") != string::npos)
        {
            m_dataSet[SENSOR_NX].item = split(line, ",");
            continue;
        }

        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
            continue;

        vector<string> dataStr = split(line, ",");
        
        if (!dataStr.empty() && !sanitizeString(dataStr[0]).empty())
        {
            m_dataSet[SENSOR_NX].dataVec.push_back(dataStr);
        }
    }

    file.close();
    std::cout << "NX CSV loaded: " << m_dataSet[SENSOR_NX].dataVec.size() << " data rows" << std::endl;
}

void DataLoader::readGRadarFile(string csvFilePath)
{
    std::cout << "\n--- Reading GRadar file ---" << std::endl;
    std::cout << "File: " << csvFilePath << std::endl;

    string line;
    std::ifstream file(csvFilePath);
    if (file.fail())
    {
        logError("Failed to open GRadar file: " + csvFilePath);
        return;
    }
    
    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
    {
        logWarning("GRadar file is not CSV format");
        file.close();
        return;
    }

    m_dataSet[SENSOR_GRADAR].snName = "GRadar";

    while (std::getline(file, line))
    {
        if (line.find("Name") != string::npos)
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            m_dataSet[SENSOR_GRADAR].item = split(line, ",");
            continue;
        }

        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
            continue;

        vector<string> dataStr = split(line, ",");
        
        if (!dataStr.empty() && !sanitizeString(dataStr[0]).empty())
        {
            m_dataSet[SENSOR_GRADAR].dataVec.push_back(dataStr);
        }
    }

    file.close();
    std::cout << "GRadar CSV loaded: " << m_dataSet[SENSOR_GRADAR].dataVec.size() << " data rows" << std::endl;
}

// ============================================================================
// addGCQTime - GCQ 데이터에 시간 정보 추가
// ============================================================================

void DataLoader::addGCQTime(stShotInfo inputData)
{
    std::cout << "\n--- Adding GCQ time information ---" << std::endl;

    if (m_dataSet[SENSOR_GCQ].dataVec.empty() || inputData.dataVec.empty())
    {
        logWarning("GCQ CSV or LOG data is empty. Skipping time synchronization.");
        return;
    }

    std::map<string, int> csvColumnMap;
    std::map<string, int> logColumnMap;

    for (size_t i = 0; i < m_dataSet[SENSOR_GCQ].item.size(); i++)
    {
        string colName = sanitizeString(m_dataSet[SENSOR_GCQ].item[i]);
        csvColumnMap[colName] = static_cast<int>(i);
    }

    for (size_t i = 0; i < inputData.item.size(); i++)
    {
        logColumnMap[inputData.item[i]] = static_cast<int>(i);
    }

    int numOfLA_csv = -1, numOfSA_csv = -1, numOfBSpin_csv = -1;

    for (const auto& pair : csvColumnMap)
    {
        string lowerCol = pair.first;
        transform(lowerCol.begin(), lowerCol.end(), lowerCol.begin(), ::tolower);

        if (lowerCol.find("launch") != string::npos && lowerCol.find("angle") != string::npos)
            numOfLA_csv = pair.second;
        else if (lowerCol.find("side") != string::npos && lowerCol.find("angle") != string::npos)
            numOfSA_csv = pair.second;
        else if (lowerCol.find("backspin") != string::npos)
            numOfBSpin_csv = pair.second;
    }

    if (numOfLA_csv == -1 && csvColumnMap.find("LaunchAngle") != csvColumnMap.end())
        numOfLA_csv = csvColumnMap["LaunchAngle"];
    if (numOfSA_csv == -1 && csvColumnMap.find("SideAngle") != csvColumnMap.end())
        numOfSA_csv = csvColumnMap["SideAngle"];
    if (numOfBSpin_csv == -1 && csvColumnMap.find("Backspin") != csvColumnMap.end())
        numOfBSpin_csv = csvColumnMap["Backspin"];

    int numOfLA_log = logColumnMap.count("LaunchAngle") ? logColumnMap["LaunchAngle"] : -1;
    int numOfSA_log = logColumnMap.count("BallDirection") ? logColumnMap["BallDirection"] : -1;
    int numOfBSpin_log = logColumnMap.count("BackSpin") ? logColumnMap["BackSpin"] : -1;
    int numOfDate_log = logColumnMap.count("Date") ? logColumnMap["Date"] : -1;
    int numOfTime_log = logColumnMap.count("Time") ? logColumnMap["Time"] : -1;

    if (numOfLA_csv == -1 || numOfSA_csv == -1 || numOfBSpin_csv == -1)
    {
        logError("Required columns not found in GCQ CSV file");
        return;
    }

    if (numOfLA_log == -1 || numOfSA_log == -1 || numOfBSpin_log == -1 ||
        numOfDate_log == -1 || numOfTime_log == -1)
    {
        logError("Required columns not found in GCQ LOG file");
        return;
    }

    stShotInfo tempDataSet;
    tempDataSet.item = m_dataSet[SENSOR_GCQ].item;
    tempDataSet.item.insert(tempDataSet.item.begin(), "Date-Time");

    vector<vector<string>> dataVecVec;
    int matchedCount = 0;
    int unmatchedCount = 0;

    for (size_t i = 0; i < inputData.dataVec.size(); i++)
    {
        const auto& logRow = inputData.dataVec[i];
        if (static_cast<size_t>(numOfLA_log) >= logRow.size() ||
            static_cast<size_t>(numOfSA_log) >= logRow.size() ||
            static_cast<size_t>(numOfBSpin_log) >= logRow.size() ||
            static_cast<size_t>(numOfDate_log) >= logRow.size() ||
            static_cast<size_t>(numOfTime_log) >= logRow.size())
        {
            logWarning("LOG data row " + std::to_string(i) + " has insufficient columns");
            continue;
        }

        bool matchFound = false;

        for (size_t j = 0; j < m_dataSet[SENSOR_GCQ].dataVec.size(); j++)
        {
            const auto& csvRow = m_dataSet[SENSOR_GCQ].dataVec[j];

            if (static_cast<size_t>(numOfLA_csv) >= csvRow.size() ||
                static_cast<size_t>(numOfSA_csv) >= csvRow.size() ||
                static_cast<size_t>(numOfBSpin_csv) >= csvRow.size())
            {
                continue;
            }

            if (logRow[numOfLA_log].empty() || csvRow[numOfLA_csv].empty() ||
                logRow[numOfSA_log].empty() || csvRow[numOfSA_csv].empty() ||
                logRow[numOfBSpin_log].empty() || csvRow[numOfBSpin_csv].empty())
            {
                continue;
            }

            try
            {
                double logLA = stod(logRow[numOfLA_log]);
                double csvLA = stod(csvRow[numOfLA_csv]);
                double logSA = stod(logRow[numOfSA_log]);
                double csvSA = stod(csvRow[numOfSA_csv]);
                double logBS = stod(logRow[numOfBSpin_log]);
                double csvBS = stod(csvRow[numOfBSpin_csv]);

                int logLA_int = static_cast<int>(round(logLA * 10));
                int csvLA_int = static_cast<int>(round(csvLA * 10));
                int logSA_int = static_cast<int>(round(logSA * 10));
                int csvSA_int = static_cast<int>(round(csvSA * 10));
                int logBS_int = static_cast<int>(round(logBS * 10));
                int csvBS_int = static_cast<int>(round(csvBS * 10));

                if (logLA_int == csvLA_int &&
                    logSA_int == csvSA_int &&
                    logBS_int == csvBS_int)
                {
                    vector<string> tempDataVec = csvRow;
                    string dateTime = logRow[numOfDate_log] + "_" + logRow[numOfTime_log];
                    tempDataVec.insert(tempDataVec.begin(), dateTime);
                    dataVecVec.push_back(tempDataVec);
                    matchFound = true;
                    matchedCount++;
                    break;
                }
            }
            catch (const std::exception&)
            {
                continue;
            }
        }

        if (!matchFound)
        {
            unmatchedCount++;
            if (unmatchedCount <= 5)
            {
                std::cout << "No match found for LOG entry " << i
                    << " (LA: " << logRow[numOfLA_log]
                    << ", SA: " << logRow[numOfSA_log]
                    << ", BS: " << logRow[numOfBSpin_log] << ")" << std::endl;
            }
        }
    }

    tempDataSet.dataVec = dataVecVec;
    m_dataSet[SENSOR_GCQ].item = tempDataSet.item;
    m_dataSet[SENSOR_GCQ].dataVec = tempDataSet.dataVec;

    std::cout << "GCQ time synchronization complete:" << std::endl;
    std::cout << "  Total LOG entries: " << inputData.dataVec.size() << std::endl;
    std::cout << "  Matched: " << matchedCount << std::endl;
    std::cout << "  Unmatched: " << unmatchedCount << std::endl;
}

// ============================================================================
// addTrackManTime - TrackMan 데이터에 시간 정보 추가
// ============================================================================

void DataLoader::addTrackManTime()
{
    std::cout << "\n--- Adding TrackMan time information ---" << std::endl;

    try {
        int nCsvTime = -1;
        if (!findColumnIndex(m_dataSet[SENSOR_TM].item, "Date", nCsvTime)) {
            logError("TrackMan: 'Date' column not found");
            return;
        }

        std::cout << "TrackMan: Date column found at index " << nCsvTime << std::endl;

        m_dataSet[SENSOR_TM].item.insert(m_dataSet[SENSOR_TM].item.begin(), "Date-Time");

        int successCount = 0;
        int failCount = 0;

        for (size_t i = 0; i < m_dataSet[SENSOR_TM].dataVec.size(); i++)
        {
            try {
                vector<string>& dataRow = m_dataSet[SENSOR_TM].dataVec[i];
                
                if (nCsvTime >= static_cast<int>(dataRow.size())) {
                    logWarning("TrackMan row " + std::to_string(i) + ": Date column index out of range");
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string val = sanitizeString(dataRow[nCsvTime]);
                
                if (val.empty()) {
                    logWarning("TrackMan row " + std::to_string(i) + ": Empty date value");
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                vector<string> strVec = split(val, " ");
                
                if (strVec.size() < 3) {
                    logWarning("TrackMan row " + std::to_string(i) + 
                              ": Invalid date format (expected 'YYYY-MM-DD AM/PM HH:MM:SS'): " + val);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string dateStr = strVec[0];
                string ampm = strVec[1];
                string timeStr = strVec[2];

                vector<string> dateVec = split(dateStr, "-");
                if (dateVec.size() != 3) {
                    logWarning("TrackMan row " + std::to_string(i) + ": Invalid date format: " + dateStr);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string date = dateVec[0] + dateVec[1] + dateVec[2];

                vector<string> timeVec = split(timeStr, ":");
                if (timeVec.size() != 3) {
                    logWarning("TrackMan row " + std::to_string(i) + ": Invalid time format: " + timeStr);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                int hour = std::stoi(timeVec[0]);
                int minute = std::stoi(timeVec[1]);
                int second = std::stoi(timeVec[2]);

                if (ampm == "PM" || ampm == "pm") {
                    if (hour != 12) {
                        hour += 12;
                    }
                } else if (ampm == "AM" || ampm == "am") {
                    if (hour == 12) {
                        hour = 0;
                    }
                }

                if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || 
                    second < 0 || second > 59) {
                    logWarning("TrackMan row " + std::to_string(i) + 
                              ": Invalid time values (H:" + std::to_string(hour) +
                              " M:" + std::to_string(minute) + 
                              " S:" + std::to_string(second) + ")");
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                std::ostringstream oss;
                oss << std::setfill('0') 
                    << std::setw(2) << hour
                    << std::setw(2) << minute
                    << std::setw(2) << second;
                string time = oss.str();

                string finalDateTime = date + "_" + time;
                dataRow.insert(dataRow.begin(), finalDateTime);
                successCount++;

            } catch (const std::exception& e) {
                logError("TrackMan row " + std::to_string(i) + ": Exception - " + string(e.what()));
                m_dataSet[SENSOR_TM].dataVec[i].insert(m_dataSet[SENSOR_TM].dataVec[i].begin(), "");
                failCount++;
            }
        }

        std::cout << "TrackMan time processing complete:" << std::endl;
        std::cout << "  Success: " << successCount << std::endl;
        std::cout << "  Failed: " << failCount << std::endl;

    } catch (const std::exception& e) {
        logError("TrackMan time processing failed: " + string(e.what()));
    }
}

// ============================================================================
// addMaxTime, addWaveTime, addNXTime, addGRadarTime
// (이전과 동일하므로 생략 - 위의 전체 코드 참조)
// ============================================================================

void DataLoader::addMaxTime()
{
    std::cout << "\n--- Adding MAX time information ---" << std::endl;

    try {
        int nCsvTime = -1;
        if (!findColumnIndex(m_dataSet[SENSOR_MAX].item, "Time", nCsvTime)) {
            logError("MAX: 'Time' column not found");
            return;
        }

        std::cout << "MAX: Time column found at index " << nCsvTime << std::endl;
        m_dataSet[SENSOR_MAX].item.insert(m_dataSet[SENSOR_MAX].item.begin(), "Date-Time");

        int successCount = 0, failCount = 0;

        for (size_t i = 0; i < m_dataSet[SENSOR_MAX].dataVec.size(); i++)
        {
            try {
                vector<string>& dataRow = m_dataSet[SENSOR_MAX].dataVec[i];
                
                if (nCsvTime >= static_cast<int>(dataRow.size())) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string val = sanitizeString(dataRow[nCsvTime]);
                if (val.empty()) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                vector<string> strVec = split(val, " ");
                if (strVec.size() < 3) {
                    logWarning("MAX row " + std::to_string(i) + ": Invalid format: " + val);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string dateStr = strVec[1];
                string timeStr = strVec[2];

                dateStr.erase(std::remove(dateStr.begin(), dateStr.end(), '.'), dateStr.end());
                timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), ':'), timeStr.end());

                string finalDateTime = dateStr + "_" + timeStr;
                dataRow.insert(dataRow.begin(), finalDateTime);
                successCount++;

            } catch (const std::exception& e) {
                logError("MAX row " + std::to_string(i) + ": " + string(e.what()));
                m_dataSet[SENSOR_MAX].dataVec[i].insert(m_dataSet[SENSOR_MAX].dataVec[i].begin(), "");
                failCount++;
            }
        }

        std::cout << "MAX time processing: Success=" << successCount << ", Failed=" << failCount << std::endl;

    } catch (const std::exception& e) {
        logError("MAX time processing failed: " + string(e.what()));
    }
}

void DataLoader::addWaveTime()
{
    std::cout << "\n--- Adding Wave time information ---" << std::endl;

    try {
        int nCsvTime = -1;
        if (!findColumnIndex(m_dataSet[SENSOR_WAVE].item, "Name", nCsvTime)) {
            logError("Wave: 'Name' column not found");
            return;
        }

        std::cout << "Wave: Name column found at index " << nCsvTime << std::endl;
        m_dataSet[SENSOR_WAVE].item.insert(m_dataSet[SENSOR_WAVE].item.begin(), "Date-Time");

        int successCount = 0, failCount = 0;

        for (size_t i = 0; i < m_dataSet[SENSOR_WAVE].dataVec.size(); i++)
        {
            try {
                vector<string>& dataRow = m_dataSet[SENSOR_WAVE].dataVec[i];
                
                if (nCsvTime >= static_cast<int>(dataRow.size())) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string val = sanitizeString(dataRow[nCsvTime]);
                if (val.empty()) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                vector<string> strVec = split(val, "_");
                if (strVec.size() < 3) {
                    logWarning("Wave row " + std::to_string(i) + ": Invalid format: " + val);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string date = strVec[1];
                string time = strVec[2];

                date.erase(std::remove(date.begin(), date.end(), '.'), date.end());
                time.erase(std::remove(time.begin(), time.end(), '.'), time.end());

                string finalDateTime = date + "_" + time;
                dataRow.insert(dataRow.begin(), finalDateTime);
                successCount++;

            } catch (const std::exception& e) {
                logError("Wave row " + std::to_string(i) + ": " + string(e.what()));
                m_dataSet[SENSOR_WAVE].dataVec[i].insert(m_dataSet[SENSOR_WAVE].dataVec[i].begin(), "");
                failCount++;
            }
        }

        std::cout << "Wave time processing: Success=" << successCount << ", Failed=" << failCount << std::endl;

    } catch (const std::exception& e) {
        logError("Wave time processing failed: " + string(e.what()));
    }
}

void DataLoader::addNXTime()
{
    std::cout << "\n--- Adding NX time information ---" << std::endl;

    try {
        int nCsvTime = -1;
        if (!findColumnIndex(m_dataSet[SENSOR_NX].item, "Date", nCsvTime)) {
            logError("NX: 'Date' column not found");
            return;
        }

        std::cout << "NX: Date column found at index " << nCsvTime << std::endl;
        m_dataSet[SENSOR_NX].item.insert(m_dataSet[SENSOR_NX].item.begin(), "Date-Time");

        int successCount = 0, failCount = 0;

        for (size_t i = 0; i < m_dataSet[SENSOR_NX].dataVec.size(); i++)
        {
            try {
                vector<string>& dataRow = m_dataSet[SENSOR_NX].dataVec[i];
                
                if (nCsvTime >= static_cast<int>(dataRow.size())) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string val = sanitizeString(dataRow[nCsvTime]);
                if (val.empty()) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                vector<string> strVec = split(val, "_");
                if (strVec.size() < 2) {
                    logWarning("NX row " + std::to_string(i) + ": Invalid format: " + val);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string date = strVec[0];
                string time = strVec[1];

                date.erase(std::remove(date.begin(), date.end(), '.'), date.end());
                time.erase(std::remove(time.begin(), time.end(), '.'), time.end());

                string finalDateTime = date + "_" + time;
                dataRow.insert(dataRow.begin(), finalDateTime);
                successCount++;

            } catch (const std::exception& e) {
                logError("NX row " + std::to_string(i) + ": " + string(e.what()));
                m_dataSet[SENSOR_NX].dataVec[i].insert(m_dataSet[SENSOR_NX].dataVec[i].begin(), "");
                failCount++;
            }
        }

        std::cout << "NX time processing: Success=" << successCount << ", Failed=" << failCount << std::endl;

    } catch (const std::exception& e) {
        logError("NX time processing failed: " + string(e.what()));
    }
}

void DataLoader::addGRadarTime()
{
    std::cout << "\n--- Adding GRadar time information ---" << std::endl;

    try {
        int nCsvTime = -1;
        if (!findColumnIndex(m_dataSet[SENSOR_GRADAR].item, "Name", nCsvTime)) {
            logError("GRadar: 'Name' column not found");
            return;
        }

        std::cout << "GRadar: Name column found at index " << nCsvTime << std::endl;
        m_dataSet[SENSOR_GRADAR].item.insert(m_dataSet[SENSOR_GRADAR].item.begin(), "Date-Time");

        int successCount = 0, failCount = 0;

        for (size_t i = 0; i < m_dataSet[SENSOR_GRADAR].dataVec.size(); i++)
        {
            try {
                vector<string>& dataRow = m_dataSet[SENSOR_GRADAR].dataVec[i];
                
                if (nCsvTime >= static_cast<int>(dataRow.size())) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string val = sanitizeString(dataRow[nCsvTime]);
                if (val.empty()) {
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                vector<string> strVec = split(val, "_");
                if (strVec.size() < 3) {
                    logWarning("GRadar row " + std::to_string(i) + ": Invalid format: " + val);
                    dataRow.insert(dataRow.begin(), "");
                    failCount++;
                    continue;
                }

                string date = strVec[1];
                string time = strVec[2];

                date.erase(std::remove(date.begin(), date.end(), '.'), date.end());
                time.erase(std::remove(time.begin(), time.end(), '.'), time.end());

                string finalDateTime = date + "_" + time;
                dataRow.insert(dataRow.begin(), finalDateTime);
                successCount++;

            } catch (const std::exception& e) {
                logError("GRadar row " + std::to_string(i) + ": " + string(e.what()));
                m_dataSet[SENSOR_GRADAR].dataVec[i].insert(m_dataSet[SENSOR_GRADAR].dataVec[i].begin(), "");
                failCount++;
            }
        }

        std::cout << "GRadar time processing: Success=" << successCount << ", Failed=" << failCount << std::endl;

    } catch (const std::exception& e) {
        logError("GRadar time processing failed: " + string(e.what()));
    }
}

// ============================================================================
// excute - 시간 정보 추출 실행
// ============================================================================

void DataLoader::excute(stShotInfo* outputData[(int)SENSOR_TOTALCNT])
{
    std::cout << "\n=== Starting time information extraction ===" << std::endl;

    if (m_dataSet[SENSOR_TM].dataVec.size() != 0)
        addTrackManTime();

    if (m_dataSet[SENSOR_MAX].dataVec.size() != 0)
        addMaxTime();

    if (m_dataSet[SENSOR_WAVE].dataVec.size() != 0)
        addWaveTime();

    if (m_dataSet[SENSOR_NX].dataVec.size() != 0)
        addNXTime();

    if (m_dataSet[SENSOR_GRADAR].dataVec.size() != 0)
        addGRadarTime();

    for (int i = 0; i < SENSOR_TOTALCNT; i++)
        outputData[i] = &m_dataSet[i];

    std::cout << "\n=== Time extraction complete ===" << std::endl;
    std::cout << "Total errors: " << m_errorLog.size() << std::endl;
    std::cout << "Total warnings: " << m_warningLog.size() << std::endl;
}