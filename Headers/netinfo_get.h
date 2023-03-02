#ifndef __NETINFO_GET__H
#define __NETINFO_GET__H

#include <QtNetwork>
#include <QImage>	
#include <iostream>
#include <qtextcodec.h>
#include <QObject>
#include <qeventloop.h>
#include <QtMultimedia/qmediaplaylist.h>
#include <QMediaPlayer>


class Net_songs: public QObject
{
    Q_OBJECT

public:
    QNetworkAccessManager *manager_getinfo;
    QNetworkAccessManager *manager_importinfo;
    struct findResult
    {
        QString hash;
        QString songName;
        QString singername;
        QString album_name;               // 专辑名
        QString filesize;
        QString playPath;
        QString timelength;
        QString image;
        QString imagePath;
        QString album_id;
    }result_list;

    
    QList<QString> m_id;
    findResult info;
    QList<findResult> m_listResult;
    bool flag = true;

    void find(QString searchName);
    void init();
    void get_net_path();
    void parse_songs(QString json);
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    void play_all_net();
    int next_song();
    int last_song();
    QList<QMap<int,QString>> lc_list;
    QMap<int,QString> lc_map;
    void send_lc_singals(qint64 pos);
    void lyrics_net_show(qint64 position);

private slots:
    void get_song_info(QNetworkReply *reply);
    void parseSongInfo(QString json);
    int get_song_url(QNetworkReply *reply);

signals:
    void get_songs_info_over();
    void continue_songs();
    void get_timelength_over();
    void show_lc();
    // void solve_lc(qint64 pos, int row);
};

#endif