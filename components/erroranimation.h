#ifndef ERRORANIMATION_H
#define ERRORANIMATION_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QRectF>

class ErrorAnimation : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(int animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ErrorAnimation(QWidget *parent = nullptr);
    ~ErrorAnimation();

    QRectF rect() const {
        return m_rect;
    }

    void setRect(const QRectF &rect);

    qreal opacity() const {
        return m_opacity;
    }

    void setOpacity(qreal opacity);

    int animationSpeed() const {
        return m_animationSpeed;
    }

    void setAnimationSpeed(int speed);

    QColor color() const {
        return m_color;
    }

    void setColor(const QColor &color);

signals:
    void rectChanged(const QRectF &rect);
    void opacityChanged(qreal opacity);
    void animationSpeedChanged(int speed);
    void colorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void startAnimation();

private:
    QRectF m_rect;
    qreal m_opacity;
    int m_animationSpeed;
    QColor m_color;
    QPropertyAnimation *opacityAnimation;
};

#endif // ERRORANIMATION_H
