    #include "script.h"
#include "QProcess"
#include <QThread>
#include<QDebug>
#include<QFile>
#include<sendtounity.h>


QString filename = "C:/Users/Administrator/Documents/Qt_controls_Unity/images/tp.txt";
void write(QString data);

Script::Script(QObject *parent) : QObject(parent)
{

}
void Script::submitdata(QString Input)
{
    if (Input == "run") {
        QThread* workerThread = new QThread;
        QProcess* p = new QProcess;

        QStringList params;
        params << "C:/Users/sriha/PycharmProjects/ImageClassifierFlask/pythonProject2/main.py";
        p->start("python", params);

        p->moveToThread(workerThread);

        workerThread->start();

        QObject::connect(workerThread, &QThread::finished, [this, workerThread, p]() {
            p->deleteLater();
            workerThread->deleteLater();
        });
    }



}

void Script::filedata(QString FileValue)
{
    qDebug()<<FileValue;
    write(FileValue);
}


void write(QString data)
{
    QFile file(filename);
    if(!file.open(QFile::Append |QFile::Text) )
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    QTextStream out(&file);
    out << QVariant(data).toString()+" ";
    out << "\r\n";
    file.flush();
    file.close();
}
Script::~Script()
{
    QFile file(filename);
    file.remove();
}
