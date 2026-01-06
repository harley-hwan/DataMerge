#include "pch.h"
#include "DataMatching.h"
#include <ctime>
#include <cmath>

void DataMatching::setDataSet(stShotInfo inputData[SENSOR_TOTALCNT])
{
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
        m_dataSet[i] = inputData[i];
}

void DataMatching::setMaxDiffTime(int val)
{
    m_maxDiffSec = val;
}

void DataMatching::excute()
{
    matchingWithTime();
    writeCSV(m_dataSet);
}

time_t parseDateTime(const string& dateTime) {
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

void DataMatching::matchingWithTime()
{
    std::cout << "\n=== Starting time-based matching ===" << std::endl;
    std::cout << "Max time difference: " << m_maxDiffSec << " seconds" << std::endl;

    // 임시 데이터셋
    stShotInfo tempDataSet[SENSOR_TOTALCNT];

    // 헤더 정보 복사
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        tempDataSet[i].snName = m_dataSet[i].snName;
        tempDataSet[i].item = m_dataSet[i].item;
    }

    // 각 센서별 인덱스
    int sensorIdx[SENSOR_TOTALCNT] = { 0 };
    bool sensorEnd[SENSOR_TOTALCNT] = { false };
    time_t sensorTimes[SENSOR_TOTALCNT] = { 0 };

    int processedRows = 0;
    int errorCount = 0;

    while (true)
    {
        time_t minTime = LLONG_MAX;
        bool hasValidTime = false;

        // 각 센서의 현재 시간 확인
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (!sensorEnd[sensor] && m_dataSet[sensor].dataVec.size() > 0)
            {
                if (sensorIdx[sensor] < static_cast<int>(m_dataSet[sensor].dataVec.size()))
                {
                    const auto& dataRow = m_dataSet[sensor].dataVec[sensorIdx[sensor]];

                    if (!dataRow.empty() && !dataRow[0].empty())
                    {
                        time_t currentTime = parseDateTime(dataRow[0]);

                        if (currentTime > 0)
                        {
                            sensorTimes[sensor] = currentTime;
                            if (currentTime < minTime) {
                                minTime = currentTime;
                                hasValidTime = true;
                            }
                        }
                        else
                        {
                            // 파싱 실패 - 다음 행으로
                            std::cout << "Warning: Invalid time format for sensor "
                                << m_dataSet[sensor].snName << " at row "
                                << sensorIdx[sensor] << ": " << dataRow[0] << std::endl;
                            sensorIdx[sensor]++;
                            errorCount++;
                            continue;
                        }
                    }
                    else
                    {
                        sensorIdx[sensor]++;
                        continue;
                    }
                }
                else
                {
                    sensorEnd[sensor] = true;
                }
            }
            else
            {
                sensorEnd[sensor] = true;
            }
        }

        // 모든 센서가 종료되었는지 확인
        bool allEnd = true;
        for (int i = 0; i < SENSOR_TOTALCNT; i++)
        {
            if (!sensorEnd[i])
            {
                allEnd = false;
                break;
            }
        }

        if (allEnd || !hasValidTime)
        {
            break;
        }

        // 시간 윈도우 계산 (초 단위로 정확하게)
        time_t maxTime = minTime + m_maxDiffSec;

        // 각 센서별로 시간 범위에 맞는 데이터 추가
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (m_dataSet[sensor].dataVec.size() > 0)
            {
                if (!sensorEnd[sensor] &&
                    sensorTimes[sensor] >= minTime &&
                    sensorTimes[sensor] <= maxTime)
                {
                    // 시간 범위 내에 있음
                    tempDataSet[sensor].dataVec.push_back(
                        m_dataSet[sensor].dataVec[sensorIdx[sensor]++]
                    );
                }
                else
                {
                    // 빈 데이터 생성
                    size_t dataSize = m_dataSet[sensor].item.size();
                    vector<string> emptyData(dataSize, "");
                    tempDataSet[sensor].dataVec.push_back(emptyData);
                }
            }
        }

        processedRows++;

        if (processedRows % 100 == 0)
        {
            std::cout << "Processing... " << processedRows << " rows completed" << std::endl;
        }
    }

    // 결과 저장
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        m_dataSet[i].dataVec = tempDataSet[i].dataVec;
    }

    std::cout << "\n=== Time matching complete ===" << std::endl;
    std::cout << "Total processed rows: " << processedRows << std::endl;
    std::cout << "Errors encountered: " << errorCount << std::endl;

    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        if (!m_dataSet[i].dataVec.empty())
        {
            std::cout << "  " << m_dataSet[i].snName
                << ": " << m_dataSet[i].dataVec.size() << " rows" << std::endl;
        }
    }
}

void DataMatching::writeCSV(stShotInfo inputData[SENSOR_TOTALCNT])
{
    time_t timer;
    tm t;
    timer = time(nullptr);
    localtime_s(&t, &timer);

    char buf[80];
    strftime(buf, sizeof(buf), "..\\DataMerge_%Y%m%d_%H%M%S.csv", &t);

    ofstream file(buf);

    if (!file.is_open()) {
        std::cout << "ERROR: Failed to create output file: " << buf << std::endl;
        return;
    }

    std::cout << "\n=== Writing CSV file: " << buf << " ===" << std::endl;

    // 데이터 이름 (헤더)
    string itemStr;
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        if (inputData[i].snName == "")
            continue;

        itemStr += "SensorName,";

        for (size_t idx = 0; idx < inputData[i].item.size(); idx++)
            itemStr += inputData[i].item[idx] + "(" + inputData[i].snName + ")" + ",";

        itemStr += ",";
    }
    file << itemStr << endl;

    int maxShotCnt = 0;
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        int cnt = static_cast<int>(inputData[i].dataVec.size());
        if (cnt > maxShotCnt)
            maxShotCnt = cnt;
    }

    // 데이터 행 작성
    for (int i = 0; i < maxShotCnt; i++)
    {
        string str;
        for (int j = 0; j < SENSOR_TOTALCNT; j++)
        {
            if (inputData[j].snName == "")
                continue;

            str += inputData[j].snName + ",";

            if (inputData[j].dataVec.size() == 0)
            {
                for (size_t idx = 0; idx < inputData[j].item.size(); idx++)
                    str += ", ";
            }
            else if (i < static_cast<int>(inputData[j].dataVec.size()))
            {
                for (size_t idx = 0; idx < inputData[j].dataVec[i].size(); idx++)
                    str += inputData[j].dataVec[i][idx] + ", ";
            }
            else
            {
                // 이 센서의 데이터가 부족한 경우
                for (size_t idx = 0; idx < inputData[j].item.size(); idx++)
                    str += ", ";
            }

            str += ", ";
        }
        file << str << endl;
    }

    file.close();
    std::cout << "CSV file created successfully!" << std::endl;
    std::cout << "Total rows written: " << maxShotCnt << std::endl;
}

vector<string> DataMatching::split(string inputStr, string delimiter)
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
