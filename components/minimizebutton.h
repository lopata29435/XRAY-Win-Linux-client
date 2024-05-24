#ifndef MINIMIZEBUTTON_H
#define MINIMIZEBUTTON_H

#include <QWidget>

class MinimizeButton : public QWidget
{
    Q_OBJECT
public:
    explicit MinimizeButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) ;
    void enterEvent(QEnterEvent *event) ;
    void leaveEvent(QEvent *event) ;
    void mousePressEvent(QMouseEvent *event) ;

private:
    bool isHovered;
};

#endif // MINIMIZEBUTTON_H
