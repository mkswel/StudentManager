﻿#ifndef GLOBEL_H
#define GLOBEL_H

#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>
#include <QMessageBox>
#include <QSqlError>
#include <QButtonGroup>
#include <QStyleFactory>
#include<QStandardItemModel>
#include <QAbstractItemView>
#include <QThread>
#include <QProgressBar>
#include <QProgressDialog>
#include <QFile>
#include <QTextCodec>
enum MODIFY_ADD{
    MODIFY,
    ADD,
    CHECK
};
enum sqlInit{
    EXIST,
    INEXIST
};
const QString USERNAME = "root";
const QString PASSWORD = "812814";
const QString PASSWORD_WWY1 = "123456";
const QString PASSWORD_WWY2 = "142857Wwy!";
const QString HOST1 = "localhost";
const QString HOST2 = "192.168.0.188";
const QString HOST3 = "43.137.49.86";
const QString DATABASE1 = "studentmanager";
const QString DATABASE2 = "studentmanagerfromlinux";
const QString DATABASE3 = "shuaka";
#endif // GLOBEL_H

/*
 * 2023/1/3 wwy
 * 新增学生选课中心
 * 新增师生修改密码
 * 完善学生管理的新增和删除按键功能
 * 完善所有表格格式：显示表头，字体居中
 * 美化消息弹窗
 * 成绩录入的统计按键
 * 修改成绩查询学期为下拉选项框
 *
 * 数据库部分要添加一条 alter table sc add primary key(sno, cno);
 */
