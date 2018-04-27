#include "CommonFunc.h"

quint16 CRC16_Check(QByteArray Pushdata, qint8 length)
{
    quint16 Reg_CRC = 0xffff;
    qint8 i, j;
    for (i = 0; i < length; i++)
    {
        quint8 crc_1 = (quint8)Pushdata.at(i);
        Reg_CRC ^= crc_1;
        for (j = 0; j < 8; j++)
        {
            if (Reg_CRC & 0x0001)
                Reg_CRC = (Reg_CRC >> 1) ^ 0xA001;

            else
                Reg_CRC = Reg_CRC >> 1;

        }
    }

    return Reg_CRC;
}
