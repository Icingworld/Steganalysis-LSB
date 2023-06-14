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
    QString coded{"1010101010101010101010111"};  // 编码后的01字符串
    QString raw;
    QString temp;  // 编码后的16进制
public:
    LSB();
    ~LSB();
    void LoadImg(const QString & path);
    void WriteImg(const QString & path);
    void SetText(const QString & string);
    void CoverPixel();
    void SplitPixel();
    void Encode();
    void Decode();
};

#endif // LSB_H
