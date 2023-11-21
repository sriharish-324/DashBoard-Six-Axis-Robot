#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>

class Script : public QObject
{
    Q_OBJECT
public:
    explicit Script(QObject *parent = nullptr);
    ~Script();

public slots:
    void submitdata(QString Input);
    void filedata(QString FileValue);
};

#endif // SCRIPT_H
