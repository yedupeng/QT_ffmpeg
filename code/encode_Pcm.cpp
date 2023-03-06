#include "../Headers/encode_Pcm.h"

void encode_pcm::find_pcm()
{
    QFileInfo fileinfo;
    pcm_path = QFileDialog::getOpenFileNames(nullptr, QStringLiteral("选择文件"), "", tr("(*.pcm)"));
    if(pcm_path.size()==0)
    {
        emit cmd_show("encode_pcm : cancel find pcm");
        return;
    }
    for(int i=0;i<pcm_path.size();i++)
    {
        fileinfo = QFileInfo(pcm_path[i]);
        file_name.append(fileinfo.baseName());
        qDebug()<<file_name[i];
        cmd_show("encode_pcm : find "+fileinfo.baseName());
    }
}

int encode_pcm::pcm_to_acc()
{
    av_register_all();
    char *output;
    char *input;
    for(int i=0;i<pcm_path.size();i++)
    {
        infile_path = pcm_path[i].toStdString();
        input = (char*)infile_path.data();
        encode_fmt.file_name = file_name[i].toStdString();
        string name = outfile_path+encode_fmt.file_name+".aac";
        output = (char*)name.data();
        cout<<"output:"<<output;
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
        cnt_code->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        ret = avcodec_open2(cnt_code, codec_, NULL);
        if(ret < 0)
        {
            emit cmd_show("AAC : open avcodec error");
            return -1;
        }
        
        AVFormatContext *oc = NULL;
        avformat_alloc_output_context2(&oc, NULL, NULL, output);
        if(!oc)
        {
            emit cmd_show("AAC : create avformatcontext error");
            return -1;
        }
        AVStream *st = avformat_new_stream(oc, NULL);
        st->codecpar->codec_tag = 0;
        avcodec_parameters_from_context(st->codecpar, cnt_code);
        av_dump_format(oc, 0, output, 1);

        avio_open(&oc->pb, output, AVIO_FLAG_WRITE);
        avformat_write_header(oc, NULL);
        SwrContext *swr = NULL;
        AVSampleFormat format;
        switch (encode_fmt.in_fmt)
        {
        case 0:
            format = AV_SAMPLE_FMT_S16;
            break;
        
        default:
            break;
        }

        int64_t layout;
        switch (encode_fmt.in_channel_layout)
        {
        case 0:
            layout = AV_CH_LAYOUT_STEREO;
            break;
        
        default:
            break;
        } 

        swr = swr_alloc_set_opts(swr, cnt_code->channel_layout, cnt_code->sample_fmt, cnt_code->sample_rate,
                                    layout, format, encode_fmt.in_sample_rate, 0, 0);
        if(!swr)
        {
            emit cmd_show("AAC : create SwrContext error");
            return -1;  
        }
        ret = swr_init(swr);
        if(ret<0)
        {
            emit cmd_show("AAC : create swr_init error");
            return -1;  
        }

        frame = av_frame_alloc();
        frame->format = cnt_code->sample_fmt;
        frame->channel_layout = cnt_code->channel_layout;
        frame->channels = cnt_code->channels;
        frame->nb_samples = 1024;

        ret = av_frame_get_buffer(frame, 0);
        if(ret<0)
        {
            emit cmd_show("AAC : create frame get buffer error");
            return -1;
        }

        int readSize = frame->nb_samples * 2 * 2;
        char* pcm = new char[readSize];
        FILE *fp = fopen(input, "rb");
        for(;;)
        {
            ret = fread(pcm, 1, readSize, fp);
            if(ret<=0)
            {
                break;
            }
            const uint8_t* data[1];
            data[0] = (uint8_t*)pcm;
            int len = swr_convert(swr, frame->data, frame->nb_samples, data, frame->nb_samples);
            if(ret<0)
            {
                emit cmd_show("AAC : swr_convert error");
                break;
            }

            AVPacket pkt;
            av_init_packet(&pkt);
            ret = avcodec_send_frame(cnt_code, frame);
            if(ret != 0)
            {
                continue;
            }
            ret = avcodec_receive_packet(cnt_code, &pkt);
            if(ret != 0)
            {
                continue;
            }
            pkt.stream_index = 0;
            pkt.dts = 0;
            pkt.pts = 0;
            av_interleaved_write_frame(oc, &pkt);
        }
        delete[] pcm;
        pcm = NULL;
        av_write_trailer(oc);

        avio_close(oc->pb);
        avcodec_close(cnt_code);
        avcodec_free_context(&cnt_code);
        avformat_free_context(oc);
    }
}