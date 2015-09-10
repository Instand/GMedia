#include "widget.h"

//конструктор медиа
SoundPlayer::SoundPlayer(QWidget *pwgt): QWidget(pwgt)
{
    this->setFixedSize(420,110);        //установим неизменяющийся размер окна плеера
    this->setWindowTitle("God's Media");  //устаноим верхнюю надпись окна
    this->setWindowIcon(QIcon(":/ringtones"));
    //установить стиль программы
    //файлик css
    styleCSS = new QFile(":/appStyle.css");
    styleCSS->open(QFile::ReadOnly);
    //применить стиль
    this->setStyleSheet(QString(styleCSS->readAll()));

    //строка нахождения переводов
    strTransPath=qApp->applicationDirPath() + "/translation/";

    //создать кнопку Open
    btnOpen = new QPushButton(QObject::tr("Open"));
    slrVolume = new QSlider;       //установить слайдер уровня громкости

    //выделить память под переводчики
    appTrans = new QTranslator;
    qtTrans = new QTranslator;
    //выделим память под about
    dlg = new AboutDialog;

    //выделить память под элементы private полей
    btnPlay = new QPushButton;
    btnStop = new QPushButton;
    slPosition = new QSlider;
    timeCurrent = new QLabel(msecsToString(0));
    timeRemain = new QLabel(msecsToString(0));
    fileName = new QLabel(QObject::tr("None"));
    player = new QMediaPlayer;  //встроенный медиа плейер
    //кнопка Play
    btnPlay->setEnabled(false); //изначально кнопка enabled
    btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));    //отображать иконку
    //кнопка Stop
    btnStop->setEnabled(false);
    btnStop->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));

    slPosition->setRange(0, 0);
    slPosition->setOrientation(Qt::Horizontal);

    slrVolume->setRange(0, 100);
    slrVolume->setValue(50);
    player->setVolume(50);
    //соединяем сигналы и слоты
    QObject::connect(btnOpen, SIGNAL(clicked(bool)), this, SLOT(slotOpen()));
    QObject::connect(btnPlay, SIGNAL(clicked(bool)), this, SLOT(slotPlay()));   //
    QObject::connect(btnStop, SIGNAL(clicked(bool)), player, SLOT(stop()));     //нажатие кнопки стоп -> плейр стоп
    //изменение громкости приводит к изменению громкости проигрывателя
    QObject::connect(slrVolume, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    //движение слайдера приводит к изменению дорожки
    QObject::connect(slPosition, SIGNAL(sliderMoved(int)), this, SLOT(slotSetMediaPos(int)));
    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(slotSetSliderPos(qint64)));
    QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(slotSetDuration(qint64)));
    //отслеживание изменения состояния плейра
    QObject::connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slotStatusChanged(QMediaPlayer::State)));

    //создаем менеджеры компановки
    QHBoxLayout* firstLay = new QHBoxLayout;
    firstLay->addWidget(btnOpen);
    firstLay->addWidget(btnPlay);
    firstLay->addWidget(btnStop);

    QHBoxLayout* secondLay = new QHBoxLayout;
    secondLay->addWidget(timeCurrent);
    secondLay->addWidget(slPosition);
    secondLay->addWidget(timeRemain);

    vbox = new QVBoxLayout;
    vbox->addLayout(firstLay);
    vbox->addLayout(secondLay);

    QHBoxLayout* thirdLay = new QHBoxLayout;
    thirdLay->addLayout(vbox);
    thirdLay->addWidget(slrVolume);

    QVBoxLayout* lastLay = new QVBoxLayout;
    lastLay->addLayout(thirdLay);

    QHBoxLayout* fileNameAndRepeat = new QHBoxLayout;
    fileNameAndRepeat->addWidget(fileName, 0, Qt::AlignLeft);
    //создание кнопки проверки повтора
    repeatCheck = new QCheckBox(QObject::tr("Repeat "));
    fileNameAndRepeat->addWidget(repeatCheck, 0 ,Qt::AlignRight);

    lastLay->addLayout(fileNameAndRepeat);

    this->setLayout(lastLay);

    //создаем меню
    menu = new MediaMenu;
    QObject::connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slotMenuActivated(QAction*)));   //соединить клик меню с слотом активации меню
    //cвязать паузу плеера
    QObject::connect(menu->getPauseAction(), SIGNAL(triggered(bool)), player, SLOT(pause()));
    //связать игру плеера
    QObject::connect(menu->getPlayAction(), SIGNAL(triggered(bool)), player, SLOT(play()));
    //связать стоп плеера
    QObject::connect(menu->getStopAction(), SIGNAL(triggered(bool)), player, SLOT(stop()));
    //связать смену языка
    QObject::connect(menu->getLangMenu(), SIGNAL(triggered(QAction*)), this, SLOT(slotLanguageChange(QAction*)));
    //связано событие появления окна
    QObject::connect(menu->getAboutAction(), SIGNAL(triggered(bool)), this, SLOT(slotShowAbout()));

    //установить переводчки
    qApp->installTranslator(qtTrans);
    qApp->installTranslator(appTrans);

    this->setAcceptDrops(true);     //разрешить перетаскивание данных плееру
}

//чистка памяти
SoundPlayer::~SoundPlayer()
{
    delete player;      //чистим основной плеер
    delete menu;    //чистим меню
    delete dlg;     //чистим about
    delete styleCSS;    //чистим файл дизайна
}

//переводим
void SoundPlayer::retranslateGUI()
{
    fileName->setText(tr("None"));
    repeatCheck->setText(tr("Repeat "));
}

