#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include "mainwindow.h"

bool initDatabase()
{
    QSqlDatabase db;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("library.db");

    if (!QFile::exists("library.db"))
    {
        if (!db.open())
        {
            QMessageBox::critical(nullptr, QObject::tr("Veri tabanı açılamadı"),
                                  QObject::tr("Veri tabanına bağlanılamadı.\n"), QMessageBox::Cancel);
            return false;
        }

        QSqlQuery query;
        query.exec("CREATE TABLE uye ("
                   "uye_no INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "uye_ad TEXT NOT NULL,"
                   "uye_soyad TEXT NOT NULL"
                   ");");

        query.exec("CREATE TABLE kitap ("
                   "kitap_no INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "kitap_ad TEXT NOT NULL,"
                   "kitap_sayisi INTEGER NOT NULL"
                   ");");

        query.exec("CREATE TABLE odunc_alinan ("
                   "uye_no INTEGER,"
                   "kitap_no INTEGER,"
                   "odunc_alma_tarihi TEXT,"
                   "FOREIGN KEY (uye_no) REFERENCES uye(uye_no),"
                   "FOREIGN KEY (kitap_no) REFERENCES kitap(kitap_no)"
                   ");");

        query.exec("CREATE TABLE odunc_teslim_edilen ("
                   "uye_no INTEGER,"
                   "kitap_no INTEGER,"
                   "alma_tarihi TEXT,"
                   "verme_tarihi TEXT,"
                   "borc INTEGER,"
                   "FOREIGN KEY (uye_no) REFERENCES uye(uye_no),"
                   "FOREIGN KEY (kitap_no) REFERENCES kitap(kitap_no)"
                   ");");
    }

    return true;
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (!db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Veri tabanı açılamadı"),
                              QObject::tr("Veri tabanına bağlanılamadı.\n"), QMessageBox::Cancel);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!initDatabase())
    {
        return -1;
    }

    bool dbStatus = createConnection();

    MainWindow w(nullptr, dbStatus);
    w.show();
    return a.exec();
}
