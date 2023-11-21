#include "unitywidget.h"
#include "ui_unitywidget.h"
#include<QQmlContext>
#include <QQmlApplicationEngine>
#include<QFile>
QList<QString> color;
QString filename ="C:\\Users\\Administrator\\Desktop\\Qt_controls_Unity\\images\\log.txt";
UnityWidget::UnityWidget(QWidget *parent): QWidget(parent), ui(new Ui::UnityWidget)
{
    connect(newTimer, SIGNAL(timeout()), this, SLOT(dirSend()));
    ui->setupUi(this);
    unityWindow->showMaximized();
    QQuickView *view = new QQuickView;
    QWidget *container = QWidget::createWindowContainer(view,this);
    view->rootContext()->setContextProperty("unityWidgetClass",this);
    view->setSource(QUrl("qrc:/main.qml"));
    container->setMinimumSize(600,600);
    ui->horizontalLayout->addWidget(container);
    ui->horizontalLayout->addWidget(unityWindow);
}

UnityWidget::~UnityWidget()
{
    delete ui;
}

void UnityWidget::changeVal(QString direction)
{
    m_direction = direction;
    qDebug()<<Q_FUNC_INFO<<"====="<<direction;
    newTimer->start(20);
}

void UnityWidget::writeinlog(QString File){
    QFile file(filename);
        if(!file.open(QFile::Append |QFile::Text) )
        {
            qDebug() << " Could not open file for writing";
            return;
        }

        QTextStream out(&file);
        out << QVariant(File).toString()+" ";
        out << "\r\n";
        file.flush();
        file.close();

}
void UnityWidget::colorcomand(){

    m_sendToUnity.threadfunction();
}

QString UnityWidget::pyspeech()
{
    m_sendToUnity.pySpeech();
    return "";
}

int UnityWidget::isconnect()
{
    return m_sendToUnity.connectflag();
}

void UnityWidget::savetofile(QString work_name)
{
    m_sendToUnity.write_color_to_file(work_name);

}

int UnityWidget::canISet()
{

    return m_sendToUnity.setSendFlag();

}

int UnityWidget::canIOff()
{
    return m_sendToUnity.offSendFlag();
}

void UnityWidget::dirSend()
{
    m_sendToUnity.sendDirection(m_direction.toUtf8());
}

void UnityWidget::stopSending()
{
    newTimer->stop();
}

QString UnityWidget::sendvalues()
{
    return m_sendToUnity.readvalues();
}

void UnityWidget::sendval(QString command)
{
   m_sendToUnity.arduinosend(QString(command+"2"));

}
void UnityWidget::clearcolor()
{
    m_sendToUnity.erasecolor();
}


