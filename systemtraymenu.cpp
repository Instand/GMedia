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

//перевод меню
void SystemTrayMenu::retranslateTrayMenu()
{
    actionOpen->setText(tr("&Open"));
    //перевод меню
    playerMenu->setTitle(tr("Pla&yer menu"));
    actionPlay->setText(tr("&Play"));
    actionPause->setText(tr("Pa&use"));
    actionStop->setText(tr("&Stop"));
    previousSong->setText(tr("Previous"));
    nextSong->setText(tr("Next"));
    //перевод меню
    advanceMenu->setTitle(tr("Advance"));
    aboutMenu->setText(tr("About GMedia"));
    actionMessage->setText(tr("Song Message"));
    actionShow->setText(tr("Show player"));
    actionExit->setText(tr("&Exit"));
}


QAction *SystemTrayMenu::getNextSong() const
{
    return nextSong;
}

QAction *SystemTrayMenu::getPreviousSong() const
{
    return previousSong;
}

void SystemTrayMenu::designWindows()
{
    QFile file("");
    file.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(file.readAll()));
}

void SystemTrayMenu::designFusion()
{
    QFile file("");
    file.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(file.readAll()));
}

void SystemTrayMenu::designDefault()
{
    QFile file(":/menuStyle.css");
    file.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(file.readAll()));
}

//основная менюшка
SystemTrayMenu::SystemTrayMenu(QWidget* pwgt): QMenu(pwgt)
{
    actionOpen = this->addAction(QObject::tr("&Open"));     //открыть
    this->addSeparator();
    playerMenu = new QMenu(QObject::tr("Pla&yer menu"), this);
    this->addMenu(playerMenu);
    actionPlay = playerMenu->addAction(QObject::tr("&Play"));
    actionPause = playerMenu->addAction(QObject::tr("Pa&use"));
    actionStop = playerMenu->addAction(QObject::tr("&Stop"));
    playerMenu->addSeparator();
    previousSong = playerMenu->addAction(QObject::tr("Previous"));
    nextSong = playerMenu->addAction(QObject::tr("Next"));
    playerMenu->addSeparator();
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


