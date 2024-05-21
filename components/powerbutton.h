#ifndef POWERBUTTON_H
#define POWERBUTTON_H

#include <QWidget>

enum State {
    Off,
    On,
    Connecting,
    Error
};

class PowerButton : public QWidget
{
    Q_OBJECT

public:
    explicit PowerButton(QWidget *parent = nullptr);
    void SetState(State state);
    State GetState() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void toggled(bool on);

private:
    bool isOn;
    State state_;
};

#endif // POWERBUTTON_H
