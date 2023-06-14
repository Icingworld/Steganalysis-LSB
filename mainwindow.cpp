#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LSB lsb;
    QString path1{"C:\\Users\\zwjiang\\Desktop\\test.png"};
    QString path2{"C:\\Users\\zwjiang\\Desktop\\new.png"};
    lsb.LoadImg(path1);
    lsb.CoverPixel();
    lsb.WriteImg(path2);
    lsb.SplitPixel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

