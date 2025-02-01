#include "adddatadialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDateTime>

AddDataDialog::AddDataDialog(QWidget *parent)
    : QDialog(parent),
    modelComboBox(new QComboBox(this)),
    locationComboBox(new QComboBox(this)),
    valueEdit(new QLineEdit(this)),
    timeEdit(new QDateTimeEdit(this)),
    confirmButton(new QPushButton("确认", this)),
    cancelButton(new QPushButton("取消", this))
{
    setWindowTitle("Add Data");
    // 设置 modelComboBox 中的传感器型号
    modelComboBox->addItem("DHT22");
    modelComboBox->addItem("HIH-4000-001");
    modelComboBox->addItem("HMP110");
    modelComboBox->addItem("HTS221");
    modelComboBox->addItem("SHT31");

    // 设置 locationComboBox 中的位置选项
    locationComboBox->addItem("南长街");
    locationComboBox->addItem("江南大学");
    locationComboBox->addItem("融创乐园");
    locationComboBox->addItem("长广溪湿地公园");
    locationComboBox->addItem("鼋头渚");

    // 设置时间编辑框的默认时间为当前时间
    timeEdit->setDateTime(QDateTime::currentDateTime());
    timeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

    // 布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(new QLabel("Model:"), modelComboBox);
    formLayout->addRow(new QLabel("Location:"), locationComboBox);
    formLayout->addRow(new QLabel("Value:"), valueEdit);
    formLayout->addRow(new QLabel("Time:"), timeEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(confirmButton);
    mainLayout->addWidget(cancelButton);

    setLayout(mainLayout);

    // 信号与槽的连接
    connect(confirmButton, &QPushButton::clicked, this, &AddDataDialog::onConfirm);
    connect(cancelButton, &QPushButton::clicked, this, &AddDataDialog::onCancel);
}

// 获取用户输入的内容
QString AddDataDialog::getModel() const {
    return modelComboBox->currentText();
}

QString AddDataDialog::getLocation() const {
    return locationComboBox->currentText();
}

double AddDataDialog::getValue() const {
    return valueEdit->text().toDouble();
}

QString AddDataDialog::getTime() const {
    return timeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
}

// 设置初始值，用于编辑功能
void AddDataDialog::setModel(const QString &model) {
    modelComboBox->setCurrentText(model);
}

void AddDataDialog::setLocation(const QString &location) {
    locationComboBox->setCurrentText(location);
}

void AddDataDialog::setValue(double value) {
    valueEdit->setText(QString::number(value));
}

void AddDataDialog::setTime(const QString &time) {
    timeEdit->setDateTime(QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss"));
}

// 确认操作
void AddDataDialog::onConfirm() {
    // 简单数据验证
    if (getModel().isEmpty() || getLocation().isEmpty() || valueEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all fields!");
        return;
    }

    accept();  // 提交并关闭对话框
}

// 取消操作
void AddDataDialog::onCancel() {
    reject();  // 取消并关闭对话框
}
