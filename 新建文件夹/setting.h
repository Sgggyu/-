#ifndef SETTING_H
#define SETTING_H

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

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();
private slots:
    void on_chkBoxBackGround_clicked(bool checked);
    void on_chkBoxGrid_clicked(bool checked);
    void on_chkBoxSmooth_clicked(bool checked);
    void on_chkBoxAxisTitle_clicked(bool checked);
    void on_btnBarColor_clicked();
    void on_SliderH_valueChanged(int value);
private:
    Ui::Setting *ui;
    void Initgraph2D();  // 创建2D图表
    QChart *chart;       // 2D图表
    QLineSeries *series; // 线性图数据系列
    QChartView *chartView; // 图表视图
};

#endif // SETTING_H
