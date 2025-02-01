#ifndef SET_H
#define SET_H

#include <QMainWindow>
#include <QWidget>
#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts/QChart>
#include <QRandomGenerator>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include<QSplitter>
#include<QColorDialog>

namespace Ui {
class set;
}

class set : public QMainWindow
{
    Q_OBJECT

public:
    explicit set(QWidget *parent = nullptr);
    ~set();

private slots:
    void on_chkBoxBackGround_clicked(bool checked);
    void on_chkBoxGrid_clicked(bool checked);
    void on_chkBoxSmooth_clicked(bool checked);
    void on_chkBoxAxisTitle_clicked(bool checked);
    void on_btnBarColor_clicked();
    // void on_SliderH_valueChanged(int value);

    void on_btnSaveConfig_clicked();

private:
    Ui::set *ui;
    void Initgraph2D();  // 创建2D图表
    QChart *chart;       // 2D图表
    QLineSeries *series; // 线性图数据系列
    QChartView *chartView; // 图表视图
    QValueAxis* axisX; //x
    QValueAxis* axisY; //y
    void loadConfig();
    void saveConfig();
    void updateChartStyle();
};

#endif // SET_H
