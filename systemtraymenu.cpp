#include "systemtraymenu.h"

//чутка Геттеров
QAction *SystemTrayMenu::getActionPlay() const
{
    return actionPlay;
}

QAction *SystemTrayMenu::getActionPause() const
{
    return actionPause;
}

QAction *SystemTrayMenu::getActionStop() const
{
    return actionStop;
}

QAction *SystemTrayMenu::getActionOpen() const
{
    return actionOpen;
}

//инициализация

QAction *SystemTrayMenu::getActionShow() const
{
    return actionShow;
}


QAction *SystemTrayMenu::getAboutMenu() const
{
    return aboutMenu;
}

QAction *SystemTrayMenu::getActionMessage() const
{
    return actionMessage;
}
SystemTrayMenu::SystemTrayMenu(QWidget* pwgt):QMenu(pwgt)
{
    actionOpen = this->addAction(QObject::tr("&Open"));     //открыть
    this->addSeparator();
    playerMenu = new QMenu(QObject::tr("Pla&yer menu"), this);
    this->addMenu(playerMenu);
    actionPlay = playerMenu->addAction(QObject::tr("&Play"));
    actionPause = playerMenu->addAction(QObject::tr("Pa&use"));
    actionStop = playerMenu->addAction(QObject::tr("&Stop"));
    advanceMenu = new QMenu(QObject::tr("Advance"), this);
    aboutMenu = advanceMenu->addAction(QObject::tr("About GMedia"));
    actionMessage = advanceMenu->addAction(QObject::tr("Song Message"));        //вывод в систем трей сообщение о песне
    this->addMenu(advanceMenu);
    actionShow = this->addAction(QObject::tr("Show player"));
    this->addSeparator();
    actionExit = this->addAction(QObject::tr("&Exit"));     //выход
    QObject::connect(actionExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));     //щелчек меню на выход
    QFile file(":/menuStyle.css");
    file.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(file.readAll()));
}


