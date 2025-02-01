#include "sensor1.h"
#include "ui_sensor1.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include"clickableheader.h"
#include"filterdialog.h"
#include"chartwindow.h"
#include"adddatadialog.h"
#include"set.h"
#include<QTimeEdit>
#include<QDialog>
#include<QtConcurrent>
#include<QMovie>

sensor1::sensor1(QString kindofsensor,QWidget *parent) : QWidget(parent), ui(new Ui::sensor1), sensor_name(kindofsensor)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &sensor1::updateTime);
    connect(ui->chartshowbtn,&QPushButton::clicked, this, &sensor1::chartshow);
    //图初始化

    chartView = new QChartView;
    chartView->setFixedHeight(450); // 调整图表高度，增加1/3


    sensor1::updateTime();

    setupDatabase();
    // setuptable();
    // loadTableData();

    timer->start(1000); // 每1000毫秒触发一次
    //tableview的一些初始化
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    //column部分
     currentFilters.fill("TRUE", 6);

    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &sensor1::on_tableView_customContextMenuRequested);

    connect(ui->prevButton, &QPushButton::clicked, this, &sensor1::onPrevPage);
    connect(ui->nextButton, &QPushButton::clicked, this, &sensor1::onNextPage);
    // 自定义表头
    ClickableHeader *header = new ClickableHeader(Qt::Horizontal, this);
    ui->tableView->setHorizontalHeader(header);
    connect(header, &ClickableHeader::columnClicked, this, &sensor1::onColumnHeaderClicked);
    // setColumnWidths();
}


//创建数据库连接
void sensor1::setupDatabase() {
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Available database drivers:";
    for (const QString &driver : drivers) {
        qDebug() << driver;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("123456");

    if (!db.open()) {
        qFatal("Failed to connect to database");
    }

    if (!db.isValid()) {
        qDebug() << "Invalid database connection!";
    }



}

//加载表

void sensor1::setuptable(){


    tableModel = new QSqlTableModel(this);
    tableModel->setTable(sensor_name);

}

//加载数据库数据
void sensor1::loadTableData() {


    ui->tableView->setModel(tableModel);
    int offset = currentPage * rowsPerPage;
    tableModel->setFilter(QString("id >= %1 AND id < %2").arg(offset + 1).arg(offset + rowsPerPage + 1));
    tableModel->select();
    setColumnWidths();
}

//表视图的初始化
void sensor1::setColumnWidths()
{
    // 获取表格的宽度
    int totalWidth = 540;

    // 设置比例：0.5:1:1:0.5:1.2
    int totalProportion = 3.7;

    // 计算每列的宽度
    int width1 = totalWidth / totalProportion * 0.3;    // 第一列
    int width2 = totalWidth / totalProportion * 1;    // 第二列
    int width3 = totalWidth / totalProportion * 1;    // 第三列
    int width4 = totalWidth / totalProportion * 0.3;    // 第四列
    int width5 = totalWidth / totalProportion * 1.1;  // 第五列

    // 设置每列的宽度
    ui->tableView->horizontalHeader()->resizeSection(0, width1);
    ui->tableView->horizontalHeader()->resizeSection(1, width2);
    ui->tableView->horizontalHeader()->resizeSection(2, width3);
    ui->tableView->horizontalHeader()->resizeSection(3, width4);
    ui->tableView->horizontalHeader()->resizeSection(4, width5);
}
//更新图
void sensor1::updateChart() {
 //已经挪至showchartbtn内部了

}

//翻页实现
void sensor1::onPrevPage() {
    if (currentPage > 0) {
        --currentPage;
        loadTableData();
    }
}

void sensor1::onNextPage() {
    ++currentPage;
    loadTableData(); //翻页有一些bug
}



void sensor1::onColumnHeaderClicked(int column) {
    QString columnName = tableModel->headerData(column, Qt::Horizontal).toString();
    // 如果该列的唯一值尚未计算过，进行计算并缓存
    if (!columnUniqueValues.contains(column)) {
        QSet<QString> uniqueValues;
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            uniqueValues.insert(tableModel->index(row, column).data().toString());
        }
        columnUniqueValues[column] = uniqueValues; // 缓存计算结果
    }

    // 根据列类型调用不同的筛选逻辑
    if (column == 3) {  // 假设第4列是“值”列
        showRangeFilterDialog(columnName, column);
    } else if (column == 4) {  // 假设第5列是“时间”列
        showDateTimeFilterDialog(columnName, column);
    } else {
        // 对于其他列的筛选
        FilterDialog filterDialog(columnName, columnUniqueValues[column].values(), this);
        connect(&filterDialog, &FilterDialog::filterApplied, this, [=](const QStringList &selectedValues) {
            applyColumnFilter(column, selectedValues);
        });
        filterDialog.exec();
    }
}



