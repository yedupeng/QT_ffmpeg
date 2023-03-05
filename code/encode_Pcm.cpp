#include "../Headers/encode_Pcm.h"

void encode_pcm::init()
{
    encode_fmt.bit_rate = 64000;
    encode_fmt.channels = 2;
    encode_fmt.channel_layout = 0;
    encode_fmt.sample_rate = 44100;
    encode_fmt.fmt = 0;
}

int encode_pcm::pcm_to_acc()
{
    av_register_all();
    codec_ = avcodec_find_encoder(AV_CODEC_ID_AAC);
    int ret = 0;
    cnt_code = avcodec_alloc_context3(codec_);
    cnt_code->bit_rate = encode_fmt.bit_rate;
    cnt_code->channels = encode_fmt.channels;
    cnt_code->sample_rate = encode_fmt.sample_rate;
    switch (encode_fmt.fmt)
    {
    case 0:
        cnt_code->sample_fmt = AV_SAMPLE_FMT_FLTP;
        break;
    
    default:
        break;
    }

    switch (encode_fmt.channel_layout)
    {
    case 0:
        cnt_code->channel_layout = AV_CH_LAYOUT_STEREO;
        break;
    
    default:
        break;
    } 
    ret = avcodec_open2(cnt_code, codec_, NULL);
    if(ret < 0)
    {
        qDebug()<<"open avcodec error";
    }
    
}