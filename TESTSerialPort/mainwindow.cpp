#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Serial Port
      serial = new QSerialPort(this);
      serial->close();
      serial->setBaudRate(921600);
      serial->setDataBits(QSerialPort::Data8);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
      serial->setFlowControl(QSerialPort::NoFlowControl);

      // Discnnect
      ui->pushButton->setDisabled(true);

      QDir mDir("F:/Work/Raiwan/BLE/TestPackets/SERIALPORT/Files");
      if(mDir.exists()) {
          qDebug() << "exists" ;
          foreach (QFileInfo item, mDir.entryInfoList()) {
             qDebug() << "QFileInfo :" << item.fileName() ;
//             qDebug() << "QFileInfo :" << item.absoluteFilePath() ;
          }
      } else { qDebug() << "Not exists" ; }


}

MainWindow::~MainWindow()
{
    delete serial;
    delete ui;
}

void MainWindow::delay(int t_ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(t_ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QString MainWindow::convertToString(uint8_t *str)
{
    return QString((char *)str);
}

void MainWindow::writeSerial(QString temp)
{
    QByteArray sendBytes;
    sendBytes.append(temp);
    serial->write(sendBytes);
}

void MainWindow::reactToDatas(QString data)
{
    switch (stateReadData) {
    case 0:

        break;
    case 1:

        break;
    default:
        break;
    }
}

QByteArray MainWindow::sendFile(string date, string time)
{
    if((!date.compare(exampleDate1)) && (!time.compare(exampleTime1))) {
        QFile file("F:/Work/Raiwan/BLE/TestPackets/SERIALPORT/Files/1.wav");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Not read" ;
            } else {
                qDebug() << "Readable" ;
            }
            QByteArray line;
            while (!file.atEnd()) {
                line = file.readAll(); //file.readLine();
            }
//            qDebug() << "size : " << line.size();
            qDebug() << "SendFile";
            return line;
    } else if((!date.compare(exampleDate1)) && (!time.compare(exampleTime1))) {
        QFile file("F:/Work/Raiwan/BLE/TestPackets/SERIALPORT/Files/SDREC2.WAV");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Not read" ;
            } else {
                qDebug() << "Readable" ;
            }
            QByteArray line;
            while (!file.atEnd()) {
                line = file.readAll(); //file.readLine();
            }
            qDebug() << "SendFile";
            return line;
    }

    //    return;
}

void MainWindow::decodePacket(char *message)
{
    messageJSON = cJSON_Parse(message);
    if(messageJSON != NULL) {
        cJSON *requestType;cJSON *buffString = NULL;
        requestType = cJSON_GetObjectItem(messageJSON, REQUEST);
        if(requestType != NULL) {
            qDebug() <<"requestType->valuestring : " << requestType->valuestring;
            string temp = requestType->valuestring;
            if(!temp.compare(REQUEST_READ)) {
                  qDebug() << "Read" ;
                  sendMessageJSON = cJSON_CreateObject();
                   buffString = cJSON_CreateString(REQUEST_WRITE);cJSON_AddItemToObject(sendMessageJSON, REQUEST, buffString);
                  getVoices();sendMessage();
            } else if(!temp.compare(REQUEST_WRITE)) {
                qDebug() << "Write" ;

            } else if(!temp.compare(REQUEST_FILE)) {
                    qDebug() << "Read File" ;
                if( (cJSON_GetObjectItem(messageJSON, CALENDER_DATE) != NULL) && (cJSON_GetObjectItem(messageJSON, CALENDER_TIME) != NULL) ) {
                     qDebug() << "CALENDER_DATE and CALENDER_TIME is not NULL" ;
                     cJSON *calenderDate = cJSON_GetObjectItem(messageJSON, CALENDER_DATE);
                     cJSON *calenderTime = cJSON_GetObjectItem(messageJSON, CALENDER_TIME);
                     sendMessageJSON = cJSON_CreateObject();
                     buffString = cJSON_CreateString(REQUEST_FILE);cJSON_AddItemToObject(sendMessageJSON, REQUEST, buffString);
                     FileDate = calenderDate->valuestring ; FileTime = calenderTime->valuestring;
                    getFileSize(FileDate, FileTime);sendMessage();
                } else {
                    qDebug() << "CALENDER_DATE and CALENDER_TIME is NULL, Sending File" ;
                   serial->write(sendFile(FileDate, FileTime));
                   delay(1000);delay(1000);
                   writeSerial("*");delay(500);writeSerial("*");delay(500);writeSerial("*");
                   qDebug() << "Sending File finished" ;
                }
            } else {qDebug() << "none of them" ;}
        } else {
            qDebug() <<"requestType is null";
        }
    } else {
       qDebug() <<"messageJSON is null ";
    }
}

