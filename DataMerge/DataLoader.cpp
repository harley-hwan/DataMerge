#include "pch.h"
#include "DataLoader.h"
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

void DataLoader::loadIncludePath(string path)
{
    // 각 센서마다 폴더 개별 존재할 때 가능

    string gcqPath = path + "GCQ";
    if (fs::exists(gcqPath) == true)
    {
        gcqPath += "\\";
        string tmpPath = gcqPath + "\\*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        string gcqLogPath;
        string gcqCsvPath;
        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");
            vector<string> fileStr2 = split(str, " ");

            string fullPath = gcqPath + fd.name;
            if (fileStr[0] == GCQLogFile)
                gcqLogPath = fullPath;
            else if (fileStr1[0] == GCQLogFile)
                gcqLogPath = fullPath;


            if (fileStr[0] == GCQCsvFile)
                gcqCsvPath = fullPath;
            else if (fileStr1[0] == GCQCsvFile)
                gcqCsvPath = fullPath;
            else if (fileStr2[0] == GCQCsvFile)
                gcqCsvPath = fullPath;

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
        readGCQFile(gcqCsvPath, gcqLogPath);
    }

    string wavePath = path + "WAVE\\";
    if (fs::exists(wavePath) == true)
    {
        wavePath += "Result\\";
        string tmpPath = wavePath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");

            string fullPath = wavePath + fd.name;

            if (fileStr[0] == WaveFile)
                readWaveFile(fullPath);
            else if (fileStr1[0] == WaveFile)
                readWaveFile(fullPath);

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }


    string tmPath = path + "TM";
    if (fs::exists(tmPath) == true)
    {
        tmPath += "\\";
        string tmpPath = tmPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");
            vector<string> fileStr2 = split(str, " ");

            string fullPath = tmPath + fd.name;
            if (fileStr[0] == TM4File || fileStr[0] == TM4File1)
                readTrackManFile(fullPath);
            else if (fileStr1[0] == TM4File || fileStr1[0] == TM4File1)
                readTrackManFile(fullPath);
            else if (fileStr2[0] == TM4File || fileStr2[0] == TM4File1)
                readTrackManFile(fullPath);

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }



    string maxPath = path + "MAX";
    if (fs::exists(maxPath) == true)
    {
        maxPath += "\\";
        string tmpPath = maxPath + "\\*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");

            string fullPath = maxPath + fd.name;
            if (fileStr[0] == MaxFile)
                readMaxFile(fullPath);
            else if (fileStr1[0] == MaxFile)
                readMaxFile(fullPath);

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }



    string nxPath = path + "NX";
    if (fs::exists(nxPath) == true)
    {
        nxPath += "\\";
        string tmpPath = nxPath + "\\*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");

            string fullPath = nxPath + fd.name;
            if (fileStr[0] == NXFile)
                readNXFile(fullPath);
            else if (fileStr1[0] == NXFile)
                readNXFile(fullPath);

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }



    string gradarPath = path + "GRADAR\\";
    if (fs::exists(gradarPath) == true)
    {
        gradarPath += "Result\\";
        string tmpPath = gradarPath + "*.*";
        struct _finddata_t fd;
        intptr_t handle;

        if ((handle = _findfirst(tmpPath.c_str(), &fd)) == -1L)
            return;

        do
        {
            string str = fd.name;
            vector<string> fileStr = split(str, "_");
            vector<string> fileStr1 = split(str, ".");

            string fullPath = gradarPath + fd.name;
            if (fileStr[0] == GRadarFile)
                readGRadarFile(fullPath);
            else if (fileStr1[0] == GRadarFile)
                readGRadarFile(fullPath);

        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }

    m_dataSet;
    int bb = 0;
}

