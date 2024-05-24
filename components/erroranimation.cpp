#include "erroranimation.h"

#include <QPainter>

ErrorAnimation::ErrorAnimation(QWidget *parent)
    : QWidget(parent)
    , m_rect(0, 0, 400, 550)
    , m_opacity(1.0)
    , m_animationSpeed(4000)
    , m_color(QColor(0xEA3943))
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(400, 550);

    startAnimation();
}

ErrorAnimation::~ErrorAnimation() {
    if (opacityAnimation) {
        opacityAnimation->stop();
        delete opacityAnimation;
    }
}

void ErrorAnimation::setRect(const QRectF &rect) {
    if (m_rect == rect)
        return;
    m_rect = rect;
    emit rectChanged(m_rect);
    update();
}

void ErrorAnimation::setOpacity(qreal opacity) {
    if (qFuzzyCompare(m_opacity, opacity))
        return;
    m_opacity = opacity;
    emit opacityChanged(m_opacity);
    update();
}

void ErrorAnimation::setAnimationSpeed(int speed) {
    if (m_animationSpeed == speed)
        return;
    m_animationSpeed = speed;
    emit animationSpeedChanged(speed);
    // Restart animation with new speed
    startAnimation();
}

void ErrorAnimation::setColor(const QColor &color) {
    if (m_color == color)
        return;
    m_color = color;
    emit colorChanged(color);
    update();
}

void ErrorAnimation::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill the background with the specified color
    QColor backgroundColor = QColor(0x1c212b); // Example: semi-transparent black
    painter.fillRect(this->rect(), backgroundColor);

    painter.setOpacity(m_opacity); // Set opacity for fading effect

    // Draw the error rectangle
    painter.setPen(QPen(m_color, 2)); // Use the current color
    painter.setBrush(QBrush(m_color));
    painter.drawRect(m_rect);
}

void ErrorAnimation::startAnimation() {
    // Animation for changing opacity
    opacityAnimation = new QPropertyAnimation(this, "opacity");
    opacityAnimation->setDuration(m_animationSpeed); // Use the current animation speed
    opacityAnimation->setLoopCount(-1); // Infinite animation loop

    // Set keyframes for opacity animation
    opacityAnimation->setKeyValueAt(0.0, 0.0);   // Full transparency
    opacityAnimation->setKeyValueAt(0.4, 1.0);   // Full visibility
    opacityAnimation->setKeyValueAt(0.6, 1.0);   // Full visibility
    opacityAnimation->setKeyValueAt(1.0, 0.0);   // Full transparency

    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        // Swap the start and end values for opacity animation
        qreal startOpacity = opacityAnimation->startValue().toReal();
        qreal endOpacity = opacityAnimation->endValue().toReal();
        opacityAnimation->setStartValue(endOpacity);
        opacityAnimation->setEndValue(startOpacity);
    });
    opacityAnimation->start();
}
