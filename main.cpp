#include "widget.h"
#include <QApplication>
#include <QtWidgets>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundPlayer sound;

    sound.setWindowFlags(Qt::Window| Qt::WindowShadeButtonHint);   //для debuga

    sound.show();
    return a.exec();
}