void DataLoader::loadIncludePath_v1(string path)
{
    string inputPath = path + "*.*";
    struct _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(inputPath.c_str(), &fd)) == -1L)
        return;

    string gcqLogPath;
    string gcqCsvPath;
    do
    {
        string str = fd.name;
        vector<string> fileStr = split(str, "_");
        vector<string> fileStr1 = split(str, ".");


        string fullPath = path + fd.name;
        if (fileStr[0] == GCQLogFile)
            gcqLogPath = fullPath;
        else if (fileStr1[0] == GCQLogFile)
            gcqLogPath = fullPath;


        if (fileStr[0] == GCQCsvFile)
            gcqCsvPath = fullPath;
        else if (fileStr1[0] == GCQCsvFile)
            gcqCsvPath = fullPath;

        if (fileStr[0] == TM4File)
            readTrackManFile(fullPath);
        else if (fileStr1[0] == TM4File)
            readTrackManFile(fullPath);

        if (fileStr[0] == WaveFile)
            readWaveFile(fullPath);
        else if (fileStr1[0] == WaveFile)
            readWaveFile(fullPath);

        if (fileStr[0] == MaxFile)
            readMaxFile(fullPath);
        else if (fileStr1[0] == MaxFile)
            readMaxFile(fullPath);

        if (fileStr[0] == NXFile)
            readNXFile(fullPath);
        else if (fileStr1[0] == NXFile)
            readNXFile(fullPath);

        if (fileStr[0] == GRadarFile)
            readGRadarFile(fullPath);
        else if (fileStr1[0] == GRadarFile)
            readGRadarFile(fullPath);

        
    } while (_findnext(handle, &fd) == 0);
    _findclose(handle);

    readGCQFile(gcqCsvPath, gcqLogPath);

}

void DataLoader::readGCQFile(string csvFilePath, string logFilePath)
{
    string line;
    std::ifstream csvFile(csvFilePath);
    if (true == csvFile.fail())
    {
        std::cout << "Failed to open CSV file: " << csvFilePath << std::endl;
        return;
    }

    std::ifstream logFile(logFilePath);
    if (true == logFile.fail())
    {
        std::cout << "Failed to open LOG file: " << logFilePath << std::endl;
        return;
    }

    m_dataSet[SENSOR_GCQ].snName = "GCQ";

    // CSV 파일 읽으면서 데이터 추출
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

        // 맨 윗줄 데이터 이름만 추출
        if (!headerFound && line.find("Foresight") != string::npos)
        {
            // 공백 제거
            string cleanLine = line;
            cleanLine.erase(remove(cleanLine.begin(), cleanLine.end(), ' '), cleanLine.end());

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

                // 공백 제거
                string trimmed = itemVec[i];
                trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
                trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

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
            line.find("Std.") != string::npos ||
            line.find("��") != string::npos)  // 한글 단위 표시
        {
            continue;
        }

        // 빈 줄 스킵
        if (line.empty() || line.find_first_not_of(" ,\t\r\n") == string::npos)
        {
            continue;
        }

        // 데이터 부분만 추출
        // 따옴표 제거
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        vector<string> dataVec = split(line, ",");

        // 데이터가 있는 행인지 확인
        bool hasData = false;
        for (const auto& data : dataVec)
        {
            if (!data.empty() && data != " ")
            {
                hasData = true;
                break;
            }
        }

        if (!hasData || dataVec.empty() || dataVec[0].empty())
        {
            continue;
        }

        // 열 개수 정규화 - 헤더가 설정된 경우에만
        if (expectedColumns > 0)
        {
            // 열이 부족한 경우 빈 문자열로 채움
            while (dataVec.size() < expectedColumns)
            {
                dataVec.push_back("");
            }

            // 열이 초과하는 경우 잘라냄
            if (dataVec.size() > expectedColumns)
            {
                dataVec.resize(expectedColumns);
                std::cout << "Warning: Row has " << dataVec.size()
                    << " columns, expected " << expectedColumns
                    << ". Extra columns removed." << std::endl;
            }
        }

        m_dataSet[SENSOR_GCQ].dataVec.push_back(dataVec);
    }

    std::cout << "GCQ CSV loaded: " << m_dataSet[SENSOR_GCQ].dataVec.size() << " data rows" << std::endl;

    // LOG 파일 처리
    stShotInfo logDataSet;
    logDataSet.snName = "GCQlog";

    // log 데이터 항목
    vector<string> logItemVec;
    logItemVec.push_back("Date");
    logItemVec.push_back("Time");
    logItemVec.push_back("BallSpeed");
    logItemVec.push_back("LaunchAngle");
    logItemVec.push_back("BallDirection");
    logItemVec.push_back("BackSpin");
    logItemVec.push_back("SideSpin");
    logItemVec.push_back("TotalSpin");
    logDataSet.item = logItemVec;

    bool isSpinInfo = false;
    string date;
    vector<string> dataVec;
    vector<vector<string>> dataVecVec;

    // LOG 파일 읽으면서 데이터 추출
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
                        string value = datastr[1];
                        value.erase(0, value.find_first_not_of(" \t"));
                        value.erase(value.find_last_not_of(" \t") + 1);
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
                string value = datastr[1];
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
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
                string value = datastr[1];
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
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
                    string value = datastr[1];
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
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
                    string value = datastr[1];
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
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
                    string value = dataStr[1];
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    dataVec.push_back(value);

                    // 데이터가 완성되었는지 확인
                    if (dataVec.size() >= 8)  // 모든 필드가 채워졌는지
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

    logDataSet.dataVec = dataVecVec;
    std::cout << "GCQ LOG loaded: " << logDataSet.dataVec.size() << " data entries" << std::endl;

    // log에서 추출한 시간 데이터 추가
    addGCQTime(logDataSet);
}

