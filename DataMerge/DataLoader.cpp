#include "pch.h"
#include "DataLoader.h"
#include <filesystem>

namespace fs = std::filesystem;

void DataLoader::loadIncludePath(string path)
{
    // �� �������� ���� ���� ������ �� ����

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

    // csv ���� �����鼭 ������ ����
    while (getline(csvFile, line))
    {
        // �� ���� ������ �̸��� ����
        if (line.find("Foresight") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            vector<string> itemVec = split(line, ",");

            // �޸��� ������ �̸� �з��ߴµ� ��ĭ�� ��� ����
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


        // ������ �κи� ����
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataVec = split(line, ",");

        // �޸��� ������ �̸� �з��ߴµ� ��ĭ�� ��� ����
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



    // log ���� �б�
    stShotInfo logDataSet;
    logDataSet.snName = "GCQlog";

    // log ������ �׸�
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

    // txt ���� �����鼭 ������ ����
    while (getline(logFile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            line.erase(find(line.begin(), line.end(), '\0'));
            line.erase(find(line.begin(), line.end(), '\r'));
        }

        // Date�� �α׿� �ѹ��� ��ϵǾ� �ѹ��� �����(Date: 02/12/2024   Time: 16:15:15)
        if (line.find("Date:") == 0)
        {
            // ��¥ ���� (02/12/2024)
            string splitDate = "Date: ";
            string tmp = line.substr(splitDate.length(), 10);
            vector<string> dateVec = split(tmp, "/");
            date = dateVec[2] + dateVec[1] + dateVec[0];
            continue;
        }

        if (line.find("Ball Speed (MPH)") != string::npos)
        {
            // �ð� ����(16:15:15)
            string tmp = line.substr(line.find("[") + 1, 8);
            vector<string> dateVec = split(tmp, ":");
            string time = dateVec[0] + dateVec[1] + dateVec[2];

            // ������ ��¥ �����ϰ�, �Ź� ����
            dataVec.push_back(date);
            dataVec.push_back(time);

            // �� ���ǵ� ���� ([16:22:39:189] Ball Speed (MPH)    = 98.22)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }

        if (line.find("Launch Angle (Deg)") != string::npos)
        {
            // �� �߻簢 ���� ([16:21:38:262] Launch Angle (Deg)  = 19.0)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }

        if (line.find("Azimuth (Deg)") != string::npos)
        {
            // �� ���Ⱒ ���� ([16:21:38:262] Azimuth (Deg)       = 1.1)
            vector<string> datastr = split(line, "=");
            dataVec.push_back(datastr[1]);
            continue;
        }


        // �ش� ������ ������ �� ���� ���� ���� ���� ����
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

                // �� ���ɷ����� �����ϸ� �α׿����� �׸� �̾Ƶ� ��
                dataVecVec.push_back(dataVec);
                dataVec.clear();
                isSpinInfo = false;
                continue;
            }
        }
    }

    logDataSet.dataVec = dataVecVec;


    // log���� ������ �ð� ������ �߰�
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

    // csv ���� �����鼭 ������ ����
    while (std::getline(file, line))
    {
        if (line.find("sep=,") != string::npos)
            continue;

        // �� ���� ������ �̸��� ����
        if (line.find("Date") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_TM].item = split(line, ",");
            continue;
        }

        if (line.find("m/s") != string::npos)
            continue;

        // ������ �κи� ����
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

    // csv ���� �����鼭 ������ ����
    while (std::getline(file, line))
    {
        // �� ���� ������ �̸��� ����
        if (line.find("Name") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_WAVE].item = split(line, ",");
            continue;
        }

        // ������ �κи� ����
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

    // csv ���� �����鼭 ������ ����
    while (std::getline(file, line))
    {
        // �� ���� ������ �̸��� ����
        if (line.find("Shot ID") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_MAX].item = split(line, ",");
            continue;
        }

        // ������ �κи� ����
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

    // csv ���� �����鼭 ������ ����
    while (std::getline(file, line))
    {
        if (line.find("NXSensor") != string::npos)
            continue;

        // �� ���� ������ �̸��� ����
        if (line.find("ShotDB") != string::npos)
        {
            m_dataSet[SENSOR_NX].item = split(line, ",");
            continue;
        }

        // ������ �κи� ����
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

    // csv ���� �����鼭 ������ ����
    while (std::getline(file, line))
    {
        // �� ���� ������ �̸��� ����
        if (line.find("Name") != string::npos)
        {
            for (int i = 0; i < line.length(); i++)
                line.erase(find(line.begin(), line.end(), ' '));

            m_dataSet[SENSOR_GRADAR].item = split(line, ",");
            continue;
        }

        // ������ �κи� ����
        for (int i = 0; i < line.length(); i++)
            line.erase(find(line.begin(), line.end(), '\"'));

        vector<string> dataStr = split(line, ",");
        m_dataSet[SENSOR_GRADAR].dataVec.push_back(dataStr);
    }
}

void DataLoader::excute(stShotInfo* outputData[(int)SENSOR_TOTALCNT])
{
    // GCQ�� ���� �ε��� �� �ð� ���� �߰� ��

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
    // csv ������ item �� ��Ī�ϰ� ���� ������ �� ��ġ ����
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

    // log ������ item �� ��Ī�ϰ� ���� ������ �� ��ġ ����
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

    // csv ���ϰ� item ��Ī���� �����͸� �ӽ� ���� 
    stShotInfo tempDataSet[SENSOR_TOTALCNT];
    tempDataSet[SENSOR_GCQ].item = m_dataSet[SENSOR_GCQ].item;
    tempDataSet[SENSOR_GCQ].item.insert(tempDataSet[SENSOR_GCQ].item.begin(), "Date-Time");

    vector<vector<string>> dataVecVec;

    // csv�� ���� log�� ���� Ȯ�� �� log�� �ð� ���� �߰�
    for (int i = 0; i < inputData.dataVec.size(); i++)
    {
        if (i == 9)
            int a = 0;

        // LOG ������ ���� �˻�
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
            // CSV ������ ���� �˻�
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
                // �� �߻簢 �� ��ġ ���� Ȯ�� - �ε��� ������!           
                double dLogVal_la = stof(inputData.dataVec[i][numOfLA_log]);  // LOG �ε��� ���
                double dCsvVal_la = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfLA_csv]);  // CSV �ε��� ���

                // �ε��Ҽ��������� �Ҽ��� ù°�ڸ� ���·� ���� �� ������ ��
                int nCsvVal_la = (int)(round(dCsvVal_la * 10) / 10 * 1000);
                int nLogVal_la = (int)(round(dLogVal_la * 10) / 10 * 1000);

                if (nCsvVal_la == nLogVal_la)
                    isSameLA = true;

                // �� ���Ⱒ �� ��ġ ���� Ȯ��            
                double dLogVal_sa = stof(inputData.dataVec[i][numOfSA_log]);
                double dCsvVal_sa = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfSA_csv]);

                // �ε��Ҽ��������� �Ҽ��� ù°�ڸ� ���·� ���� �� ������ ��
                int nCsvVal_sa = (int)(round(dCsvVal_sa * 10) / 10 * 1000);
                int nLogVal_sa = (int)(round(dLogVal_sa * 10) / 10 * 1000);

                if (nCsvVal_sa == nLogVal_sa)
                    isSameSA = true;

                // �齺�� �� ��ġ ���� Ȯ��            
                double dLogVal_bs = stof(inputData.dataVec[i][numOfBSpin_log]);
                double dCsvVal_bs = stof(m_dataSet[SENSOR_GCQ].dataVec[j][numOfBSpin_csv]);

                // �ε��Ҽ��������� �Ҽ��� ù°�ڸ� ���·� ���� �� ������ ��
                int nCsvVal_bs = (int)(round(dCsvVal_bs * 10) / 10 * 1000);
                int nLogVal_bs = (int)(round(dLogVal_bs * 10) / 10 * 1000);

                if (nCsvVal_bs == nLogVal_bs)
                    isSameBSpin = true;

            }
            catch (const std::exception& e) {
                std::cout << "Err occurred (i=" << i << ", j=" << j << "): " << e.what() << std::endl;
                continue;
            }

            // ���� �����Ͱ� ��� ������ �� �ش� �ð��� �����͸� csv���� ������
            //if (isSameBS == true && isSameLA == true && isSameSA == true)
            if (isSameBSpin == true && isSameLA == true && isSameSA == true)
            {
                vector<string> tempDataVec = m_dataSet[SENSOR_GCQ].dataVec[j];

                // �ð� ���� �߰���               
                string val = inputData.dataVec[i][numOfDate_log] + "_" + inputData.dataVec[i][numOfTime_log];
                tempDataVec.insert(tempDataVec.begin(), val);
                dataVecVec.push_back(tempDataVec);
                break;
            }
        }
    }

    tempDataSet[SENSOR_GCQ].dataVec = dataVecVec;

    // �ð� �߰��Ͽ� �����͸� �ٽ� ����ġ
    m_dataSet[SENSOR_GCQ].item = tempDataSet[SENSOR_GCQ].item;
    m_dataSet[SENSOR_GCQ].dataVec = tempDataSet[SENSOR_GCQ].dataVec;
}
void DataLoader::addTrackManTime()
{
    // csv ������ item �� Date ��ġ ����
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_TM].item.size(); i++)
    {
        if (m_dataSet[SENSOR_TM].item[i] == "Date")
            nCsvTime = i;
    }


    // TrackMan�� csv �����Ϳ� �ð� ���� �߰���
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

        
        //24�ð����� ����
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
    // csv ������ item �� Date ��ġ ����
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_MAX].item.size(); i++)
    {
        if (m_dataSet[SENSOR_MAX].item[i] == "Time")
            nCsvTime = i;
    }


    // Max�� csv �����Ϳ� �ð� ���� �߰���
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
    // csv ������ item �� Date ��ġ ����
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_WAVE].item.size(); i++)
    {
        if (m_dataSet[SENSOR_WAVE].item[i] == "Name")
            nCsvTime = i;
    }


    // Wave�� csv �����Ϳ� �ð� ���� �߰���
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
    // csv ������ item �� Date ��ġ ����
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_NX].item.size(); i++)
    {
        if (m_dataSet[SENSOR_NX].item[i] == "Date")
            nCsvTime = i;
    }


    // NX�� csv �����Ϳ� �ð� ���� �߰���
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
{ // csv ������ item �� Date ��ġ ����
    int nCsvTime;
    for (int i = 0; i < m_dataSet[SENSOR_GRADAR].item.size(); i++)
    {
        if (m_dataSet[SENSOR_GRADAR].item[i] == "Name")
            nCsvTime = i;
    }


    // GRadar�� csv �����Ϳ� �ð� ���� �߰���
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
