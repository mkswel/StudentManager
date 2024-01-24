#ifndef SQL_CONNECT_H
#define SQL_CONNECT_H

#include <QObject>

class sql_connect : public QObject
{
    Q_OBJECT
public:
    explicit sql_connect(QObject *parent = nullptr);

signals:

};

#endif // SQL_CONNECT_H