void DataLoader::readTrackManFile(string csvFilePath)
{
    string line;
    std::ifstream file(csvFilePath);
    if (true == file.fail())
        return;

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
        return;

    m_dataSet[SENSOR_TM].snName = "TM4";

    // csv 파일 읽으면서 데이터 추출
    while (std::getline(file, line))
    {
        if (line.find("sep=,") != string::npos)
            continue;

        // 맨 윗줄 데이터 이름만 추출
        if (line.find("Date") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_TM].item = split(line, ",");
            continue;
        }

        if (line.find("m/s") != string::npos)
            continue;

        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_TM].dataVec.push_back(dataStr);
    }
}

void DataLoader::readWaveFile(string csvFilePath)
{
    string line;
    std::ifstream file(csvFilePath);
    if (true == file.fail())
        return;

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
        return;

    m_dataSet[SENSOR_WAVE].snName = "Wave";

    // csv 파일 읽으면서 데이터 추출
    while (std::getline(file, line))
    {
        // 맨 윗줄 데이터 이름만 추출
        if (line.find("Name") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_WAVE].item = split(line, ",");
            continue;
        }

        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_WAVE].dataVec.push_back(dataStr);
    }
}

void DataLoader::readMaxFile(string csvFilePath)
{
    string line;
    std::ifstream file(csvFilePath);
    if (true == file.fail())
        return;

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
        return;

    m_dataSet[SENSOR_MAX].snName = "Max";

    // csv 파일 읽으면서 데이터 추출
    while (std::getline(file, line))
    {
        // 맨 윗줄 데이터 이름만 추출
        if (line.find("Shot ID") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_MAX].item = split(line, ",");
            continue;
        }

        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_MAX].dataVec.push_back(dataStr);
    }
}

void DataLoader::readNXFile(string csvFilePath)
{
    string line;
    std::ifstream file(csvFilePath);
    if (true == file.fail())
        return;

    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
        return;

    m_dataSet[SENSOR_NX].snName = "NX";

    // csv 파일 읽으면서 데이터 추출
    while (std::getline(file, line))
    {
        if (line.find("NXSensor") != string::npos)
            continue;

        // 맨 윗줄 데이터 이름만 추출
        if (line.find("ShotDB") != string::npos)
        {
            m_dataSet[SENSOR_NX].item = split(line, ",");
            continue;
        }

        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_NX].dataVec.push_back(dataStr);
    }
}

