#ifndef LSB_H
#define LSB_H

#include <iostream>
#include <QImage>
#include <QString>
#include <QTextCodec>
#include <QVector>
#include <QDebug>

class LSB
{
private:
    QImage img;
    int width;
    int height;
    int max;
    QString coded;
    QString decoded;
    QString raw;

public:
    LSB();
    ~LSB();
    void LoadImg(const QString & path);
    void WriteImg(const QString & path);
    void SetText(const QString & string);
    void SetCoded(const QString & string);
    int GetMax();
    int GetHeight();
    int GetWidth();
    QImage GetImg();
    QString GetDecoded();
    void CoverPixel();
    QString SplitPixel();
    void Encode();
    void Decode();
};

#endif // LSB_H
