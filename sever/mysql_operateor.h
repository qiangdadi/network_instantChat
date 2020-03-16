#ifndef MYSQL_OPERATEOR_H
#define MYSQL_OPERATEOR_H
#include <QSql>
#include <QSqlDatabase>
class MySql_operateor
{
public:
    MySql_operateor();
    bool operator_sql(QString type,QString msg);
    //bool query_sql(QString sentence);
    //void delete_sql(QString sentence);
    //void update_sql(QString sentence);
private:
    static int i;//连接数据库的连接名
    QSqlDatabase openDataBase(QString connectionName);
    void closeDataBase(QString connectionName);
};
#endif // MYSQL_OPERATEOR_H
