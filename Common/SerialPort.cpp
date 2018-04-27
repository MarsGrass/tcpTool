/*
* =====================================================================================
*   
*   文 件 名：SerialPort.cpp
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
#include "SerialPort.h"
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>


CSerialPort::CSerialPort(QObject* parent)
:QObject(parent)
{
	m_OpenFlg =  false;
	m_pSerialObj = NULL;
}

CSerialPort::~CSerialPort()
{
	try
	{
		if (m_pSerialObj)
		{
            m_pSerialObj->close();
		}
	}
	catch (QString msg)
	{
		//qWarning("close port", msg.toUtf8().data());
	}
	catch(...)
	{
		//qCritical("close port");
	}
}

void CSerialPort::SetConfig(QString strComName, QSerialPort::BaudRate baud, QSerialPort::DataBits databits,
               QSerialPort::Parity parity, QSerialPort::StopBits stopbits)
{
    m_strComName = strComName;
    m_baud = baud;
    m_databits = databits;
    m_parity = parity;
    m_stopbits = stopbits;
}

bool CSerialPort::GetOpenStatus()
{
	return m_OpenFlg;
}

void CSerialPort::OpenSlots()
{
	try
	{
		if (m_OpenFlg)
		{
			return;
		}

        if (m_pSerialObj == NULL)
		{
            m_pSerialObj = new QSerialPort(m_strComName);
            m_pSerialObj->setFlowControl(QSerialPort::NoFlowControl);
            connect(m_pSerialObj, SIGNAL(readyRead()), this, SLOT(DataReceived()));
		}
		else
		{
			m_pSerialObj->setPortName(m_strComName);
		}

		if (m_pSerialObj->open(QIODevice::ReadWrite))
		{
            m_pSerialObj->setBaudRate(m_baud);
            m_pSerialObj->setDataBits(m_databits);
            m_pSerialObj->setParity(m_parity);
            m_pSerialObj->setStopBits(m_stopbits);

			m_OpenFlg = true;
            emit ReportStatusSignals(1);
		}
	}
	catch (QString msg)
	{
		qWarning("%s,%d,%s", __FILE__, __LINE__, msg.toUtf8().data());
	}
	catch (...)
	{
		qCritical("%s,%d,%s", __FILE__, __LINE__, "serial error");
	}	
}

void CSerialPort::CloseSlots()
{
	if (m_OpenFlg)
	{
		m_pSerialObj->close();
		m_OpenFlg = false;
        emit ReportStatusSignals(0);
	}
}

void CSerialPort::DataReceived()
{
	try
	{
		if (m_pSerialObj->bytesAvailable() > 0)
		{
            emit ReportStatusSignals(2);
		}
	}
	catch (QString msg)
	{
		qWarning("%s,%d,%s", __FILE__, __LINE__, msg.toUtf8().data());
	}
	catch (...)
	{
		qCritical("%s,%d,%s", __FILE__, __LINE__, QObject::tr("error!").toUtf8().data());
	}
}

void CSerialPort::SendDataSlots(QByteArray ByteSendData)
{
    if (!m_OpenFlg || !m_pSerialObj)
	{
		return;
	}
	m_pSerialObj->write(ByteSendData);
}

QByteArray CSerialPort::GetReadData()
{
    QByteArray byteArray;
    if(m_pSerialObj)
        byteArray = m_pSerialObj->readAll();

    return byteArray;
}

#include "moc_SerialPort.cpp"

