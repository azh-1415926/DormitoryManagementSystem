#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include "constans.h"
#include "utils.hpp"

class _DataBase
{
    private:
        QSqlDatabase m_Database;
        QString m_LastError;

    public:
        _DataBase()
        {
            if (QSqlDatabase::contains("qt_sql_default_connection"))
            {
                m_Database = QSqlDatabase::database("qt_sql_default_connection");
            }
            else
            {
                m_Database = QSqlDatabase::addDatabase(DataBaseType);
                m_Database.setHostName(Host);
                m_Database.setDatabaseName(DatabaseName);
                m_Database.setUserName(UserName);
                m_Database.setPassword(UserPassword);
            }

            if (m_Database.open())
            {
                qDebug() << "Database opened successfully！";
            }
            else
            {
                qDebug() << "Database opened failed:" << m_Database.lastError().text();
            }

            // execSql(":/sql/drop_tables");
            execSql(":/sql/create_tables");
        }

        QString lastError() { return m_LastError; }

        void execSql(const QString& filePath)
        {
            QString str=readFile(filePath);
            QSqlQuery query;

            for(const auto& i : str.split(";"))
            {
                if(query.exec(i))
                {
                    qDebug()<<"execSql Successful!";
                }
                else
                {
                    m_LastError="execSql Failed!";
                }
            }
        }

        bool query(const QString& str)
        {
            QSqlQuery query;

            bool isSuccess=query.exec(str);

            if(!isSuccess)
                m_LastError=query.lastError().text();

            return isSuccess;
        }

        bool insert(const QString& table_name,const QList<QVariant>& fields)
        {
            QString str=toString(fields,",");

            QString query_str="INSERT INTO "+table_name+" VALUES ("+str+");";

            QSqlQuery query;

            if(!query.exec(query_str))
            {
                m_LastError="insert student table Failed! "+query.lastError().text();
                return false;
            }

            return true;
        }

        bool remove(const QString& table_name,const QString& condition)
        {
            QList<QList<QVariant>> result;

            QString query_str="DELETE FROM "+table_name+" WHERE "+condition;
            QSqlQuery query;

            query.exec(query_str);
            query.exec("SELECT changes();");

            if(!query.next())
                return false;

            if(query.value(0).toInt()==0)
            {
                m_LastError="delete student table Failed! "+query.lastError().text();
                return false;
            }

            return true;
        }

        QList<QList<QVariant>> select(const QString& table_name,const QStringList& fields={})
        {
            QList<QList<QVariant>> result;
            QString field=((fields.size()>1)?(fields.join(',')):(fields.isEmpty())?"*":fields[0]);
            QString query_str="SELECT "+field+" FROM "+table_name;
            QSqlQuery query;

            if(query.exec(query_str))
            {
                qDebug()<<"select student table Successful!";
            }
            else
            {
                m_LastError="select student table Failed! "+query.lastError().text();
                return result;
            }

            while(query.next())
            {
                QList<QVariant> list;
                const QSqlRecord& record=query.record();

                for(int i=0;i<record.count();i++)
                {
                    list<<record.value(i);
                }
                
                result<<list;
            }

            return result;
        }

        QList<QList<QVariant>> select(const QString& table_name,const QString& condition,const QStringList& fields={})
        {
            QList<QList<QVariant>> result;

            QString field=((fields.size()>1)?(fields.join(',')):(fields.isEmpty())?"*":fields[0]);
            QString query_str="SELECT "+field+" FROM "+table_name+" WHERE "+condition;
            QSqlQuery query;

            if(query.exec(query_str))
            {
                qDebug()<<"select student table Successful!";
            }
            else
            {
                m_LastError="select student table Failed! "+query.lastError().text();
                return result;
            }

            while(query.next())
            {
                QList<QVariant> list;
                const QSqlRecord& record=query.record();

                for(int i=0;i<record.count();i++)
                {
                    list<<record.value(i);
                }
                
                result<<list;
            }

            return result;
        }

        bool update(const QString& table_name,const QString& content,const QString& condition)
        {
            QList<QList<QVariant>> result;

            QString query_str="UPDATE "+table_name+" SET "+content+" WHERE "+condition;
            QSqlQuery query;

            qDebug()<<query_str;

            query.exec(query_str);
            query.exec("SELECT changes();");

            if(!query.next())
                return false;

            if(query.value(0).toInt()==0)
            {
                m_LastError="update student table Failed! "+query.lastError().text();
                return false;
            }

            return true;
        }
};

class DataBase
{
    private:
        ;

    public:
        DataBase() { }
        ~DataBase() { }

        _DataBase* getInstance()
        {
            static _DataBase* m_Data;
            if(!m_Data)
                m_Data=new _DataBase;
            return m_Data;
        }
};

#endif /* DATABASE_H */
