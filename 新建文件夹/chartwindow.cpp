#include "chartwindow.h"
#include <QVBoxLayout>

ChartWindow::ChartWindow(QChartView *charview_r,QWidget *parent) : QWidget(parent), chartView(charview_r)
{
    // 创建一个垂直布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);  // 将 QChartView 添加到布局中

    // 设置布局
    setLayout(layout);

    // 设置图表视图的固定大小
    chartView->setFixedSize(800, 600);
}

void ChartWindow::setChart(QChart *chart)
{
    // 设置传入的图表到 QChartView 中
    chartView->setChart(chart);
}