void DataLoader::readGRadarFile(string csvFilePath)
{
    string line;
    std::ifstream file(csvFilePath);
    if (true == file.fail())
        return;
    
    string fileInfo = csvFilePath.substr(csvFilePath.length() - 3, 3);
    if (fileInfo != "csv")
        return;

    m_dataSet[SENSOR_GRADAR].snName = "GRadar";

    // csv 파일 읽으면서 데이터 추출
    while (std::getline(file, line))
    {
        // 맨 윗줄 데이터 이름만 추출
        if (line.find("Name") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_GRADAR].item = split(line, ",");
            continue;
        }

        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_GRADAR].dataVec.push_back(dataStr);
    }
}

void DataLoader::excute(stShotInfo* outputData[(int)SENSOR_TOTALCNT])
{
    // GCQ는 파일 로드할 때 시간 정보 추가 함

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


}

vector<string> DataLoader::split(string inputStr, string delimiter)
{
    vector<string> rtnstr;
    long long pos = 0;
    string token = "";

    while ((pos = inputStr.find(delimiter)) != string::npos)
    {
        token = inputStr.substr(0, pos);
        rtnstr.push_back(token);
        inputStr.erase(0, pos + delimiter.length());
    }
    rtnstr.push_back(inputStr);

    return rtnstr;
}

