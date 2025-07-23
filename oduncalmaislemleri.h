#ifndef ODUNCALMAISLEMLERI_H
#define ODUNCALMAISLEMLERI_H

#include <QDialog>

namespace Ui {
class OduncAlmaIslemleri;
}

class OduncAlmaIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit OduncAlmaIslemleri(QWidget *parent = nullptr);
    ~OduncAlmaIslemleri();

private slots:
    void get_uye_list();
    void get_kitap_list();
    void get_odunc_list();
    void on_uye_table_select();
    void on_kitap_table_select();
    void on_btn_odunc_alma_clicked();

private:
    Ui::OduncAlmaIslemleri *ui;
};

#endif // ODUNCALMAISLEMLERI_H
