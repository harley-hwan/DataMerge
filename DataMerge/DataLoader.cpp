#include "pch.h"
#include "DataLoader.h"
#include <filesystem>

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
        return;

    std::ifstream logFile(logFilePath);
    if (true == logFile.fail())
        return;

    m_dataSet[SENSOR_GCQ].snName = "GCQ";

    // csv 파일 읽으면서 데이터 추출
    while (getline(csvFile, line))
    {
        // 맨 윗줄 데이터 이름만 추출
        if (line.find("Foresight") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            vector<string> itemVec = split(line, ",");

            // 콤마로 데이터 이름 분류했는데 빈칸인 경우 있음
            vector<string> newItemVec;
            for (int i = 0; i < itemVec.size(); i++)
            {
                if (itemVec[i] == "")
                    continue;
                else
                    newItemVec.push_back(itemVec[i]);
            }

            m_dataSet[SENSOR_GCQ].item = newItemVec;
            continue;
        }


        if (line.find("M/S") != string::npos)
            continue;

        if (line.find("Average") != string::npos)
            continue;

        if (line.find("Std.") != string::npos)
            continue;


        // 데이터 부분만 추출
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataVec = split(line, ",");

        // 콤마로 데이터 이름 분류했는데 빈칸인 경우 있음
        vector<string> newDataVec;
        for (int i = 0; i < dataVec.size(); i++)
        {
            if (dataVec[i] == "")
                continue;
            else
                newDataVec.push_back(dataVec[i]);
        }
         

        if (dataVec[0] == "")
            continue;

        m_dataSet[SENSOR_GCQ].dataVec.push_back(newDataVec);
    }



    // log 파일 읽기
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

    // txt 파일 읽으면서 데이터 추출
    while (getline(logFile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            line.erase(find(line.begin(), line.end(), '\0'));
            line.erase(find(line.begin(), line.end(), '\r'));
        }

        // Date는 로그에 한번만 기록되어 한번만 추출됨(Date: 02/12/2024   Time: 16:15:15)
        if (line.find("Date:") == 0)
        {
            // 날짜 추출 (02/12/2024)
            string splitDate = "Date: ";
            string tmp = line.substr(splitDate.length(), 10);
            vector<string> dateVec = split(tmp, "/");
            date = dateVec[2] + dateVec[1] + dateVec[0];
            continue;
        }

        if (line.find("Ball Speed (MPH)") != string::npos)
        {
            // 시간 추출(16:15:15)
            string tmp = line.substr(line.find("[") + 1, 8);
            vector<string> dateVec = split(tmp, ":");
            string time = dateVec[0] + dateVec[1] + dateVec[2];

            // 위에서 날짜 추출하고, 매번 저장
            dataVec.push_back(date);
            dataVec.push_back(time);

            // 볼 스피드 추출 ([16:22:39:189] Ball Speed (MPH)    = 98.22)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }

        if (line.find("Launch Angle (Deg)") != string::npos)
        {
            // 볼 발사각 추출 ([16:21:38:262] Launch Angle (Deg)  = 19.0)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }

        if (line.find("Azimuth (Deg)") != string::npos)
        {
            // 볼 방향각 추출 ([16:21:38:262] Azimuth (Deg)       = 1.1)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }


        // 해당 문구가 나오면 그 다음 줄의 스핀 정보 추출
        if (line.find("Processing SPIN data") != string::npos)
            isSpinInfo = true;

        if (isSpinInfo)
        {
            if (line.find("BackSpinRpm") != string::npos)
            {
                vector<string> datastr = split(line, "=");
                dataVec.push_back(datastr[1]);
                continue;
            }

            if (line.find("SideSpinRpm") != string::npos)
            {
                vector<string> datastr = split(line, "=");
                dataVec.push_back(datastr[1]);
                continue;
            }

            if (line.find("TotalSpinRpm") != string::npos)
            {
                vector<string> dataStr = split(line, "=");
                dataVec.push_back(dataStr[1]);

                // 총 스핀량까지 추출하면 로그에서는 그만 뽑아도 됨
                dataVecVec.push_back(dataVec);
                dataVec.clear();
                isSpinInfo = false;
                continue;
            }
        }
    }

    logDataSet.dataVec = dataVecVec;


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
    // csv 파일의 item 중 매칭하고 싶은 변수의 열 위치 추출
    int numOfBS_csv = -1;
    int numOfLA_csv = -1;
    int numOfSA_csv = -1;
    int numOfBSpin_csv = -1;

    for (int i = 0; i < m_dataSet[SENSOR_GCQ].item.size(); i++)
    {
        if (m_dataSet[SENSOR_GCQ].item[i] == "BallSpeed")
            numOfBS_csv = i;
        if (m_dataSet[SENSOR_GCQ].item[i] == "LaunchAngle")
            numOfLA_csv = i;
        if (m_dataSet[SENSOR_GCQ].item[i] == "SideAngle")
            numOfSA_csv = i;
        if (m_dataSet[SENSOR_GCQ].item[i] == "Backspin")
            numOfBSpin_csv = i;
    }

    // log 파일의 item 중 매칭하고 싶은 변수의 열 위치 추출
    int numOfBS_log = -1;
    int numOfLA_log = -1;
    int numOfSA_log = -1;
    int numOfBSpin_log = -1;
    int numOfDate_log = -1;
    int numOfTime_log = -1;

    for (int i = 0; i < inputData.item.size(); i++)
    {
        if (inputData.item[i] == "BallSpeed")
            numOfBS_log = i;
        if (inputData.item[i] == "LaunchAngle")
            numOfLA_log = i;
        if (inputData.item[i] == "BallDirection")
            numOfSA_log = i;
        if (inputData.item[i] == "BackSpin")
            numOfBSpin_log = i;
        if (inputData.item[i] == "Date")
            numOfDate_log = i;
        if (inputData.item[i] == "Time")
            numOfTime_log = i;
    }

    // csv 파일과 item 매칭위해 데이터만 임시 저장 
    stShotInfo tempDataSet[SENSOR_TOTALCNT];
    tempDataSet[SENSOR_GCQ].item = m_dataSet[SENSOR_GCQ].item;
    tempDataSet[SENSOR_GCQ].item.insert(tempDataSet[SENSOR_GCQ].item.begin(), "Date-Time");

    vector<vector<string>> dataVecVec;

    // csv의 값과 log의 값과 확인 후 log의 시간 정보 추가
    for (int i = 0; i < inputData.dataVec.size(); i++)
    {
        if (i == 9)
            int a = 0;

        // LOG 데이터 범위 검사
        if (numOfLA_log >= inputData.dataVec[i].size() ||
            numOfSA_log >= inputData.dataVec[i].size() ||
            numOfBSpin_log >= inputData.dataVec[i].size() ||
            numOfDate_log >= inputData.dataVec[i].size() ||
            numOfTime_log >= inputData.dataVec[i].size())
        {
            std::cout << "Err: LOG data[" << i << "] insufficient columns (size: " << inputData.dataVec[i].size() << ")" << std::endl;
            continue;
        }

        for (int j = 0; j < m_dataSet[SENSOR_GCQ].dataVec.size(); j++)
        {
            // CSV 데이터 범위 검사
            if (numOfLA_csv >= m_dataSet[SENSOR_GCQ].dataVec[j].size() ||
                numOfSA_csv >= m_dataSet[SENSOR_GCQ].dataVec[j].size() ||
                numOfBSpin_csv >= m_dataSet[SENSOR_GCQ].dataVec[j].size())
            {
                std::cout << "Err: CSV data[" << j << "] insufficient columns (size: " << m_dataSet[SENSOR_GCQ].dataVec[j].size() << ")" << std::endl;
                continue;
            }

            //bool isSameBS = false;
            bool isSameLA = false;
            bool isSameSA = false;
            bool isSameBSpin = false;
            bool isSameSS = false;

            try {
                // 볼 발사각 값 일치 여부 확인 - 인덱스 수정됨!           
                double dLogVal_la = stof(inputData.dataVec[i][numOfLA_log]);  // LOG 인덱스 사용
                double dCsvVal_la = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfLA_csv]);  // CSV 인덱스 사용

                // 부동소수점때문에 소수점 첫째자리 형태로 만든 후 정수로 비교
                int nCsvVal_la = (int)(round(dCsvVal_la * 10) / 10 * 1000);
                int nLogVal_la = (int)(round(dLogVal_la * 10) / 10 * 1000);

                if (nCsvVal_la == nLogVal_la)
                    isSameLA = true;

                // 볼 방향각 값 일치 여부 확인            
                double dLogVal_sa = stof(inputData.dataVec[i][numOfSA_log]);
                double dCsvVal_sa = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfSA_csv]);

                // 부동소수점때문에 소수점 첫째자리 형태로 만든 후 정수로 비교
                int nCsvVal_sa = (int)(round(dCsvVal_sa * 10) / 10 * 1000);
                int nLogVal_sa = (int)(round(dLogVal_sa * 10) / 10 * 1000);

                if (nCsvVal_sa == nLogVal_sa)
                    isSameSA = true;

                // 백스핀 값 일치 여부 확인            
                double dLogVal_bs = stof(inputData.dataVec[i][numOfBSpin_log]);
                double dCsvVal_bs = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfBSpin_csv]);

                // 부동소수점때문에 소수점 첫째자리 형태로 만든 후 정수로 비교
                int nCsvVal_bs = (int)(round(dCsvVal_bs * 10) / 10 * 1000);
                int nLogVal_bs = (int)(round(dLogVal_bs * 10) / 10 * 1000);

                if (nCsvVal_bs == nLogVal_bs)
                    isSameBSpin = true;

            }
            catch (const std::exception& e) {
                std::cout << "Err occurred (i=" << i << ", j=" << j << "): " << e.what() << std::endl;
                continue;
            }

            // 세개 데이터가 모두 동일할 때 해당 시간의 데이터를 csv에서 가져옴
            //if (isSameBS == true && isSameLA == true && isSameSA == true)
            if (isSameBSpin == true && isSameLA == true && isSameSA == true)
            {
                vector<string> tempDataVec = m_dataSet[SENSOR_GCQ].dataVec[j];

                // 시간 정보 추가함               
                string val = inputData.dataVec[i][numOfDate_log] + "_" + inputData.dataVec[i][numOfTime_log];
                tempDataVec.insert(tempDataVec.begin(), val);
                dataVecVec.push_back(tempDataVec);
                break;
            }
        }
    }

    tempDataSet[SENSOR_GCQ].dataVec = dataVecVec;

    // 시간 추가하여 데이터만 다시 원위치
    m_dataSet[SENSOR_GCQ].item = tempDataSet[SENSOR_GCQ].item;
    m_dataSet[SENSOR_GCQ].dataVec = tempDataSet[SENSOR_GCQ].dataVec;
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
