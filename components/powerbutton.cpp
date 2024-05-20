#include "powerbutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QIcon>
#include <QSvgRenderer>

PowerButton::PowerButton(QWidget *parent)
    : QWidget(parent), isOn(false)
{
    setFixedSize(120, 120);
}

void PowerButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем круг
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#020A18"));
    painter.drawEllipse(0, 0, 120, 120);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Рисуем иконку в центре
    QSvgRenderer svgRenderer(isOn ? QString(":/icons/PowerButton_On.svg") : QString(":/icons/PowerButton_Off.svg"));
    QRect iconRect((120 - 36) / 2 , (120 - 36) / 2, 36, 36);
    svgRenderer.render(&painter, iconRect);
}

void PowerButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isOn = !isOn;
        emit toggled(isOn);
        update();
    }
}
