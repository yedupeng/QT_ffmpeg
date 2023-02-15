#ifndef __NETINFO_GET__H
#define __NETINFO_GET__H

#include <QtNetwork>
#include <QImage>	
#include <iostream>
#include <qtextcodec.h>

class Net_songs
{
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
    QList<findResult> m_listResult;

    void find(QString searchName);

private slots:
    void get_song_info(QNetworkReply *reply);
    void parseSongInfo(QString json);
};

#endif