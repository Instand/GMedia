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
public:
    SystemTrayMenu(QWidget* pwgt=0);
    QAction* getActionOpen() const;
    QAction* getActionPlay() const;
    QAction* getActionPause() const;
    QAction* getActionStop() const;
    QAction* getActionShow() const;

public slots:
    //void slotShowMessage();
};

#endif // SYSTEMTRAYMENU_H