void DataLoader::addGCQTime(stShotInfo inputData)
{
    // 데이터가 없는 경우 조기 종료
    if (m_dataSet[SENSOR_GCQ].dataVec.empty() || inputData.dataVec.empty())
    {
        std::cout << "Warning: GCQ CSV or LOG data is empty. Skipping time synchronization." << std::endl;
        return;
    }

    // 열 이름을 인덱스로 매핑하는 맵 생성
    std::map<string, int> csvColumnMap;
    std::map<string, int> logColumnMap;

    // CSV 파일의 열 매핑
    for (int i = 0; i < m_dataSet[SENSOR_GCQ].item.size(); i++)
    {
        string colName = m_dataSet[SENSOR_GCQ].item[i];
        // 공백 제거
        colName.erase(0, colName.find_first_not_of(" \t"));
        colName.erase(colName.find_last_not_of(" \t") + 1);
        csvColumnMap[colName] = i;
    }

    // LOG 파일의 열 매핑
    for (int i = 0; i < inputData.item.size(); i++)
    {
        logColumnMap[inputData.item[i]] = i;
    }

    // 필요한 열 찾기
    int numOfBS_csv = -1;
    int numOfLA_csv = -1;
    int numOfSA_csv = -1;
    int numOfBSpin_csv = -1;

    // CSV 열 찾기 (대소문자 구분 없이, 유연하게)
    for (const auto& pair : csvColumnMap)
    {
        string lowerCol = pair.first;
        transform(lowerCol.begin(), lowerCol.end(), lowerCol.begin(), ::tolower);

        if (lowerCol.find("ball") != string::npos && lowerCol.find("speed") != string::npos)
            numOfBS_csv = pair.second;
        else if (lowerCol.find("launch") != string::npos && lowerCol.find("angle") != string::npos)
            numOfLA_csv = pair.second;
        else if (lowerCol.find("side") != string::npos && lowerCol.find("angle") != string::npos)
            numOfSA_csv = pair.second;
        else if (lowerCol.find("backspin") != string::npos)
            numOfBSpin_csv = pair.second;
    }

    // 대체 이름으로 다시 찾기
    if (numOfBS_csv == -1 && csvColumnMap.find("BallSpeed") != csvColumnMap.end())
        numOfBS_csv = csvColumnMap["BallSpeed"];
    if (numOfLA_csv == -1 && csvColumnMap.find("LaunchAngle") != csvColumnMap.end())
        numOfLA_csv = csvColumnMap["LaunchAngle"];
    if (numOfSA_csv == -1 && csvColumnMap.find("SideAngle") != csvColumnMap.end())
        numOfSA_csv = csvColumnMap["SideAngle"];
    if (numOfBSpin_csv == -1 && csvColumnMap.find("Backspin") != csvColumnMap.end())
        numOfBSpin_csv = csvColumnMap["Backspin"];

    // LOG 열 인덱스
    int numOfBS_log = logColumnMap.count("BallSpeed") ? logColumnMap["BallSpeed"] : -1;
    int numOfLA_log = logColumnMap.count("LaunchAngle") ? logColumnMap["LaunchAngle"] : -1;
    int numOfSA_log = logColumnMap.count("BallDirection") ? logColumnMap["BallDirection"] : -1;
    int numOfBSpin_log = logColumnMap.count("BackSpin") ? logColumnMap["BackSpin"] : -1;
    int numOfDate_log = logColumnMap.count("Date") ? logColumnMap["Date"] : -1;
    int numOfTime_log = logColumnMap.count("Time") ? logColumnMap["Time"] : -1;

    // 필수 열 확인
    if (numOfLA_csv == -1 || numOfSA_csv == -1 || numOfBSpin_csv == -1)
    {
        std::cout << "Error: Required columns not found in GCQ CSV file" << std::endl;
        std::cout << "  Launch Angle column: " << (numOfLA_csv == -1 ? "NOT FOUND" : "Found") << std::endl;
        std::cout << "  Side Angle column: " << (numOfSA_csv == -1 ? "NOT FOUND" : "Found") << std::endl;
        std::cout << "  Backspin column: " << (numOfBSpin_csv == -1 ? "NOT FOUND" : "Found") << std::endl;
        return;
    }

    if (numOfLA_log == -1 || numOfSA_log == -1 || numOfBSpin_log == -1 ||
        numOfDate_log == -1 || numOfTime_log == -1)
    {
        std::cout << "Error: Required columns not found in GCQ LOG file" << std::endl;
        return;
    }

    // 시간 정보를 추가한 임시 데이터셋
    stShotInfo tempDataSet;
    tempDataSet.item = m_dataSet[SENSOR_GCQ].item;
    tempDataSet.item.insert(tempDataSet.item.begin(), "Date-Time");

    vector<vector<string>> dataVecVec;
    int matchedCount = 0;
    int unmatchedCount = 0;

    // 매칭 프로세스
    for (int i = 0; i < inputData.dataVec.size(); i++)
    {
        // LOG 데이터 범위 검사
        const auto& logRow = inputData.dataVec[i];
        if (numOfLA_log >= logRow.size() ||
            numOfSA_log >= logRow.size() ||
            numOfBSpin_log >= logRow.size() ||
            numOfDate_log >= logRow.size() ||
            numOfTime_log >= logRow.size())
        {
            std::cout << "Warning: LOG data row " << i << " has insufficient columns" << std::endl;
            continue;
        }

        bool matchFound = false;

        // CSV 데이터와 매칭
        for (int j = 0; j < m_dataSet[SENSOR_GCQ].dataVec.size(); j++)
        {
            const auto& csvRow = m_dataSet[SENSOR_GCQ].dataVec[j];

            // CSV 데이터 범위 검사
            if (numOfLA_csv >= csvRow.size() ||
                numOfSA_csv >= csvRow.size() ||
                numOfBSpin_csv >= csvRow.size())
            {
                continue;
            }

            // 빈 데이터 체크
            if (logRow[numOfLA_log].empty() || csvRow[numOfLA_csv].empty() ||
                logRow[numOfSA_log].empty() || csvRow[numOfSA_csv].empty() ||
                logRow[numOfBSpin_log].empty() || csvRow[numOfBSpin_csv].empty())
            {
                continue;
            }

            try
            {
                // 값 비교 - 허용 오차 설정
                double logLA = stod(logRow[numOfLA_log]);
                double csvLA = stod(csvRow[numOfLA_csv]);
                double logSA = stod(logRow[numOfSA_log]);
                double csvSA = stod(csvRow[numOfSA_csv]);
                double logBS = stod(logRow[numOfBSpin_log]);
                double csvBS = stod(csvRow[numOfBSpin_csv]);

                // 반올림하여 비교 (소수점 첫째자리)
                int logLA_int = (int)round(logLA * 10);
                int csvLA_int = (int)round(csvLA * 10);
                int logSA_int = (int)round(logSA * 10);
                int csvSA_int = (int)round(csvSA * 10);
                int logBS_int = (int)round(logBS * 10);
                int csvBS_int = (int)round(csvBS * 10);

                // 매칭 조건: 세 값이 모두 일치
                if (logLA_int == csvLA_int &&
                    logSA_int == csvSA_int &&
                    logBS_int == csvBS_int)
                {
                    vector<string> tempDataVec = csvRow;

                    // 시간 정보 추가
                    string dateTime = logRow[numOfDate_log] + "_" + logRow[numOfTime_log];
                    tempDataVec.insert(tempDataVec.begin(), dateTime);

                    dataVecVec.push_back(tempDataVec);
                    matchFound = true;
                    matchedCount++;
                    break;
                }
            }
            catch (const std::exception& e)
            {
                // 변환 오류는 무시하고 계속 진행
                continue;
            }
        }

        if (!matchFound)
        {
            unmatchedCount++;
            if (unmatchedCount <= 5)  // 처음 5개만 출력
            {
                std::cout << "No match found for LOG entry " << i
                    << " (LA: " << logRow[numOfLA_log]
                    << ", SA: " << logRow[numOfSA_log]
                    << ", BS: " << logRow[numOfBSpin_log] << ")" << std::endl;
            }
        }
    }

    // 결과 저장
    tempDataSet.dataVec = dataVecVec;
    m_dataSet[SENSOR_GCQ].item = tempDataSet.item;
    m_dataSet[SENSOR_GCQ].dataVec = tempDataSet.dataVec;

    std::cout << "GCQ time synchronization complete:" << std::endl;
    std::cout << "  Total LOG entries: " << inputData.dataVec.size() << std::endl;
    std::cout << "  Matched: " << matchedCount << std::endl;
    std::cout << "  Unmatched: " << unmatchedCount << std::endl;
}


