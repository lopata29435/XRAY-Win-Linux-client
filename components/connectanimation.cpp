#include "connectanimation.h"

#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>

ConnectAnimation::ConnectAnimation(QWidget *parent)
    : QWidget(parent)
    , m_outerRect(50, 50, 100, 100)
    , innerRect(175, 203, 50, 50) // Initialize innerRect
    , thickness(1.0)
    , m_opacity(1.0)
    , m_animationSpeed(4000) // Default animation speed
    , m_ringColor(QColor(0xB84CEE)) // Default ring color
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(600, 600);

    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(50);
    setGraphicsEffect(blurEffect);

    startAnimation();
}

ConnectAnimation::~ConnectAnimation() {
    if (sizeAnimation) {
        sizeAnimation->stop();
        delete sizeAnimation;
    }

    if (opacityAnimation) {
        opacityAnimation->stop();
        delete opacityAnimation;
    }
}

void ConnectAnimation::setOuterRect(const QRectF &rect) {
    if (m_outerRect == rect)
        return;
    m_outerRect = rect;
    emit outerRectChanged(m_outerRect);
    update();
}

void ConnectAnimation::setOpacity(qreal opacity) {
    if (qFuzzyCompare(m_opacity, opacity))
        return;
    m_opacity = opacity;
    emit opacityChanged(m_opacity);
    update();
}

void ConnectAnimation::setAnimationSpeed(int speed) {
    if (m_animationSpeed == speed)
        return;
    m_animationSpeed = speed;
    emit animationSpeedChanged(speed);
    // Restart animation with new speed
    startAnimation();
}

void ConnectAnimation::setRingColor(const QColor &color) {
    if (m_ringColor == color)
        return;
    m_ringColor = color;
    emit ringColorChanged(color);
    update();
}

void ConnectAnimation::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill the background with the specified color
    QColor backgroundColor = QColor(0x1c212b); // Example: semi-transparent black
    painter.fillRect(this->rect(), backgroundColor);

    painter.setOpacity(m_opacity); // Set opacity for fading effect

    // Draw the outer circle (ring)
    painter.setPen(QPen(m_ringColor, thickness)); // Use the current ring color
    painter.drawEllipse(m_outerRect);

    // Draw the inner circle
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(innerRect);
}

void ConnectAnimation::startAnimation() {
    // Center coordinates
    QPointF center(200, 228);

    // Animation for changing the size of the ring
    sizeAnimation = new QPropertyAnimation(this, "outerRect");
    sizeAnimation->setDuration(m_animationSpeed); // Use the current animation speed
    sizeAnimation->setLoopCount(-1); // Infinite animation loop

    // Set keyframes for size animation
    QRectF startOuterRect(center.x() - 300, center.y() - 300, 600, 600); // Initial size and position of the ring
    QRectF endOuterRect(center.x() - 10, center.y() - 10, 20, 20); // Final size and position of the ring

    sizeAnimation->setKeyValueAt(0.0, startOuterRect);
    sizeAnimation->setKeyValueAt(1.0, endOuterRect);

    connect(sizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value) {
        setOuterRect(value.toRectF());
        // Calculate the ring thickness depending on the distance from the center
        qreal distance = QLineF(m_outerRect.center(), QPointF(200, 228)).length();
        thickness = qMax(50.0 - distance / 50, 1.0);
    });

    // Animation for changing opacity
    opacityAnimation = new QPropertyAnimation(this, "opacity");
    opacityAnimation->setDuration(m_animationSpeed); // Use the current animation speed
    opacityAnimation->setLoopCount(-1); // Infinite animation loop

    // Set keyframes for opacity animation
    opacityAnimation->setKeyValueAt(0.0, 1.0);   // Full visibility
    opacityAnimation->setKeyValueAt(1.0, 0.0);   // Full transparency (disappearance)

    sizeAnimation->start();
    opacityAnimation->start();
}
