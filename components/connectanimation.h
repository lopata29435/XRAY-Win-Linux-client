#ifndef CONNECTANIMATION_H
#define CONNECTANIMATION_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QRectF>

class ConnectAnimation : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QRectF outerRect READ outerRect WRITE setOuterRect NOTIFY outerRectChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)
    Q_PROPERTY(QColor ringColor READ ringColor WRITE setRingColor NOTIFY ringColorChanged)

public:
    explicit ConnectAnimation(QWidget *parent = nullptr);
    ~ConnectAnimation();

    QRectF outerRect() const {
        return m_outerRect;
    }

    void setOuterRect(const QRectF &rect);

    qreal opacity() const {
        return m_opacity;
    }

    void setOpacity(qreal opacity);

    int animationSpeed() const {
        return m_animationSpeed;
    }

    void setAnimationSpeed(int speed);

    QColor ringColor() const {
        return m_ringColor;
    }

    void setRingColor(const QColor &color);

signals:
    void outerRectChanged(const QRectF &rect);
    void opacityChanged(qreal opacity);
    void animationSpeedChanged(int speed);
    void ringColorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void startAnimation();

private:
    QRectF m_outerRect;
    QRectF innerRect;
    qreal thickness;
    qreal m_opacity;
    int m_animationSpeed;
    QColor m_ringColor;
    QPropertyAnimation *sizeAnimation;
    QPropertyAnimation *opacityAnimation;
};

#endif // CONNECTANIMATION_H
