#ifndef MENU_H
#define MENU_H
#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include<QMessageBox>

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QMainWindow *mainWindow = nullptr, QWidget *parent = nullptr); // 接受 QMainWindow 指针
    ~menu();
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
    void on_btn_1_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::menu *ui;
    QMainWindow *mainWindow; // 保存指向 MainWindow 的指针

    void showLoadingScreen();
    void hideLoadingScreen();
};

#endif // MENU_H
