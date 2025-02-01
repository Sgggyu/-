#include "setting.h"
#include "ui_setting.h"
Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    setWindowTitle("传感器数据可视化");

    Initgraph2D(); // 创建2D图表

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->groupBox);
    splitter->addWidget(chartView); // 添加chartView到主窗口
    this->setCentralWidget(splitter); // 设置主窗口的中心组件

    connect(ui->chkBoxBackGround, &QCheckBox::clicked, this, &Setting::on_chkBoxBackGround_clicked);
    connect(ui->chkBoxGrid, &QCheckBox::clicked, this, &Setting::on_chkBoxGrid_clicked);
    connect(ui->chkBoxSmooth, &QCheckBox::clicked, this, &Setting::on_chkBoxSmooth_clicked);
    connect(ui->chkBoxAxisTitle, &QCheckBox::clicked, this, &Setting::on_chkBoxAxisTitle_clicked);
    connect(ui->btnBarColor, &QPushButton::clicked, this, &Setting::on_btnBarColor_clicked);
    connect(ui->SliderH, &QSlider::valueChanged, this, &Setting::on_SliderH_valueChanged);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::Initgraph2D() {
    chart = new QChart();
    series = new QLineSeries();

    // 生成数据并添加到系列
    for (int i = 0; i < 250; ++i) {
        double value = QRandomGenerator::global()->generateDouble() * 20.0;
        series->append(i, value); // 将数据添加到折线图中
    }

    // 设置系列
    chart->addSeries(series);
    chart->createDefaultAxes();  // 自动生成X、Y轴

    // 配置X轴
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 250);  // 设置X轴范围
    axisX->setLabelFormat("%d"); // 设置X轴标签格式
    chart->setAxisX(axisX, series);

    // 配置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 20);  // 设置Y轴范围
    chart->setAxisY(axisY, series);

    // 设置图表视图
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // 启用抗锯齿
    chartView->setMinimumSize(800, 600); // 设置最小尺寸
}
