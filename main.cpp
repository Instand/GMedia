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
    //sound.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);            //базовая фомра окна

    sound.setWindowFlags(Qt::Window| Qt::WindowShadeButtonHint | Qt::WindowDoesNotAcceptFocus);   //для debuga

    sound.setWindowTitle("God's Media");  //устаноить верхнюю надпись окна
    //виджет Option
    sound.setFixedSize(420,100);
    sound.show();
    return a.exec();
}
