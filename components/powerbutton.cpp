#include "powerbutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QIcon>
#include <QSvgRenderer>

PowerButton::PowerButton(QWidget *parent)
    : QWidget(parent)
    , isOn(false)
    , state_(Off)
{
    setFixedSize(192, 192);
}

void PowerButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString buttonIcon;
    switch(state_){
    case On:
        buttonIcon = ":/buttons/Buttons/PowerButton_On.svg";
        break;
    case Off:
        buttonIcon = ":/buttons/Buttons/PowerButton_Off.svg";
        break;
    case Connecting:
        buttonIcon = ":/buttons/Buttons/PowerButton_Off.svg";
        break;
    case Error:
        buttonIcon = ":/buttons/Buttons/PowerButton_Error.svg";
        break;
    default:
        buttonIcon = ":/buttons/Buttons/PowerButton_Off.svg";
        break;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0x020A18));
    painter.drawEllipse((192 - 120) / 2, (192 - 120) / 2, 120, 120);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QSvgRenderer svgRendererButton(buttonIcon);
    QRect iconRectButton((192 - 36) / 2 , (192 - 36) / 2, 36, 36);
    svgRendererButton.render(&painter, iconRectButton);
}

void PowerButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isOn = !isOn;
        emit toggled(isOn);
        update();
    }
}

void PowerButton::SetState(State state)
{
    state_ = state;
    update();
}

State PowerButton::GetState() const
{
    return state_;
}
