#include "mainwindow.h"
#include "ui_mainwindow.h"

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
            demo->SendData(ui->textEdit->toPlainText().toLatin1());
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
            demo->SendData(ui->textEdit->toPlainText().toLatin1());
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


