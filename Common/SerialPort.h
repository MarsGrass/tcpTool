/*
* =====================================================================================
*   
*   文 件 名：SerialPort.h
*
*   描    述：定义
*
*   创建时间：
*
*   更新历史：
*
*   版权所有：
* 
* =====================================================================================
*/
#pragma once
#include <QSerialPort>


class CSerialPort : public QObject
{
	Q_OBJECT

public:
	CSerialPort(QObject * parent = 0);
	~CSerialPort();

    void SetConfig(QString strComName, QSerialPort::BaudRate baud, QSerialPort::DataBits data,
                   QSerialPort::Parity parity, QSerialPort::StopBits stopbits);

	bool GetOpenStatus();

    QByteArray GetReadData();

signals:
	void ReportStatusSignals(int);

public slots:
	void OpenSlots();

	void CloseSlots();

	void SendDataSlots(QByteArray ByteSendData);

private slots:
	void DataReceived();

private:
	QString m_strComName;
    QSerialPort::BaudRate m_baud;
    QSerialPort::DataBits m_databits;
    QSerialPort::Parity m_parity;
    QSerialPort::StopBits m_stopbits;

	bool m_OpenFlg;
	QByteArray m_data;

private:
    QSerialPort *m_pSerialObj;
};
