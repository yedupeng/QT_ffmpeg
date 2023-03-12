#ifndef __YUV_PROCESSING__H
#define __YUV_PROCESSING__H

#include <iostream>
#include <QObject>
#include <qdebug.h>
#include <QFileDialog>
#include <QTime>
#include <QMediaPlayer>


extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavdevice/avdevice.h"
    #include "libavutil/dict.h"
    #include "libswresample/swresample.h"
    #include "libavutil/pixfmt.h"
    #include <libavutil/imgutils.h>
}

using namespace std;

class yuv_processing : public QObject
{
    Q_OBJECT
public:
    string show_path = "";
    QString currentFileName = " ";


signals:
    void set_window();

private slots:
    void show_yuv();
    void slotOpenFile();

};

#endif