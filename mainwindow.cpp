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
    powerButton->setGeometry(140, 168, 400, 400);
    connect(powerButton, &PowerButton::toggled, this, &MainWindow::OnPowerButtonToggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPowerButtonToggled(bool on)
{
    QString sourceFilePath = __FILE__;
    QString appDir = sourceFilePath.left(sourceFilePath.lastIndexOf(QDir::separator()) + 1);

    if (on) {
        OnButtonClicked(appDir);
    } else {
        OffButtonClicked();
    }
}

void MainWindow::OnButtonClicked(const QString& appDir)
{
    QString xrayPath = QDir(appDir).filePath("Xray/xray-core/xray.exe");

    if (!QFile::exists(xrayPath)) {
        qDebug() << "Не удалось найти исполняемый файл Xray по пути:" << xrayPath;
        return;
    }

    m_xrayProcess = new QProcess(this);

    QStringList arguments;
    arguments << "run" << "-c" << QDir(appDir).filePath("Xray/configs/config.json");

    m_xrayProcess->start(xrayPath, arguments);

    if (!m_xrayProcess->waitForStarted()) {
        qDebug() << "Ошибка при запуске процесса Xray";
        return;
    }

    qDebug() << "Процесс Xray успешно запущен";
    SetSystemProxy(2081, 2080);
}

void MainWindow::OffButtonClicked()
{
    if (m_xrayProcess && m_xrayProcess->state() == QProcess::Running) {
        ClearSystemProxy();
        m_xrayProcess->kill(); // Или m_xrayProcess->terminate();
        qDebug() << "Процесс Xray завершен";
    } else {
        qDebug() << "Процесс Xray не запущен";
    }
}
