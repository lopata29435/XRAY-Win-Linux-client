#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QWidget>

class CloseButton : public QWidget
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent = nullptr);

signals:
};

#endif // CLOSEBUTTON_H