//событие попадания перетаскиваемого файла на виджет
void SoundPlayer::dragEnterEvent(QDragEnterEvent* pe)
{
   //проверка нужных форматов
   if (int i=pe->mimeData()->text().indexOf(".mp3")!= -1 || (i=pe->mimeData()->text().indexOf(".WAV")!=-1)) {
       pe->acceptProposedAction();       //разрешим перетаскивание файла
   } else fileName->setText(QObject::tr("Wrong format"));
}

//если разрешено перетаскивание
void SoundPlayer::dropEvent(QDropEvent* pe)
{
    QString str = pe->mimeData()->text().remove("file:///");        //удаляем лишнее из строки
    player->setMedia(QUrl::fromLocalFile(str)); //загружаем из перетаскиваемого файла путь
    player->play();
    //активировать кнопки
    btnPlay->setEnabled(true);
    btnStop->setEnabled(true);
    if (str.length() > 45) {    //смена длины, при большом размере
        str.insert(45, "\n");
    }
    fileName->setText(str); //отобразить в файл пути
}

//вызов меню
void SoundPlayer::contextMenuEvent(QContextMenuEvent *me)
{
    menu->exec(me->globalPos());    //открывать меню там, где находится указатель мыши
}

//событие смены языка
void SoundPlayer::changeEvent(QEvent *pe)
{
    if (pe->type() == QEvent::LanguageChange) {
        menu->retranslateMenu();
        //ToDo перевод основного интерфейса
    } else QWidget::changeEvent(pe);
}

//переводит милисек в QString
QString SoundPlayer::msecsToString(int n)
{
    int hours = (n / (60*60*1000));   //получение часов
    int minutes = ((n % (60*60*1000))/(60*1000));
    int seconds = ((n % (60*1000))/1000);
    return QTime(hours, minutes, seconds).toString("hh:mm:ss");     //получить формат времени
}

//управление открытием файлов
void SoundPlayer::slotOpen()
{
    int i=0;
    QString file = QFileDialog::getOpenFileName(this, "Open File");     //вызов файл диалога
    //если файл все-таки открыт
    if (!file.isEmpty()) {
        if ((i = file.indexOf(".mp3"))!=-1 || (i= file.indexOf(".WAV"))!=-1) {
            player->setMedia(QUrl::fromLocalFile(file));     //загрузить файл в медиа плейер
            btnPlay->setEnabled(true);
            btnStop->setEnabled(true);
            if (file.length() > 45) {
                file.insert(45, "\n");
            }
            fileName->setText(file);        //показать текущий файл на GUI
            //начать воспроизведение файла сразу
            player->play();
       } else fileName->setText(QObject::tr("Wrong format"));
    }
}

//управление нажатием кнопки Play
void SoundPlayer::slotPlay()
{
    switch(player->state()){
        case (QMediaPlayer::PlayingState):        //приостановить проигрывание если оно уже активно
            player->pause();
            break;
        default:
            player->play();     //включить проигрывание
            break;
    }
}

//установка положения слайдера
void SoundPlayer::slotSetSliderPos(qint64 n)
{
    slPosition->setValue(n);
    //обновить текст
    timeCurrent->setText(msecsToString(n));
    int Duration = slPosition->maximum();
    timeRemain->setText(msecsToString(Duration - n));
    //повторять проигрывание если установлен флажок Repeat
    if (slPosition->value()==Duration) {
        if (repeatCheck->isChecked()) {
            player->play();
        }
    }
}

//движение слайдера контроля времени
void SoundPlayer::slotSetMediaPos(int n)
{
    player->setPosition(n);
}

//получение времени проигрывания
void SoundPlayer::slotSetDuration(qint64 n)
{
    slPosition->setRange(0, n);
    timeCurrent->setText(msecsToString(0));
    timeRemain->setText(msecsToString(n));
}


//реакция на смену состояния плеера
void SoundPlayer::slotStatusChanged(QMediaPlayer::State state)
{
    switch(state) {
        case(QMediaPlayer::PlayingState):
            btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

//нажатие правой кнопкой для меню
void SoundPlayer::slotMenuActivated(QAction* action)
{
    int i=0;
    //если открытие, то открываем файлик
    if ((QString(QObject::tr("Open"))) == action->text().remove("&")) {
        QString file = QFileDialog::getOpenFileName(this, "Open File");     //вызов файл диалога
        //если файл все-таки открыт
        if (!file.isEmpty()) {
            if ((i = file.indexOf(".mp3"))!=-1 || (i= file.indexOf(".WAV"))!=-1) {
                player->setMedia(QUrl::fromLocalFile(file));     //загрузить файл в медиа плейер
                btnPlay->setEnabled(true);
                btnStop->setEnabled(true);
                if (file.length() > 45) {
                    file.insert(45, "\n");
                }
             fileName->setText(file);        //показать текущий файл на GUI
                //начать воспроизведение файла сразу
                player->play();
           } else fileName->setText(QObject::tr("Wrong format"));
        }
    }
}

//отработка смены дизайна
void SoundPlayer::slotDesignChange(QAction* action)
{

}

//смена языка
void SoundPlayer::slotLanguageChange(QAction* action)
{
    //qtTrans->load("qt_" + QLocale::system().name(),
                     //QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTrans);

    if (action->objectName()=="Rus") {
        appTrans->load("qmedia_ru", strTransPath);
        qDebug() << strTransPath;
        qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }

    if (action->objectName()=="Eng") {
       appTrans->load("qmedia_en", strTransPath);
       qDebug() << strTransPath;
       qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }
}


//показать окно About
void SoundPlayer::slotShowAbout()
{
    dlg->slotShowHide();
}
