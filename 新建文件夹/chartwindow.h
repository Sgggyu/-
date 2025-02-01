#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>
#include <QChartView>
#include <QChart>


    class ChartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWindow(QChartView *charview_r,QWidget *parent = nullptr);

    // 设置图表
    void setChart(QChart *chart);

private:
    QChartView *chartView;  // 用于显示图表的视图
};

#endif // CHARTWINDOW_H
