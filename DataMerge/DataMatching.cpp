#include "pch.h"
#include "DataMatching.h"

void DataMatching::setDataSet(stShotInfo inputData[SENSOR_TOTALCNT])
{
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
        m_dataSet[i] = inputData[i];
}

void DataMatching::setMaxDiffTime(int val)
{
    // 몇초 안에 오는것을 동시간에 센싱된 것이라 볼건지
    m_maxDiffSec = val;
}

void DataMatching::excute()
{
	matchingWithTime();
	writeCSV(m_dataSet);
}

void DataMatching::matchingWithTime()
{
    // 로드된 데이터셋과 시간으로 매칭 후 임시 저장
    stShotInfo tempDataSet[SENSOR_TOTALCNT];

    // 헤더 정보 복사
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        tempDataSet[i].snName = m_dataSet[i].snName;
        tempDataSet[i].item = m_dataSet[i].item;
    }

    // 각 센서별 인덱스
    int sensorIdx[SENSOR_TOTALCNT] = { 0 };

    // 각 센서 데이터 종료 플래그
    bool sensorEnd[SENSOR_TOTALCNT] = { false };

    // 각 센서별 현재 시간 저장
    int sensorTimes[SENSOR_TOTALCNT] = { 0 };

    int processedRows = 0;
    int errorCount = 0;

    // 매칭 프로세스
    while (true)
    {
        int minTime = INT_MAX;

        // 각 센서의 현재 시간 확인
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (!sensorEnd[sensor] && m_dataSet[sensor].dataVec.size() > 0)
            {
                if (sensorIdx[sensor] < m_dataSet[sensor].dataVec.size())
                {
                    const auto& dataRow = m_dataSet[sensor].dataVec[sensorIdx[sensor]];

                    // 데이터가 있고 첫 열(시간)이 비어있지 않은지 확인
                    if (!dataRow.empty() && !dataRow[0].empty())
                    {
                        vector<string> dateTimeStr = split(dataRow[0], "_");

                        if (dateTimeStr.size() >= 2)
                        {
                            try
                            {
                                // 시간을 정수로 변환
                                string timeStr = dateTimeStr[1];
                                // 숫자가 아닌 문자 제거
                                timeStr.erase(remove_if(timeStr.begin(), timeStr.end(),
                                    [](char c) { return !isdigit(c); }), timeStr.end());

                                if (!timeStr.empty())
                                {
                                    sensorTimes[sensor] = stoi(timeStr);
                                    minTime = min(minTime, sensorTimes[sensor]);
                                }
                                else
                                {
                                    std::cout << "Warning: Invalid time format for sensor "
                                        << sensor << " at row " << sensorIdx[sensor] << std::endl;
                                    sensorIdx[sensor]++;
                                    continue;
                                }
                            }
                            catch (const std::exception& e)
                            {
                                std::cout << "Error parsing time for sensor " << sensor
                                    << ": " << e.what() << std::endl;
                                errorCount++;
                                sensorIdx[sensor]++;
                                continue;
                            }
                        }
                        else
                        {
                            // 시간 형식이 잘못된 경우 건너뛰기
                            sensorIdx[sensor]++;
                            continue;
                        }
                    }
                    else
                    {
                        // 빈 데이터인 경우 다음으로
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

        if (allEnd || minTime == INT_MAX)
        {
            break;
        }

        // 시간 범위 계산 (오버플로우 처리 포함)
        int diffTime = minTime + m_maxDiffSec;

        // 초 처리 (60초 넘어가면 +1분)
        int seconds = diffTime % 100;
        if (seconds >= 60)
        {
            diffTime += 100;
            diffTime -= 60;
        }

        // 분 처리 (60분 넘어가면 +1시간)
        int minutes = (diffTime / 100) % 100;
        if (minutes >= 60)
        {
            diffTime += 10000;
            diffTime -= 6000;
        }

        // 각 센서별로 시간 범위에 맞는 데이터 추가
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (m_dataSet[sensor].dataVec.size() > 0)
            {
                // 센서가 시간 범위 내에 있고 아직 끝나지 않았으면
                if (!sensorEnd[sensor] &&
                    sensorTimes[sensor] >= minTime &&
                    sensorTimes[sensor] <= diffTime)
                {
                    tempDataSet[sensor].dataVec.push_back(
                        m_dataSet[sensor].dataVec[sensorIdx[sensor]++]
                    );
                }
                else
                {
                    // 빈 데이터 생성
                    vector<string> emptyData;

                    // 해당 센서의 적절한 크기로 빈 데이터 생성
                    size_t dataSize = 0;

                    // 현재 행의 크기 사용
                    if (!m_dataSet[sensor].dataVec.empty() &&
                        sensorIdx[sensor] < m_dataSet[sensor].dataVec.size())
                    {
                        dataSize = m_dataSet[sensor].dataVec[sensorIdx[sensor]].size();
                    }
                    // 이전 행의 크기 사용
                    else if (!m_dataSet[sensor].dataVec.empty() && sensorIdx[sensor] > 0)
                    {
                        dataSize = m_dataSet[sensor].dataVec[sensorIdx[sensor] - 1].size();
                    }
                    // 헤더 크기 + 1 사용 (Date-Time 열 포함)
                    else if (!m_dataSet[sensor].item.empty())
                    {
                        dataSize = m_dataSet[sensor].item.size() + 1;
                    }
                    // 기본값
                    else
                    {
                        dataSize = 10; // 적절한 기본값
                    }

                    emptyData.resize(dataSize, "");
                    tempDataSet[sensor].dataVec.push_back(emptyData);
                }
            }
        }

        processedRows++;

        // 진행 상황 표시 (100행마다)
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

    std::cout << "Time matching complete:" << std::endl;
    std::cout << "  Total processed rows: " << processedRows << std::endl;
    std::cout << "  Errors encountered: " << errorCount << std::endl;

    // 각 센서별 결과 요약
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

    if (!file.is_open())
        return;

    // 2024/12/17일 송승혁 프로님 요청으로 첫번째 행 센서 이름 제거
    //// 첫번째 행- 센서 이름
    //string nameStr;
    //for (int i = 0; i < SENSOR_TOTALCNT; i++)
    //{
    //    if (inputData[i].snName == "")
    //        continue;

    //    for (int idx = 0; idx < inputData[i].item.size(); idx++)
    //    {
    //        if (idx == 0)
    //            nameStr += inputData[i].snName + ",";
    //        else
    //            nameStr += ",";
    //    }

    //    // 센서 사이 열 띄우기
    //    nameStr += ",";
    //}
    //file << nameStr << endl;



    // 두번째 행- 데이터 이름
    string itemStr;
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        if (inputData[i].snName == "")
            continue;

        itemStr += "SensorName,";

        for (int idx = 0; idx < inputData[i].item.size(); idx++)
            itemStr += inputData[i].item[idx] + "(" + inputData[i].snName + ")" + ",";

        // 센서 사이 열 띄우기
        itemStr += ",";
    }
    file << itemStr << endl;




    int maxShotCnt = -1;
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        int cnt = inputData[i].dataVec.size();
        if (cnt > maxShotCnt)
            maxShotCnt = cnt;
    }

#if 0
    // 하나의 센서라도 값이 없으면 삭제
    for (int i = 0; i < maxShotCnt; i++)
    {
        int zeroCnt = 0;
        string str;
        string strTmp;
        for (int j = 0; j < SENSOR_TOTALCNT; j++)
        {
            if (inputData[j].snName == "")
                continue;

            strTmp += inputData[j].snName + ",";


            if (i < inputData[j].dataVec.size())
            {
                if (inputData[j].dataVec[i][0] == "")
                    zeroCnt += 1;

                for (int idx = 0; idx < inputData[j].dataVec[i].size(); idx++)
                    strTmp += inputData[j].dataVec[i][idx] + ",";

            }
            // 데이터 매칭 안된 경우
            else
                zeroCnt += 1;


            strTmp += ",";
        }

        if (zeroCnt != 0)
            continue;
        else
            str = strTmp;


        // 센서 사이 열 띄우기
        str += ",";

        file << str << endl;
    }
#else

    // 빈 셀 있어도 그냥 저장
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
                // 2025/02/04 김장환 프로님 요청으로, 데이터 비어있을 때 자리는 유지해 달라.
                // 파일에 데이터 값은 없고 항목만 있을 때
                for (int idx = 0; idx < inputData[j].item.size(); idx++)
                    str += ", ";
            }
            else
            {
                for (int idx = 0; idx < inputData[j].dataVec[i].size(); idx++)
                    str += inputData[j].dataVec[i][idx] + ", ";
            }

            // 센서 사이 열 띄우기
            str += ", ";
        }
        file << str << endl;
    }
#endif

    file.close();
}

vector<string> DataMatching::split(string inputStr, string delimiter)
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
