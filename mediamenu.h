#pragma once

#include <QtWidgets>

class MediaMenu: public QMenu {
    Q_OBJECT

public:
    MediaMenu(QWidget* pwgt=0);
    virtual ~MediaMenu();
    void retranslateMenu();     //перевод меню на другие языки
    virtual void contextMenuEvent(QContextMenuEvent* me);       //переопределяем для своей реализации всплывающего меню
    QAction* getPauseAction() const;
    QAction* getPlayAction() const;
    QAction* getStopAction() const;
    QAction* getRusAction() const;
    QAction* getEngAction() const;
    QMenu* getLangMenu() const;
    QAction* getAboutAction() const;
    QAction* getNextAction() const;
    QAction* getPreviousAction() const;

private:
    QAction* mOpen;     //меню Open
    QAction* playAction;        //меню Play
    QAction* stopAction;        //меню Stop
    QAction* pauseAction;       //меню Pause
    QAction* nextAction;    //следующий трек
    QAction* previousAction;        //предыдущий трек
    QAction* exitAction;    //меню Exit
    QAction* aboutAction;
    QMenu* optionMenu;      //подменю Options
    QMenu* langMenu;    //подменю Language
    QMenu* designMenu;  //подменю Design
    QAction* engAction; //меню English
    QAction* rusAction; //меню Russian
    QAction* deAction;  //меню Germany
    QAction* frAction;  //меню French
    //дизайн
    QAction* defaultAction; //дизайн по умолчанию
    QAction* magicAction;   //Магический дизайн
    QAction* mnitiAction;   //дизайн МНИТИ
    QFile* menuStyleCSS;   //дизайн меню
};


