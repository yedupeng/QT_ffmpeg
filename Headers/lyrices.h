#ifndef __LYRICES__H
#define __LYRICES__H


#include <QObject>
#include "netinfo_get.h"

class LC_classer: public QObject
{
    Q_OBJECT
public:
    struct ly_c
    {
        QString lyc1;
        QString lyc2;
        QString lyc3;
        QString lyc4;
        QString lyc5;
    }choose_ly_c;

    QList<QMap<int,QString>> lc_list;
    QMap<int,QString> lc_map;

    void lyrics_net_show();
    qint64 pos;
    int row = 0;

signals:
    void show_lc();
};

#endif