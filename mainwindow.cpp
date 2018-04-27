#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    demo = new TCPClientDemo();

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(Timeout()));
    connect(demo, SIGNAL(ClientSignals(int)), this, SLOT(ClientSlots(int)));

    lblStatus = new QLabel();
    ui->statusBar->addWidget(lblStatus);

    ui->btnSend->setEnabled(false);

    nCount_ = 0;

    m_data1 = 0;
    m_data2 = 0;
    m_data3 = 0;
    m_data4 = 0;



    data.resize(38);
    data[0] = 0xFF;
    data[1] = 0x00;
    data[2] = 0x26;
    data[3] = 0x02;
    data[4] = 0x31;
    data[5] = 0x32;
    data[6] = 0x33;
    data[7] = 0x34;
    data[8] = 0x01;

    data[9] = 0x01;

    char* data1 = (char*)&m_data1;
    char* data2 = (char*)&m_data2;
    char* data3 = (char*)&m_data3;
    char* data4 = (char*)&m_data4;

    data[10] = 0x01;
    data[11] = data1[3];
    data[12] = data1[2];
    data[13] = data1[1];
    data[14] = data1[0];
    data[15] = 0x02;
    data[16] = data2[3];
    data[17] = data2[2];
    data[18] = data2[1];
    data[19] = data2[0];
    data[20] = 0x03;
    data[21] = data3[3];
    data[22] = data3[2];
    data[23] = data3[1];
    data[24] = data3[0];
    data[25] = 0x04;
    data[26] = data4[3];
    data[27] = data4[2];
    data[28] = data4[1];
    data[29] = data4[0];

    data[30] = 0xAA;
    QDateTime datetime = QDateTime::currentDateTime();
    data[31] = (datetime.date().year() - 2000) % 10 + (datetime.date().year() - 2000) / 10 * 16;
    data[32] = datetime.date().month() % 10 +datetime.date().month() / 10 * 16;
    data[33] = datetime.date().day() % 10 + datetime.date().day() / 10 * 16;
    data[34] = datetime.time().hour() % 10 + datetime.time().hour() / 10 * 16;
    data[35] = datetime.time().minute() % 10 + datetime.time().minute() / 10 * 16;
    data[36] = datetime.time().second() % 10 + datetime.time().second() / 10 * 16;

    data[37] = 0xFF;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    demo->Set_IP_PORT(ui->edtIP->text(), ui->edtPort->text());
    if(demo->pSocketObj->GetOpenStatus())
    {
        demo->Close();
    }
    else
    {
        demo->Open();
    }
}

void MainWindow::LoadData()
{
    char* data1 = (char*)&m_data1;
    char* data2 = (char*)&m_data2;
    char* data3 = (char*)&m_data3;
    char* data4 = (char*)&m_data4;

    data[10] = 0x01;
    data[11] = data1[3];
    data[12] = data1[2];
    data[13] = data1[1];
    data[14] = data1[0];
    data[15] = 0x02;
    data[16] = data2[3];
    data[17] = data2[2];
    data[18] = data2[1];
    data[19] = data2[0];
    data[20] = 0x03;
    data[21] = data3[3];
    data[22] = data3[2];
    data[23] = data3[1];
    data[24] = data3[0];
    data[25] = 0x04;
    data[26] = data4[3];
    data[27] = data4[2];
    data[28] = data4[1];
    data[29] = data4[0];

    data[30] = 0xAA;
    QDateTime datetime = QDateTime::currentDateTime();
    data[31] = (datetime.date().year() - 2000) % 10 + (datetime.date().year() - 2000) / 10 * 16;
    data[32] = datetime.date().month() % 10 +datetime.date().month() / 10 * 16;
    data[33] = datetime.date().day() % 10 + datetime.date().day() / 10 * 16;
    data[34] = datetime.time().hour() % 10 + datetime.time().hour() / 10 * 16;
    data[35] = datetime.time().minute() % 10 + datetime.time().minute() / 10 * 16;
    data[36] = datetime.time().second() % 10 + datetime.time().second() / 10 * 16;

    m_data1++;
    m_data2++;
    m_data3++;
    m_data4++;
}

void MainWindow::on_btnSend_clicked()
{
    if(demo->pSocketObj->GetOpenStatus())
    {
        if(ui->checkBox->isChecked())
        {
            if(timer->isActive())
            {
               timer->stop();
            }
            else
            {
                timer->start(ui->edtPer->text().toInt());
            }
        }
        else
        {
            nCount_++;
            ui->edtCount->setText(QString::number(nCount_));

            LoadData();
            demo->SendData(data);
            //demo->SendData(ui->textEdit->toPlainText().toLatin1());
        }
    }
}

void MainWindow::ClientSlots(int nCount)
{
    if(nCount == 0){
        lblStatus->setText(tr("连接失败"));
        ui->btnConnect->setText(tr("连接"));
        ui->btnSend->setEnabled(false);
    }else if(nCount == 1){
        lblStatus->setText(tr("连接成功"));
        ui->btnConnect->setText(tr("断开"));
        ui->btnSend->setEnabled(true);
    }else if(nCount == 2){
        ui->plainTextEdit->appendPlainText(demo->m_strRecvData);
        ui->plainTextEdit->update();
        demo->m_strRecvData.clear();
    }else{

    }
}

void MainWindow::Timeout()
{
    if(ui->checkBox->isChecked())
    {
        if(demo->pSocketObj->GetOpenStatus())
        {
            nCount_++;
            ui->edtCount->setText(QString::number(nCount_));

            LoadData();
            demo->SendData(data);
            //demo->SendData(ui->textEdit->toPlainText().toLatin1());
        }
    }
    else
    {
        timer->stop();
    }
}

void MainWindow::on_btnClear_clicked()
{
    nCount_ = 0;
    ui->edtCount->setText(QString::number(nCount_));
}


