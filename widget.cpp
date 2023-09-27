#include "widget.h"
#include "ui_widget.h"
#include <QStyle>
#include <QFileDialog>
#include <QDir>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    m_player=new QMediaPlayer(this);
    m_player->setVolume(70);


    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());


    connect(m_player,&QMediaPlayer::positionChanged,this,&Widget::on_position_changed);
    connect(m_player,&QMediaPlayer::durationChanged,this,&Widget::on_duration_changed);

    ////////////////////////////////////// PlayList

    m_playList_model=new QStandardItemModel(this);
    ui->playListView->setModel(m_playList_model);
    m_playList_model->setHorizontalHeaderLabels(QStringList()<<tr("Audio track")<<tr("File Path"));

    ui->playListView->hideColumn(1);
    //ui->playListView->verticalHeader()->setVisible(false);
    ui->playListView->horizontalHeader()->setStretchLastSection(true);

    ui->playListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playListView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playListView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    m_playList =new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playList);
    connect(ui->playListView,&QTableView::doubleClicked,
            [this](const QModelIndex& index){
                     m_playList->setCurrentIndex(index.row());m_player->play();ui->pushButtonPlay->setChecked(true);}
            );
    connect(m_playList, &QMediaPlaylist::currentIndexChanged,
            [this](int index){
            ui->labelComposition->setText(m_playList_model->data(m_playList_model->index(index,0)).toString());}
            );

}

Widget::~Widget()
{
    delete m_player;
    delete ui;
}


void Widget::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(value)));
}


void Widget::on_pushButtonOpen_clicked()
{
//    QString file =QFileDialog::getOpenFileName(this,tr("Open files"),QString(),tr("Audio files (*.mp3)"));
//    m_player->setMedia(QUrl::fromLocalFile(file));
//    ui->labelComposition->setText(file.mid(file.lastIndexOf('/')+1));

    QStringList files =QFileDialog::getOpenFileNames(this,tr("Open files"),
                                                     QString("C:/Users/Фаниль/Downloads"
                                                      "/Linkin Park - 2000 - Hybrid Theory "
                                                      "(20th Anniversary Edition)"),tr("Audio files (*.mp3)"));
    for(QString filePath: files)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        m_playList_model->appendRow(items);
        m_playList->addMedia(QUrl(filePath));
    }
}

void Widget::on_pushButtonStop_clicked()
{
    m_player->stop();
}

void Widget::on_position_changed(qint64 value)
{
    ui->horizontalSliderTrack->setValue(value);
    QTime played =QTime::fromMSecsSinceStartOfDay(value);
    ui->labelPlayed->setText(QString("Played: ").append(played.toString("mm:ss")));
}

void Widget::on_duration_changed(qint64 duration)
{
    ui->horizontalSliderTrack->setMaximum(duration);
    QTime qt_duration =QTime::fromMSecsSinceStartOfDay(duration);
    ui->labelDuration->setText(QString("Duration: ").append(qt_duration.toString("mm:ss")));
}

void Widget::on_horizontalSliderTrack_valueChanged(qint64 position)
{
     m_player->setPosition(position);
}

void Widget::on_horizontalSliderTrack_sliderMoved(int position)
{
     m_player->setPosition(position);
}

void Widget::on_pushButtonMute_clicked(bool checked)
{
        if(checked)
    {
        volum=m_player->volume();
        m_player->setVolume(0);
        ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
        ui->horizontalSliderVolume->setValue(m_player->volume());
    }else{
        m_player->setVolume(volum);
        ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
        ui->horizontalSliderVolume->setValue(m_player->volume());
    }
}


void Widget::on_checkBoxLoop_clicked(bool checked)
{
    if(checked){
        m_playList->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else{
        m_playList->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}


void Widget::on_checkBoxShuffle_clicked(bool checked)
{
    if(checked){
        m_playList->setPlaybackMode(QMediaPlaylist::Random);
    }
    else{
        m_playList->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}


void Widget::on_pushButtonPrev_clicked()
{
    m_playList->previous();
}


void Widget::on_pushButtonNext_clicked()
{
    m_playList->next();
}


void Widget::on_pushButtonPlay_clicked(bool checked)
{
    if(checked){
        m_player->play();
        ui->pushButtonPause->setChecked(false);
    }else{
        m_player->setPosition(0);
        ui->pushButtonPlay->setChecked(true);
    }
}


void Widget::on_pushButtonPause_clicked(bool checked)
{
    if(checked){
        m_player->pause();
        ui->pushButtonPlay->setChecked(false);
    } else{
         m_player->play();
         ui->pushButtonPlay->setChecked(true);
    }
}

