#include "widget.h"

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;     //разрещение

//конструктор медиа
SoundPlayer::SoundPlayer(QWidget *pwgt): QWidget(pwgt)
{
    this->setFixedSize(500, 190);        //установим неизменяющийся размер окна плеера
    this->setWindowTitle("GMedia");  //устаноим верхнюю надпись окна
    this->setWindowIcon(QIcon(QApplication::applicationDirPath() + "/add/ringtones.ico"));
    showTime=false;
    changer = false;    //по умолчанию скрыт
    qt_ntfs_permission_lookup++;                //look up
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
    //выделим память под about
    dlg = new AboutDialog;
    //выделим память под информацию файлов
    fileInfo = new QFileInfo;

    //выделить память под элементы private полей
    btnPlay = new QPushButton;
    btnStop = new QPushButton;
    slPosition = new QSlider;
    timeCurrent = new QLabel(msecsToString(0));
    timeRemain = new QLabel(msecsToString(0));
    fileName = new QLabel;
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

    //по идее сюда можно залить QToolBox
    QHBoxLayout* helpBox = new QHBoxLayout;
    playerTime = new QLabel;
    songSize = new QLabel;
    //для теста и дизайна
    fileName->setMinimumHeight(30);
    repeatCheck->setMinimumHeight(30);
    //-------------------------------------------
    playerTime->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy   hh:mm:ss"));
    helpBox->addWidget(songSize, 0, Qt::AlignLeft);
    helpBox->addWidget(playerTime, 0, Qt::AlignRight);

    QVBoxLayout* theLastLayout = new QVBoxLayout;
    theLastLayout->addLayout(lastLay);
    theLastLayout->addLayout(helpBox);

    songList = new QListWidget;
    //создадим еще один менеджер компоновки для добавлени Списка песен;
    QVBoxLayout* vertBox = new QVBoxLayout;
    vertBox->addLayout(theLastLayout);
    vertBox->addWidget(songList);
    this->setLayout(vertBox);
    listSong=0;
    QObject::connect(songList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotStartListSong(QListWidgetItem*)));
    //соединим событие таймера с слотом показа времени
    currentTime = new QTimer;
    QObject::connect(currentTime, SIGNAL(timeout()), this, SLOT(slotShowTime()));
    //активировать таймер
    currentTime->start(500);

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
    //связка next & previous
    QObject::connect(menu->getNextAction(), SIGNAL(triggered(bool)), this, SLOT(slotNextSong()));
    QObject::connect(menu->getPreviousAction(), SIGNAL(triggered(bool)), this, SLOT(slotPreviousSong()));
    //дизайн
    QObject::connect(menu, SIGNAL(triggered(QAction*)), this , SLOT(slotDesignChange(QAction*)));

    //установить переводчки
    qApp->installTranslator(appTrans);

    this->setAcceptDrops(true);     //разрешить перетаскивание данных плееру

    //реализация сворачивания
    hiddenMenu = new SystemTrayMenu(this);
    //сворачивание
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(hiddenMenu);
    trayIcon->setToolTip("Your GMedia using a hidden mode");
    trayIcon->setIcon(QIcon(QApplication::applicationDirPath() + "/add/ringtones.ico"));
    //связать слот Показа виджета
    QObject::connect(hiddenMenu->getActionShow(), SIGNAL(triggered(bool)), this, SLOT(slotHideShow()));
    //связать нижнее меню
    QObject::connect(hiddenMenu->getActionOpen(), SIGNAL(triggered(bool)), this, SLOT(slotOpen()));
    //связать кнопки скрытого меню с функциями плеера
    //cвязать паузу плеера
    QObject::connect(hiddenMenu->getActionPause(), SIGNAL(triggered(bool)), player, SLOT(pause()));
    //связать игру плеера
    QObject::connect(hiddenMenu->getActionPlay(), SIGNAL(triggered(bool)), player, SLOT(play()));
    //связать стоп плеера
    QObject::connect(hiddenMenu->getActionStop(), SIGNAL(triggered(bool)), player, SLOT(stop()));
    QObject::connect(hiddenMenu->getAboutMenu(), SIGNAL(triggered(bool)), this, SLOT(slotShowAbout()));
    QObject::connect(hiddenMenu->getActionMessage(), SIGNAL(triggered(bool)), this, SLOT(slotShowSongMessage()), Qt::AutoConnection);      //автоматическое соединение
    //дополнительная обработка переключений треков
    QObject::connect(hiddenMenu->getNextSong(), SIGNAL(triggered(bool)), this, SLOT(slotNextSong()));
    QObject::connect(hiddenMenu->getPreviousSong(), SIGNAL(triggered(bool)), this, SLOT(slotPreviousSong()));
}

