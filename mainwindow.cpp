﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Steganalysis-LSB");
    setWindowIcon(QIcon(":/Icon.png"));
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    ui->choose1->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ImgShow(QLabel * label, QImage img)
{
    label->setPixmap(QPixmap::fromImage(img));
    label->show();
}

void MainWindow::Reset()
{
    ui->rawfile->setEnabled(false);
    ui->name->setEnabled(false);
    ui->save->setEnabled(false);
    ui->button->setEnabled(false);
    ui->text->setEnabled(false);
    ui->clear->setEnabled(false);
    ui->copy->setEnabled(false);
    ui->raw->clear();
    ui->encrypt->clear();
}


void MainWindow::Warning(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Warning");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("confirm", QMessageBox::AcceptRole);
    messageBox.exec();
}


void MainWindow::on_choose_clicked()
{
    Path = QFileDialog::getOpenFileName(this, "Choose a image file", "/", "(*.png *.jpg *.jpeg *.bmp)");
    if (Path.isEmpty())
    {
        qDebug() << "Error";
    }
    else
    {
        QFileInfo fileInfo(Path);
        path = fileInfo.path();
        fileName = fileInfo.fileName();
        suffix = fileInfo.completeSuffix();
        ui->rawfile->setText(fileName);
        ui->button->setEnabled(true);

        lsb.LoadImg(Path);
        max = QString::number(lsb.GetMax());
        if (mode == ENC)
        {
            ui->text->setEnabled(true);
            ui->clear->setEnabled(true);
            ui->num->setText("0 / " + max);
        }
        ImgShow(ui->raw, lsb.GetImg());
    }
}


void MainWindow::on_save_clicked()
{
    lsb.WriteImg(path + "/" + ui->name->text());
}


void MainWindow::on_button_clicked()
{
    switch (mode) {
    case ENC:
    {
        QString text = ui->text->toPlainText();
        if (text == "")
        {
            Warning("At least one word");
        } else {
            if (text.length() >= max.toInt())
            {
                Warning("Words over the threshold");
            }
            lsb.SetText(text);
            lsb.Encode();
            lsb.CoverPixel();
            ImgShow(ui->encrypt, lsb.GetImg());
            ui->name->setText(fileName.remove(fileName.length() - suffix.length() - 1, suffix.length() + 1) + "_encrypt." + suffix);
            ui->name->setReadOnly(true);
            ui->save->setEnabled(true);
        }
        break;
    }
    case DEC:
    {
        lsb.SetCoded(lsb.SplitPixel());
        lsb.Decode();
        ui->text->setText(lsb.GetDecoded());
        ui->copy->setEnabled(true);
        break;
    }
    }

}


void MainWindow::on_choose1_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        mode = ENC;
        on_clear_clicked();
        ui->num->setText("text to be encrypted");
        ui->button->setText("encrypt");
        ui->choose2->setChecked(false);
        Reset();
    }
}


void MainWindow::on_choose2_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        mode = DEC;
        on_clear_clicked();
        ui->num->setText("text decrypted");
        ui->button->setText("decrypt");
        ui->choose1->setChecked(false);
        Reset();
    }
}


void MainWindow::on_clear_clicked()
{
    ui->text->setText("");
}


void MainWindow::on_copy_clicked()
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(ui->text->toPlainText());
}


void MainWindow::on_text_textChanged()
{
    if (ui->text->isEnabled())
    {
        QString s = QString::number(ui->text->toPlainText().length());
        ui->num->setText(s + " / " + max);
    }
}

