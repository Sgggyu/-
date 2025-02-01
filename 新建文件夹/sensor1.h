#ifndef SENSOR1_H
#define SENSOR1_H
class menu;  // 前向声明 menu 类zz
#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QCloseEvent>
#include<QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QChartView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QtCharts>
#include"menu.h"

namespace Ui {
class sensor1;
}

class sensor1 : public QWidget
{
    Q_OBJECT

public:
    explicit sensor1(QString kindofsensor,QWidget *parent = nullptr);
    ~sensor1();
    void setupDatabase();
    void setuptable();
    void loadTableData();
protected:
    void closeEvent(QCloseEvent *event){
        QString dlgtitile = "退出";
        QString strInfo = "确定要退出吗？";
        QMessageBox::StandardButton result = QMessageBox::question(this,dlgtitile,strInfo,QMessageBox::Yes|QMessageBox::No);
        if(result == QMessageBox::Yes){
            event -> accept();
        }
        else
            event -> ignore();
    }
private slots:
    void updateTime();
    void onPrevPage();
    void onNextPage();
    // void onTableContextMenu(const QPoint &pos);
    void onColumnHeaderClicked(int column);
    void applyColumnFilter(int column, const QStringList &selectedValues);
    void chartshow();
    void on_addbtn_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_editbtn_clicked();

    void on_delbtn_clicked();

    void on_toolButton_clicked();

private:
    Ui::sensor1 *ui;
    QSqlDatabase db;
    QString sensor_name;
    menu *menuptr;
    QTimer *timer;
    // QTableView *tableView;
    QSqlTableModel *tableModel;
    QChartView *chartView;
    // QPushButton *prevButton, *nextButton,*chartshowbtn,*addbtn;
    QLineEdit *filterEdit;
    QComboBox *filterColumn;
    QStringList currentFilters; ;//用来储存已有的条件
    QMap<int, QSet<QString>> columnUniqueValues;//用来储存每个列的唯一值
    QLabel *time; // 时间显示标签

    int currentPage = 0;
    const int rowsPerPage = 500;


    void showDateTimeFilterDialog(const QString &columnName, int column);
    void applyDateTimeFilter(const QString &columnName, int column, const QDateTime &start, const QDateTime &end);
    void removeDateTimeFilter(const QString &columnName, int column);
    void showRangeFilterDialog(const QString &columnName, int column);
    void applyRangeFilter(const QString &columnName, int column, const QString &minValue, const QString &maxValue);
    void removeRangeFilter(const QString &columnName, int column);
    void updateChart();
    void onDeleteData(const QModelIndex &index);
    void onEditData(const QModelIndex &index);
    void setColumnWidths();
    // void showLoadingScreen();
    // void hideLoadingScreen();
};

#endif // SENSOR1_H