//чистка памяти
SoundPlayer::~SoundPlayer()
{
    delete player;      //чистим основной плеер
    delete menu;    //чистим меню
    delete dlg;     //чистим about
    delete styleCSS;    //чистим файл дизайна
    delete fileInfo; //чисти инфу по файлам
    if (!searchItem)
        delete searchItem;         //удалим, если пусто
}

//переводим
void SoundPlayer::retranslateGUI()
{
    repeatCheck->setText(tr("Repeat  "));
    btnOpen->setText(tr("Open"));           //основная кнопка
    //перевод основного меню
    menu->retranslateMenu();
    //перевод скрытого меню
    hiddenMenu->retranslateTrayMenu();
    dlg->retranslateAbout();
}

//расчет размера песни в Mb
QString SoundPlayer::calculateSongSize(qint64 songSize)
{
    double size = songSize/1048576.00;
    return "Size: " + QString().setNum(size, 'g', 3) + " " + "Mb";
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
    QString str = pe->mimeData()->text();
    fileInfo->setFile(str); //файл для информации
    player->setMedia(QUrl::fromLocalFile(str)); //загружаем из перетаскиваемого файла путь
    player->play();
    //активировать кнопки
    btnPlay->setEnabled(true);
    btnStop->setEnabled(true);
    str=fileInfo->fileName();       //копируем только имя файла
    if (str.length() > 65) {    //смена длины, при большом размере
        str.insert(65, "\n");
    }
    fileName->setText(str); //отобразить в файл пути
    songSize->setText(calculateSongSize(fileInfo->size()));       //расчет и вывод размера файла
    //если нет в списке, то добавляем
    if (checkListItem()) {
        listSong = new QListWidgetItem(fileInfo->filePath(), songList);
        listSong->setIcon(QIcon(QApplication::applicationDirPath() + "/add/ringtones.ico"));
        listSong->setSelected(true);
    } else fileName->setText(QObject::tr("File currently in song list"));
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
        this->retranslateGUI();
        //ToDo перевод основного интерфейса
    } else QWidget::changeEvent(pe);
}

//событие закрытия основного окна
void SoundPlayer::closeEvent(QCloseEvent *ce)
{
    trayIcon->show();
    this->hide();
    trayIcon->setToolTip(fileInfo->fileName());     //текущая композиция
    //то не выводим сообщение
    if (!fileName->text().isEmpty()) {
        trayIcon->showMessage("GMedia", fileInfo->baseName(), QSystemTrayIcon::Information, 100);
    }
}

//событие нажатия кнопки
void SoundPlayer::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key()==Qt::Key_Delete && songList->selectedItems().back()->isSelected()) {
        QListWidgetItem* item = songList->selectedItems().back();
        delete item;    //удаляем песню из списка
        player->stop();
        fileName->clear();
        trayIcon->setToolTip("");
    }
}

//переводит милисек в QString
QString SoundPlayer::msecsToString(int n)
{
    int hours = (n / (60*60*1000));   //получение часов
    int minutes = ((n % (60*60*1000))/(60*1000));
    int seconds = ((n % (60*1000))/1000);
    return QTime(hours, minutes, seconds).toString("hh:mm:ss");     //получить формат времени
}

//переход, при возможности, на следущую песню из плей листа/либо на первую, если это последняя песня
void SoundPlayer::nextSong()
{
    int i=0;
    player->stop();
    searchItem = songList->findItems(fileName->text(), Qt::MatchContains).back();    //ищем текущую песню
    i = songList->row(searchItem);  //получим строку элемента
    //смотрим последний это элемент или нет
    if (i == songList->count()-1) {
        searchItem = songList->item(0);
        player->setMedia(QUrl::fromLocalFile(searchItem->text()));
        fileInfo->setFile(searchItem->text());
        fileName->setText(fileInfo->fileName());
        songSize->setText(calculateSongSize(fileInfo->size()));
        searchItem->setSelected(true);
        //погнали
        player->play();
        //смотрим, где наш виджет
        if (trayIcon->isVisible()) {
            trayIcon->setToolTip(fileName->text());
        }
    } else {
        searchItem = songList->item(++i);
        searchItem->setSelected(true);
        player->setMedia(QUrl::fromLocalFile(searchItem->text()));
        fileInfo->setFile(searchItem->text());
        fileName->setText(fileInfo->fileName());
        songSize->setText(calculateSongSize(fileInfo->size()));
        //погнали
        player->play();

        if (trayIcon->isVisible()) {
            trayIcon->setToolTip(fileName->text());
        }
    }
}

