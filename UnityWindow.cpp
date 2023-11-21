#include "UnityWindow.h"

#include <QApplication>
#include <QResizeEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QMessageBox>

#include <QDebug>

UnityWindow::UnityWindow(const QString& unityExePath, QWidget* parent)
    : QWidget(parent)
    , m_unityHandle(0)
#ifdef Q_OS_WIN
    , m_pUnityProcess(nullptr)
#endif
    , m_unityExecutablePath(unityExePath)
{
    connect(qApp, &QApplication::applicationStateChanged, this, &UnityWindow::applicationStateChanged);
}

UnityWindow::~UnityWindow()
{
    disconnect(qApp, &QApplication::applicationStateChanged, this, &UnityWindow::applicationStateChanged);

    if (m_pUnityProcess && m_pUnityProcess->state() == QProcess::ProcessState::Running)
    {
        m_pUnityProcess->kill();
    }
}

bool UnityWindow::isValid() const
{
    return m_bIsValid;
}

#ifdef Q_OS_WIN
bool UnityWindow::enumChildWindows(HWND handle)
{
    m_unityHandle = handle;
    auto qtMainThread = GetWindowThreadProcessId(reinterpret_cast<HWND>(winId()), nullptr);
    auto unityMainThread = GetWindowThreadProcessId(m_unityHandle, nullptr);
    AttachThreadInput(qtMainThread, unityMainThread, FALSE);
    AttachThreadInput(unityMainThread, qtMainThread, FALSE);
    SendMessageA(m_unityHandle, WM_ACTIVATE, WA_ACTIVE, 0);
    MoveWindow(m_unityHandle, 0, 0, width(), height(), true);
    return false; // first iteration should be Unity window
}
#endif

void UnityWindow::applicationStateChanged(Qt::ApplicationState state)
{
#ifdef Q_OS_WIN
    if (state == Qt::ApplicationActive)
    {
        SendMessageA(m_unityHandle, WM_ACTIVATE, WA_ACTIVE, 0);
    }
    else if (state == Qt::ApplicationSuspended)
    {
        SendMessageA(m_unityHandle, WM_ACTIVATE, WA_INACTIVE, 0);
    }
    else if (state == Qt::ApplicationInactive)
    {
        // Apparently, Qt calls ApplicationInactive just after ApplicationActive...
        // so I have disabled handling this state
        //SendMessage(m_unityHandle, WM_ACTIVATE, WA_INACTIVE, 0);
    }
    else if (state == Qt::ApplicationHidden)
    {
        SendMessageA(m_unityHandle, WM_ACTIVATE, WA_INACTIVE, 0);
    }
#endif
}

void UnityWindow::resizeEvent(QResizeEvent *ev)
{
    QWidget::resizeEvent(ev);

#ifdef Q_OS_WIN
    if (m_unityHandle != 0)
    {
        MoveWindow(m_unityHandle, 0, 0, width(), height(), true);
    }
#endif
}

#ifdef Q_OS_WIN
BOOL UnityWindow::sEnumChildWindows(HWND handle, LPARAM params)
{
    UnityWindow *w = reinterpret_cast<UnityWindow*>(params);
    return w->enumChildWindows(handle);
}
#endif

void UnityWindow::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);

#ifdef Q_OS_WIN
    if (m_unityHandle == 0)
    {
        QFileInfo unityFileInfo(QDir(QCoreApplication::applicationDirPath()), m_unityExecutablePath);

        m_pUnityProcess = new QProcess(this);
        QStringList args2;
        args2 << "-parentHWND";
        args2 << QString::number(winId());
        m_pUnityProcess->startDetached(unityFileInfo.canonicalFilePath(), args2);

        HWND qtHandle = reinterpret_cast<HWND>(winId());

        while (m_unityHandle == nullptr)
            EnumChildWindows(qtHandle, sEnumChildWindows, reinterpret_cast<LPARAM>(this));

        m_bIsValid = true;
    }
#endif
}

void UnityWindow::hideEvent(QHideEvent *ev)
{
    QWidget::hideEvent(ev);

#ifdef Q_OS_WIN
    SendMessage(m_unityHandle, WM_ACTIVATE, WA_INACTIVE, 0);
#endif
}

bool UnityWindow::nativeEvent(const QByteArray& eventType, void* message, long * result)
{
    return QWidget::nativeEvent(eventType, message, result);
}
