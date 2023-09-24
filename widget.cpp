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

    m_player=new QMediaPlayer(this);
    m_player->setVolume(70);

    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());


    connect(m_player,&QMediaPlayer::positionChanged,this,&Widget::on_position_changed);
    connect(m_player,&QMediaPlayer::durationChanged,this,&Widget::on_duration_changed);


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
    QString file =QFileDialog::getOpenFileName(this,tr("Open files"),QString(),tr("Audio files (*.mp3)"));
    m_player->setMedia(QUrl::fromLocalFile(file));
    ui->labelComposition->setText(file.mid(file.lastIndexOf('/')+1));
}


void Widget::on_pushButtonPlay_clicked()
{
    m_player->play();

}


void Widget::on_pushButtonPause_clicked()
{
    m_player->pause();
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

void Widget::on_horizontalSliderTrack_valueChanged(int position)
{
     m_player->setPosition(position);
}

void Widget::on_horizontalSliderTrack_sliderMoved(qint64 position)
{
     m_player->setPosition(position);
}

