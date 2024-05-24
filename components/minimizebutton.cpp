#include "minimizebutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QSvgRenderer>

MinimizeButton::MinimizeButton(QWidget *parent) : QWidget(parent), isHovered(false)
{
    setFixedSize(24, 24);
}

void MinimizeButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (isHovered)
    {
        painter.setBrush(QColor(0xB84CEE));
    }
    else
    {
        painter.setBrush(QColor(0x020A18));
    }
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    QString iconPath = ":/buttons/Buttons/MinimizeButton.svg";
    QSvgRenderer svgRendererButton(iconPath);
    QRect iconRectButton((24 - 12) / 2 , (24 - 12) / 2, 12, 12);
    svgRendererButton.render(&painter, iconRectButton);
}

void MinimizeButton::enterEvent(QEnterEvent *event)
{
    isHovered = true;
    QWidget::enterEvent(event);
    update();
}

void MinimizeButton::leaveEvent(QEvent *event)
{
    isHovered = false;
    QWidget::leaveEvent(event);
    update();
}

void MinimizeButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (window())
        {
            window()->showMinimized();
        }
    }
}
