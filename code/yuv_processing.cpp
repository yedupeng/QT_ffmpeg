#include "../Headers/yuv_processing.h"

void yuv_processing::show_yuv()
{
    av_register_all();
    AVFormatContext *cnt_ = avformat_alloc_context();
    int ret = avformat_open_input(&cnt_, (char *)show_path.data(), NULL, NULL);
    if(ret<0)
    {
        qDebug()<<"YUV : open file error";
    }
    AVCodec *codec_ = NULL;
    AVCodecContext *cnt_codec_;
    AVFrame *frame, *frame_rgb;
    AVPacket *pkt;
    int vedio_index = -1;
    struct SwsContext *img_convert_ctx;
    ret = avformat_find_stream_info(cnt_, NULL);
    for(int i=0; i<cnt_->nb_streams; i++)
    {
        if(cnt_->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        cnt_codec_ = cnt_->streams[i]->codec;
    }
    codec_ = avcodec_find_decoder(cnt_codec_->codec_id);
    if(codec_ == NULL)
    {
        qDebug()<<"YUV : get the codec error";
    }
    if(avcodec_open2(cnt_codec_, codec_, NULL) < 0)
    {
        qDebug()<<"YUV : open codec error";
    }
    frame = av_frame_alloc();
    frame_rgb = av_frame_alloc();
    unsigned char* buffer;
    buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, cnt_codec_->width, 
                                cnt_codec_->height, 1));
    av_image_fill_arrays(frame_rgb->data, frame_rgb->linesize, buffer, AV_PIX_FMT_RGB32,
                            cnt_codec_->width, cnt_codec_->height, 1);
    pkt = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_dump_format(cnt_, 0, (char *)show_path.data(), 0);
    img_convert_ctx = sws_getContext(cnt_codec_->width, cnt_codec_->height, cnt_codec_->pix_fmt,
                        cnt_codec_->width, cnt_codec_->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    int flag;
    while(av_read_frame(cnt_, pkt))
    {
        if(pkt->stream_index = vedio_index)
        {
            ret = avcodec_decode_video2(cnt_codec_, frame, &flag, pkt);
            if(ret<0)
            {
                qDebug()<<"YUV : decode_video error";
            }
            if(flag)
            {
                sws_scale(img_convert_ctx, (const unsigned char* const*)frame->data, frame->linesize, 0, cnt_codec_->height,
                            frame_rgb->data, frame_rgb->linesize);
                QImage img((uchar*)frame_rgb->data[0], cnt_codec_->width, cnt_codec_->height, QImage::Format_RGB32)
            }
        }
    }
}