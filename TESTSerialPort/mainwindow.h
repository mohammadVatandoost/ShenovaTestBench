#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/qserialportglobal.h>
#include <QDebug>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include "QMessageBox"
#include <stdint.h>
#include "cJSON.h"
#include "QDir"
#include "QFileInfo"
#include "QFile"
#include <string>

#define REQUEST_READ "RR"
#define REQUEST_WRITE "RW"
#define REQUEST_FILE "RF"
#define USER_NAME "U"
#define PASSWORD "P"
#define REQUEST "R"
#define TOKEN "T"
#define KEY "K"
#define VOICES "V"
#define VOICE_DURATION "VD"
#define VOICE_ID "VI"
#define CALENDER_DATE "CD"
#define CALENDER_TIME "CT"
#define BATTERY_CHARGE "B"
#define SD_CART "SD"
#define FILE_SIZE "FS"

#define exampleDate1 "1397/02/01"
#define exampleDate2 "1397/02/21"

#define exampleTime1 "10:22"
#define exampleTime2 "11:12"

#define exampleVoiceDuration1 "7:46"
#define exampleVoiceDuration2 "8:18"

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort *serial;
    QString come_port;
    void delay(int t_ms);
    QString convertToString(uint8_t *str);
    void writeSerial(QString temp);
    QString username = "Mohammad";QString password = "12345678";
    QString token;
    uint8_t stateReadData = 0;
    void reactToDatas(QString data);
    QByteArray sendFile(string date, string time);
    void decodePacket(char *message);
    cJSON *messageJSON, *sendMessageJSON;
    void sendMessage();
    void getVoices();
    void getFileSize(string date, string time);
    int counter = 0 ;
    QByteArray recievedData;
    int getSize(char * s);
    QString removeSpaceAndEnter(char *s);
    string FileDate,FileTime;
private slots:
    void machineState();

    void on_connect_clicked();

    void on_scan_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
