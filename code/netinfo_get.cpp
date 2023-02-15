#include "../Headers/netinfo_get.h"

void Net_songs::find(QString searchName)
{
    m_listResult.clear();
    QTextCodec *codec_ = QTextCodec::codecForName("utf-8");
    QByteArray bytecodec_ = codec_->fromUnicode(searchName);
    QByteArray byteencodec = bytecodec_.toPercentEncoding();
    QString str_name = QString(byteencodec);

    QNetworkRequest request;
    QString strurl = QString("http://mobilecdn.kugou.com/api/v3/search/song?keyword=%1").arg(str_name);
    request.setUrl(QUrl(strurl));
    manager_getinfo->get(request);
}

void Net_songs::get_song_info(QNetworkReply *reply)
{
    std::cout<<"start get songs"<<std::endl;
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QString result(bytes);
        parseSongInfo(result);
    }
}

void Net_songs::parseSongInfo(QString json)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_document = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_document.isObject())
        {
            QJsonObject object = parse_document.object();
            if(object.contains("data"))
            {
                QJsonValue value_data = object.take("data");
                if(value_data.isObject())
                {
                    QJsonObject object_data = value_data.toObject();
                    if(object_data.contains("info"))
                    {
                        QJsonValue value_info = object_data.take("info");
                        if(value_info.isArray())
                        {
                            QJsonArray array_info = value_info.toArray();
                            int size = array_info.size();
                            for(int i=0; i<size; i++)
                            {
                                QJsonValue value_first = array_info.at(i);
                                if(value_first.isObject())
                                {
                                    QString songname;
                                    QString hash;
                                    QString singername;
                                    QString album_name;
                                    int filesize;
                                    QJsonObject object_first = value_first.toObject();
                                    if(object_first.contains("filename"))
                                    {
                                        QJsonValue filename = object_first.take("filename");
                                        songname = filename.toString();
                                    }
                                    if(object_first.contains("hash"))
                                    {
                                        QJsonValue filehash = object_first.take("hash");
                                        hash = filehash.toString();    
                                    }
                                    if(object_first.contains("singername"))
                                    {
                                        QJsonValue singer = object_first.take("singername");
                                        hash = singer.toString();    
                                    }
                                    if(object_first.contains("album_name"))
                                    {
                                        QJsonValue album = object_first.take("album_name");
                                        album_name = album.toString();    
                                    }
                                    if(object_first.contains("filesize"))
                                    {
                                        QJsonValue value_filesize  = object_first.take("filesize");
                                        filesize = value_filesize.toInt();    
                                    }   
                                    result_list.songName = songname;
                                    result_list.hash = hash;
                                    result_list.filesize = QString("%1").arg(filesize/1000);
                                    result_list.album_name = album_name;
                                    result_list.singername = singername; 
                                    m_listResult.append(result_list);                     
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}