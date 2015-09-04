#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <QWidget>
#include <QPainter>
#include <QMediaPlayer>

//основной класс мультимедийного плеера
class SoundPlayer: public QWidget {
    Q_OBJECT
public:
    SoundPlayer(QWidget* pwgt=0);
    virtual ~SoundPlayer();
    void retranslateGUI();
protected:
    QMediaPlayer* player;
    QVBoxLayout* vbox;
    //переопределить события обработки drag & drop
    virtual void dragEnterEvent(QDragEnterEvent * pe);
    virtual void dropEvent(QDropEvent* pe);
    //событие меню
    virtual void contextMenuEvent(QContextMenuEvent* me);
    virtual void changeEvent(QEvent* pe);   //событие смены языка
private:
    QPushButton* btnPlay;
    QPushButton* btnStop;
    QSlider* slPosition;
    QLabel* timeCurrent;
    QLabel* timeRemain;
    QLabel* fileName;
    QCheckBox* repeatCheck;
    QString msecsToString(int n);
    QMenu* mainMenu;
private slots:
    void slotOpen();
    void slotPlay();
    void slotSetSliderPos(qint64);
    void slotSetMediaPos(int);
    void slotSetDuration(qint64);
    void slotStatusChanged(QMediaPlayer::State);
    //слот под активацию меню
    void slotMenuActivated(QAction*);
    //слот смены дизайна
    void slotDesignChange(QAction*);
    //слот смены языка
    void slotLanguageChange(QAction* );
};

#endif // WIDGET_H
