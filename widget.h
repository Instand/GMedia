#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <QMediaPlayer>
#include "mediamenu.h"
#include "aboutdialog.h"
#include "systemtraymenu.h"

//основной класс мультимедийного плеера
class SoundPlayer: public QWidget {
    Q_OBJECT
public:
    SoundPlayer(QWidget* pwgt=0);
    virtual ~SoundPlayer();
    void retranslateGUI();
    QString calculateSongSize(qint64 songSize);
protected:
    QMediaPlayer* player;   //основной плеер
    QVBoxLayout* vbox;
    //переопределить события обработки drag & drop
    virtual void dragEnterEvent(QDragEnterEvent * pe);
    virtual void dropEvent(QDropEvent* pe);
    //событие меню
    virtual void contextMenuEvent(QContextMenuEvent* me);
    virtual void changeEvent(QEvent* pe);   //событие смены языка
    //событие при сворачивании приложения
    virtual void closeEvent(QCloseEvent* ce);
private:
    QPushButton* btnPlay;
    QPushButton* btnStop;
    QSlider* slPosition;
    QLabel* timeCurrent;
    QLabel* timeRemain;
    QLabel* fileName;
    QLabel* playerTime;
    QCheckBox* repeatCheck;
    QString msecsToString(int n);
    //кнопка открытия файлов
    QPushButton* btnOpen;
    //ползунок громкости
    QSlider* slrVolume;
    MediaMenu* menu;            //выплывающее меню
    //добавим переводчики
    QTranslator* appTrans;
    QTranslator* qtTrans;
    //строка определения пути файлов перевода
    QString strTransPath;
    //открытие файла для стиля
    QFile* styleCSS;
    //окно about
    AboutDialog* dlg;
    //меню для панели задач
    QSystemTrayIcon* trayIcon;
    SystemTrayMenu* hiddenMenu;
    bool changer;   //контроль сворачивания/открытия
    //файл инфо
    QFileInfo* fileInfo;
    QListWidget* songList;      //компонент для реализации списка песен
    //таймер для отображения времени
    QTimer* currentTime;
    //флаг для времени
    bool showTime;
    QLabel* songSize;   //отображение размера песен
    QListWidgetItem* listSong;
private slots:
    void slotOpen();        //открытие окна файлов
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
    void slotShowAbout();
    void slotHideShow();
    void slotChangePoisition(int);
    //слот отработки времени таймера показа времени
    void slotShowTime();
    void slotShowSongMessage();     //слот показа сообщения о текущей песне
    void slotStartListSong(QListWidgetItem* item);   //проигрывать песню из плей листа
};

#endif // WIDGET_H
