#pragma once

#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QString>

class QProcess;

class UnityWindow : public QWidget
{
    Q_OBJECT
public:
    UnityWindow(const QString& unityExePath, QWidget* parent = nullptr);
    ~UnityWindow();

    bool isValid() const;

protected:
    virtual void applicationStateChanged(Qt::ApplicationState state);
    virtual void resizeEvent(QResizeEvent * ev) override;
    virtual void showEvent(QShowEvent * ev) override;
    virtual void hideEvent(QHideEvent * ev) override;
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long * result) override;


private:

#ifdef Q_OS_WIN
    static BOOL CALLBACK sEnumChildWindows(HWND handle, LPARAM params);
    bool enumChildWindows(HWND handle);
    HWND m_unityHandle;
#endif

    bool m_bIsValid = false;
    QProcess* m_pUnityProcess = nullptr;
    QString m_unityExecutablePath;
};
