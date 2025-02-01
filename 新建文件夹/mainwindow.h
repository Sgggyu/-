#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QCloseEvent>
#include<QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void checkAndCreateConfigFile();
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
    void on_btn_begin_clicked();

    void on_toolbtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
