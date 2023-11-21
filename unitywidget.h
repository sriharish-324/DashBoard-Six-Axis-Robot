#ifndef UNITYWIDGET_H
#define UNITYWIDGET_H

#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QQuickView>
#include <QTimer>

#include "unitywindow.h"
#include "sendtounity.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UnityWidget; }
QT_END_NAMESPACE

class UnityWidget : public QWidget
{
    Q_OBJECT

    UnityWindow *unityWindow = new UnityWindow("../UnityBuildClient3/RobotArm.exe", this);
    SendToUnity m_sendToUnity;
    QString m_direction;
    QTimer *newTimer = new QTimer;

public:
    UnityWidget(QWidget *parent = nullptr);
    ~UnityWidget();

public slots:
    void changeVal(QString direction);
    int isconnect();
    void savetofile(QString);
    int canISet();
    int canIOff();
    void dirSend();
    void clearcolor();
    void stopSending();
    void sendval(QString command);
    QString sendvalues();
    void colorcomand();
    QString pyspeech();
    void writeinlog(QString File);


private:
    Ui::UnityWidget *ui;
};
#endif // UNITYWIDGET_H
