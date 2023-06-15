#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QPixmap>
#include <QClipboard>
#include <QLabel>
#include <QMessageBox>
#include "lsb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ImgShow(QLabel * label, QImage img);
    void Reset();
    void Warning(const QString & string);
    LSB lsb;
    QString Path;
    QString path;
    QString fileName;
    QString newName;
    QString suffix;
    enum MODE {ENC, DEC};
    int mode;
    QString max;

private slots:
    void on_choose_clicked();
    void on_save_clicked();
    void on_button_clicked();
    void on_choose1_stateChanged(int arg1);
    void on_choose2_stateChanged(int arg1);
    void on_clear_clicked();
    void on_copy_clicked();
    void on_text_textChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
