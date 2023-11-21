#include "sendtounity.h"
#include "QTimer"
#include "QThread"
#include "QFile"
#include "QProcess"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
void threadfunction();
short uCanSend =0;
short voice = 0;
short connectFlag =0;
QString value;
QString ds;
QList<QByteArray> colors;
void sendDirection(QByteArray direction);
QTimer *timer = new QTimer;
QFile file("C:\\Users\\Administrator\\Desktop\\Qt_controls_Unity\\images\\tp.txt");
QFile pyfile("C:\\Users\\Administrator\\Desktop\\Qt_controls_Unity\\images\\recognized_text.txt");


SendToUnity::SendToUnity(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"Server Started.";
    this->StartServer();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading:" << file.errorString();
        return;
    }
    QTextStream in(&file);
    QString line;
    while (!(line = in.readLine()).isNull()) {
        QStringList colorTokens = line.split(',');
        foreach (const QString &colorToken, colorTokens) {
            QByteArray colorData = colorToken.toUtf8();
            colors.append(colorData);
        }
    }
    file.close();
    qDebug()<<"Your Records had been Restored";

    //ARDUINO
    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
        qDebug()<<("Port error", "Couldn't find the Arduino!");
    }

}

void SendToUnity::StartServer()
{
    connect(m_server, &QTcpServer::newConnection, this, &SendToUnity::onNewConnection);
    m_server->listen(QHostAddress("192.168.56.1"), 6381);
    qDebug()<<"Server Running on ip 192.168.56.1 in port 6381.";
}


void SendToUnity::handleClientData(QByteArray data)
{
    qDebug() << "Received data from client:" << data;
    value = data;
    ds = data;
//    if(voice){
//    if(data == "color" || data == "sorting" || data == "color sorting"){
//        voice = 0;
//        qDebug()<<"running Voice Command";
//        threadfunction();
//        qDebug()<<"stop1";

//    }
//    else{
//        qDebug()<<"running Voice Command";
//         voice = 0;
//        threadfunction();
//    }
//    }
}

void SendToUnity::onNewConnection()
{
    totalClients++;
    client = m_server->nextPendingConnection();
    clients.append(client);
    client->moveToThread(thread());
    socket->moveToThread(thread());
    connect(client, &QTcpSocket::readyRead, this, [this, client = this->client]() {
        QByteArray data = client->readAll();
        handleClientData(data);
    });
    connectFlag = 1;
    qDebug() << "Server Connection Established.";
}

int SendToUnity::connectflag()
{
    return connectFlag;
}

int SendToUnity::setSendFlag()
{
    uCanSend =1;
    return uCanSend;
}

int SendToUnity::offSendFlag()
{
    uCanSend =0;
    return uCanSend;
}

void SendToUnity::write_color_to_file(QString work)
{
    if (!file.isOpen()) {
        if (!file.open(QIODevice::Append | QIODevice::Text | QIODevice::Truncate)) {
            qDebug() << "Could not open file for writing:" << file.errorString();
            return;
        }
    }

    QTextStream out(&file);
    if (work == "ColorSorting") {
        ;
        foreach (const QByteArray &color, colors) {
            out<< color+",";
        }
        file.flush();
        file.close();
    }
}

void SendToUnity::threadfunction()
{
    Concurrent = QtConcurrent::run(this, &SendToUnity::sendcolor);

}

void SendToUnity::arduinosend(QString command)
{
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
        qDebug()<<command.toStdString().c_str();
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}
QString SendToUnity::txtread(){
//    QString fileContent;

//        if (!pyfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            qDebug() << "Could not open file for reading:" << pyfile.errorString();
//        }

//        QTextStream in(&pyfile);
//        QString line;
//        while (!(line = in.readLine()).isNull()) {
//            qDebug() << line;
//            fileContent += line + "\n";
//        }

//        pyfile.close();
//        qDebug() << line;
//        if(line  == "color sorting"){
//            threadfunction();
//        }
    return ds;
}

void SendToUnity::pySpeech()
{
    QThread* workerThread = new QThread;
        QProcess* p = new QProcess;

        QStringList params;
        params << "C:/Users/Administrator/Downloads/realsense_depth/realsense_depth.py";
        p->start("C:/Users/Administrator/AppData/Local/Programs/Python/Python310/python.exe", params);
        p->moveToThread(workerThread);
        workerThread->start();
        qDebug() << "Python script started";
        QObject::connect(workerThread, &QThread::finished, [this, workerThread, p]() {
            p->deleteLater();
            workerThread->deleteLater();
            qDebug() << "Python script finished";
        });
       QThread::sleep(5);
}

QString SendToUnity::readvalues()
{
    return value;
}

void SendToUnity::erasecolor()
{
    colors.clear();

}

void SendToUnity::sendcolor()
{
    int delayInterval = 20;
    foreach(QString y, colors) {
        sendDirection(y.toUtf8());
        QThread::msleep(delayInterval);
    }
}

void SendToUnity::sendDirection(QByteArray direction)
{
    if(uCanSend == 1){
        QList temp =direction.split('|');
        if(temp[0] == "color"){
            for(int i=0; i<totalClients; i++){
                if(clients[i]->isOpen()){
                    // qDebug()<<((temp[1]+"|"+temp[2]));
                    clients[i]->write((temp[1]+"|"+temp[2]));
                    clients[i]->flush();
                    qDebug()<<"DataSent";
                    colors.append((temp[1]+"|"+temp[2]));
                }
            }
        }
        else{
            qDebug() << Q_FUNC_INFO;
            for(int i=0; i<totalClients; i++){
                if(clients[i]->isOpen()){
                    clients[i]->write(direction);
                    clients[i]->flush();
                    qDebug()<<"Data Sent";
                }
            }
        }
    }
    else{
        qDebug()<<"flag not set";
    }
}
