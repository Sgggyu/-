#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(const QString &columnName, const QStringList &values, QWidget *parent = nullptr);
    ~FilterDialog();

signals:
    void filterApplied(const QStringList &selectedValues);

private slots:
    void onSearchTextChanged(const QString &text);  // 搜索栏过滤槽
    void onSelectAllToggled(bool checked);          // 全选复选框槽函数
    void onApplyButtonClicked();                   // 应用筛选槽

private:
    QLineEdit *searchLineEdit;     // 搜索栏
    QCheckBox *selectAllCheckBox;  // 全选复选框
    QListWidget *listWidget;       // 列表项
    QPushButton *applyButton;      // 应用按钮
    QStringList allValues;         // 完整选项列表

    void updateListWidget(const QStringList &values);  // 更新列表项
};

#endif // FILTERDIALOG_H