void DataLoader::addTrackManTime()
{
    // csv 파일의 item 중 Date 위치 추출
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_TM].item.size(); i++)
    {
        if (m_dataSet[SENSOR_TM].item[i] == "Date")
            nCsvTime = i;
    }


    // TrackMan의 csv 데이터에 시간 정보 추가함
    m_dataSet[SENSOR_TM].item.insert(m_dataSet[SENSOR_TM].item.begin(), "Date-Time");


    for (int i = 0; i < m_dataSet[SENSOR_TM].dataVec.size(); i++)
    {
        string val = m_dataSet[SENSOR_TM].dataVec[i][nCsvTime];
        vector<string> strVec = split(val, " ");
        string date = strVec[0];
        string ampm = strVec[1];
        string time = strVec[2];

        for (int i = 0; i < date.length(); i++)
            date.erase(find(date.begin(), date.end(), '-'));

        for (int i = 0; i < time.length(); i++)
            time.erase(find(time.begin(), time.end(), ':'));

        
        //24시간제로 변경
        if (ampm == "PM")
        {
            if (stoi(time.substr(0, 2)) == 12)
            {
                int val = stoi(time);
                time = to_string(val);
            }
            else
            {
                int val = stoi(time) + 120000;
                time = to_string(val);
            }

        }
        else
            time = "0" + time;

        val = date + "_" + time;
        m_dataSet[SENSOR_TM].dataVec[i].insert(m_dataSet[SENSOR_TM].dataVec[i].begin(), val);
    }
}

void DataLoader::addMaxTime()
{
    // csv 파일의 item 중 Date 위치 추출
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_MAX].item.size(); i++)
    {
        if (m_dataSet[SENSOR_MAX].item[i] == "Time")
            nCsvTime = i;
    }


    // Max의 csv 데이터에 시간 정보 추가함
    m_dataSet[SENSOR_MAX].item.insert(m_dataSet[SENSOR_MAX].item.begin(), "Date-Time");

    for (int i = 0; i < m_dataSet[SENSOR_MAX].dataVec.size(); i++)
    {
        string val = m_dataSet[SENSOR_MAX].dataVec[i][nCsvTime];
        vector<string> strVec = split(val, " ");
        string date = strVec[1];
        string time = strVec[2];

        for (int i = 0; i < date.length(); i++)
            date.erase(find(date.begin(), date.end(), '.'));

        for (int i = 0; i < time.length(); i++)
            time.erase(find(time.begin(), time.end(), ':'));

        val = date + "_" + time;
        m_dataSet[SENSOR_MAX].dataVec[i].insert(m_dataSet[SENSOR_MAX].dataVec[i].begin(), val);
    }
}

