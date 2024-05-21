#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "3rdparty/v2/components/proxy/QvProxyConfigurator.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #020A18;");
    PowerButton *powerButton = new PowerButton(this);
    powerButton->setGeometry(104, 132, 192, 192);
    connect(powerButton, &PowerButton::toggled, this, [=](bool on) {
        OnPowerButtonToggled(on, powerButton);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPowerButtonToggled(bool on, PowerButton *powerButton)
{
    QString sourceFilePath = __FILE__;
    QString appDir = sourceFilePath.left(sourceFilePath.lastIndexOf(QDir::separator()) + 1);

    if (on) {
        OnButtonClicked(appDir, powerButton);
    } else {
        OffButtonClicked(powerButton);
    }
}

void MainWindow::OnButtonClicked(const QString& appDir, PowerButton *powerButton)
{
    powerButton->SetState(Connecting);
    QString xrayPath = QDir(appDir).filePath("Xray/xray-core/xray.exe");

    if (!QFile::exists(xrayPath)) {
        powerButton->SetState(Error);
        qDebug() << "Не удалось найти исполняемый файл Xray по пути:" << xrayPath;
        return;
    }

    m_xrayProcess = new QProcess(this);

    QStringList arguments;
    arguments << "run" << "-c" << QDir(appDir).filePath("Xray/configs/config.json");

    m_xrayProcess->start(xrayPath, arguments);

    if (!m_xrayProcess->waitForStarted()) {
        qDebug() << "Ошибка при запуске процесса Xray";
        powerButton->SetState(Error);
        return;
    }

    qDebug() << "Процесс Xray успешно запущен";
    powerButton->SetState(On);
    SetSystemProxy(2081, 2080);
}

void MainWindow::OffButtonClicked(PowerButton *powerButton)
{
    if (m_xrayProcess && m_xrayProcess->state() == QProcess::Running) {
        ClearSystemProxy();
        m_xrayProcess->kill(); // Или m_xrayProcess->terminate();
        qDebug() << "Процесс Xray завершен";
        powerButton->SetState(Off);
    } else {
        qDebug() << "Процесс Xray не запущен";
        powerButton->SetState(Error);
    }
}
