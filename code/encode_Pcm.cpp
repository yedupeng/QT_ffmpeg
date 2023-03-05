#include "../Headers/encode_Pcm.h"

void encode_pcm::init()
{
    encode_fmt.bit_rate = 64000;
    encode_fmt.channels = 2;
    encode_fmt.channel_layout = 0;
    encode_fmt.sample_rate = 44100;
    encode_fmt.fmt = 0;
}

void encode_pcm::find_pcm()
{
    QFileInfo fileinfo;
    pcm_path = QFileDialog::getOpenFileNames(nullptr, QStringLiteral("选择文件"), "", tr("(*.pcm)"));
    if(pcm_path.size()==0)
    {
        emit cmd_show("encode_pcm : cancel find pcm");
        return;
    }
    qDebug()<<pcm_path[0];
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
    char output[] = {};
    for(int i=0;i<pcm_path.size();i++)
    {
        encode_fmt.file_name = file_name[i].toStdString();
        string name = outfile_path+encode_fmt.file_name+".aac";
        cout<<name<<endl;
        strcpy(output,name.c_str());
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
            return -1;
        }
        
        AVFormatContext *oc = NULL;
        avformat_alloc_output_context2(&oc, NULL, NULL, output);
        if(!oc)
        {
            qDebug()<<"create avformatcontext error";
            return -1;
        }
        AVStream *st = avformat_new_stream(oc, NULL);
        st->codecpar->codec_tag = 0;
        avcodec_parameters_from_context(st->codecpar, cnt_code);
        av_dump_format(oc, 0, output, 1);

        avio_open(&oc->pb, output, AVIO_FLAG_WRITE);
    }
}