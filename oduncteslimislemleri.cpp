#include "oduncteslimislemleri.h"
#include "ui_oduncteslimislemleri.h"
#include <QSqlQuery>
#include <QSqlError>

OduncTeslimIslemleri::OduncTeslimIslemleri(QWidget *parent)
    : QDialog(parent), ui(new Ui::OduncTeslimIslemleri)
{
    ui->setupUi(this);
    get_odunc_list();
    get_teslim_list();
    connect(ui->odunc_tablo, &QTableWidget::cellClicked, this, &OduncTeslimIslemleri::on_odunc_table_select);
    connect(ui->teslim_tablo, &QTableWidget::cellClicked, this, &OduncTeslimIslemleri::on_teslim_table_select);
    connect(ui->btn_teslim, &QPushButton::clicked, this, &OduncTeslimIslemleri::on_btn_teslim_clicked);
}

OduncTeslimIslemleri::~OduncTeslimIslemleri()
{
    delete ui;
}

void OduncTeslimIslemleri::get_odunc_list()
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

void OduncTeslimIslemleri::get_teslim_list()
{
    ui->teslim_tablo->setRowCount(0);

    QSqlQuery query("SELECT * FROM odunc_teslim_edilen");
    ui->teslim_tablo->setColumnCount(5);
    ui->teslim_tablo->setHorizontalHeaderLabels(QStringList() << "Üye No" << "Kitap No" << "Ödünç Alma Tarihi" << "Ödünç Verme Tarihi" << "Borç");

    while (query.next())
    {
        int newRow = ui->teslim_tablo->rowCount();
        ui->teslim_tablo->insertRow(newRow);

        ui->teslim_tablo->setItem(newRow, 0, new QTableWidgetItem(query.value("uye_no").toString()));
        ui->teslim_tablo->setItem(newRow, 1, new QTableWidgetItem(query.value("kitap_no").toString()));
        ui->teslim_tablo->setItem(newRow, 2, new QTableWidgetItem(query.value("alma_tarihi").toString()));
        ui->teslim_tablo->setItem(newRow, 3, new QTableWidgetItem(query.value("verme_tarihi").toString()));
        ui->teslim_tablo->setItem(newRow, 4, new QTableWidgetItem(query.value("borc").toString()));
    }
}

void OduncTeslimIslemleri::on_odunc_table_select()
{
    int currentRow = ui->odunc_tablo->currentRow();
    if (currentRow != -1)
    {
        QTableWidgetItem *item_uye_no = ui->odunc_tablo->item(currentRow, 0);
        QTableWidgetItem *item_kitap_no = ui->odunc_tablo->item(currentRow, 1);

        if (item_uye_no && item_kitap_no)
        {
            ui->uye_no->setText(item_uye_no->text());
            ui->kitap_no->setText(item_kitap_no->text());
        }
    }
}

void OduncTeslimIslemleri::on_teslim_table_select()
{
    int currentRow = ui->teslim_tablo->currentRow();
    if (currentRow != -1)
    {
        QTableWidgetItem *item_uye_no = ui->teslim_tablo->item(currentRow, 0);
        QTableWidgetItem *item_kitap_no = ui->teslim_tablo->item(currentRow, 1);

        if (item_uye_no && item_kitap_no)
        {
            ui->uye_no->setText(item_uye_no->text());
            ui->kitap_no->setText(item_kitap_no->text());
        }
    }
}

void OduncTeslimIslemleri::on_btn_teslim_clicked()
{
    QString uye_no = ui->uye_no->text();
    QString kitap_no = ui->kitap_no->text();
    QString teslim_tarihi = ui->teslim_tarihi->date().toString("dd.MM.yyyy");

    QSqlQuery getOduncAlmaTarihi;
    getOduncAlmaTarihi.prepare("SELECT odunc_alma_tarihi FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    getOduncAlmaTarihi.bindValue(":uye_no", uye_no);
    getOduncAlmaTarihi.bindValue(":kitap_no", kitap_no);
    getOduncAlmaTarihi.exec();

    if (getOduncAlmaTarihi.next())
    {
        QDate odunc_alma_tarihi = QDate::fromString(getOduncAlmaTarihi.value(0).toString(), "dd.MM.yyyy");
        QDate teslim_tarihi_date = QDate::fromString(teslim_tarihi, "dd.MM.yyyy");

        int days = odunc_alma_tarihi.daysTo(teslim_tarihi_date);
        int borc = 0;
        if (days > 15)
        {
            borc = (days - 15) * 2;
        }

        QString borc_str = QString::number(borc) + " TL";

        QSqlQuery query;
        query.prepare("INSERT INTO odunc_teslim_edilen (uye_no, kitap_no, alma_tarihi, verme_tarihi, borc) VALUES (:uye_no, :kitap_no, :alma_tarihi, :verme_tarihi, :borc)");
        query.bindValue(":uye_no", uye_no);
        query.bindValue(":kitap_no", kitap_no);
        query.bindValue(":alma_tarihi", odunc_alma_tarihi.toString("dd.MM.yyyy"));
        query.bindValue(":verme_tarihi", teslim_tarihi);
        query.bindValue(":borc", borc_str);

        if (!query.exec())
        {
            qDebug() << query.lastError().text();
        }
        QSqlQuery updateStock;
        updateStock.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi + 1 WHERE kitap_no = :kitap_no");
        updateStock.bindValue(":kitap_no", kitap_no);
        updateStock.exec();

        QSqlQuery deleteBorrowed;
        deleteBorrowed.prepare("DELETE FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
        deleteBorrowed.bindValue(":uye_no", uye_no);
        deleteBorrowed.bindValue(":kitap_no", kitap_no);
        deleteBorrowed.exec();

        get_odunc_list();
        get_teslim_list();
    }
}