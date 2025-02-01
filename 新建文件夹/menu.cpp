#include "menu.h"
#include "ui_menu.h"
#include"sensor1.h"
#include<QMainWindow>
#include<QtConcurrent>

menu::menu(QMainWindow *mainWindow, QWidget *parent)
    : QWidget(parent), ui(new Ui::menu), mainWindow(mainWindow)
{
    ui->setupUi(this);
    ui->loadingLabel->hide();
    ui->loadingtext->hide();
}

menu::~menu()
{
    delete ui;
}




void menu::on_pushButton_clicked()
{
    mainWindow -> show();
    this -> hide();

}
//打开第二个界面后选择不同的表
void menu::on_btn_1_clicked()
{


    sensor1 *sensorWindow = new sensor1("temperature");
    sensorWindow->show();
    sensorWindow->setuptable();
    sensorWindow->loadTableData();

}

//把原来第二线程放回到主线程
//所有都在主线程上操作
//…………，把问题交给ai看看ai有什么看法

void menu::on_pushButton_2_clicked()
{
    sensor1 *sensorWindow = new sensor1("humidity");
    sensorWindow->show();
    sensorWindow->setuptable();
    sensorWindow->loadTableData();
}

void menu::on_pushButton_3_clicked()
{

    sensor1 *sensorWindow = new sensor1("pressure");
    sensorWindow->show();
    sensorWindow->setuptable();
    sensorWindow->loadTableData();
}



void menu::showLoadingScreen()
{
    // 在 menu 窗口显示加载界面
    QMovie *movie = new QMovie(":/image/loading.gif");  // 根据你的文件路径设置
    ui->loadingLabel->setMovie(movie);
    movie->start();
    ui->loadingLabel->show();
    ui->loadingtext->show();
}

void menu::hideLoadingScreen()
{
    // 隐藏加载界面
    ui->loadingLabel->hide();
    ui->loadingtext->hide();
}


//
