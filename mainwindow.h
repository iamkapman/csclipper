#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QSignalMapper>
#include <QCursor>
#include <QMimeData>
#include <QSettings>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include <settings.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void setClip();
    void setBuff(QString text);
    void showAbout();
    void showSettings();
    void readSettings();
    void saveBuffer();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
#ifdef Q_OS_WIN
    bool winEvent(MSG *message, long *result);
#endif
    Settings *s;
    quint8 count;
    bool save;

private:
    void createActions();
    void createTray();
    void addAct(bool withSep);
    void readList(bool withSep);

    QAction *settingsAction;
    QAction *aboutAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QClipboard *clip;
    QSignalMapper *mapper;
    QList<QString> *list;
};

#endif // MAINWINDOW_H
