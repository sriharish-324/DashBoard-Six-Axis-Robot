#ifndef SENDTOUNITY_H
#define SENDTOUNITY_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QtSerialPort/QSerialPort>

class SendToUnity : public QObject
{
    Q_OBJECT
    QTcpSocket* socket = new QTcpSocket(this);
    QTcpServer* m_server = new QTcpServer(this);
    QTcpSocket* client;
    QList<QTcpSocket*> clients;
    int totalClients = 0;

public:
    explicit SendToUnity(QObject *parent = nullptr);
    void sendDirection(QByteArray direction);
    void StartServer();
    void readcolor();
    void erasecolor();
    void handleClientData(QByteArray dat);
    void write_color_to_file(QString);
    QFuture<void>Concurrent;
    void threadfunction();
    QString txtread();
    void sendcolor();
    void arduinosend(QString);
    void pySpeech();

private:
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

public slots:
    void onNewConnection();
    int connectflag();
    int setSendFlag();
    int offSendFlag();
    QString readvalues();
signals:

};

#endif // SENDTOUNITY_H
