#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "3rdparty/v2/components/proxy/QvProxyConfigurator.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QColor>
#include <QLabel>
#include <QGraphicsBlurEffect>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QFontDatabase>
#include <QTimer>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QHBoxLayout>
#include <QSvgRenderer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint); // Удаление стандартной рамки окна

    // Создание кастомных кнопок:
    QPushButton *closeButton = new QPushButton("Закрыть", this);
    QPushButton *minimizeButton = new QPushButton("Свернуть", this);

    // Настройка стилей кнопок:
    closeButton->setStyleSheet("QPushButton { background-color: red; }");
    minimizeButton->setStyleSheet("QPushButton { background-color: blue; }");

    // Установка положения кнопок и их размеров:
    closeButton->setGeometry(10, 10, 50, 30);
    minimizeButton->setGeometry(70, 10, 50, 30);

    // Соединение сигналов и слотов:
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(minimizeButton, &QPushButton::clicked, this, &MainWindow::showMinimized);

    this->setStyleSheet("background-image: url(:/ui/UI/Background.svg);");

    QString unbounded = loadFont(":/fonts/Fonts/Unbounded/Unbounded-Medium.ttf");
    QString openSans = loadFont(":/fonts/Fonts/OpenSans/OpenSans-Regular.ttf");

    QLabel *titleLabel = new QLabel("CrossBorder", this);
    QFont unboundedFont(unbounded, 24);
    titleLabel->setFont(unboundedFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(75, 32, 250, 30);


    QLabel *footerVersionLabel = new QLabel("Ver: Alpha 1.0.0", this);
    QFont openSansFont(openSans, 8);
    footerVersionLabel->setFont(openSansFont);
    footerVersionLabel->setStyleSheet("color: #80FAFAFA; line-height: 102%;");
    footerVersionLabel->setAlignment(Qt::AlignLeft);
    footerVersionLabel->setGeometry(16, 522, 79, 15);

    QPushButton *hyperlinkButton = new QPushButton(this);
    hyperlinkButton->setFlat(true);
    hyperlinkButton->setStyleSheet("color: #80FAFAFA;");
    hyperlinkButton->setCursor(Qt::PointingHandCursor);
    hyperlinkButton->setStyleSheet("QPushButton:pressed { background-color: transparent; }");

    QHBoxLayout *layout = new QHBoxLayout(hyperlinkButton);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    QLabel *textLabel = new QLabel("Техподдержка", hyperlinkButton);
    textLabel->setFont(openSansFont);
    textLabel->setStyleSheet("color: #80FAFAFA;");

    QSvgRenderer *svgRenderer = new QSvgRenderer(QStringLiteral(":/icons/Icons/TgSupport.svg"));
    QPixmap pixmap(svgRenderer->defaultSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    svgRenderer->render(&painter);

    QLabel *iconLabel = new QLabel(hyperlinkButton);
    iconLabel->setPixmap(pixmap);

    layout->addWidget(textLabel);
    layout->addWidget(iconLabel);
    hyperlinkButton->setLayout(layout);

    hyperlinkButton->setGeometry(280, 522, 100, 15);

    connect(hyperlinkButton, &QPushButton::clicked, [=](){
        QDesktopServices::openUrl(QUrl("http://t.me/lopata29435"));
    });

    // Ensure other widgets are created and managed as before
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
    ShowConnectAnimation();

    // Создание таймера
    QTimer::singleShot(1000, this, [=]() {
        QString xrayPath = QDir(appDir).filePath("Xray/xray-core/xray.exe");

        if (!QFile::exists(xrayPath)) {
            powerButton->SetState(Error);
            ShowErrorAnimation();
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
            ShowErrorAnimation();
            return;
        }

        qDebug() << "Процесс Xray успешно запущен";
        powerButton->SetState(On);
        ShowOnAnimation();
        //TODO: надо обрабатывать ошибки подъема системного прокси
        SetSystemProxy(2081, 2080);
    });
}


void MainWindow::OffButtonClicked(PowerButton *powerButton)
{
    if (m_xrayProcess && m_xrayProcess->state() == QProcess::Running) {
        ClearSystemProxy();
        m_xrayProcess->kill(); // Или m_xrayProcess->terminate();
        qDebug() << "Процесс Xray завершен";
        powerButton->SetState(Off);
        HideConnectAnimation();
        HideErrorAnimation();
        HideOnAnimation();
    } else {
        qDebug() << "Процесс Xray не запущен";
        powerButton->SetState(Error);
        ShowErrorAnimation();
    }
}

void MainWindow::SetBackgroundColor(const QColor &color)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, color);
    this->setPalette(palette);
}

void MainWindow::ShowOnAnimation()
{
    HideConnectAnimation();
    HideErrorAnimation();
    onAnimation = new OnAnimation(this);
    onAnimation->setGeometry(0, 0, width(), height());
    onAnimation->lower();
    onAnimation->show();
}

void MainWindow::HideOnAnimation()
{
    if (onAnimation) {
        delete onAnimation;
        onAnimation = nullptr;
    }
}

void MainWindow::ShowConnectAnimation()
{
    HideErrorAnimation();
    HideOnAnimation();
    connectAnimation = new ConnectAnimation(this);
    connectAnimation->setGeometry(0, 0, width(), height());
    connectAnimation->lower();
    connectAnimation->show();
}

void MainWindow::HideConnectAnimation()
{
    if (connectAnimation) {
        delete connectAnimation;
        connectAnimation = nullptr;
    }
}

void MainWindow::ShowErrorAnimation()
{
    HideConnectAnimation();
    HideOnAnimation();
    errorAnimation = new ErrorAnimation(this);
    errorAnimation->setGeometry(0, 0, width(), height());
    errorAnimation->lower();
    errorAnimation->show();
}

void MainWindow::HideErrorAnimation()
{
    if (errorAnimation) {
        delete errorAnimation;
        errorAnimation = nullptr;
    }
}

QString MainWindow::loadFont(const QString &fontPath){
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId == -1) {
        qDebug() << "Failed to load font:" << fontPath;
        return QString();
    }
    return QFontDatabase::applicationFontFamilies(fontId).at(0);
}
