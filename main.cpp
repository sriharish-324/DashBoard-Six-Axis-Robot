#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QDebug>

#include "unitywidget.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    UnityWidget m_widget;// = new UnityWidget;
    //qDebug()<<"Sizee========="<<sizeof(&m_widget);
    qDebug()<<"Loading Interface. ";
    m_widget.show();
    return app.exec();
}
