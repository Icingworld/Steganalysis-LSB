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
    max = (width * height * 3 - 12) / 4;
}

void LSB::WriteImg(const QString & path)
{
    img.save(path);
}

void LSB::CoverPixel(int TYPE)
{
    int len = coded.length();
    int index = 0;
    int red, green, blue, alpha;
    QRgb pixelValue, newPixelValue;
    QVector <QString> list;
    QString value;
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            if (index <= len)
            {
            pixelValue = img.pixel(i, j);
            red = qRed(pixelValue);
            green = qGreen(pixelValue);
            blue = qBlue(pixelValue);
            if (TYPE == PNG)
            {
                alpha = qAlpha(pixelValue);
            }
            list.clear();
            list.shrink_to_fit();
            list.append(QString::number(red, 2));
            list.append(QString::number(green, 2));
            list.append(QString::number(blue, 2));
            for (int t=0;t<3;t++) {
                value = list[t];
                if (index >= len)
                {
                    value[value.length()-1] = '0';
                } else {
                    value[value.length()-1] =  coded[index];
                }
                list[t] = value;
                index++;
            }
            bool n;
            red = list[0].toInt(&n, 2);
            green = list[1].toInt(&n, 2);
            blue = list[2].toInt(&n, 2);
            if (TYPE == PNG)
            {
                newPixelValue = qRgba(red, green, blue, alpha);
            } else {
                newPixelValue = qRgb(red, green, blue);
            }
            img.setPixel(i, j, newPixelValue);
            }
        }
    }
}

QString LSB::SplitPixel()
{
    QString head = "";
    int len = 0;
    bool flag = false;
    QString decoded = "", value;
    QRgb pixelValue;
    int index = 0;
    QVector <QString> list;
    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            pixelValue = img.pixel(i, j);
            int red = qRed(pixelValue);
            int green = qGreen(pixelValue);
            int blue = qBlue(pixelValue);
            list.clear();
            list.shrink_to_fit();
            list.append(QString::number(red, 2));
            list.append(QString::number(green, 2));
            list.append(QString::number(blue, 2));
            for (int t=0;t<3;t++) {
                value = list[t];
                if (index < 12)
                {
                    head += value[value.length() - 1];
                } else if (!flag) {
                    bool n;
                    len = head.toInt(&n, 2);
                    flag = true;
                    if (len > 0)
                    {
                        decoded += value[value.length() - 1];
                    }
                } else if (index < 12 + len) {
                    decoded += value[value.length() - 1];
                } else {
                    return decoded;
                }
                index++;
            }
        }
    }
    return "";
}


void LSB::SetText(const QString & string)
{
    raw = string;
}

void LSB::SetCoded(const QString & string)
{
    coded = string;
}

QString LSB::GetDecoded()
{
    return decoded;
}

QImage LSB::GetImg()
{
    return img;
}

int LSB::GetMax()
{
    return max;
}

int LSB::GetWidth()
{
    return width;
}

int LSB::GetHeight()
{
    return height;
}

void LSB::Encode()
{
    QByteArray utf8Bytes = raw.toUtf8();
    QString binaryString;
    for (char byte : utf8Bytes) {
        binaryString += QString("%1").arg(static_cast<unsigned char>(byte), 8, 2, QLatin1Char('0'));
    }
    coded = binaryString;
    int len = coded.length();
    QString lens = QString::number(len, 2).rightJustified(12, '0');
    coded = lens + coded;
}

void LSB::Decode()
{
    QByteArray utf8Data;
    QStringList byteStrings;
    int len = coded.length();
    QString temp = "";
    for (int i=0;i<(len/8);i++) {
        temp += coded[8*i];
        temp += coded[8*i+1];
        temp += coded[8*i+2];
        temp += coded[8*i+3];
        temp += coded[8*i+4];
        temp += coded[8*i+5];
        temp += coded[8*i+6];
        temp += coded[8*i+7];
        byteStrings.append(temp);
        temp = "";
    }
    for (const QString& byteString : byteStrings) {
        bool ok;
        int byteValue = byteString.toInt(&ok, 2);
        if (ok) {
            utf8Data.append(static_cast<char>(byteValue));
        } else {
            qWarning() << "Invalid binary string:" << byteString;
        }
    }
    decoded = QString::fromUtf8(utf8Data);
}
