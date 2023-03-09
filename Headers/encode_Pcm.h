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

    struct Wav_fmt
    {
        int channels;
        int sample_rate;
        int fmt_size;
        int bits = 16;
    }wav_fmt;
    

    typedef struct WAV_HEAD
    {
        char chunkid[4];
        unsigned long chunksize;
        char format[4];
    };

    typedef struct WAV_FMT
    {
        char subformat[4];
        unsigned long sbusize;
        unsigned short audioFormat;
        unsigned short numchannels;
        unsigned long sampleRate;
        unsigned long byteRate;
        unsigned short blockAlign;
        unsigned short bitPerSample;
    };

    typedef struct WAV_DATA
    {
        char wavdata[4];
        unsigned long datasize;
    };

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
    QString encode_type2 = "WAV";
    long get_size(string path);

private slots:
    void find_pcm();
    int pcm_to_acc();
    int pam_to_wav();
    void select_method();

signals:
    void cmd_show(QString msg);
    void add_iem_encode_(QString inputfile, QString outputfile, QString method, QString channel);
    void show_input(QString input);
};

#endif