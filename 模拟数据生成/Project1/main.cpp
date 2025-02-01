#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip>
#include <locale>

#pragma warning(disable:4996)  // 禁用编译器警告
using namespace std;

// 定义文件路径
#define totalData 1000 // 总数据条数（每个文件）
#define timeInterval 5      // 时间间隔（单位：秒）

std::wstring path[] = { L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/温度/temperature.txt",
                        L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/湿度/humidity.txt",
                        L"C:/Users/adminsun/Desktop/data/SenseTrack/datas/simulated data/压力/pressure.txt" };
// 定义数据结构Row
typedef struct Row {
    string id;      // 唯一标志
    string name;    // 设备型号
    string locate;  // 部署地点
    string value;   // 采集的值
    string time;    // 采集时间（格式：2023-12-08 21:45:23）
} row;
// 生成0到2之间的小数
float random() {
    return (rand() % int(pow(2, 10))) / pow(2, 9);
}

// 生成符合逻辑的温度数据
float generateTemperature(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    int hour = timeinfo->tm_hour;
    int month = timeinfo->tm_mon;
    float htem[12] = { 10,10,18,23,27,30,33,33,29,25,19,11 };
    float ltem[12] = { -1,2,7,12,17,21,25,25,22,14,7,1 };
    float temp = 0;
    float deta = (htem[month] - ltem[month]) / 3;
    // 根据时间段调整温度范围
    if (hour >= 6 && hour < 12) {
        temp = (rand() % int(deta) + ltem[month]+1*deta); 
    }
    else if (hour >= 12 && hour < 18) {
        temp = (rand() % int(deta) + ltem[month] + 2 * deta);  
    }
    else {
        temp = (rand() % int(deta) + ltem[month ]);  // 早上和晚上：-1到4度
    }

    return temp;
}

// 生成符合逻辑的湿度数据
float generateHumidity() {
    return (rand() % 40 + 40);  // 湿度：40%到80%
}

// 生成符合逻辑的大气压力数据
float generatePressure() {
    return (rand() % 400 + 800) / 1000.0;  // 大气压力：约1000hPa
}

// 获取当前时间的函数
char* gettime(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    static char buffer[128];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}



// 获取当前设备信息
void generateDeviceInfo(row& p) {
    int flag;
    // 随机生成设备型号
    flag = rand() % 5;
    switch (flag) {
    case 0: { p.name = "DHT22"; break; }
    case 1: { p.name = "HIH-4000-001"; break; }
    case 2: { p.name = "HTS221"; break; }
    case 3: { p.name = "SHT31"; break; }
    case 4: { p.name = "HMP110"; break; }
    }

    // 随机生成部署地点
    flag = rand() % 5;
    switch (flag) {
    case 0: { p.locate = "长广溪湿地公园"; break; }
    case 1: { p.locate = "江南大学"; break; }
    case 2: { p.locate = "南长街"; break; }
    case 3: { p.locate = "融创乐园"; break; }
    case 4: { p.locate = "鼋头渚"; break; }
    }
}


// 根据路径名选择不同的封装函数
void writeDataToFile(int index, time_t currentTime) {
    // 创建文件
    srand(time(NULL));
    wofstream fp(path[index]);  // 使用wofstream打开文件
    if (!fp) {
        wcout << L"Failed to open file: " << path[index] << endl; // 输出路径
        return;
    }

    fp << "id" << "\t" << "name" << "\t" << "locate" << "\t" << "value" << "\t" << "time" << endl;  // 写入表头

    // 创建结构体变量
    row p;

    // 生成数据
    for (int i = 0; i < totalData; i++) {
        // 生成当前数据
        generateDeviceInfo(p);  // 随机生成设备信息
        p.id = to_string(i + 1);  // 唯一标识符

        if (index == 0) { // 温度路径
            p.value = to_string(generateTemperature(currentTime));  // 温度
        }
        else if (index == 1) { // 湿度路径
            p.value = to_string(generateHumidity());  // 湿度
        }
        else if (index == 2) { // 压力路径
            p.value = to_string(generatePressure());  // 压力
        }
        p.time = gettime(currentTime);  // 获取当前时间

        // 写入数据到文件
        fp << p.id.c_str() << L"\t" << p.name.c_str() << L"\t" << p.locate.c_str() << L"\t"
            << p.value.c_str() << L"\t" << p.time.c_str() << endl;

        // 时间递增 5 秒
        currentTime += timeInterval;
    }

    // 关闭文件
    fp.close();
}

int main() {
    // 设置当前时间（起始时间）
    time_t currentTime = time(NULL);

    // 分别为温度、湿度、压力生成数据
     
    for (int i = 0; i < 1; i++) {
        writeDataToFile(i, currentTime);  // 为每个文件生成数据
    }

    cout << "数据模拟完成" << endl; 
    return 0;
}
