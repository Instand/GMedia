#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtWidgets>

//всплывающее окно About God's Media
class AboutDialog: public QDialog {
        Q_OBJECT
private:
    QLabel* lbl;        //основное поле текста
    QLabel* pic_lbl;    //для картинки
    bool switcher;  //переключение показа/скрытия окна
    QPushButton* btnOk;
    QFile* aboutStyleCSS;
public:
    AboutDialog(QWidget* pwgt=0);
    virtual ~AboutDialog();
    void retranslateAbout();
public slots:
    void slotShowHide();            //слот показа/скрытия окна
    void slotDesignChange(QAction* );   //слот смены дизайна
};



#endif // ABOUTDIALOG_H
