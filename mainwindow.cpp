#include "mainwindow.h"

#define HOTKEYPRESS 0x000001

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

#ifdef Q_OS_WIN
    // Регистрируем глобальные клавиши
    RegisterHotKey(winId(), HOTKEYPRESS, MOD_CONTROL, 0x54);
#endif

    s = new Settings(this);

    // Список буферов
    list = new QList<QString>();

    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString &)), this, SLOT(setBuff(const QString &)));

    // Добавляем иконку в трей
    createTray();

    // Буфер обмена
    clip = QApplication::clipboard();
    connect(clip, SIGNAL(dataChanged()), this, SLOT(setClip()));

    // Читаем настройки
    readSettings();
    if(list->size()) {
        readList(true);
    }
}

MainWindow::~MainWindow()
{

#ifdef Q_OS_WIN
    UnregisterHotKey(winId(), HOTKEYPRESS);
#endif

    saveBuffer();
}

// Создание иконки в трее
void MainWindow::createTray()
{
    trayIconMenu = new QMenu();

    // Стандартные экшены
    createActions();

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    trayIcon->setIcon(QIcon(":/clip.ico"));
    trayIcon->show();
}

// Чтение настроек
void MainWindow::readSettings()
{
    QSettings qset("CraftSoft", "CSClipper");

    qset.beginGroup("MainWindow");
    count = qset.value("count", "10").toUInt();
    save = qset.value("save", true).toBool();
    if(!list->size()) {
        list->append(qset.value("buffer", QStringList()).toStringList());
    }
    qset.endGroup();
}

// Читаем список буферов
void MainWindow::readList(bool withSep = false)
{
    trayIconMenu->clear();

    for(int i = 0; i < list->size(); i++) {

        QAction *act = trayIconMenu->addAction(list->at(i).trimmed().left(25), mapper, SLOT(map()));
        mapper->setMapping(act, list->at(i));
    }

    addAct(withSep);
}

// Сохраняем список буферов
void MainWindow::saveBuffer()
{
    QSettings qset("CraftSoft", "CSClipper");

    qset.beginGroup("MainWindow");
    if(save && list->size()) {
        qset.setValue("buffer", QStringList(*list));
    }
    else {
        qset.setValue("buffer", QStringList());
    }
    qset.endGroup();
}

#ifdef Q_OS_WIN
// Обрабатываем горячие клавиши
bool MainWindow::winEvent(MSG *message, long *result) {

    if(message->message == WM_HOTKEY)
    {
        if(message->wParam == (unsigned)HOTKEYPRESS)
        {
            trayIconMenu->exec(QCursor::pos());
        }
        return true;
    }

    return false;
}
#endif

// Обрабатываем изменение буфера
void MainWindow::setClip() {

    // Если текст
    if(clip->mimeData()->hasText()) {

        // Если нет в списке
        if(list->contains(clip->mimeData()->text())) {
            return;
        }

        // Добавляем в начало списка
        list->prepend(clip->mimeData()->text());

        // Если список большой, удаляем
        if(list->size() > count) {
            QObject *act = mapper->mapping(list->last());
            mapper->removeMappings(act);
            list->removeLast();
            delete act;
        }

        readList(true);
    }
}

// Изменение текста в буфере
void MainWindow::setBuff(QString text) {
    clip->setText(text);
}

// Создание стандартных экшенов
void MainWindow::createActions() {
    settingsAction = new QAction(QString::fromUtf8("Настройки"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

    aboutAction = new QAction(QString::fromUtf8("О программе"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    quitAction = new QAction(QString::fromUtf8("Выход"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    addAct(false);
}

// Добавление стандартных экшенов в контекстное меню
void MainWindow::addAct(bool withSep = false)
{
    if(withSep) {
        trayIconMenu->addSeparator();
    }

    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);
}

// Информация о программе
void MainWindow::showAbout() {

    QString msg = QString::fromUtf8("Автор: Евланов Александр (me@kapman.ru)");

    trayIcon->showMessage(QString::fromUtf8("CS:Clipper"), msg);
}

// Настройки
void MainWindow::showSettings()
{
    s->show();
}
