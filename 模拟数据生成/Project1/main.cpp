#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <locale>

#pragma warning(disable:4996)  // ���ñ���������
using namespace std;

// �����ļ�·��
#define totalData 1000 // ������������ÿ���ļ���
#define timeInterval 5      // ʱ��������λ���룩

std::wstring path[] = { L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/�¶�/temperature.txt",
                        L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/ʪ��/humidity.txt",
                        L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/ѹ��/pressure.txt" };
// �������ݽṹRow
typedef struct Row {
    string id;      // Ψһ��־
    string name;    // �豸�ͺ�
    string locate;  // ����ص�
    string value;   // �ɼ���ֵ
    string time;    // �ɼ�ʱ�䣨��ʽ��2023-12-08 21:45:23��
} row;
// ����0��2֮���С��
float random() {
    return (rand() % int(pow(2, 10))) / pow(2, 9);
}

// ���ɷ����߼����¶�����
float generateTemperature(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    int hour = timeinfo->tm_hour;
    int month = timeinfo->tm_mon;
    float htem[12] = { 10,10,18,23,27,30,33,33,29,25,19,11 };
    float ltem[12] = { -1,2,7,12,17,21,25,25,22,14,7,1 };
    float temp = 0;
    float deta = (htem[month] - ltem[month]) / 3;
    // ����ʱ��ε����¶ȷ�Χ
    if (hour >= 6 && hour < 12) {
        temp = (rand() % int(deta) + ltem[month]+1*deta); 
    }
    else if (hour >= 12 && hour < 18) {
        temp = (rand() % int(deta) + ltem[month] + 2 * deta);  
    }
    else {
        temp = (rand() % int(deta) + ltem[month ]);  // ���Ϻ����ϣ�-1��4��
    }

    return temp;
}

// ���ɷ����߼���ʪ������
float generateHumidity() {
    return (rand() % 40 + 40);  // ʪ�ȣ�40%��80%
}

// ���ɷ����߼��Ĵ���ѹ������
float generatePressure() {
    return (rand() % 400 + 800) / 1000.0;  // ����ѹ����Լ1000hPa
}

// ��ȡ��ǰʱ��ĺ���
char* gettime(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    static char buffer[128];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}



// ��ȡ��ǰ�豸��Ϣ
void generateDeviceInfo(row& p) {
    int flag;
    // ��������豸�ͺ�
    flag = rand() % 5;
    switch (flag) {
    case 0: { p.name = "DHT22"; break; }
    case 1: { p.name = "HIH-4000-001"; break; }
    case 2: { p.name = "HTS221"; break; }
    case 3: { p.name = "SHT31"; break; }
    case 4: { p.name = "HMP110"; break; }
    }

    // ������ɲ���ص�
    flag = rand() % 5;
    switch (flag) {
    case 0: { p.locate = "����Ϫʪ�ع�԰"; break; }
    case 1: { p.locate = "���ϴ�ѧ"; break; }
    case 2: { p.locate = "�ϳ���"; break; }
    case 3: { p.locate = "�ڴ���԰"; break; }
    case 4: { p.locate = "��ͷ�"; break; }
    }
}


// ����·����ѡ��ͬ�ķ�װ����
void writeDataToFile(int index, time_t currentTime) {
    // �����ļ�
    srand(time(NULL));
    wofstream fp(path[index]);  // ʹ��wofstream���ļ�
    if (!fp) {
        wcout << L"Failed to open file: " << path[index] << endl; // ���·��
        return;
    }

    fp << "id" << "\t" << "name" << "\t" << "locate" << "\t" << "value" << "\t" << "time" << endl;  // д���ͷ

    // �����ṹ�����
    row p;

    // ��������
    for (int i = 0; i < totalData; i++) {
        // ���ɵ�ǰ����
        generateDeviceInfo(p);  // ��������豸��Ϣ
        p.id = to_string(i + 1);  // Ψһ��ʶ��

        if (index == 0) { // �¶�·��
            p.value = to_string(generateTemperature(currentTime));  // �¶�
        }
        else if (index == 1) { // ʪ��·��
            p.value = to_string(generateHumidity());  // ʪ��
        }
        else if (index == 2) { // ѹ��·��
            p.value = to_string(generatePressure());  // ѹ��
        }
        p.time = gettime(currentTime);  // ��ȡ��ǰʱ��

        // д�����ݵ��ļ�
        fp << p.id.c_str() << L"\t" << p.name.c_str() << L"\t" << p.locate.c_str() << L"\t"
            << p.value.c_str() << L"\t" << p.time.c_str() << endl;

        // ʱ����� 5 ��
        currentTime += timeInterval;
    }

    // �ر��ļ�
    fp.close();
}

int main() {
    // ���õ�ǰʱ�䣨��ʼʱ�䣩
    time_t currentTime = time(NULL);

    // �ֱ�Ϊ�¶ȡ�ʪ�ȡ�ѹ����������
     
    for (int i = 0; i < 1; i++) {
        writeDataToFile(i, currentTime);  // Ϊÿ���ļ���������
    }

    cout << "����ģ�����" << endl; 
    return 0;
}
