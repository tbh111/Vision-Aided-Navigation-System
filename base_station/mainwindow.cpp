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
        imgProc(pix_x+ui->x_dir->text().toFloat(), pix_y+ui->y_dir->text().toFloat());
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
    pix_x = myImg.cols;
    pix_y = myImg.rows;
    imgProc(pix_x, pix_y);
}

void MainWindow::imgProc(float x, float y)
{
    Mat imgSrc = myImg;
    circle(imgSrc, Point2f(imgSrc.cols/2+x, imgSrc.rows/2+y), 20, Scalar(255, 0, 0), -1, LINE_AA);
//    imgSrc.convertTo(imgDst, -1, contrast, brightness);
    myQImg = QImage((const unsigned char*)(imgSrc.data), imgSrc.cols, imgSrc.rows, imgSrc.step, QImage::Format_RGB888);
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
