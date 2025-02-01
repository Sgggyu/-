#include "set.h"
#include "ui_set.h"
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include<QMessageBox>

set::set(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::set)
{
    ui->setupUi(this);
    setWindowTitle("传感器数据可视化");

    Initgraph2D(); // 创建2D图表

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->groupBox);
    splitter->addWidget(chartView); // 添加chartView到主窗口
    this->setCentralWidget(splitter); // 设置主窗口的中心组件

    // 加载配置
    loadConfig();

    connect(ui->chkBoxBackGround, &QCheckBox::clicked, this, &set::on_chkBoxBackGround_clicked);
    connect(ui->chkBoxGrid, &QCheckBox::clicked, this, &set::on_chkBoxGrid_clicked);
    connect(ui->chkBoxSmooth, &QCheckBox::clicked, this, &set::on_chkBoxSmooth_clicked);
    connect(ui->chkBoxAxisTitle, &QCheckBox::clicked, this, &set::on_chkBoxAxisTitle_clicked);
    connect(ui->btnBarColor, &QPushButton::clicked, this, &set::on_btnBarColor_clicked, Qt::UniqueConnection);

}

set::~set()
{
    delete ui;
}

// 二维图表显示
void set::Initgraph2D()
{
    chart = new QChart();
    series = new QLineSeries();

    // 生成数据并添加到系列，修改为60个点
    for (int i = 0; i < 60; ++i) {
        double value = QRandomGenerator::global()->generateDouble() * 20.0;
        series->append(i, value);
    }

    // 设置系列
    chart->addSeries(series);
    chart->createDefaultAxes();

    // 配置X轴
    axisX = new QValueAxis();
    axisX->setRange(0, 60);  // 将X轴的范围改为 0 到 60
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Sample Index"); // 添加标题
    axisX->setGridLineVisible(true);
    chart->setAxisX(axisX, series);

    // 配置Y轴
    axisY = new QValueAxis();
    axisY->setRange(0, 20);
    axisY->setLabelFormat("%.1f");
    axisY->setTitleText("Sensor Value"); // 添加标题
    axisY->setGridLineVisible(true);
    chart->setAxisY(axisY, series);

    // 设置图表样式
    chart->setTitle("2D Line Chart for Sensor Data");
    chart->setAnimationOptions(QChart::SeriesAnimations); // 添加动画效果

    // 设置图表视图
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    // 这里更新图表样式
    updateChartStyle();  // 根据加载的配置文件更新样式
}


// 槽函数
void set::on_chkBoxBackGround_clicked(bool checked)
{
    if (checked)
        chart->setBackgroundBrush(QBrush(Qt::lightGray));
    else
        chart->setBackgroundBrush(QBrush(Qt::NoBrush));
}

void set::on_btnBarColor_clicked()
{
    // 使用静态方法，确保对话框仅弹出一次
    QColor currentColor = series->color();
    QColor selectedColor = QColorDialog::getColor(currentColor, this, "Select Line Color");

    if (selectedColor.isValid()) {
        series->setColor(selectedColor);
    }
}


void set::on_chkBoxGrid_clicked(bool checked)
{
    axisX->setGridLineVisible(checked);
    axisY->setGridLineVisible(checked);
}

void set::on_chkBoxSmooth_clicked(bool checked)
{
    QPen currentPen = series->pen(); // 获取当前画笔
    if (checked) {
        currentPen.setWidth(2);               // 设置线条宽度
        currentPen.setStyle(Qt::DashLine);    // 设置虚线样式
    } else {
        currentPen.setWidth(1);               // 设置默认线条宽度
        currentPen.setStyle(Qt::SolidLine);   // 设置实线样式
    }
    series->setPen(currentPen); // 更新画笔
}

void set::on_chkBoxAxisTitle_clicked(bool checked)
{
    if (!axisX->titleText().isEmpty() && !axisY->titleText().isEmpty()) {
        axisX->setTitleVisible(checked);
        axisY->setTitleVisible(checked);
    }
}


void set::saveConfig()
{
    QString configFilePath = "config.txt"; // 配置文件路径
    QFile configFile(configFilePath);

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&configFile);

        // 保存线条颜色
        out << "lineColor=" << series->color().name() << "\n";

        // 保存背景设置
        out << "background=" << (ui->chkBoxBackGround->isChecked() ? "true" : "false") << "\n";

        // 保存网格设置
        out << "grid=" << (ui->chkBoxGrid->isChecked() ? "true" : "false") << "\n";

        // 保存平滑线条设置
        out << "DashLine=" << (ui->chkBoxSmooth->isChecked() ? "true" : "false") << "\n";

        // 保存坐标轴标题设置
        out << "axisTitle=" << (ui->chkBoxAxisTitle->isChecked() ? "true" : "false") << "\n";

        configFile.close();
    }
}
void set::loadConfig()
{
    QString configFilePath = "config.txt"; // 配置文件路径
    QFile configFile(configFilePath);

    if (configFile.exists() && configFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&configFile);
        QString line;

        while (!in.atEnd()) {
            line = in.readLine();
            QStringList keyValue = line.split("=");

            if (keyValue.size() == 2) {
                QString key = keyValue[0].trimmed();
                QString value = keyValue[1].trimmed();

                if (key == "lineColor") {
                    QColor color(value);
                    series->setColor(color); // 更新线条颜色
                } else if (key == "background") {
                    ui->chkBoxBackGround->setChecked(value == "true"); // 设置背景勾选框
                } else if (key == "grid") {
                    ui->chkBoxGrid->setChecked(value == "true"); // 设置网格勾选框
                } else if (key == "smooth") {
                    ui->chkBoxSmooth->setChecked(value == "true"); // 设置平滑勾选框
                } else if (key == "axisTitle") {
                    ui->chkBoxAxisTitle->setChecked(value == "true"); // 设置坐标轴标题勾选框
                }
            }
        }
        configFile.close();
    }
    // 加载配置后，立即更新图表样式
    updateChartStyle();

}

// 根据配置更新图表的样式
void set::updateChartStyle()
{
    // 更新背景
    if (ui->chkBoxBackGround->isChecked()) {
        chart->setBackgroundBrush(QBrush(Qt::lightGray));
    } else {
        chart->setBackgroundBrush(QBrush(Qt::NoBrush));
    }

    // 更新网格
    axisX->setGridLineVisible(ui->chkBoxGrid->isChecked());
    axisY->setGridLineVisible(ui->chkBoxGrid->isChecked());

    // 更新线条样式
    QPen currentPen = series->pen(); // 获取当前画笔
    if (ui->chkBoxSmooth->isChecked()) {
        currentPen.setWidth(2);               // 设置线条宽度
        currentPen.setStyle(Qt::DashLine);    // 设置虚线样式
    } else {
        currentPen.setWidth(1);               // 设置默认线条宽度
        currentPen.setStyle(Qt::SolidLine);   // 设置实线样式
    }
    series->setPen(currentPen); // 更新画笔

    // 更新坐标轴标题的显示
    axisX->setTitleVisible(ui->chkBoxAxisTitle->isChecked());
    axisY->setTitleVisible(ui->chkBoxAxisTitle->isChecked());
}


void set::on_btnSaveConfig_clicked()
{
    saveConfig(); // 保存当前设置
    QMessageBox::information(this, "保存设置", "设置已保存到配置文件！"); // 显示保存成功的提示
}