void sensor1::applyColumnFilter(int column, const QStringList &selectedValues) {
    QString columnName = tableModel->headerData(column, Qt::Horizontal).toString();
    QStringList conditions;

    // 构造筛选条件
    for (const QString &value : selectedValues) {
        conditions.append(QString("%1 = '%2'").arg(columnName).arg(value));
    }

    // 使用 OR 合并所有选中的条件
    QString newCondition = conditions.join(" OR ");

    // 使用 AND 连接多个列的筛选条件
    if (selectedValues.isEmpty()) {
        // 如果没有选中值，清空当前列的筛选条件
        currentFilters[column] = "TRUE";
    } else {
        currentFilters[column] = newCondition;
    }

    // 合并所有列的筛选条件
    QString finalFilter = currentFilters.join(" AND ");

    // 使用 SQL 语句来执行筛选
    QString filterQuery = QString("SELECT * FROM %1 WHERE %2").arg(sensor_name).arg(finalFilter);
    tableModel->setQuery(filterQuery);
}



//时间字段筛选另有函数
void sensor1::showRangeFilterDialog(const QString &columnName, int column) {
    QDialog *filterDialog = new QDialog(this);  // 创建 QDialog 对象
    filterDialog ->setWindowTitle("value:筛选");
    QFormLayout *layout = new QFormLayout(filterDialog);

    QLineEdit *minEdit = new QLineEdit(filterDialog);
    QLineEdit *maxEdit = new QLineEdit(filterDialog);

    layout->addRow("最小值:", minEdit);
    layout->addRow("最大值:", maxEdit);

    QPushButton *applyButton = new QPushButton("确认", filterDialog);
    QPushButton *cancelButton = new QPushButton("取消筛选", filterDialog);
    layout->addRow(applyButton, cancelButton);

    // 连接信号与槽
    connect(applyButton, &QPushButton::clicked, this, [=]() {
        applyRangeFilter(columnName, column, minEdit->text(), maxEdit->text());
        filterDialog->accept();  // 使用非const的方式调用 accept()
    });

    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        removeRangeFilter(columnName, column);
        filterDialog->reject();  // 使用非const的方式调用 reject()
    });

    filterDialog->exec();  // 显式调用 exec()，在弹窗显示时等待用户输入
}


void sensor1::applyDateTimeFilter(const QString &columnName, int column, const QDateTime &start, const QDateTime &end) {
    QString dateFormat = "yyyy-MM-dd HH:mm:ss";  // 根据数据库字段的格式修改
    QString filterCondition = QString("%1 BETWEEN '%2' AND '%3'")
                                  .arg(columnName)
                                  .arg(start.toString(dateFormat))
                                  .arg(end.toString(dateFormat));

    // 更新筛选条件
    currentFilters[column] = filterCondition;

    // 合并所有列的筛选条件
    QString finalFilter = currentFilters.join(" AND ");

    // 使用 SQL 语句来执行筛选
    QString filterQuery = QString("SELECT * FROM %1 WHERE %2").arg(sensor_name).arg(finalFilter);
    tableModel->setQuery(filterQuery);
}

void sensor1::removeDateTimeFilter(const QString &columnName, int column) {
    currentFilters[column] = "TRUE"; // 清除该列的筛选条件

    // 合并所有列的筛选条件
    QString finalFilter =currentFilters.join(" AND ");

    // 使用 SQL 语句来执行筛选
    QString filterQuery = QString("SELECT * FROM %1 WHERE %2").arg(sensor_name).arg(finalFilter);
    tableModel->setQuery(filterQuery);
}

