#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDir>
#include <QSettings>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    Ui::Settings *ui;

private slots:
#ifdef Q_OS_WIN
    void autoRun();
#endif
    void saveSettings();
};

#endif // SETTINGS_H
