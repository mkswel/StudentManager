#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "teacherwindow.h"
#include "studentwindow.h"
#include "reset.h"
#include "sqlconnect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();


private slots:
    void on_Btn_Login_clicked();

    void on_Btn_Reset_clicked();

    void on_Btn_Exit_clicked();

    void on_Btn_Pass_clicked();

    void sql_connect(QSqlDatabase, int);



private:
    Ui::LoginWindow *ui;
    QProgressDialog *progressDialog;
    QSqlDatabase db;
    QThread * thread;
    void connected();

signals:
    void starting();

};

#endif // LOGINWINDOW_H
