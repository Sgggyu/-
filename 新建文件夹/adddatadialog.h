#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QLineEdit>

class AddDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDataDialog(QWidget *parent = nullptr);

    // 获取用户输入
    QString getModel() const;
    QString getLocation() const;
    double getValue() const;
    QString getTime() const;

    // 设置初始值（编辑模式用）
    void setModel(const QString &model);
    void setLocation(const QString &location);
    void setValue(double value);
    void setTime(const QString &time);

private slots:
    void onConfirm();
    void onCancel();

public:
    QComboBox *modelComboBox;    // 传感器型号
    QComboBox *locationComboBox; // 位置选项
    QLineEdit *valueEdit;        // 数值
    QDateTimeEdit *timeEdit;     // 时间
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};

#endif // ADDDATADIALOG_H