void sensor1::showDateTimeFilterDialog(const QString &columnName, int column) {
    QDialog *filterDialog = new QDialog(this);  // 创建 QDialog 对象
    filterDialog -> setWindowTitle("Time:筛选");
    QFormLayout *layout = new QFormLayout(filterDialog);

    QDateTimeEdit *startEdit = new QDateTimeEdit(filterDialog);
    QDateTimeEdit *endEdit = new QDateTimeEdit(filterDialog);

    layout->addRow("开始时间:", startEdit);
    layout->addRow("结束时间:", endEdit);

    QPushButton *applyButton = new QPushButton("应用", filterDialog);
    QPushButton *cancelButton = new QPushButton("取消筛选", filterDialog);
    layout->addRow(applyButton, cancelButton);

    // 连接信号与槽
    connect(applyButton, &QPushButton::clicked, this, [=]() {
        applyDateTimeFilter(columnName, column, startEdit->dateTime(), endEdit->dateTime());
        filterDialog->accept();  // 使用非const的方式调用 accept()
    });

    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        removeDateTimeFilter(columnName, column);
        filterDialog->reject();  // 使用非const的方式调用 reject()
    });

    filterDialog->exec();  // 显式调用 exec()，在弹窗显示时等待用户输入
}
//值的筛选需要另算
void sensor1::applyRangeFilter(const QString &columnName, int column, const QString &minValue, const QString &maxValue) {
    QString filterCondition = QString("%1 BETWEEN '%2' AND '%3'")
    .arg(columnName)
        .arg(minValue)
        .arg(maxValue);

    // 更新筛选条件
    currentFilters[column] = filterCondition;

    // 合并所有列的筛选条件

    QString finalFilter = currentFilters.join(" AND ");

    // 使用 SQL 语句来执行筛选
    QString filterQuery = QString("SELECT * FROM %1 WHERE %2").arg(sensor_name).arg(finalFilter);
    tableModel->setQuery(filterQuery);
}

void sensor1::removeRangeFilter(const QString &columnName, int column) {
    currentFilters[column] = "TRUE"; // 清除该列的筛选条件

    // 合并所有列的筛选条件
    QString finalFilter =currentFilters.join(" AND ");

    // 使用 SQL 语句来执行筛选
    QString filterQuery = QString("SELECT * FROM %1 WHERE %2").arg(sensor_name).arg(finalFilter);
    tableModel->setQuery(filterQuery);
}

//时间显示
void sensor1::updateTime() {
    // 获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy年MM月dd日  hh:mm:ss");
    // 更新 QLabel 的文本
    ui->time->setText(currentTime);
}


