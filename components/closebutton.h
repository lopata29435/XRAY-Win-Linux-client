#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QWidget>

class CloseButton : public QWidget
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) ;
    void enterEvent(QEnterEvent *event) ;
    void leaveEvent(QEvent *event) ;
    void mousePressEvent(QMouseEvent *event) ;

private:
    bool isHovered;
};

#endif // CLOSEBUTTON_H
