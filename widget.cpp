#include "widget.h"

//конструктор медиа
SoundPlayer::SoundPlayer(QWidget *pwgt):QWidget(pwgt)
{
    //создать кнопку Open
    QPushButton* btnOpen = new QPushButton("&Open");
    QSlider* slrVolume = new QSlider;       //установить слайдер уровня громкости

    //выделить память под элементы private полей
    btnPlay = new QPushButton;
    btnStop = new QPushButton;
    slPosition = new QSlider;
    timeCurrent = new QLabel(msecsToString(0));
    timeRemain = new QLabel(msecsToString(0));
    fileName = new QLabel("None");
    player = new QMediaPlayer;  //встроенный медиа плейер
    //кнопка Play
    btnPlay->setEnabled(false); //изначально кнопка enabled
    btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));    //отображать иконку
    //кнопка Stop
    btnStop->setEnabled(false);
    btnStop->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));

    slPosition->setRange(0, 0);
    //slPosition->setValue(0);
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
    //firstLay->addWidget(slrVolume);

    QHBoxLayout* secondLay = new QHBoxLayout;
    secondLay->addWidget(timeCurrent);
    secondLay->addWidget(slPosition);
    secondLay->addWidget(timeRemain);

    vbox = new QVBoxLayout;
    vbox->addLayout(firstLay);
    vbox->addLayout(secondLay);
    //vbox->addWidget(fileName);

    QHBoxLayout* thirdLay = new QHBoxLayout;
    thirdLay->addLayout(vbox);
    thirdLay->addWidget(slrVolume);

    QVBoxLayout* lastLay = new QVBoxLayout;
    lastLay->addLayout(thirdLay);
    lastLay->addWidget(fileName, 0, Qt::AlignCenter);

    this->setLayout(lastLay);
}

//чистка памяти
SoundPlayer::~SoundPlayer()
{
    //ToDo
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
    QString file = QFileDialog::getOpenFileName(this, "Open File");     //вызов файл диалога
    //если файл все-таки открыт
    if (!file.isEmpty()) {
        player->setMedia(QUrl::fromLocalFile(file));     //загрузить файл в медиа плейер
        btnPlay->setEnabled(true);
        btnStop->setEnabled(true);
        fileName->setText(file);        //показать текущий файл на GUI
        //начать воспроизведение файла сразу
        player->play();
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
    switch(state){
        case(QMediaPlayer::PlayingState):
            btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPause));
            break;
        default:
            btnPlay->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
