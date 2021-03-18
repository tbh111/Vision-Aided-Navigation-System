#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainWindow();
    void imgProc(float x, float y);
    void imgShow();

private slots:
//    void RcvData();

    void server_New_Connect();

    void socket_Read_Data();

    void on_connect_button_clicked();

//    void on_close_button_clicked();

    void on_forward_pressed();

    void on_left_pressed();

    void on_backward_pressed();

    void on_right_pressed();

    void on_up_pressed();

    void on_down_pressed();

private:
    Ui::MainWindow *ui;
    Mat myImg;
    QImage myQImg;
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    void decode_pkg(QString s);
    float pix_x = 0;
    float pix_y = 0;
};
#endif // MAINWINDOW_H