void MainWindow::sendMessage()
{
    qDebug() << "sendingMessage begin : ";
//  char *sendingMessage =  sendMessageJSON->valuestring;
//  qDebug() << "sendingMessage getSize : " << getSize(sendingMessage);
//  qDebug() << "sendingMessage : " <<  cJSON;
  writeSerial(removeSpaceAndEnter(cJSON_Print(sendMessageJSON))+"***");
}

void MainWindow::getVoices()
{
    cJSON *getVoices = cJSON_GetObjectItem(messageJSON, VOICES);
    if(getVoices != NULL) {
        qDebug() <<"Get voices is OK " ;
        cJSON *voicesArray, *voice;
        voicesArray = cJSON_CreateArray();
        /* add cars array to root */
        cJSON_AddItemToObject(sendMessageJSON, VOICES, voicesArray);

        /* add 1st voice to sendMessageJSON array */
        cJSON_AddItemToArray(voicesArray, voice = cJSON_CreateObject());
        cJSON_AddItemToObject(voice, CALENDER_DATE, cJSON_CreateString(exampleDate1));
        cJSON_AddItemToObject(voice, CALENDER_TIME, cJSON_CreateString(exampleTime1));
        cJSON_AddItemToObject(voice, VOICE_DURATION, cJSON_CreateString(exampleVoiceDuration1));
        /* add 2nd car to cars array */
        cJSON_AddItemToArray(voicesArray, voice = cJSON_CreateObject());
        cJSON_AddItemToObject(voice, CALENDER_DATE, cJSON_CreateString(exampleDate2));
        cJSON_AddItemToObject(voice, CALENDER_TIME, cJSON_CreateString(exampleTime2));
        cJSON_AddItemToObject(voice, VOICE_DURATION, cJSON_CreateString(exampleVoiceDuration2));
//        if(sendMessageJSON != NULL) {
//            qDebug() << "sendMessageJSON is not null ";
//            cJSON *temp = cJSON_GetObjectItem(messageJSON, VOICES);
//            if(temp != NULL) {qDebug() << "VOICES is not null ";} else {qDebug() << "VOICES is null ";}
//        } else {
//            qDebug() << "sendMessageJSON is null ";
//        }
        qDebug() << "getVoices : " << cJSON_Print(sendMessageJSON);
    } else {
        qDebug() <<"Get voices is null " ;
    }
}

