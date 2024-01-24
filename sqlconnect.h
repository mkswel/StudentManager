#ifndef SQLCONNECT_H
#define SQLCONNECT_H

#include <QObject>
#include <global.h>


class SqlConnect : public QObject
{
    Q_OBJECT
public:
    explicit SqlConnect(QObject *parent = nullptr);
    void connecting();

    QSqlDatabase db;
signals:
    void database_feedback(QSqlDatabase, int);
};

#endif // SQLCONNECT_H
