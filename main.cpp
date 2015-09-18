#include "widget.h"
#include <QApplication>
#include <QtWidgets>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundPlayer sound;
    QApplication::setQuitOnLastWindowClosed(false);  //чтобы приложение сворачивалось в SystemTray
    sound.show();
    return a.exec();
}
