#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

UyeIslemleri::UyeIslemleri(QWidget *parent)
    : QDialog(parent), ui(new Ui::UyeIslemleri)
{
    ui->setupUi(this);
    get_uye_list();
    connect(ui->btn_kayit, &QPushButton::clicked, this, &UyeIslemleri::on_btn_uye_ekle_clicked);
    connect(ui->data_table, &QTableWidget::itemSelectionChanged, this, &UyeIslemleri::on_table_select);
    connect(ui->btn_sil, &QPushButton::clicked, this, &UyeIslemleri::on_btn_uye_sil_clicked);
    connect(ui->btn_guncelle, &QPushButton::clicked, this, &UyeIslemleri::on_btn_uye_guncelle_clicked);
}

UyeIslemleri::~UyeIslemleri()
{
    delete ui;
}

void UyeIslemleri::get_uye_list()
{
    ui->data_table->setRowCount(0);

    QSqlQuery query("SELECT * FROM uye");
    ui->data_table->setColumnCount(3);
    ui->data_table->setHorizontalHeaderLabels(QStringList() << "No" << "Ad" << "Soyad");

    while (query.next())
    {
        int newRow = ui->data_table->rowCount();
        ui->data_table->insertRow(newRow);

        ui->data_table->setItem(newRow, 0, new QTableWidgetItem(query.value("uye_no").toString()));
        ui->data_table->setItem(newRow, 1, new QTableWidgetItem(query.value("uye_ad").toString()));
        ui->data_table->setItem(newRow, 2, new QTableWidgetItem(query.value("uye_soyad").toString()));
    }
}

void UyeIslemleri::on_table_select()
{
    int currentRow = ui->data_table->currentRow();

    if (currentRow != -1)
    {
        QTableWidgetItem *item_ad = ui->data_table->item(currentRow, 1);
        QTableWidgetItem *item_soyad = ui->data_table->item(currentRow, 2);

        if (item_ad && item_soyad)
        {
            ui->input_no->setText(ui->data_table->item(currentRow, 0)->text());
            ui->input_ad->setText(item_ad->text());
            ui->input_soyad->setText(item_soyad->text());
        }
    }
}

void UyeIslemleri::on_btn_uye_ekle_clicked()
{
    QString ad = ui->input_ad->text();
    QString soyad = ui->input_soyad->text();

    if (ad.isEmpty() || soyad.isEmpty())
    {
        QMessageBox::warning(this, "Hata!" , "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO uye (uye_ad, uye_soyad) VALUES (:ad, :soyad)");
    query.bindValue(":ad", ad);
    query.bindValue(":soyad", soyad);

    if (query.exec())
    {
        ui->data_table->clearContents();
        get_uye_list();
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    ui->input_ad->clear();
    ui->input_soyad->clear();
}

void UyeIslemleri::on_btn_uye_sil_clicked()
{
    QString no = ui->input_no->text();

    QSqlQuery checkBorrowed;
    checkBorrowed.prepare("SELECT * FROM odunc_alinan WHERE uye_no = :no");
    checkBorrowed.bindValue(":no", no);
    checkBorrowed.exec();

    if (checkBorrowed.next())
    {
        QMessageBox::warning(this, "Hata!", "Bu üye silinemez. Üyenin henüz teslim etmediği kitaplar vardır.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM uye WHERE uye_no = :no");
    query.bindValue(":no", no);

    if (query.exec())
    {
        ui->data_table->clearContents();
        get_uye_list();
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    ui->input_no->clear();
    ui->input_ad->clear();
    ui->input_soyad->clear();
}

void UyeIslemleri::on_btn_uye_guncelle_clicked()
{
    QString no = ui->input_no->text();
    QString ad = ui->input_ad->text();
    QString soyad = ui->input_soyad->text();

    if (ad.isEmpty() || soyad.isEmpty())
    {
        QMessageBox::warning(this, "Hata!" , "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE uye SET uye_ad = :ad, uye_soyad = :soyad WHERE uye_no = :no");
    query.bindValue(":ad", ad);
    query.bindValue(":soyad", soyad);
    query.bindValue(":no", no);

    if (query.exec())
    {
        ui->data_table->clearContents();
        get_uye_list();
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    ui->input_no->clear();
    ui->input_ad->clear();
    ui->input_soyad->clear();
}
