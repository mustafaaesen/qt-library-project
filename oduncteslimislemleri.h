#ifndef ODUNCTESLIMISLEMLERI_H
#define ODUNCTESLIMISLEMLERI_H

#include <QDialog>

namespace Ui {
class OduncTeslimIslemleri;
}

class OduncTeslimIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit OduncTeslimIslemleri(QWidget *parent = nullptr);
    ~OduncTeslimIslemleri();

private slots:
    void get_odunc_list();
    void get_teslim_list();
    void on_odunc_table_select();
    void on_teslim_table_select();
    void on_btn_teslim_clicked();
private:
    Ui::OduncTeslimIslemleri *ui;
};

#endif // ODUNCTESLIMISLEMLERI_H