//отработка перехода указателя назад
void SoundPlayer::previousSong()
{
    int i=0;
    player->stop();
    searchItem = songList->findItems(fileName->text(), Qt::MatchContains).back();    //ищем текущую песню
    i = songList->row(searchItem);  //получим строку элемента
    //если это первый элемент, то переходим к последнему элементу
    if (i==0) {
        searchItem = songList->item(songList->count()-1);   //ищем последний элемент
        player->setMedia(QUrl::fromLocalFile(searchItem->text()));
        fileInfo->setFile(searchItem->text());
        fileName->setText(fileInfo->fileName());
        songSize->setText(calculateSongSize(fileInfo->size()));
        searchItem->setSelected(true);
        //погнали
        player->play();
        //смотрим, где наш виджет
        if (trayIcon->isVisible()) {
            trayIcon->setToolTip(fileName->text());
        }
    } else {
        searchItem = songList->item(--i);
        searchItem->setSelected(true);
        player->setMedia(QUrl::fromLocalFile(searchItem->text()));
        fileInfo->setFile(searchItem->text());
        fileName->setText(fileInfo->fileName());
        songSize->setText(calculateSongSize(fileInfo->size()));
        //погнали
        player->play();

        if (trayIcon->isVisible()) {
            trayIcon->setToolTip(fileName->text());
        }
    }
}

//проверка наличия трека в листе
bool SoundPlayer::checkListItem()
{
    bool ch = false;    //есть в списке данный трек
    if (songList->findItems(fileInfo->filePath(), Qt::MatchContains).count() == 0)
        return ch=true; //списка нет, можно добавлять
}

//управление открытием файлов
void SoundPlayer::slotOpen()
{
    int i=0;
    QString file = QFileDialog::getOpenFileName(this, "Open File");     //вызов файл диалога
    //если файл все-таки открыт
    if (!file.isEmpty()) {
        if ((i = file.indexOf(".mp3"))!=-1 || (i= file.indexOf(".WAV"))!=-1) {
            fileInfo->setFile(file);
            player->setMedia(QUrl::fromLocalFile(file));     //загрузить файл в медиа плейер
            btnPlay->setEnabled(true);
            btnStop->setEnabled(true);
            file = fileInfo->fileName();     //нужно только имя файла
            if (file.length() > 65) {
                file.insert(65, "\n");
            }
            fileName->setText(file);        //показать текущий файл на GUI
            songSize->setText(calculateSongSize(fileInfo->size()));       //расчет и вывод размера файла

            if (checkListItem()) {
                listSong = new QListWidgetItem(fileInfo->filePath(), songList);
                listSong->setIcon(QIcon(QApplication::applicationDirPath() + "/add/ringtones.ico"));
                listSong->setSelected(true);
            } else fileName->setText(QObject::tr("File currently in song list"));

            //начать воспроизведение файла сразу
            player->play();
       } else {
            fileName->setText(QObject::tr("Wrong format"));
            songSize->setText("");       //расчет и вывод размера файла
        }
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
            if (trayIcon->isVisible()) {
                trayIcon->showMessage(fileInfo->baseName(), QObject::tr("Repeated"), QSystemTrayIcon::Information, 0);
            }
        }
        //если есть доступные песни в списке, то запускаем следующую песню
        else if (songList->count() > 1) {
            //вызов функции
            nextSong();
            if (trayIcon->isVisible()) {
                trayIcon->showMessage(fileInfo->baseName(), QObject::tr("Next song"), QSystemTrayIcon::Information, 0);
            }
        }
    }
}

//движение слайдера контроля времени
void SoundPlayer::slotSetMediaPos(int n)
{
    player->stop();         //немного алгоритма предотвращения искажения звука
    player->setPosition(n);
    player->play();
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
    trayIcon->setToolTip(fileInfo->baseName());
}

