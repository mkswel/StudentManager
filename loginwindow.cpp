#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(630, 420);

    //创建进度条
    progressDialog = new QProgressDialog;
    progressDialog->reset();

    //创建connect sql线程
    thread = new QThread(this);
    SqlConnect * sql = new SqlConnect;
    sql->moveToThread(thread);
    connect(this, &LoginWindow::starting, sql, &SqlConnect::connecting);
    connect(sql, &SqlConnect::database_feedback, this, &LoginWindow::sql_connect);
    connect(progressDialog, &QProgressDialog::canceled, this, [=]()
    {
        if(thread->isRunning()){
            thread->terminate();
            if(thread->isFinished())qDebug()<<"the thread for sql-connecting has stopped";//中止
        }
    });

    ui->Login_Password->setEchoMode(QLineEdit::Password);   // 小黑点代替输入
    ui->Login_Username->setPlaceholderText("请输入用户名");
    ui->Login_Password->setPlaceholderText("请输入密码");
    ui->Mode_Student->setChecked(true);        // 设置默认选项


}

LoginWindow::~LoginWindow() {
    delete ui;
}

/**
 * @brief LoginWindow::connected
 * 用户登陆
 */
void LoginWindow::connected(){
    QString username = ui->Login_Username->text();
    QString password = ui->Login_Password->text();

    qDebug() << "用户名：" << username << "密码:" << password;

    if (ui->Mode_Teacher->isChecked()) {                                        // 登陆教师端
        QSqlQuery query(db);
        query.exec("select password "
                   "from teacher "
                   "where tno = '" + username + "'");

        if (query.next() && password == query.value(0).toString()) {
                this->hide();
                TeacherWindow *teacher = new TeacherWindow(db, username);
                teacher->show();

                connect(teacher, &TeacherWindow::back, this, [=]() {
                    this->show();
                });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.jpg"));
            box.setWindowTitle("警告");
            box.setText("用户名或密码错误");
            box.exec();
        }
    }

    else {                                                                      // 登陆学生端
        QSqlQuery query(db);
        query.exec("select password "
                   "from student "
                   "where sno = '" + username + "'");

        if (query.next() && password == query.value(0).toString()) {
                this->hide();
                StudentWindow *student = new StudentWindow(db, username);
                student->show();

                connect(student, &StudentWindow::back, this, [=]() {
                    this->show();
                });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.jpg"));
            box.setWindowTitle("警告");
            box.setText("用户名或密码错误");
            box.exec();
        }

    }
}

/**
 * @brief LoginWindow::sql_connect
 * @param database
 * 数据库连接的槽函数
 */
void LoginWindow::sql_connect(QSqlDatabase database, int STATE){
   thread->deleteLater();
   progressDialog->close();
   db = database;
   QMessageBox box;
   switch (STATE) {
   case EXIST:
       box.setIcon(QMessageBox::Information);
       box.setWindowIcon(QIcon(":/res/logo.jpg"));
       box.setWindowTitle("数据库连接");
       box.setText("数据库存在");
       box.exec();
       break;
   default:
       box.setIcon(QMessageBox::Warning);
       box.setWindowIcon(QIcon(":/res/logo.jpg"));
       box.setWindowTitle("数据库连接");
       box.setText("数据库不存在，已初始化数据库");
       box.exec();
       break;
   }
   if(!db.open()){
       QMessageBox::critical(this, "数据库连接", "连接失败！报错原因：" + db.lastError().text());
       qDebug() << "Failed to connect to root mysql admin";
       qDebug() << db.lastError().text();
   }
    else{
       qDebug() << "Successfully connected to " + db.databaseName();
       connected();
   }
}


/**
 * @brief LoginWindow::on_Btn_Login_clicked
 * 登陆按钮
 */
void LoginWindow::on_Btn_Login_clicked() {

    if(db.isOpen()){
        qDebug()<<"db is open";
        connected();
        return ;
    }

    progressDialog->setWindowTitle("数据库连接");//对话框标题
    progressDialog->setLabelText("连接到服务器...");//对话框文本
    progressDialog->setCancelButtonText("取消");//设置取消按钮
    progressDialog->setFixedWidth(300);
    progressDialog->setRange(0,0);//设置进度条范围
    progressDialog->setModal(true);//以模态方式弹出对话框

//    progressDialog->setWindowFlags(Qt::FramelessWindowHint);//无边框
//    progressDialog->setCancelButtonText(nullptr);//去除对话框内的Cancel按钮
//    progressDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);//禁用右上角所有按钮，但最小化按钮可用
//    progressDialog->autoClose();
    progressDialog->showNormal();
    thread->start();
    emit starting();
}

/**
 * @brief LoginWindow::on_Btn_Reset_clicked
 * 重置按钮
 */
void LoginWindow::on_Btn_Reset_clicked() {
    ui->Login_Username->clear();
    ui->Login_Password->clear();
}

/**
 * @brief LoginWindow::on_Btn_Exit_clicked
 * 退出按钮
 */
void LoginWindow::on_Btn_Exit_clicked() {
    this->close();
}

/**
 * @brief LoginWindow::on_Btn_Pass_clicked
 * 忘记密码
 */
void LoginWindow::on_Btn_Pass_clicked() {
    QString username = ui->Login_Username->text();
    if (username == "") {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.jpg"));
        box.setWindowTitle("警告");
        box.setText("请输入用户名");
        box.exec();
        return;
    }

    if (ui->Mode_Teacher->isChecked()) {
        QSqlQuery query(db);
        query.exec("select tname from teacher where tno = '" + username + "'");
        if (query.next()) {
            Reset *reset = new Reset(query.value(0).toString());
            reset->setWindowModality(Qt::ApplicationModal);
            reset->show();

            connect(reset, &Reset::reset, this, [=]() {
                QSqlQuery query(db);
                query.exec("update teacher set password = '" + username + "' where tno = '" + username + "'");
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.jpg"));
                box.setWindowTitle("警告");
                box.setText("密码已重置为您的职工号");
                box.exec();
            });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.jpg"));
            box.setWindowTitle("警告");
            box.setText("请输入正确的用户名");
            box.exec();
        }

    }
    else {
        QSqlQuery query(db);
        query.exec("select sname from student where sno = '" + username + "'");
        if (query.next()) {
            Reset *reset = new Reset(query.value(0).toString());
            reset->setWindowModality(Qt::ApplicationModal);
            reset->show();

            connect(reset, &Reset::reset, this, [=]() {
                QSqlQuery query(db);
                query.exec("update student set password = '" + username + "' where sno = '" + username + "'");
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.jpg"));
                box.setWindowTitle("警告");
                box.setText("密码已重置为您的学号");
                box.exec();
            });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.jpg"));
            box.setWindowTitle("警告");
            box.setText("请输入正确的用户名");
            box.exec();
        }

    }
}

