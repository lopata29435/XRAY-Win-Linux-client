#include "CloseButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QSvgRenderer>

CloseButton::CloseButton(QWidget *parent) : QWidget(parent), isHovered(false)
{
    setFixedSize(24, 24);
}

void CloseButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (isHovered)
    {
        painter.setBrush(QColor(0xEA3943));
    }
    else
    {
        painter.setBrush(QColor(0x020A18));
    }
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    QString iconPath = ":/buttons/Buttons/CloseButton.svg";
    QSvgRenderer svgRendererButton(iconPath);
    QRect iconRectButton((24 - 12) / 2 , (24 - 12) / 2, 12, 12);
    svgRendererButton.render(&painter, iconRectButton);
}

void CloseButton::enterEvent(QEnterEvent *event)
{
    isHovered = true;
    QWidget::enterEvent(event);
    update();
}

void CloseButton::leaveEvent(QEvent *event)
{
    isHovered = false;
    QWidget::leaveEvent(event);
    update();
}

void CloseButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QApplication::quit();
    }
}
