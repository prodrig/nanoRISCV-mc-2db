#include <QApplication>
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <systemc.h>
#include <pthread.h>
#include "scqt_worker.h"


// -------------- From "How do I create a pause/wait function using Qt?" -----
// -------------- https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt -----
// -------------- answered Mar 24, 2017 at 15:18 by dvntehn00bz -----------------
#include <QEventLoop>
#include <QTimer>
inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_scqtThread(nullptr)
    , m_scqtWorker(nullptr)
{
    connect(this, &MainWindow::Message, this, &MainWindow::onMessage);
    setupUi();
    setupSim();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onMessage(QString const &msg_origin, QString const &msg_level, QString const &msg)
{
    QString logMessage(msg_level);
    logMessage.append(" msg (").append(msg_origin).append(") :: ").append(msg);
    logConsole->append(logMessage);
}

void MainWindow::onResetClicked(bool checked)
{
    (void)checked;
    _sendMessage("DEBUG", "Reset clicked");
}

void MainWindow::onClockClicked(bool checked)
{
    (void)checked;
    _sendMessage("DEBUG", "Clock clicked");
    if (m_scqtWorker != nullptr) {
        emit setNewState(SC_ST_COMMAND_RUN);
        emit requestReport();
    }
}

void MainWindow::onExitClicked(bool checked)
{
    (void)checked;
    _sendMessage("DEBUG", "Exit clicked, terminating program");
    if (m_scqtWorker != nullptr) {
        emit setNewState(SC_ST_COMMAND_EXIT);
    }
    delay(1000);
    QApplication::quit();
}

void MainWindow::newHierarchy(QStringList hier)
{
    QString debugMsg("Hierarchy received:");
    for (qsizetype i=0; i<hier.size(); i++) {
        if (i == 0) {
            debugMsg.append(" ");
        } else {
            debugMsg.append(", ");
        }
        debugMsg.append(hier.at(i));
    }
    _sendMessage("DEBUG", debugMsg);
}

void MainWindow::newReport(QStringList rep)
{
    QString debugMsg("Report received:");
    for (qsizetype i=0; i<rep.size(); i++) {
        if (i == 0) {
            debugMsg.append(" ");
        } else {
            debugMsg.append(", ");
        }
        debugMsg.append(rep.at(i));
    }
    _sendMessage("DEBUG", debugMsg);
}

void MainWindow::_sendMessage(QString const &msg_level, QString const &msg)
{
    emit Message("GUI", msg_level, msg);
}

void MainWindow::setupUi()
{
    QGridLayout *gridLayout = new QGridLayout;
    QPushButton *btnReset;
    QPushButton *btnClock;
    QPushButton *btnExit;

    QWidget *cWidget = new QWidget();
    setCentralWidget(cWidget);
    centralWidget()->setLayout(gridLayout);

    btnReset = new QPushButton("Reset");
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    btnClock = new QPushButton("Clock");
    connect(btnClock, &QPushButton::clicked, this, &MainWindow::onClockClicked);
    btnExit = new QPushButton("Exit");
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::onExitClicked);

    logConsole = new QTextBrowser();

    // Layout grid
    // First row:   <Reset>  ...splitter...  <Clock>
    // Second row:  <          logConsole          >
    // Third row:   <Exit>
    gridLayout->addWidget(btnReset, 0, 0);
    gridLayout->addWidget(new QSplitter(Qt::Horizontal), 0, 1);
    gridLayout->addWidget(btnClock, 0, 2);
    gridLayout->addWidget(logConsole, 2, 0, 1, 3); // Span the three columns
    gridLayout->addWidget(btnExit, 3, 0);

    setWindowTitle("SystemC Test");
}

void MainWindow::setupSim()
{
    m_scqtThread = new QThread;
    m_scqtWorker = new scQtWorker;
    m_scqtWorker->moveToThread(m_scqtThread);

    // Prepare config parameters
    m_ConfigParams.rst_act_level = false; // Reset: low-level active
    m_ConfigParams.rst_act_microsteps = 20; // Reset active for 25 usteps (2.5 clock periods)
    m_ConfigParams.clk_act_edge  = false; // Clock: falling-edge sensitive
    m_ConfigParams.ena_act_level = true;  // Enable: high-level active
    m_ConfigParams.clk_period = sc_time(10, SC_NS);   // CLock period: 10 ns
    m_ConfigParams.clk_semiperiod_microsteps = 4;
    m_ConfigParams.microstep = m_ConfigParams.clk_period/m_ConfigParams.clk_semiperiod_microsteps;

    m_scqtWorker->setConfigParameters(m_ConfigParams);

    connect(m_scqtThread, &QThread::started, m_scqtWorker, &scQtWorker::doStart);
    connect(m_scqtWorker, &scQtWorker::Message, this, &MainWindow::onMessage);
    connect(this, &MainWindow::setNewState, m_scqtWorker, &scQtWorker::setNewState);
    connect(this, &MainWindow::requestHierarchy, m_scqtWorker, &scQtWorker::getHierarchy);
    connect(this, &MainWindow::requestReport, m_scqtWorker, &scQtWorker::getReport);
    connect(this, &MainWindow::setTraceList, m_scqtWorker, &scQtWorker::setTraceList);
    connect(m_scqtWorker, &scQtWorker::signalHierarchy, this, &MainWindow::newHierarchy);
    connect(m_scqtWorker, &scQtWorker::signalReport, this, &MainWindow::newReport);

    m_scqtThread->start();

    emit setNewState(SC_ST_COMMAND_RUN);
    emit requestHierarchy();
}
