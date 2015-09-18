#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget* pwgt):QDialog(pwgt, Qt::ToolTip)       //формируем Диалоговое окно
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
    lbl->setText(QObject::tr("GMedia v. 2.7.\nCorporation: JSC 'MNITI'\nAuthor: Arew (c) 2015"));
    btnOk = new QPushButton("&Ok");
    btnOk->setObjectName("btnOk");
    QObject::connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(slotShowHide()));   //нажатие кнопки
    QVBoxLayout* box = new QVBoxLayout;
    box->addWidget(pic_lbl);
    box->addWidget(lbl, 0, Qt::AlignCenter);
    box->addWidget(btnOk, 0, Qt::AlignCenter);      //кнопку по центру
    this->setLayout(box);
    this->setWindowTitle(QObject::tr("About GMedia"));
    this->setWindowIcon(QIcon(":/ringtones"));
    //установим модальность для прерывания приложения
    this->setModal(true);
}

AboutDialog::~AboutDialog()
{
    delete aboutStyleCSS;
}

//перевести About;
void AboutDialog::retranslateAbout()
{
    lbl->setText(tr("GMedia v. 2.7.\nCorporation: JSC 'MNITI'\nAuthor: Arew (c) 2015"));
    this->setWindowTitle(tr("About GMedia"));
}

//убрать дизайн
void AboutDialog::designWindows()
{
    //дизайн
    aboutStyleCSS->setFileName("");
    aboutStyleCSS->open(QFile::ReadOnly);
    //применить стиль
    this->setStyleSheet(QString(aboutStyleCSS->readAll()));
}

//убрать дизайн
void AboutDialog::designFusion()
{
    //дизайн
    aboutStyleCSS->setFileName("");
    aboutStyleCSS->open(QFile::ReadOnly);
    //применить стиль
    this->setStyleSheet(QString(aboutStyleCSS->readAll()));
}

//по умолчанию
void AboutDialog::designDefault()
{
    //дизайн
    aboutStyleCSS->setFileName(":/aboutStyle.css");
    aboutStyleCSS->open(QFile::ReadOnly);
    //применить стиль
    this->setStyleSheet(QString(aboutStyleCSS->readAll()));
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

