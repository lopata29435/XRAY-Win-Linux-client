#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QColor>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QtWidgets>
#include <QPainter>

class BreathingWidget : public QWidget {
public:
    BreathingWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground);
        setFixedSize(200, 200);

        QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurRadius(50);
        setGraphicsEffect(blurEffect);
        startAnimation();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        //параметры отрисовки, они тебе не нужны
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        // Рисуем внешний круг (кольцо)
        painter.setPen(QPen(QColor(0xFFFFFF), thickness)); // Толщина кольца
        painter.drawEllipse(outerRect);

        // Рисуем внутренний круг
        painter.setBrush(Qt::blue);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(innerRect);
    }

private:
    QRectF outerRect;
    QRectF innerRect;
    qreal thickness;
    QPropertyAnimation *sizeAnimation;

    void startAnimation() {
        // Анимация изменения размера кольца
        sizeAnimation = new QPropertyAnimation(this, "outerRect");
        sizeAnimation->setDuration(2000); // Длительность анимации (в миллисекундах)
        sizeAnimation->setLoopCount(-1); // Бесконечное повторение анимации

        // Устанавливаем начальное и конечное состояния анимации
        QRectF startOuterRect(50, 50, 100, 100); // Начальный размер и положение кольца
        QRectF endOuterRect(0, 0, 200, 200);   // Конечный размер и положение кольца
        sizeAnimation->setKeyValueAt(0, startOuterRect);
        sizeAnimation->setKeyValueAt(1, endOuterRect);

        connect(sizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value) {
            outerRect = value.toRectF();
            // Рассчитываем толщину кольца в зависимости от удаленности от центра
            qreal distance = QLineF(outerRect.center(), rect().center()).length();//QLineF - линейная функция
            thickness = qMax(10.0 - distance / 10, 1.0);
            update();
        });

        sizeAnimation->start();
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CrossBorder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    a.setWindowIcon(QIcon(":/icons/icon.ico"));
    MainWindow w;

    BreathingWidget breathingWidget;
    breathingWidget.show();

    w.show();
    return a.exec();
}
