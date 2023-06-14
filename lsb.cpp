#include "lsb.h"

LSB::LSB()
{

}

LSB::~LSB()
{

}

void LSB::LoadImg(const QString & path)
{
    img.load(path);
    width = img.width();
    height = img.height();
    max = width * height;
}

void LSB::WriteImg(const QString & path)
{
    img.save(path);
}

void LSB::CoverPixel()
{
    int len = coded.length();
    int index = 0;
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            QRgb pixelValue = img.pixel(i, j);
            int red = qRed(pixelValue);
            int green = qGreen(pixelValue);
            int blue = qBlue(pixelValue);
            QVector <QString> list;
            list.append(QString::number(red, 2));
            list.append(QString::number(green, 2));
            list.append(QString::number(blue, 2));
            for (int t=0;t<3;t++) {
                QString value = list[t];
                if (index >= len)
                {
                    value[value.length()-1] = '0';
                } else {
                    value[value.length()-1] =  coded[index];
                }
                index++;
            }
            bool n;
            red = list[0].toInt(&n, 2);
            green = list[1].toInt(&n, 2);
            blue = list[2].toInt(&n, 2);
            QRgb newPixelValue = qRgb(red, green, blue);
            img.setPixel(i, j, newPixelValue);
        }
    }
}

void LSB::SplitPixel()
{
    raw = "";
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            QRgb pixelValue = img.pixel(i, j);
            int red = qRed(pixelValue);
            int green = qGreen(pixelValue);
            int blue = qBlue(pixelValue);
            QVector <QString> list;
            list.append(QString::number(red, 2));
            list.append(QString::number(green, 2));
            list.append(QString::number(blue, 2));
            for (int t=0;t<3;t++) {
                QString value = list[t];
                raw += value[value.length() - 1];
            }
        }
    }
    qDebug() << raw.length();
}

void LSB::SetText(const QString & string)
{
    raw = string;
}

void LSB::Encode()
{
    QByteArray utf8Bytes = raw.toUtf8();
    temp = utf8Bytes.toHex();
    qDebug() << temp;
}

void LSB::Decode()
{
    QByteArray hexBytes = QByteArray::fromHex(temp.toUtf8());
    QString decodedString = QString::fromUtf8(hexBytes);
    qDebug() << decodedString;
}