//нажатие правой кнопкой для меню
void SoundPlayer::slotMenuActivated(QAction* action)
{
    int i=0;
    //если открытие, то открываем файлик
    if (action->objectName() == "Open") {
        QString file = QFileDialog::getOpenFileName(this, "Open File");     //вызов файл диалога
        //если файл все-таки открыт
        if (!file.isEmpty()) {
            if ((i = file.indexOf(".mp3"))!=-1 || (i= file.indexOf(".WAV"))!=-1) {
                player->setMedia(QUrl::fromLocalFile(file));     //загрузить файл в медиа плейер
                fileInfo->setFile(file);
                btnPlay->setEnabled(true);
                btnStop->setEnabled(true);
                file = fileInfo->fileName();
                if (file.length() > 65) {
                    file.insert(65, "\n");
                }
             fileName->setText(file);        //показать текущий файл на GUI
             songSize->setText(calculateSongSize(fileInfo->size()));       //расчет и вывод размера файла
             //проверка наличия в листе файла
             if (checkListItem()) {
                 listSong = new QListWidgetItem(fileInfo->filePath(), songList);
                 listSong->setIcon(QIcon(QApplication::applicationDirPath() + "/add/ringtones.ico"));
                 listSong->setSelected(true);
             } else fileName->setText(QObject::tr("File currently in song list"));
                //начать воспроизведение файла сразу
                player->play();
           } else {
                fileName->setText(QObject::tr("Wrong format"));
                songSize->setText("");       //расчет и вывод размера файла
            }
        }
    }
}

//отработка смены дизайна
void SoundPlayer::slotDesignChange(QAction* action)
{
    if (action->objectName() == "Magic") {
        //файлик css
        styleCSS->setFileName("");
        styleCSS->open(QFile::ReadOnly);
        //применить стиль
        this->setStyleSheet(QString(styleCSS->readAll()));
        menu->designFusionMenu();
        dlg->designFusion();
        hiddenMenu->designFusion();
        //запуск стиля Fusion
        qApp->setStyle("Fusion");
    }

    if (action->objectName() == "Default") {
        //файлик css
        styleCSS->setFileName(":/appStyle.css");
        styleCSS->open(QFile::ReadOnly);
        //применить стиль
        menu->designDefaultMenu();
        dlg->designDefault();
        hiddenMenu->designDefault();
        this->setStyleSheet(QString(styleCSS->readAll()));
    }

    if (action->objectName() == "Windows") {
        //файлик css
        styleCSS->setFileName("");
        styleCSS->open(QFile::ReadOnly);
        //применить стиль
        this->setStyleSheet(QString(styleCSS->readAll()));
        menu->designWindowsMenu();
        dlg->designWindows();
        hiddenMenu->designWindows();
        qApp->setStyle("WindowsVista");
    }
}

//смена языка
void SoundPlayer::slotLanguageChange(QAction* action)
{

    if (action->objectName()=="Rus") {
        appTrans->load("gmedia_ru", strTransPath);
        qDebug() << strTransPath;
        qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }

    if (action->objectName()=="Eng") {
       appTrans->load("gmedia_en", strTransPath);
       qDebug() << strTransPath;
       qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }

    if (action->objectName()=="De") {
        appTrans->load("gmedia_de", strTransPath);
        qDebug() << strTransPath;
        qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }

    if (action->objectName()=="Fr") {
        appTrans->load("gmedia_fr", strTransPath);
        qDebug() << strTransPath;
        qApp->installTranslator(appTrans); //загрузим в приложение транслятор
    }
}


//показать окно About
void SoundPlayer::slotShowAbout()
{
    dlg->slotShowHide();
}

//показать/свернуть в панель задач и обратно
void SoundPlayer::slotHideShow()
{
    this->show();
    trayIcon->hide();
}

//для тестов
void SoundPlayer::slotChangePoisition(int pos)
{
    slPosition->setValue(pos);
    player->setPosition(pos);
}

//показ времени
void SoundPlayer::slotShowTime()
{
    showTime=!showTime;
    if (showTime) {
        playerTime->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy   hh:mm ss"));
    } else playerTime->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy   hh:mm:ss"));
}

//показать в скрытом меню сообщение о текущей песне
void SoundPlayer::slotShowSongMessage()
{
    trayIcon->showMessage("GMedia", fileInfo->baseName(), QSystemTrayIcon::Information, 1);
}

//проигрывать песню из плей листа
void SoundPlayer::slotStartListSong(QListWidgetItem* item)
{
    player->stop(); //остановим плеер
    player->setMedia(QUrl::fromLocalFile(item->text()));    //новые данные из плей листа
    player->play(); //начать проигрывание
    //обновляем интерфейс
    fileInfo->setFile(item->text());
    fileName->setText(fileInfo->fileName());
    songSize->setText(calculateSongSize(fileInfo->size()));     //размер
}

void SoundPlayer::slotNextSong()
{
    nextSong();
}

void SoundPlayer::slotPreviousSong()
{
    previousSong();
}
