#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menu.h"
#include "set.h"
#include "set.cpp"
#include <filesystem>   // 包含文件系统库
#include <fstream>      // 包含文件流库


namespace fs = std::filesystem;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 检查配置文件是否存在，如果不存在则创建并写入默认值
    checkAndCreateConfigFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 检查配置文件是否存在，如果没有则创建
void MainWindow::checkAndCreateConfigFile()
{
    const char* configFilePath = "config.txt";
    if (!fs::exists(configFilePath)) {
        // 配置文件不存在，创建并写入默认配置
        std::ofstream configFile(configFilePath);
        if (configFile.is_open()) {
            // 写入五项设置的默认值
            configFile << "LineColor=0,191,255\n";  // 天蓝色 (RGB: 0, 191, 255)
            configFile << "BackgroundVisible=true\n";
            configFile << "GridVisible=true\n";
            configFile << "DashLine=true\n";
            configFile << "AxisTitleVisible=true\n";
            configFile.close();
            qDebug() << "配置文件已创建，并写入默认值。";
        } else {
            qDebug() << "无法创建配置文件！";
        }
    } else {
        qDebug() << "配置文件已存在。";
    }
}

void MainWindow::on_btn_begin_clicked()
{
    this->setWindowFlag(Qt::Window); // 将子窗口设置为独立窗口

    menu *secondwindows = new menu(this);
    this->hide();
    secondwindows->show();
}

void MainWindow::on_toolbtn_clicked()
{
    set *setting = new set();
    setting->show();
}
