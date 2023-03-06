#ifndef ENCODE__PCM__H
#define ENCODE__PCM__H

#include <iostream>
#include <QObject>
#include <qdebug.h>
#include <QFileDialog>

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavdevice/avdevice.h"
    #include "libavutil/dict.h"
    #include "libswresample/swresample.h"
}

using namespace std;

class encode_pcm : public QObject
{
    Q_OBJECT
public:
    struct encode_data
    {
        int bit_rate;
        int fmt;
        int in_fmt;
        int channels;
        int sample_rate;
        int in_sample_rate;
        int channel_layout;
        int in_channel_layout;
        string file_name;
    }encode_fmt;

    int method_flag = 0;
    QStringList  pcm_path;
    QStringList  file_name;
    AVFrame *frame;
    AVCodec *codec_;
    AVPacket *packet;
    AVCodecContext *cnt_code;
    string outfile_path = "../temp/";
    string infile_path = "";
    QString encode_type = "AAC";

private slots:
    void find_pcm();
    int pcm_to_acc();
    void select_method();

signals:
    void cmd_show(QString msg);
};

#endif