//chartshow功能实现
void sensor1::chartshow() {
    // 读取配置文件
    QString configFilePath = "config.txt"; // 配置文件路径
    QFile configFile(configFilePath);

    // 配置文件的默认值（如果文件不存在或内容不完整）
    QColor lineColor = Qt::blue; // 默认线条颜色
    bool showBackground = false; // 默认不显示背景
    bool showGrid = true; // 默认显示网格
    bool smoothLines = false; // 默认不平滑
    bool showAxisTitle = true; // 默认显示坐标轴标题

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
                    lineColor = QColor(value); // 读取线条颜色
                } else if (key == "background") {
                    showBackground = (value == "true"); // 读取背景设置
                } else if (key == "grid") {
                    showGrid = (value == "true"); // 读取网格设置
                } else if (key == "DashLine") {
                    smoothLines = (value == "true"); // 读取虚线设置
                } else if (key == "axisTitle") {
                    showAxisTitle = (value == "true"); // 读取坐标轴标题设置
                }
            }
        }
        configFile.close();
    }

    // 创建弹窗以输入 headtime 和 tailtime
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("选择时间范围");
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *headLabel = new QLabel("开始时间:");
    QDateTimeEdit *headTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(-3600), dialog); // 默认时间为一小时前
    headTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    headTimeEdit->setCalendarPopup(true);

    QLabel *tailLabel = new QLabel("结束时间:");
    QDateTimeEdit *tailTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), dialog); // 默认时间为当前时间
    tailTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    tailTimeEdit->setCalendarPopup(true);

    QPushButton *confirmButton = new QPushButton("确认", dialog);
    QPushButton *cancelButton = new QPushButton("取消", dialog);

    layout->addWidget(headLabel);
    layout->addWidget(headTimeEdit);
    layout->addWidget(tailLabel);
    layout->addWidget(tailTimeEdit);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    dialog->setLayout(layout);

    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(confirmButton, &QPushButton::clicked, [=]() {
        QDateTime headtime = headTimeEdit->dateTime();
        QDateTime tailtime = tailTimeEdit->dateTime();

        if (tailtime <= headtime) {
            QMessageBox::warning(dialog, "时间错误", "结束时间必须晚于开始时间！");
            return;
        }

        dialog->accept();

        // 计算时间差并选择分组方式
        qint64 deltaSeconds = headtime.secsTo(tailtime);
        QString groupBy;
        if (deltaSeconds <= 3600) { // 小于1小时
            groupBy = "DATE_FORMAT(time, '%Y-%m-%d %H:%i')"; // 按分钟分组
        } else if (deltaSeconds <= 432000) { // 小于5天
            groupBy = "DATE_FORMAT(time, '%Y-%m-%d %H')"; // 按小时分组
        } else if (deltaSeconds <= 7776000) { // 小于3个月
            groupBy = "DATE_FORMAT(time, '%Y-%m-%d')"; // 按天分组
        } else { // 大于3个月
            groupBy = "DATE_FORMAT(time, '%Y-%m')"; // 按月分组
        }

        // 查询数据库并分组计算平均值
        QSqlQuery query;
        QString sql = QString(
                          "SELECT %1 AS groupKey, AVG(value) AS avgValue "
                          "FROM %2 "
                          "WHERE time BETWEEN '%3' AND '%4' "
                          "GROUP BY groupKey "
                          "ORDER BY groupKey ASC"
                          ).arg(groupBy)
                          .arg(sensor_name)
                          .arg(headtime.toString("yyyy-MM-dd HH:mm:ss"))
                          .arg(tailtime.toString("yyyy-MM-dd HH:mm:ss"));



        QVector<QPointF> points;
        if (query.exec(sql)) {
            while (query.next()) {
                QString groupKey = query.value("groupKey").toString();
                double avgValue = query.value("avgValue").toDouble();

                // 将 x 轴索引和分组值存入 points
                points.append(QPointF(points.size(), avgValue));
            }
        } else {
            QMessageBox::critical(dialog, "数据库错误", "查询失败: " + query.lastError().text());
            return;
        }

        // 创建新的图表
        QLineSeries *series = new QLineSeries;
        series->append(points);

        // 设置线条颜色和平滑
        series->setColor(lineColor);
        series->setPointsVisible(smoothLines);

        QChart *chart = new QChart;
        chart->addSeries(series);

        // 创建 X 和 Y 轴
        QValueAxis *axisX = new QValueAxis;
        QValueAxis *axisY = new QValueAxis;

        // 设置 X 轴范围和刻度
        axisX->setRange(0, points.size() - 1);
        axisX->setTickCount(points.size());
        axisX->setLabelFormat("%d");

        // 设置 Y 轴范围
        if (sensor_name == "temperature") {
            axisY->setRange(-10, 40);
        } else if (sensor_name == "humidity") {
            axisY->setRange(0, 100);
        } else {
            axisY->setRange(0.5, 1.5);
        }

        // 设置坐标轴标题
        if (showAxisTitle) {
            axisX->setTitleText("记录索引");
            axisY->setTitleText("平均值");
        }
        chart->setAnimationOptions(QChart::SeriesAnimations);  //设置表动画

        // 将轴绑定到图表
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);

        // 设置背景
        if (showBackground) {
            chart->setBackgroundBrush(QBrush(Qt::lightGray));
        } else {
            chart->setBackgroundBrush(QBrush(Qt::white));
        }

        QPen currentPen = series->pen(); // 获取当前画笔
        if (smoothLines) {
            currentPen.setWidth(2);               // 设置线条宽度
            currentPen.setStyle(Qt::DashLine);    // 设置虚线样式
        } else {
            currentPen.setWidth(1);               // 设置默认线条宽度
            currentPen.setStyle(Qt::SolidLine);   // 设置实线样式
        }
        series->setPen(currentPen);

        // 设置网格显示
        axisX->setGridLineVisible(showGrid);
        axisY->setGridLineVisible(showGrid);

        // 设置图表标题（包含头尾时间）
        chart->setTitle(QString("%1 ~ %2")
                            .arg(headtime.toString("yyyy-MM-dd HH:mm"))
                            .arg(tailtime.toString("yyyy-MM-dd HH:mm")));

        // 显示图表窗口
        chartView->setChart(chart);
        ChartWindow *chartshow = new ChartWindow(chartView);
        chartshow->show();
    });

    // 显示弹窗
    dialog->exec();
}


