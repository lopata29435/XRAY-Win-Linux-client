#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "components/powerbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnPowerButtonToggled(bool on, PowerButton *powerButton);
private:
    Ui::MainWindow *ui;
    QProcess *m_xrayProcess;
    void OnButtonClicked(const QString& appDir, PowerButton *powerButton);
    void OffButtonClicked(PowerButton *powerButton);
};
#endif // MAINWINDOW_H
