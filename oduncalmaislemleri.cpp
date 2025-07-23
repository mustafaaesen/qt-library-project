#include "oduncalmaislemleri.h"
#include "ui_oduncalmaislemleri.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

OduncAlmaIslemleri::OduncAlmaIslemleri(QWidget *parent)
    : QDialog(parent), ui(new Ui::OduncAlmaIslemleri)
{
    ui->setupUi(this);
    get_uye_list();
    get_kitap_list();
    get_odunc_list();
    connect(ui->uye_tablo, &QTableWidget::cellClicked, this, &OduncAlmaIslemleri::on_uye_table_select);
    connect(ui->kitap_tablo, &QTableWidget::cellClicked, this, &OduncAlmaIslemleri::on_kitap_table_select);
}

OduncAlmaIslemleri::~OduncAlmaIslemleri()
{
    delete ui;
}

void OduncAlmaIslemleri::get_uye_list()
{
    ui->uye_tablo->setRowCount(0);

    QSqlQuery query("SELECT * FROM uye");
    ui->uye_tablo->setColumnCount(3);
    ui->uye_tablo->setHorizontalHeaderLabels(QStringList() << "No" << "Ad" << "Soyad");

    while (query.next())
    {
        int newRow = ui->uye_tablo->rowCount();
        ui->uye_tablo->insertRow(newRow);

        ui->uye_tablo->setItem(newRow, 0, new QTableWidgetItem(query.value("uye_no").toString()));
        ui->uye_tablo->setItem(newRow, 1, new QTableWidgetItem(query.value("uye_ad").toString()));
        ui->uye_tablo->setItem(newRow, 2, new QTableWidgetItem(query.value("uye_soyad").toString()));
    }
}

void OduncAlmaIslemleri::get_kitap_list()
{
    ui->kitap_tablo->setRowCount(0);

    QSqlQuery query("SELECT * FROM kitap");
    ui->kitap_tablo->setColumnCount(3);
    ui->kitap_tablo->setHorizontalHeaderLabels(QStringList() << "No" << "Ad" << "kitap_sayisi");

    while (query.next())
    {
        int newRow = ui->kitap_tablo->rowCount();
        ui->kitap_tablo->insertRow(newRow);

        ui->kitap_tablo->setItem(newRow, 0, new QTableWidgetItem(query.value("kitap_no").toString()));
        ui->kitap_tablo->setItem(newRow, 1, new QTableWidgetItem(query.value("kitap_ad").toString()));
        ui->kitap_tablo->setItem(newRow, 2, new QTableWidgetItem(query.value("kitap_sayisi").toString()));
    }
}

void OduncAlmaIslemleri::get_odunc_list()
{
    ui->odunc_tablo->setRowCount(0);

    QSqlQuery query("SELECT * FROM odunc_alinan");
    ui->odunc_tablo->setColumnCount(3);
    ui->odunc_tablo->setHorizontalHeaderLabels(QStringList() << "Üye No" << "Kitap No" << "Ödünç Alma Tarihi");

    while (query.next())
    {
        int newRow = ui->odunc_tablo->rowCount();
        ui->odunc_tablo->insertRow(newRow);

        ui->odunc_tablo->setItem(newRow, 0, new QTableWidgetItem(query.value("uye_no").toString()));
        ui->odunc_tablo->setItem(newRow, 1, new QTableWidgetItem(query.value("kitap_no").toString()));
        ui->odunc_tablo->setItem(newRow, 2, new QTableWidgetItem(query.value("odunc_alma_tarihi").toString()));
    }
}

void OduncAlmaIslemleri::on_uye_table_select()
{
    int currentRow = ui->uye_tablo->currentRow();

    if (currentRow != -1)
    {
        QTableWidgetItem *item_no = ui->uye_tablo->item(currentRow, 0);

        if (item_no)
        {
            ui->uye_no->setText(item_no->text());
        }
    }
}

void OduncAlmaIslemleri::on_kitap_table_select()
{
    int currentRow = ui->kitap_tablo->currentRow();

    if (currentRow != -1)
    {
        QTableWidgetItem *item_no = ui->kitap_tablo->item(currentRow, 0);

        if (item_no)
        {
            ui->kitap_no->setText(item_no->text());
        }
    }
}

void OduncAlmaIslemleri::on_btn_odunc_alma_clicked()
{
    QString uye_no = ui->uye_no->text();
    QString kitap_no = ui->kitap_no->text();
    QString odunc_alma_tarihi = ui->date_odunc->date().toString("dd.MM.yyyy");

    QSqlQuery checkBorrowed;
    checkBorrowed.prepare("SELECT * FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    checkBorrowed.bindValue(":uye_no", uye_no);
    checkBorrowed.bindValue(":kitap_no", kitap_no);
    checkBorrowed.exec();

    if (checkBorrowed.next())
    {
        QMessageBox::warning(this, "Hata!", "Bu üye zaten bu kitabı ödünç almış.");
        return;
    }

    QSqlQuery checkStock;
    checkStock.prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = :kitap_no");
    checkStock.bindValue(":kitap_no", kitap_no);
    checkStock.exec();

    if (checkStock.next())
    {
        int kitap_sayisi = checkStock.value(0).toInt();

        if (kitap_sayisi <= 0)
        {
            QMessageBox::warning(this, "Hata!", "Kitap stokta bulunmamaktadır.");
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO odunc_alinan (uye_no, kitap_no, odunc_alma_tarihi) VALUES (:uye_no, :kitap_no, :odunc_alma_tarihi)");
    query.bindValue(":uye_no", uye_no);
    query.bindValue(":kitap_no", kitap_no);
    query.bindValue(":odunc_alma_tarihi", odunc_alma_tarihi);

    if (query.exec())
    {
        QSqlQuery updateStock;
        updateStock.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi - 1 WHERE kitap_no = :kitap_no");
        updateStock.bindValue(":kitap_no", kitap_no);
        updateStock.exec();

        get_odunc_list();
        get_kitap_list();
        get_uye_list();
    }
    else
    {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}
