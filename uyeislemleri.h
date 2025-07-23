#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>

namespace Ui {
class UyeIslemleri;
}

class UyeIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit UyeIslemleri(QWidget *parent = nullptr);
    ~UyeIslemleri();

private slots:
    void get_uye_list();

    void on_table_select();

    void on_btn_uye_ekle_clicked();

    void on_btn_uye_sil_clicked();

    void on_btn_uye_guncelle_clicked();

private:
    Ui::UyeIslemleri *ui;
};

#endif
