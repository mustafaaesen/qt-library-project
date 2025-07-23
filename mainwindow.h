#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, bool dbStatus = false);
    ~MainWindow();

private slots:
    void on_uye_menu_clicked();
    void on_kitap_menu_clicked();
    void on_odunc_menu_clicked();
    void on_teslim_menu_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