sensor1::~sensor1()
{
    delete ui;
}




//增添数据功能
void sensor1::on_addbtn_clicked()
{// 创建并显示添加数据对话框
    AddDataDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户输入的数据
        QString model = dialog.getModel();
        QString location = dialog.getLocation();
        double value = dialog.getValue();
        QString time = dialog.getTime(); // 获取时间（已经格式化为字符串）
        qDebug()<<time;
        // 插入数据到数据库
        QSqlQuery query;
        QString queryStr = QString("INSERT INTO %1 (name, locate, value, time) "
                                   "VALUES (:model, :location, :value, :time)")
                               .arg(sensor_name);
        query.prepare(queryStr);
        query.bindValue(":model", model);
        query.bindValue(":location", location);
        query.bindValue(":value", value);
        query.bindValue(":time", time);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "添加失败: " + query.lastError().text());
        } else {
            db.commit();
            QMessageBox::information(this, "Success", "数据添加成功");
            // 重新加载数据
            tableModel->select(); // 如果需要更新表格
        }
    }
}

//右键功能
void sensor1::on_tableView_customContextMenuRequested(const QPoint &pos)
{   qDebug()<<"右键功能已触发";
    // 创建右键菜单
    QMenu contextMenu;
    QAction *editAction = contextMenu.addAction("Edit");
    QAction *deleteAction = contextMenu.addAction("Delete");

    // 获取选中的行
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) {
        return; // 如果点击的位置没有对应的有效项，就不弹出菜单
    }

    QAction *selectedAction = contextMenu.exec(ui->tableView->viewport()->mapToGlobal(pos));

    if (selectedAction == editAction) {
        // 处理编辑操作
        qDebug() << "Edit action triggered.";
        // 调用编辑功能
        onEditData(index);
    } else if (selectedAction == deleteAction) {
        // 处理删除操作
        qDebug() << "Delete action triggered.";
        // 调用删除功能
        onDeleteData(index);
    }
}

void sensor1::onDeleteData(const QModelIndex &index) {
    // 获取选中行的数据
    int row = index.row();
    QSqlRecord record = tableModel->record(row);
    int id = record.value("id").toInt();

    // 弹出确认删除对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "删除数据",
        "你确定删除这个数据？",
        QMessageBox::Yes | QMessageBox::No
        );

    // 如果用户选择 "Yes"，执行删除操作
    if (reply == QMessageBox::Yes) {
        // 删除数据库中的数据
        QString queryStr = QString("DELETE FROM %1 WHERE id = :id").arg(sensor_name); // 直接拼接表名
        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":id", id);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to delete data: " + query.lastError().text());
        } else {
            db.commit();
            QMessageBox::information(this, "Success", "Data deleted successfully!");
            tableModel->select(); // 刷新表格数据
        }
    }
}

//编辑数据（右键）
void sensor1::onEditData(const QModelIndex &index) {
    // 获取选中行的数据
    int row = index.row();
    QSqlRecord record = tableModel->record(row);

    // 获取行中的数据
    QString model = record.value("name").toString();
    QString location = record.value("locate").toString();
    double value = record.value("value").toDouble();
    QString time = record.value("time").toString();

    // 创建并显示编辑对话框
    AddDataDialog dialog(this);

    // 设置初始值
    dialog.modelComboBox->setCurrentText(model);         // 设置传感器型号
    dialog.locationComboBox->setCurrentText(location);   // 设置位置
    dialog.valueEdit->setText(QString::number(value));   // 设置值
    dialog.timeEdit->setDateTime(QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss")); // 设置时间

    if (dialog.exec() == QDialog::Accepted) {
        // 获取修改后的数据
        model = dialog.getModel();
        location = dialog.getLocation();
        value = dialog.getValue();
        time = dialog.getTime();

        // 更新数据库中的数据
        QSqlQuery query;
        QString queryStr = QString("UPDATE %1 SET name = :model, locate = :location, value = :value, time = :time WHERE id = :id")
                               .arg(sensor_name);
        query.prepare(queryStr);
        query.bindValue(":model", model);
        query.bindValue(":location", location);
        query.bindValue(":value", value);
        query.bindValue(":time", time);
        query.bindValue(":id", record.value("id").toInt());

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to update data: " + query.lastError().text());
        } else {
            db.commit();
            QMessageBox::information(this, "Success", "Data updated successfully!");
            tableModel->select(); // 刷新表格数据
        }
    }
}





