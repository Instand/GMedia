#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget* pwgt):QDialog(pwgt, Qt::WindowTitleHint)       //формируем Диалоговое окно
{
    switcher=false;     //по умолчанию скрыто
    lbl = new QLabel;
    pic_lbl = new QLabel;
    //дизайн
    aboutStyleCSS = new QFile(":/aboutStyle.css");
    aboutStyleCSS->open(QFile::ReadOnly);
    //применить стиль
    this->setStyleSheet(QString(aboutStyleCSS->readAll()));
    //установить изображение
    pic_lbl->setPixmap(QPixmap(QApplication::applicationDirPath() + "/add/About.png"));
    lbl->setText(QObject::tr("God's Media v. 1.7.\nAbout author: MNITI, Arew (c) 2015"));
    btnOk = new QPushButton("&Ok");
    btnOk->setObjectName("btnOk");
    QObject::connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(slotShowHide()));   //нажатие кнопки
    QVBoxLayout* box = new QVBoxLayout;
    box->addWidget(pic_lbl);
    box->addWidget(lbl, 0, Qt::AlignCenter);
    box->addWidget(btnOk, 0, Qt::AlignCenter);      //кнопку по центру
    this->setLayout(box);
    this->setWindowTitle("About God's Media");
    this->setWindowIcon(QIcon(":/ringtones"));
}

AboutDialog::~AboutDialog()
{
    delete aboutStyleCSS;
}

//скрытие, показ
void AboutDialog::slotShowHide()
{
    switcher=!switcher;     //всегда меняет значение
    if (switcher) {
        this->show();
    } else this->hide();
}

//ToDo
void AboutDialog::slotDesignChange(QAction *)
{

}

