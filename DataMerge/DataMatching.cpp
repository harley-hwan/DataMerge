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
    // 로드 된 데이터셋과 시간으로 매칭후 데이터셋 임시 저장
    stShotInfo tempDataSet[SENSOR_TOTALCNT];
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        tempDataSet[i].snName = m_dataSet[i].snName;
        tempDataSet[i].item = m_dataSet[i].item;
    }

       
    int gcqIdx = 0;
    int waveIdx = 0;
    int nxIdx = 0;
    int tmIdx = 0;
    int maxIdx = 0;
    int gradarIdx = 0;

    // 각 센서 데이터 다 읽으면 멈춤
    bool isGCQEnd = false;
    bool isWaveEnd = false;
    bool isNXEnd = false;
    bool isMaxEnd = false;
    bool isTmEnd = false;
    bool isGRadarEnd = false;


    vector<string> initData;

    // 가장 먼저 센싱된 것 찾기
    while (true)
    {
        int minTime = 9999999999999999;
        int gcqTime = 0;
        int waveTime = 0;
        int nxTime = 0;
        int tmTime = 0;
        int maxTime = 0;
        int gradarTime = 0;


        // 같은 열에 있는 센서 중 가장 빠른 시간 찾기
        if (m_dataSet[SENSOR_GCQ].dataVec.size() != 0)
        {
            if (gcqIdx < m_dataSet[SENSOR_GCQ].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_GCQ].dataVec[gcqIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                gcqTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, gcqTime);
            }
            else
                isGCQEnd = true;
        }
        else
            isGCQEnd = true;

        if (m_dataSet[SENSOR_TM].dataVec.size() != 0)
        {
            if (tmIdx < m_dataSet[SENSOR_TM].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_TM].dataVec[tmIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                tmTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, tmTime);
            }
            else
                isTmEnd = true;
        }
        else
            isTmEnd = true;

        if (m_dataSet[SENSOR_WAVE].dataVec.size() != 0)
        {
            if (waveIdx < m_dataSet[SENSOR_WAVE].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_WAVE].dataVec[waveIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                waveTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, waveTime);
            }
            else
                isWaveEnd = true;
        }
        else
            isWaveEnd = true;

        if (m_dataSet[SENSOR_MAX].dataVec.size() != 0)
        {
            if (maxIdx < m_dataSet[SENSOR_MAX].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_MAX].dataVec[maxIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                maxTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, maxTime);
            }
            else
                isMaxEnd = true;
        }
        else
            isMaxEnd = true;

        if (m_dataSet[SENSOR_NX].dataVec.size() != 0)
        {
            if (nxIdx < m_dataSet[SENSOR_NX].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_NX].dataVec[nxIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                nxTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, nxTime);
            }
            else
                isNXEnd = true;
        }
        else
            isNXEnd = true;

        if (m_dataSet[SENSOR_GRADAR].dataVec.size() != 0)
        {
            if (gradarIdx < m_dataSet[SENSOR_GRADAR].dataVec.size())
            {
                vector<string> dataStr = m_dataSet[SENSOR_GRADAR].dataVec[gradarIdx];
                vector<string> dateTimeStr = split(dataStr[0], "_");
                gradarTime = stoi(dateTimeStr[1]);
                minTime = min(minTime, gradarTime);
            }
            else
                isGRadarEnd = true;
        }
        else
            isGRadarEnd = true;



        // diff 초 만큼 더했을 때 60초를 넘어가면 +1분 해야함
        int diffTime = minTime + m_maxDiffSec;
        //int secOver60 = diffTime % 100;
        //if (secOver60 >= 60)
        //{
        //    diffTime += 100;
        //    diffTime -= 60;
        //}

        // 2025-02-07 KJH : 60분을 넘어가면 +1시간 추가.
        // 초 처리 (60초 넘어가면 +1분)
        int secOver60 = diffTime % 100;
        if (secOver60 >= 60) {
            diffTime += 100;
            diffTime -= 60;
        }

        // 분 처리 (60분 넘어가면 +1시간)
        int minOver60 = (diffTime / 100) % 100;
        if (minOver60 >= 60) {
            diffTime += 10000;
            diffTime -= 6000;
        }


        // 가장 먼저 센싱된 센서의 값 추가
        if (m_dataSet[SENSOR_WAVE].dataVec.size() != 0)
        {
            if (minTime <= waveTime && waveTime <= diffTime)
                tempDataSet[SENSOR_WAVE].dataVec.push_back(m_dataSet[SENSOR_WAVE].dataVec[waveIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_WAVE].dataVec[0].size());
                tempDataSet[SENSOR_WAVE].dataVec.push_back(initData);
            }
        }

        if (m_dataSet[SENSOR_GCQ].dataVec.size() != 0)
        {
            if (minTime <= gcqTime && gcqTime <= diffTime)
                tempDataSet[SENSOR_GCQ].dataVec.push_back(m_dataSet[SENSOR_GCQ].dataVec[gcqIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_GCQ].dataVec[0].size());
                tempDataSet[SENSOR_GCQ].dataVec.push_back(initData);
            }
        }

        if (m_dataSet[SENSOR_TM].dataVec.size() != 0)
        {
            if (minTime <= tmTime && tmTime <= diffTime)
                tempDataSet[SENSOR_TM].dataVec.push_back(m_dataSet[SENSOR_TM].dataVec[tmIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_TM].dataVec[0].size());
                tempDataSet[SENSOR_TM].dataVec.push_back(initData);
            }
        }

        if (m_dataSet[SENSOR_MAX].dataVec.size() != 0)
        {
            if (minTime <= maxTime && maxTime <= diffTime)
                tempDataSet[SENSOR_MAX].dataVec.push_back(m_dataSet[SENSOR_MAX].dataVec[maxIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_MAX].dataVec[0].size());
                tempDataSet[SENSOR_MAX].dataVec.push_back(initData);
            }
        }

        if (m_dataSet[SENSOR_NX].dataVec.size() != 0)
        {
            if (minTime <= nxTime && nxTime <= diffTime)
                tempDataSet[SENSOR_NX].dataVec.push_back(m_dataSet[SENSOR_NX].dataVec[nxIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_NX].dataVec[0].size());
                tempDataSet[SENSOR_NX].dataVec.push_back(initData);
            }
        }

        if (m_dataSet[SENSOR_GRADAR].dataVec.size() != 0)
        {
            if (minTime <= gradarTime && gradarTime <= diffTime)
                tempDataSet[SENSOR_GRADAR].dataVec.push_back(m_dataSet[SENSOR_GRADAR].dataVec[gradarIdx++]);
            else
            {
                initData.resize(m_dataSet[SENSOR_GRADAR].dataVec[0].size());
                tempDataSet[SENSOR_GRADAR].dataVec.push_back(initData);
            }
        }

        if (isGCQEnd && isNXEnd && isMaxEnd && isWaveEnd && isTmEnd && isGRadarEnd)
            break;

    }

    for (int i = 0; i < SENSOR_TOTALCNT; i++)
        m_dataSet[i].dataVec = tempDataSet[i].dataVec;


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
