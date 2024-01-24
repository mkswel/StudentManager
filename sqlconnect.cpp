#include "sqlconnect.h"

SqlConnect::SqlConnect(QObject *parent) : QObject(parent)
{

}


void SqlConnect::connecting(){
    //QThread::sleep(1);
    qDebug()<<QThread::currentThread();
    QStringList list = QSqlDatabase::drivers();
    qDebug()<<list;

    if (QSqlDatabase::contains("qt_sql_default_connection"))                    // 连接数据库
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
//        db = QSqlDatabase::addDatabase("QODBC");

    db.setHostName(HOST1);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD_WWY1);
    db.setConnectOptions("MYSQL_OPT_CONNECT_ATTR_SET=SET NAMES 'utf8mb4'");
    db.open();
    int init_state = -1;//数据库是否存在
    QFile file(":/data/data_init");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString SQL = in.readAll();//初始化数据库的脚本
    QSqlQuery query;
    //判断数据库是否存在
    if(query.exec("CREATE DATABASE "+ DATABASE1)){//数据库本身不存在
        init_state = INEXIST;
        db.setDatabaseName(DATABASE1);
        if(query.exec(SQL))qDebug()<<"初始化成功";
        else qDebug()<<"初始化失败";
    }
    else{
        init_state = EXIST;
        db.setDatabaseName(DATABASE1);
    }
    db.close();
    emit database_feedback(db, init_state);
}
