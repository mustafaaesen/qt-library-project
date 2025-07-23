#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

KitapIslemleri::KitapIslemleri(QWidget *parent)
    : QDialog(parent), ui(new Ui::KitapIslemleri)
{
    ui->setupUi(this);
    connect(ui->data_table, &QTableWidget::cellClicked, this, &KitapIslemleri::on_table_select);
    get_kitap_list();

    connect(ui->btn_kayit, &QPushButton::clicked, this, &KitapIslemleri::on_btn_kitap_ekle_clicked);
    connect(ui->btn_sil, &QPushButton::clicked, this, &KitapIslemleri::on_btn_kitap_sil_clicked);
    connect(ui->btn_guncelle, &QPushButton::clicked, this, &KitapIslemleri::on_btn_kitap_guncelle_clicked);
}

KitapIslemleri::~KitapIslemleri()
{
    delete ui;
}

void KitapIslemleri::get_kitap_list()
{
    ui->data_table->clearContents();
    ui->data_table->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT * FROM kitap");
    query.exec();

    ui->data_table->setColumnCount(3);
    ui->data_table->setHorizontalHeaderLabels(QStringList() << "Kitap No" << "Kitap Adı" << "Stok");

    while (query.next())
    {
        int row = ui->data_table->rowCount();
        ui->data_table->insertRow(row);

        ui->data_table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->data_table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->data_table->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
    }
}

void KitapIslemleri::on_table_select()
{
    int currentRow = ui->data_table->currentRow();

    if (currentRow != -1)
    {
        QTableWidgetItem *item_kitap_no = ui->data_table->item(currentRow, 0);
        QTableWidgetItem *item_kitap_ad = ui->data_table->item(currentRow, 1);
        QTableWidgetItem *item_kitap_yazar = ui->data_table->item(currentRow, 2);

        if (item_kitap_no && item_kitap_ad && item_kitap_yazar)
        {
            ui->input_no->setText(item_kitap_no->text());
            ui->input_ad->setText(item_kitap_ad->text());
            ui->input_stok->setText(item_kitap_yazar->text());
        }

        QString kitap_no = item_kitap_no->text();

        ui->table_oad->clearContents();
        ui->table_oad->setRowCount(0);
        ui->table_doad->clearContents();
        ui->table_doad->setRowCount(0);

        QSqlQuery query1;
        query1.prepare("SELECT * FROM odunc_alinan WHERE kitap_no = :kitap_no");
        query1.bindValue(":kitap_no", kitap_no);
        query1.exec();

        ui->table_oad->setColumnCount(3);
        ui->table_oad->setHorizontalHeaderLabels(QStringList() << "Üye No" << "Kitap No" << "Ödünç Alma Tarihi");

        while (query1.next())
        {
            int row = ui->table_oad->rowCount();
            ui->table_oad->insertRow(row);

            ui->table_oad->setItem(row, 0, new QTableWidgetItem(query1.value(0).toString()));
            ui->table_oad->setItem(row, 1, new QTableWidgetItem(query1.value(1).toString()));
            ui->table_oad->setItem(row, 2, new QTableWidgetItem(query1.value(2).toString()));
        }

        QSqlQuery query2;
        query2.prepare("SELECT * FROM odunc_teslim_edilen WHERE kitap_no = :kitap_no");
        query2.bindValue(":kitap_no", kitap_no);
        query2.exec();

        ui->table_doad->setColumnCount(5);
        ui->table_doad->setHorizontalHeaderLabels(QStringList() << "Üye No" << "Kitap No" << "Alma Tarihi" << "Verme Tarihi" << "Borç");

        while (query2.next())
        {
            int row = ui->table_doad->rowCount();
            ui->table_doad->insertRow(row);

            ui->table_doad->setItem(row, 0, new QTableWidgetItem(query2.value(0).toString()));
            ui->table_doad->setItem(row, 1, new QTableWidgetItem(query2.value(1).toString()));
            ui->table_doad->setItem(row, 2, new QTableWidgetItem(query2.value(2).toString()));
            ui->table_doad->setItem(row, 3, new QTableWidgetItem(query2.value(3).toString()));
            ui->table_doad->setItem(row, 4, new QTableWidgetItem(query2.value(4).toString()));
        }
    }
}

void KitapIslemleri::on_btn_kitap_ekle_clicked()
{
    QString kitap_ad = ui->input_ad->text();
    QString kitap_sayisi = ui->input_stok->text();

    if (kitap_ad.isEmpty() || kitap_sayisi.isEmpty())
    {
        QMessageBox::warning(this, "Hata!" , "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO kitap (kitap_ad, kitap_sayisi) VALUES (:kitap_ad, :kitap_sayisi)");
    query.bindValue(":kitap_ad", kitap_ad);
    query.bindValue(":kitap_sayisi", kitap_sayisi);
    query.exec();

    ui->input_no->clear();
    ui->input_ad->clear();
    ui->input_stok->clear();

    get_kitap_list();
}

void KitapIslemleri::on_btn_kitap_sil_clicked()
{
    int currentRow = ui->data_table->currentRow();

    if (currentRow != -1)
    {
        QString kitap_no = ui->data_table->item(currentRow, 0)->text();

        QSqlQuery checkBorrowed;
        checkBorrowed.prepare("SELECT * FROM odunc_alinan WHERE kitap_no = :kitap_no");
        checkBorrowed.bindValue(":kitap_no", kitap_no);
        checkBorrowed.exec();

        if (checkBorrowed.next())
        {
            QMessageBox::warning(this, "Hata!", "Kitap ödünç alındığı için silinemez!");
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM kitap WHERE kitap_no = :kitap_no");
        query.bindValue(":kitap_no", kitap_no);
        query.exec();

        get_kitap_list();
    }

    ui->input_no->clear();
    ui->input_ad->clear();
    ui->input_stok->clear();
}

void KitapIslemleri::on_btn_kitap_guncelle_clicked()
{
    int currentRow = ui->data_table->currentRow();

    if (currentRow != -1)
    {
        QString kitap_no = ui->data_table->item(currentRow, 0)->text();
        QString kitap_ad = ui->input_ad->text();
        QString kitap_sayisi = ui->input_stok->text();

        if (kitap_ad.isEmpty() || kitap_sayisi.isEmpty())
        {
            QMessageBox::warning(this, "Hata!" , "Gerekli alanları doldurunuz!");
            return;
        }

        QSqlQuery query;
        query.prepare("UPDATE kitap SET kitap_ad = :kitap_ad, kitap_sayisi = :kitap_sayisi WHERE kitap_no = :kitap_no");
        query.bindValue(":kitap_ad", kitap_ad);
        query.bindValue(":kitap_sayisi", kitap_sayisi);
        query.bindValue(":kitap_no", kitap_no);
        query.exec();

        get_kitap_list();
    }

    ui->input_no->clear();
    ui->input_ad->clear();
    ui->input_stok->clear();
}