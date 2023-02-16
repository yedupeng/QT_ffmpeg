#ifndef __NETINFO_GET__H
#define __NETINFO_GET__H

#include <QtNetwork>
#include <QImage>	
#include <iostream>
#include <qtextcodec.h>
#include <QObject>
#include <qeventloop.h>

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
        QImage image;
        QString imagePath;
    }result_list;
    findResult info;
    QList<findResult> m_listResult;

    void find(QString searchName);
    void init();
private slots:
    void get_song_info(QNetworkReply *reply);
    void parseSongInfo(QString json);

signals:
    void get_songs_info_over();
};

#endif