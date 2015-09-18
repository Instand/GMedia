#ifndef SYSTEMTRAYMENU_H
#define SYSTEMTRAYMENU_H
#include <QtWidgets>

class SystemTrayMenu: public QMenu {
    Q_OBJECT
private:
    QAction* actionOpen;
    QAction* actionPlay;
    QAction* actionPause;
    QAction* actionStop;
    QAction* actionExit;
    QAction* actionShow;
    QMenu* playerMenu;
    QMenu* advanceMenu;
    QAction* aboutMenu;
    QAction* actionMessage;
    QAction* nextSong;
    QAction* previousSong;
public:
    SystemTrayMenu(QWidget* pwgt=0);
    QAction* getActionOpen() const;
    QAction* getActionPlay() const;
    QAction* getActionPause() const;
    QAction* getActionStop() const;
    QAction* getActionShow() const;
    QAction* getAboutMenu() const;
    QAction *getActionMessage() const;  //сообщение о песне
    void retranslateTrayMenu(); //перевести меню на другой язык
    QAction *getNextSong() const;
    QAction *getPreviousSong() const;
    void designWindows();
    void designFusion();
    void designDefault();
};

#endif // SYSTEMTRAYMENU_H
