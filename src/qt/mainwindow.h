#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QThread>
#include "scqt_worker.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMessage(QString const &msg_origin, QString const &msg_level, QString const &msg);
    void onResetClicked(bool checked = false);
    void onClockClicked(bool checked = false);
    void onExitClicked(bool checked = false);
    void newHierarchy(QStringList hier);
    void newReport(QStringList rep);

signals:
    void Message(QString const &msg_origin, QString const &msg_level, QString const &msg);
    void setNewState(int newState);
    void requestHierarchy();
    void requestReport();
    void setTraceList(QStringList tl);

private:
    void _sendMessage(QString const &msg_level, QString const &msg);
    void setupUi();
    void setupSim();

    QThread *m_scqtThread;
    scQtWorker *m_scqtWorker;
    sc_thread_config_t m_ConfigParams;
    QTextBrowser *logConsole;

};
#endif // MAINWINDOW_H
