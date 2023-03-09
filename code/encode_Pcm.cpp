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
        emit show_input(pcm_path[i]);
    }
}

void encode_pcm::select_method()
{
    switch (method_flag)
    {
        case 0:
            cmd_show("encode_pcm : select AAC encoding");
            pcm_to_acc();
            break;

        case 1:
            cmd_show("encode_pcm : select WAV encoding");
            pam_to_wav();
            break;

        default:
            break;
    }
}

long encode_pcm::get_size(string path)
{
    char *input;
    input = (char *)path.data();
    FILE* fp = fopen(input, "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

int encode_pcm::pam_to_wav()
{
    WAV_HEAD header;
    WAV_FMT fmt;
    WAV_DATA wav_data;
    // int bits = 16;
    for(int i=0; i<pcm_path.size(); i++)
    {
        FILE *fp, *fout;
        char *output;
        encode_fmt.file_name = file_name[i].toStdString();  
        string name = outfile_path+encode_fmt.file_name+".wav";
        output = (char*)name.data();
        string input = pcm_path[i].toStdString();
        fp = fopen((char*)input.data(), "rb");
        if(fp == NULL)
        {
            emit cmd_show("WAV : open input file error");
            return -1;
        }
        memcpy(header.chunkid, "RIFF", strlen("RIFF"));
        long filesize = 44 + get_size(input) - 8;
        if(filesize)
        {
            emit cmd_show("WAV : get filesize:"+QString::number (filesize));
        }
        header.chunksize = filesize;
        memcpy(header.format, "WAVE", strlen("WAVE"));
        memcpy(fmt.subformat, "fmt ", strlen("fmt "));
        fmt.sbusize = wav_fmt.fmt_size;
        fmt.audioFormat = 1;
        fmt.numchannels = wav_fmt.channels;
        fmt.sampleRate = wav_fmt.sample_rate;
        fmt.byteRate = fmt.sampleRate * fmt.numchannels * wav_fmt.bits / 8; 

        fmt.blockAlign = fmt.numchannels * wav_fmt.bits / 8;
        fmt.bitPerSample = wav_fmt.bits;

        memcpy(wav_data.wavdata, "data", strlen("data"));
        wav_data.datasize = get_size(input);

        fout = fopen(output, "wb");
        if(!fout)
        {
            emit cmd_show("open output file error");
        }
        fwrite(&header, sizeof(header), 1, fout);
        fwrite(&fmt, sizeof(fmt), 1, fout);
        fwrite(&wav_data, sizeof(wav_data), 1, fout);
        char *buffer = (char*)malloc(512);
        int len;
        while(len = fread(buffer, sizeof(char), 512, fp))
        {
            fwrite(buffer, sizeof(char), len, fout);
        }
        emit cmd_show("WAV : finish");
        emit add_iem_encode_(QString(QLatin1String((char*)input.data())), QString(QLatin1String(output)), 
            "WAV",  QString::number(wav_fmt.channels));
        free(buffer);
        fclose(fp);
        fclose(fout);
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
        emit cmd_show("AAC : finish");
        emit add_iem_encode_(QString(QLatin1String(input)), QString(QLatin1String(output)), 
            "AAC",  QString::number(encode_fmt.channels));
        delete[] pcm;
        pcm = NULL;
        av_write_trailer(oc);

        avio_close(oc->pb);
        avcodec_close(cnt_code);
        avcodec_free_context(&cnt_code);
        avformat_free_context(oc);
    }
}