void MainWindow::getFileSize(string date, string time)
{
    cJSON *FileSize;
    if( (!date.compare(exampleDate1)) && (!time.compare(exampleTime1)) ) {
        QFile file("F:/Work/Raiwan/BLE/TestPackets/SERIALPORT/Files/1.wav");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Not read" ;
            } else {
                qDebug() << "Readable" ;
            }
            QByteArray line;
            while (!file.atEnd()) {
                line = file.readAll(); //file.readLine();
            }
            qDebug() << "size : " << line.size();
            FileSize = cJSON_CreateString( QString::number(line.size()).toStdString().c_str() );
            cJSON_AddItemToObject(sendMessageJSON, FILE_SIZE, FileSize);
            qDebug() << "getFileSize : " << cJSON_Print(sendMessageJSON);
//            return line;
    } else if((!date.compare(exampleDate2)) && (!time.compare(exampleTime2))) {
        QFile file("F:/Work/Raiwan/BLE/TestPackets/SERIALPORT/Files/SDREC2.WAV");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Not read" ;
            } else {
                qDebug() << "Readable" ;
            }
            QByteArray line;
            while (!file.atEnd()) {
                line = file.readAll(); //file.readLine();
            }
            qDebug() << "size : " << line.size();
            FileSize = cJSON_CreateString( QString::number(line.size()).toStdString().c_str() );
            cJSON_AddItemToObject(sendMessageJSON, FILE_SIZE, FileSize);
            qDebug() << "getFileSize : " << cJSON_Print(sendMessageJSON);
//            return line;
    } else {
        qDebug() << "No File" ;
    }
}

int MainWindow::getSize(char *s)
{
        char *t; // first copy the pointer to not change the original
        int size = 0;
//        qDebug() << "getSize";
        for (t = s; *t != '\0'; t++) {
            size++;
//            qDebug() << "size : " << size;
        }

        return size;
}

QString MainWindow::removeSpaceAndEnter(char *s)
{
    char *t; string temp;
    for (t = s; *t != '\0'; t++) {
      temp = temp + *t;
    }
    QString result =QString::fromStdString(temp);
    result = result.remove("\t");
    result = result.remove("\n");
//    qDebug() << "size : " << result.size() ;
    return result;
}

//*******************
void MainWindow::machineState()
{
//    QByteArray data,data2;
//    data = serial->readAll();
//    if(counter == 0) { data1 = serial->readAll();delay(100);QString test(data1) ;qDebug() << "data1 : " << test; }
//    if(counter == 1) { data2 = serial->readAll();QString test(data2) ;qDebug() << "data2 : " << test; }
//    counter++;
//    if(counter == 2) {
//      QByteArray data;
//      for()
//      data.append(data1);data.append(data2);
//      QString stringBuffer(data) ;
//      char *message = data.data();
//      qDebug() << "*message: " << message;
//      qDebug() << "data : " << stringBuffer;
//      decodePacket(message);counter =0;
//    }
//    uint8_t byteDataBuffr[240];
//    qDebug() << "data2 : " << stringBuffer2; recievedData
    QByteArray dataByteArray = serial->readAll();
    for(int i=0;i<dataByteArray.size();i++) {
       if(counter == 0) {
           if (dataByteArray[i] == '*') {
               counter++;
           } else {
               recievedData.append(dataByteArray[i]);
           }
       } else if(counter > 0) {
           if (dataByteArray[i] == '*') {
               counter++;
               // get three ***
               if(counter == 3 ) {
                  counter = 0 ;
                  char *message = recievedData.data();
                  recievedData.clear();
                  qDebug() << "*message: " << message;
                  decodePacket(message);
               }
           } else {
              while (counter > 1) {
                  recievedData.append('*');counter--;
              }
           }
       }
    }
}


void MainWindow::on_connect_clicked()
{

    if(ui->comboBox->currentText() != NULL) {

        serial->close();
        come_port = ui->comboBox->currentText();

        serial->setPortName(come_port);

        serial->close();

        while(!serial->open(QIODevice::ReadWrite));
        connect(serial, SIGNAL(readyRead()), SLOT(machineState()));
        ui->connect->setDisabled(true);
        ui->pushButton->setEnabled(true);
        if(serial->isOpen())
        {
            qDebug() << "is Open " ;
//            writeSerial("*"+username+"%"+password+"#");
        }
    }
}

void MainWindow::on_scan_clicked()
{
    ui->comboBox->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
           ui->comboBox->addItem(port.portName());
    }
}

void MainWindow::on_pushButton_clicked()
{
    serial->close();
    ui->pushButton->setDisabled(true);
    ui->connect->setEnabled(true);
}
