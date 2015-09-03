#include "widget.h"
#include <QApplication>
#include <QtWidgets>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundPlayer sound;
    //файлик css
    QFile style(":/appStyle.css");
    style.open(QFile::ReadOnly);
    //применить стиль
    a.setStyleSheet(QString(style.readAll()));
    sound.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);            //базовая фомра окна
    sound.setWindowTitle("God's MediaPlayer");  //устаноить верхнюю надпись окна
    //виджет Option
    sound.resize(420, 100);
    sound.show();
    return a.exec();
}
