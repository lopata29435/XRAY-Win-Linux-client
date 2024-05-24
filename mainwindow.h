#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "components/powerbutton.h"
#include "components/onanimation.h"
#include "components/connectanimation.h"
#include "components/erroranimation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnPowerButtonToggled(bool on, PowerButton *powerButton);
    void OnButtonClicked(const QString& appDir, PowerButton *powerButton);
    void OffButtonClicked(PowerButton *powerButton);

private:
    void ShowOnAnimation();
    void HideOnAnimation();
    void ShowConnectAnimation();
    void HideConnectAnimation();
    void ShowErrorAnimation();
    void HideErrorAnimation();
    void SetBackgroundColor(const QColor &color);
    QString loadFont(const QString &fontPath);

private:
    Ui::MainWindow *ui;
    QProcess *m_xrayProcess = nullptr;
    OnAnimation *onAnimation = nullptr;
    ConnectAnimation *connectAnimation = nullptr;
    ErrorAnimation *errorAnimation = nullptr;
};

#endif // MAINWINDOW_H
