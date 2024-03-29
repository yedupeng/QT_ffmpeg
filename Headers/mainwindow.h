#ifndef __MAINWINDOW__H
#define __MAINWINDOW__H

#include "../ui/ffpemg_player.h"
#include "lyrices.h"
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
#include "yuv_processing.h"
#include "encode_Pcm.h"
#include <QMenu>
#include <QAction>
#include <qtimer.h>
#include "netinfo_get.h"
#include <QComboBox>
#include <QThread>
#include <QMouseEvent>
#include <QtMultimediaWidgets/qvideowidget.h>

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
    int rad_num = 0;
    QStringList list_songs;
    QMenu* menu;
    QTableWidgetItem* selectedItem;
    QTableWidgetItem* selectedItem2;
    int row_index;
    QTimer *timer2;
    void delay(int i);
    void cmd_show(QString msg);
    QVideoWidget *mediawidge;
    //记录鼠标，窗口位置
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;



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
    void select_action(QAction *action);
    void get_row(int row, int col);
    void update_red();
    void get_search_song();
    void add_table();
    void add_item();
    void play_all_net();
    void show_lc();
    void lyric_show();
    void setting_init();
    void add_item_encode(QString inputfile, QString outputfile, QString method, QString channel);
    void show_settings();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void set_method();
    void show_acc_input(QString input);
    void player_yuv();

private:
    QThread* pThread = new QThread;
    QThread* pThread1 = new QThread;
    Ui_Form *ui;
    QString File;
    int num = 0;
    // AVFormatContext* cnt_avf;
    QMap<QString,QString> q_map;
    QMediaPlayer* mp3_player;
    QMediaPlaylist* playlist;
    QMediaPlaylist* playlist_local;
    // QMediaPlaylist* playlist_net;
    QMenu * m;
    Net_songs* net = new Net_songs;
    LC_classer* lc = new LC_classer;
    encode_pcm* encode_ = new encode_pcm;
    int flag_ln = 0;
    QMediaPlayer *mediaplay;
    yuv_processing* yuv_process = new yuv_processing;
};

#endif