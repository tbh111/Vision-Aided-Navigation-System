#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::RcvData()
//{
//    char buf[1024] = {0};
//    QHostAddress address;
//    quint16 port;
//    quint64 len = tcpSocket->readDatagram(buf,sizeof(buf),&address,&port);
//    if(len>0)
//    {
//        QString str = QString("%1").arg(buf);
//        String std_str = str.toStdString();
//        ui->log->append(str);
//        if(str.contains("dx", Qt::CaseSensitive))
//        {
//            String x = std_str.substr(2,std_str.length());
//            ui->x_dir->setText(QString::fromStdString(x));
//        }
//        if(str.contains("dy", Qt::CaseSensitive))
//        {
//            String y = std_str.substr(2,std_str.length());
//            ui->y_dir->setText(QString::fromStdString(y));
//        }
//        if(str.contains("dz", Qt::CaseSensitive))
//        {
//            String z = std_str.substr(2,std_str.length());
//            ui->z_dir->setText(QString::fromStdString(z));
//        }
//        if(str.contains("lx", Qt::CaseSensitive))
//        {
//            String x = std_str.substr(2,std_str.length());
//            ui->x_gps->setText(QString::fromStdString(x));
//        }
//        if(str.contains("ly", Qt::CaseSensitive))
//        {
//            String y = std_str.substr(2,std_str.length());
//            ui->y_gps->setText(QString::fromStdString(y));
//        }
//        if(str.contains("lz", Qt::CaseSensitive))
//        {
//            String z = std_str.substr(2,std_str.length());
//            ui->z_gps->setText(QString::fromStdString(z));
//        }
//    }

//}

void MainWindow::server_New_Connect(){
    tcpSocket = tcpServer->nextPendingConnection();
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
}

void MainWindow::decode_pkg(QString s){
    int pos = s.indexOf("dx",0,Qt::CaseInsensitive);
    if(pos>=0)
    {
        ui->x_dir->setText(s.mid(pos+2,6));
    }
    int pos1 = s.indexOf("dy",0,Qt::CaseInsensitive);
    if(pos1>=0)
    {
        ui->y_dir->setText(s.mid(pos1+2,6));
    }
    int pos2 = s.indexOf("dz",0,Qt::CaseInsensitive);
    if(pos2>=0)
    {
        ui->z_dir->setText(s.mid(pos2+2,6));
    }
    int pos3 = s.indexOf("lx",0,Qt::CaseInsensitive);
    if(pos3>=0)
    {
        ui->x_gps->setText(s.mid(pos3+2,6));
    }
    int pos4 = s.indexOf("ly",0,Qt::CaseInsensitive);
    if(pos4>=0)
    {
        ui->y_gps->setText(s.mid(pos4+2,6));
    }
    int pos5 = s.indexOf("lz",0,Qt::CaseInsensitive);
    if(pos5>=0)
    {
        ui->z_gps->setText(s.mid(pos5+2,6));
    }
}

void MainWindow::socket_Read_Data(){
    QByteArray buffer;
    buffer = tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        ui->log->append(buffer);
        QString str = QString(buffer);

        decode_pkg(str);
//        ui->x_dir->setText(QString::fromStdString(x));
    }
}

void MainWindow::on_connect_button_clicked()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::server_New_Connect);
    if(tcpServer->listen(QHostAddress(ui->lineEdit->text()), ui->port->value()))
    {
        qDebug()<<tcpServer->errorString();
        return;
    }
    QString str1 = QString("connect success\n");
    ui->log->append(str1);
}

void MainWindow::imgShow()
{
    ui->image_show->setPixmap(QPixmap::fromImage(myQImg.scaled(ui->image_show->size(),Qt::KeepAspectRatio)));
}

void MainWindow::initMainWindow()
{
    QString imgPath = "D:/drone/pic3/map2.jpg";
    Mat imgData = imread(imgPath.toStdString());
    cvtColor(imgData, imgData, COLOR_BGR2RGB);
    myImg = imgData;
    myQImg = QImage((const unsigned char*)(imgData.data), imgData.cols, imgData.rows, imgData.step, QImage::Format_RGB888);
    imgShow();
}

void MainWindow::imgProc(float contrast, int brightness)
{
    Mat imgSrc = myImg;
    Mat imgDst = Mat::zeros(imgSrc.size(), imgSrc.type());
    imgSrc.convertTo(imgDst, -1, contrast, brightness);
    myQImg = QImage((const unsigned char*)(imgDst.data), imgDst.cols, imgDst.rows, QImage::Format_RGB888);
    imgShow();
}

void MainWindow::on_forward_pressed()
{
    qDebug() << "forward";
    tcpSocket->write("f");
    tcpSocket->flush();
}

void MainWindow::on_left_pressed()
{
    qDebug() << "left";
    tcpSocket->write("l");
    tcpSocket->flush();
}

void MainWindow::on_backward_pressed()
{
    qDebug() << "backward";
    tcpSocket->write("b");
    tcpSocket->flush();
}

void MainWindow::on_right_pressed()
{
    qDebug() << "right";
    tcpSocket->write("r");
    tcpSocket->flush();
}

void MainWindow::on_up_pressed()
{
    qDebug() << "up";
    tcpSocket->write("p");
    tcpSocket->flush();
}

void MainWindow::on_down_pressed()
{
    qDebug() << "down";
    tcpSocket->write("d");
    tcpSocket->flush();
}
