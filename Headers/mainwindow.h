#ifndef __MAINWINDOW__H
#define __MAINWINDOW__H

#include "../ui/ffpemg_player.h"
#include <QWidget>
#include <QFileDialog>
#include <QImage>
#include <iostream>
#include <QDir>
#include <QMapIterator>
#include <String.h>
#include <io.h>
#include <QMediaPlayer>
#include <qdebug.h>
#include <QTime>
#include <QtMultimedia/qmediaplaylist.h>
#include <QMenu>
#include <QAction>


extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavdevice/avdevice.h"
    #include "libavutil/dict.h"
}


QT_BEGIN_NAMESPACE
namespace Ui {
    class Ui_Form;
}
QT_END_NAMESPACE

class Mainwindow : public QWidget
{
    Q_OBJECT;
public:
    explicit Mainwindow(QWidget* parent = nullptr);
    ~Mainwindow();
    int model_flag;
    int flag = 1;
    int flag_volum = 1;
    QStringList list_songs;
    // QMenu* menu;

private slots:
    void btn_chage_img();
    void btn_chage_page_local();
    void btn_chage_page_favor();
    void btn_chage_page_my1();
    void btn_chage_page_my2();
    void btn_chage_page_rec();
    void btn_chage_import_songs();
    void btn_chage_import_song();
    void stop();
    void play_all();
    void play_model_change();
    void volum_change();
    void btn_close_volum();
    void change_position(qint64 pos);
    void change_duration(qint64 tim);
    void seekChange(int pos);
    void btn_chage_img2();
    void next_song();
    void last_song();
    void show_meau(const QPoint&);
    void get_songs(QString file);

private:
    // struct findResult
    // {
    //     QString hash;
    //     QString songName;
    //     QString singername;
    //     QString album_name;
    //     QString filesize;
    //     QString playPath;
    //     QString timelength;
    //     QImage image;
    //     QString imagePath;
    // };
    Ui::Form *ui;
    QString File;
    int num = 0;
    AVFormatContext* cnt_avf;
    QMap<QString,QString> q_map;
    QMediaPlayer* mp3_player;
    QMediaPlaylist* playlist;
};

#endif