void DataLoader::addWaveTime()
{
    // csv 파일의 item 중 Date 위치 추출
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_WAVE].item.size(); i++)
    {
        if (m_dataSet[SENSOR_WAVE].item[i] == "Name")
            nCsvTime = i;
    }


    // Wave의 csv 데이터에 시간 정보 추가함
    m_dataSet[SENSOR_WAVE].item.insert(m_dataSet[SENSOR_WAVE].item.begin(), "Date-Time");

    for (int i = 0; i < m_dataSet[SENSOR_WAVE].dataVec.size(); i++)
    {
        string val = m_dataSet[SENSOR_WAVE].dataVec[i][nCsvTime];
        vector<string> strVec = split(val, "_");
        string date = strVec[1];
        string time = strVec[2];

        for (int i = 0; i < date.length(); i++)
            date.erase(find(date.begin(), date.end(), '.'));

        for (int i = 0; i < time.length(); i++)
            time.erase(find(time.begin(), time.end(), '.'));

        val = date + "_" + time;
        m_dataSet[SENSOR_WAVE].dataVec[i].insert(m_dataSet[SENSOR_WAVE].dataVec[i].begin(), val);
    }
}

void DataLoader::addNXTime()
{
    // csv 파일의 item 중 Date 위치 추출
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_NX].item.size(); i++)
    {
        if (m_dataSet[SENSOR_NX].item[i] == "Date")
            nCsvTime = i;
    }


    // NX의 csv 데이터에 시간 정보 추가함
    m_dataSet[SENSOR_NX].item.insert(m_dataSet[SENSOR_NX].item.begin(), "Date-Time");

    for (int i = 0; i < m_dataSet[SENSOR_NX].dataVec.size(); i++)
    {
        string val = m_dataSet[SENSOR_NX].dataVec[i][nCsvTime];
        vector<string> strVec = split(val, "_");
        string date = strVec[0];
        string time = strVec[1];

        for (int i = 0; i < date.length(); i++)
            date.erase(find(date.begin(), date.end(), '.'));

        for (int i = 0; i < time.length(); i++)
            time.erase(find(time.begin(), time.end(), '.'));

        val = date + "_" + time;
        m_dataSet[SENSOR_NX].dataVec[i].insert(m_dataSet[SENSOR_NX].dataVec[i].begin(), val);
    }
}

void DataLoader::addGRadarTime()
{ // csv 파일의 item 중 Date 위치 추출
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_GRADAR].item.size(); i++)
    {
        if (m_dataSet[SENSOR_GRADAR].item[i] == "Name")
            nCsvTime = i;
    }


    // GRadar의 csv 데이터에 시간 정보 추가함
    m_dataSet[SENSOR_GRADAR].item.insert(m_dataSet[SENSOR_GRADAR].item.begin(), "Date-Time");

    for (int i = 0; i < m_dataSet[SENSOR_GRADAR].dataVec.size(); i++)
    {
        string val = m_dataSet[SENSOR_GRADAR].dataVec[i][nCsvTime];
        vector<string> strVec = split(val, "_");
        string date = strVec[1];
        string time = strVec[2];

        for (int i = 0; i < date.length(); i++)
            date.erase(find(date.begin(), date.end(), '.'));

        for (int i = 0; i < time.length(); i++)
            time.erase(find(time.begin(), time.end(), '.'));

        val = date + "_" + time;
        m_dataSet[SENSOR_GRADAR].dataVec[i].insert(m_dataSet[SENSOR_GRADAR].dataVec[i].begin(), val);
    }
}
