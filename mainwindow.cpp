#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDialog>
#include <QStatusBar>
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncalmaislemleri.h"
#include "oduncteslimislemleri.h"

MainWindow::MainWindow(QWidget *parent, bool dbStatus)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStatusBar *statusBar = ui->statusbar;

    if (dbStatus)
    {
        statusBar->showMessage("Veri tabanına bağlanıldı!");
    }
    else
    {
        statusBar->showMessage("Veri tabanına bağlanılamadı!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_uye_menu_clicked()
{
    UyeIslemleri dialog(this);
    dialog.exec();
}

void MainWindow::on_kitap_menu_clicked()
{
    KitapIslemleri dialog(this);
    dialog.exec();
}

void MainWindow::on_odunc_menu_clicked()
{
    OduncAlmaIslemleri dialog(this);
    dialog.exec();
}

void MainWindow::on_teslim_menu_clicked()
{
    OduncTeslimIslemleri dialog(this);
    dialog.exec();
}
