#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    // Загружаем настройки
    QSettings qset("CraftSoft", "CSClipper");

    qset.beginGroup("MainWindow");
    ui->spinBox->setValue(qset.value("count", "10").toInt());
    ui->checkBox->setChecked(qset.value("save", true).toBool());
    qset.endGroup();

#ifdef Q_OS_WIN
    // Запись в автозагрузку
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(autoRun()));
#else
    ui->pushButton->setDisabled(true);
#endif

    // Сохранение и чтение настроек
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(readSettings()));
}

Settings::~Settings()
{
    delete ui;
}

// Сохранение настроек
void Settings::saveSettings()
{
    QSettings qset("CraftSoft", "CSClipper");
    qset.beginGroup("MainWindow");
    qset.setValue("count", ui->spinBox->value());
    qset.setValue("save", ui->checkBox->isChecked());
    qset.endGroup();
}

#ifdef Q_OS_WIN
// Запись в автозагрузку
void Settings::autoRun()
{
    // Ключ
    const char* pch_regpath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    // Имя параметра
    QString param = "csclipper";
    std::vector<wchar_t> wch_param((param.length() + 1), 0);
    param.toWCharArray(&wch_param[0]);

    //значение параметра
    QString exepath = QDir::toNativeSeparators(QApplication::applicationFilePath() + QApplication::applicationName());
    std::vector<wchar_t> wch_exepath((exepath.length() + 1), 0);
    exepath.toWCharArray(&wch_exepath[0]);

    HKEY k = 0;
    DWORD dwdRes = RegOpenKeyExA(HKEY_CURRENT_USER, pch_regpath, 0, KEY_ALL_ACCESS, &k);
    if(dwdRes == ERROR_SUCCESS)
    {
        RegSetValueExW(k, &wch_param[0], 0, REG_SZ, (BYTE*)&wch_exepath[0], wch_exepath.size() * sizeof(wch_exepath[0]));
        RegCloseKey(k);
        k = 0;
    }
}
#endif
