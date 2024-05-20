#ifndef POWERBUTTON_H
#define POWERBUTTON_H

#include <QWidget>

class PowerButton : public QWidget
{
    Q_OBJECT

public:
    explicit PowerButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void toggled(bool on);

private:
    bool isOn;
};

#endif // POWERBUTTON_H
