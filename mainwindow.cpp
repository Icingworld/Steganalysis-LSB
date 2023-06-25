#include "mainwindow.h"
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
    QImage img1, img2;
    if (img.width() > label->width())
    {
        img1 = img.scaled(label->width(), img.height() * (label->width() / static_cast<float>(img.width())));
    } else {
        img1 = img;
    }
    if (img1.height() > label->height())
    {
        img2 = img1.scaled(img1.width() * (label->height() / static_cast<float>(img1.height())), img1.height());
    } else {
        img2 = img1;
    }
    label->setPixmap(QPixmap::fromImage(img2));
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
    ui->rawfile->clear();
    ui->name->clear();
    ui->text->clear();
}


void MainWindow::Warning(const QString & string)
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Warning");
    messageBox.setText(string);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("确认", QMessageBox::AcceptRole);
    messageBox.exec();
}


void MainWindow::on_choose_clicked()
{
    Path = QFileDialog::getOpenFileName(this, QString("选择一张图片"), "/", "(*.png *.bmp)");
    if (Path.isEmpty())
    {
        qDebug() << "Error";
    }
    else
    {
        Reset();
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
    QString new_path = path + "/" + ui->name->text();
    lsb.WriteImg(new_path);
}


void MainWindow::on_button_clicked()
{
    switch (mode) {
    case ENC:
    {
        QString text = ui->text->toPlainText();
        if (text == "")
        {
            Warning("至少一个字符");
        } else {
            if (text.length() >= max.toInt())
            {
                Warning("字符过多");
            }
            lsb.SetText(text);
            lsb.Encode();
            if (suffix == "png")
            {
                lsb.CoverPixel(PNG);
            } else {
                lsb.CoverPixel(DEFAULT);
            }
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
        ui->num->setText("待加密密文");
        ui->button->setText("加密");
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
        ui->num->setText("解密明文");
        ui->button->setText("解密");
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

