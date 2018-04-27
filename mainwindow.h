#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QThread>

#include "Common/TCPClient.h"


class TCPClientDemo : public QObject
{
    Q_OBJECT

public:
    TCPClientDemo(QObject* parent = 0) : QObject(parent){
        pSocketObj = new TCPClient();
        m_pThread = new QThread();
        pSocketObj->moveToThread(m_pThread);

        connect(this, SIGNAL(OpenSignals()), pSocketObj, SLOT(OpenSlots()), Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(CloseSignals()), pSocketObj, SLOT(CloseSlots()), Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(SendDataSignals(QByteArray)), pSocketObj, SLOT(SendDataSlots(QByteArray)));
        connect(pSocketObj, SIGNAL(ReportStatusSignals(int)), this, SLOT(ReportStatusSlots(int)));

        m_pThread->start();
    }

    void Set_IP_PORT(const QString& ip, const QString& port){
        pSocketObj->SetConfig(ip, port);
    }

    void Open(){
        emit OpenSignals();
    }

    void Close(){
        emit CloseSignals();
    }

    void SendData(QByteArray data){
        emit SendDataSignals(data);
    }

signals:
    void OpenSignals(); //打开信号
    void CloseSignals(); //关闭信号
    void SendDataSignals(QByteArray); //发送信号

    void ClientSignals(int);

public slots:
    void ReportStatusSlots(int nConnect)
    {
        if(nConnect == 2)
        {
            m_strRecvData.append(pSocketObj->GetReadData());
        }

        emit ClientSignals(nConnect);
    }

public:
    TCPClient* pSocketObj;
    QThread* m_pThread;
    QString m_strRecvData;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_btnClear_clicked();

    void Timeout();
    void ClientSlots(int nCount);

private:
    Ui::MainWindow *ui;
    TCPClientDemo *demo;
    QTimer* timer;
    QLabel* lblStatus;

    int nCount_;
};

#endif // MAINWINDOW_H
