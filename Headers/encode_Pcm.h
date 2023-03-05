#ifndef ENCODE__PCM__H
#define ENCODE__PCM__H

#include <iostream>
#include <QObject>
#include <qdebug.h>

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavdevice/avdevice.h"
    #include "libavutil/dict.h"
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
        int channels;
        int sample_rate;
        int channel_layout;
    }encode_fmt;

    int pcm_to_acc();
    void init();

    AVFrame *frame;
    AVCodec *codec_;
    AVPacket *packet;
    AVCodecContext *cnt_code;
    string outfile_path = "../temp";
    string infile_path = "";
    QString encode_type = "AAC";
};

#endif