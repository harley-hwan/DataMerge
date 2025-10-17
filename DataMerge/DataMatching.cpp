#include "pch.h"
#include "DataMatching.h"

void DataMatching::setDataSet(stShotInfo inputData[SENSOR_TOTALCNT])
{
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
        m_dataSet[i] = inputData[i];
}

void DataMatching::setMaxDiffTime(int val)
{
    // ���� �ȿ� ���°��� ���ð��� ���̵� ���̶� ������
    m_maxDiffSec = val;
}

void DataMatching::excute()
{
	matchingWithTime();
	writeCSV(m_dataSet);
}

void DataMatching::matchingWithTime()
{
    // �ε�� �����ͼ°� �ð����� ��Ī �� �ӽ� ����
    stShotInfo tempDataSet[SENSOR_TOTALCNT];

    // ��� ���� ����
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        tempDataSet[i].snName = m_dataSet[i].snName;
        tempDataSet[i].item = m_dataSet[i].item;
    }

    // �� ������ �ε���
    int sensorIdx[SENSOR_TOTALCNT] = { 0 };

    // �� ���� ������ ���� �÷���
    bool sensorEnd[SENSOR_TOTALCNT] = { false };

    // �� ������ ���� �ð� ����
    int sensorTimes[SENSOR_TOTALCNT] = { 0 };

    int processedRows = 0;
    int errorCount = 0;

    // ��Ī ���μ���
    while (true)
    {
        int minTime = INT_MAX;

        // �� ������ ���� �ð� Ȯ��
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (!sensorEnd[sensor] && m_dataSet[sensor].dataVec.size() > 0)
            {
                if (sensorIdx[sensor] < m_dataSet[sensor].dataVec.size())
                {
                    const auto& dataRow = m_dataSet[sensor].dataVec[sensorIdx[sensor]];

                    // �����Ͱ� �ְ� ù ��(�ð�)�� ������� ������ Ȯ��
                    if (!dataRow.empty() && !dataRow[0].empty())
                    {
                        vector<string> dateTimeStr = split(dataRow[0], "_");

                        if (dateTimeStr.size() >= 2)
                        {
                            try
                            {
                                // �ð��� ������ ��ȯ
                                string timeStr = dateTimeStr[1];
                                // ���ڰ� �ƴ� ���� ����
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
                            // �ð� ������ �߸��� ��� �ǳʶٱ�
                            sensorIdx[sensor]++;
                            continue;
                        }
                    }
                    else
                    {
                        // �� �������� ��� ��������
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

        // ��� ������ ����Ǿ����� Ȯ��
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

        // �ð� ���� ��� (�����÷ο� ó�� ����)
        int diffTime = minTime + m_maxDiffSec;

        // �� ó�� (60�� �Ѿ�� +1��)
        int seconds = diffTime % 100;
        if (seconds >= 60)
        {
            diffTime += 100;
            diffTime -= 60;
        }

        // �� ó�� (60�� �Ѿ�� +1�ð�)
        int minutes = (diffTime / 100) % 100;
        if (minutes >= 60)
        {
            diffTime += 10000;
            diffTime -= 6000;
        }

        // �� �������� �ð� ������ �´� ������ �߰�
        for (int sensor = 0; sensor < SENSOR_TOTALCNT; sensor++)
        {
            if (m_dataSet[sensor].dataVec.size() > 0)
            {
                // ������ �ð� ���� ���� �ְ� ���� ������ �ʾ�����
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
                    // �� ������ ����
                    vector<string> emptyData;

                    // �ش� ������ ������ ũ��� �� ������ ����
                    size_t dataSize = 0;

                    // ���� ���� ũ�� ���
                    if (!m_dataSet[sensor].dataVec.empty() &&
                        sensorIdx[sensor] < m_dataSet[sensor].dataVec.size())
                    {
                        dataSize = m_dataSet[sensor].dataVec[sensorIdx[sensor]].size();
                    }
                    // ���� ���� ũ�� ���
                    else if (!m_dataSet[sensor].dataVec.empty() && sensorIdx[sensor] > 0)
                    {
                        dataSize = m_dataSet[sensor].dataVec[sensorIdx[sensor] - 1].size();
                    }
                    // ��� ũ�� + 1 ��� (Date-Time �� ����)
                    else if (!m_dataSet[sensor].item.empty())
                    {
                        dataSize = m_dataSet[sensor].item.size() + 1;
                    }
                    // �⺻��
                    else
                    {
                        dataSize = 10; // ������ �⺻��
                    }

                    emptyData.resize(dataSize, "");
                    tempDataSet[sensor].dataVec.push_back(emptyData);
                }
            }
        }

        processedRows++;

        // ���� ��Ȳ ǥ�� (100�ึ��)
        if (processedRows % 100 == 0)
        {
            std::cout << "Processing... " << processedRows << " rows completed" << std::endl;
        }
    }

    // ��� ����
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        m_dataSet[i].dataVec = tempDataSet[i].dataVec;
    }

    std::cout << "Time matching complete:" << std::endl;
    std::cout << "  Total processed rows: " << processedRows << std::endl;
    std::cout << "  Errors encountered: " << errorCount << std::endl;

    // �� ������ ��� ���
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

    // 2024/12/17�� �۽��� ���δ� ��û���� ù��° �� ���� �̸� ����
    //// ù��° ��- ���� �̸�
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

    //    // ���� ���� �� ����
    //    nameStr += ",";
    //}
    //file << nameStr << endl;



    // �ι�° ��- ������ �̸�
    string itemStr;
    for (int i = 0; i < SENSOR_TOTALCNT; i++)
    {
        if (inputData[i].snName == "")
            continue;

        itemStr += "SensorName,";

        for (int idx = 0; idx < inputData[i].item.size(); idx++)
            itemStr += inputData[i].item[idx] + "(" + inputData[i].snName + ")" + ",";

        // ���� ���� �� ����
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
    // �ϳ��� ������ ���� ������ ����
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
            // ������ ��Ī �ȵ� ���
            else
                zeroCnt += 1;


            strTmp += ",";
        }

        if (zeroCnt != 0)
            continue;
        else
            str = strTmp;


        // ���� ���� �� ����
        str += ",";

        file << str << endl;
    }
#else

    // �� �� �־ �׳� ����
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
                // 2025/02/04 ����ȯ ���δ� ��û����, ������ ������� �� �ڸ��� ������ �޶�.
                // ���Ͽ� ������ ���� ���� �׸� ���� ��
                for (int idx = 0; idx < inputData[j].item.size(); idx++)
                    str += ", ";
            }
            else
            {
                for (int idx = 0; idx < inputData[j].dataVec[i].size(); idx++)
                    str += inputData[j].dataVec[i][idx] + ", ";
            }

            // ���� ���� �� ����
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