void sensor1::on_editbtn_clicked()
{
    // 弹出输入对话框，让用户输入需要编辑的 ID
    bool ok;
    int id = QInputDialog::getInt(this, "Edit Data", "Enter ID:", 0, 0, 100000, 1, &ok);

    if (!ok) {
        // 用户取消输入
        return;
    }

    // 查询数据库中是否有该 ID 的数据
    QSqlQuery query;
    query.prepare("SELECT name, locate, value, time FROM "+sensor_name+" WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to query the database: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Not Found", "No data found with the specified ID.");
        return;
    }

    // 获取查询结果
    QString model = query.value(0).toString();
    QString location = query.value(1).toString();
    double value = query.value(2).toDouble();
    QString time = query.value(3).toString();

    // 弹出 AddDataDialog 对话框，填充数据
    AddDataDialog dialog(this);
    dialog.setModel(model);
    dialog.setLocation(location);
    dialog.setValue(value);
    dialog.setTime(time);

    if (dialog.exec() == QDialog::Accepted) {
        // 用户确认编辑，获取新的数据
        QString newModel = dialog.getModel();
        QString newLocation = dialog.getLocation();
        double newValue = dialog.getValue();
        QString newTime = dialog.getTime();

        // 更新数据库
        QSqlQuery updateQuery;

        updateQuery.prepare("UPDATE "+ sensor_name+" SET name = :model, locate = :location, value = :value, time = :time WHERE id = :id");
        updateQuery.bindValue(":model", newModel);
        updateQuery.bindValue(":location", newLocation);
        updateQuery.bindValue(":value", newValue);
        updateQuery.bindValue(":time", newTime);
        updateQuery.bindValue(":id", id);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to update the data: " + updateQuery.lastError().text());
        } else {
            QMessageBox::information(this, "Success", "Data updated successfully!");
        }
    }
    tableModel->select();
}

//删除按钮
void sensor1::on_delbtn_clicked()
{
    // 弹出输入对话框，让用户输入需要删除的 ID
    bool ok;
    int id = QInputDialog::getInt(this, "Delete Data", "Enter ID:", 0, 0, 100000, 1, &ok);

    if (!ok) {
        // 用户取消输入
        return;
    }

    // 查询数据库中是否有该 ID 的数据
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM "+sensor_name+ " WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to query the database: " + query.lastError().text());
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Not Found", "No data found with the specified ID.");
        return;
    }

    // 弹出确认删除对话框
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Confirmation",
                                                              QString("你确定要删除ID为 %1的数据?").arg(id),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 用户确认删除
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM "+sensor_name+" WHERE id = :id");
        deleteQuery.bindValue(":id", id);

        if (!deleteQuery.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to delete the data: " + deleteQuery.lastError().text());
        } else {
            QMessageBox::information(this, "Success", "Data deleted successfully!");
        }
    }
    tableModel->select();
}


// void sensor1::showLoadingScreen()
// {
//     // 在 menu 窗口显示加载界面
//     QMovie *movie = new QMovie(":/image/loading.gif");  // 根据你的文件路径设置
//     ui->loadLabel->setMovie(movie);
//     movie->start();
//     ui->loadLabel->show();
//     ui->loadtext->show();
// }

// void sensor1::hideLoadingScreen()
// {
//     // 隐藏加载界面
//     ui->loadLabel->hide();
//     ui->loadtext->hide();
// }


void sensor1::on_toolButton_clicked()
{
    set *setting = new set();
    setting->show();
}


