#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QDialog>

namespace Ui {
class KitapIslemleri;
}

class KitapIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit KitapIslemleri(QWidget *parent = nullptr);
    ~KitapIslemleri();

private slots:
    void get_kitap_list();

    void on_table_select();

    void on_btn_kitap_ekle_clicked();

    void on_btn_kitap_sil_clicked();

    void on_btn_kitap_guncelle_clicked();

private:
    Ui::KitapIslemleri *ui;
};

#endif // KITAPISLEMLERI_H
