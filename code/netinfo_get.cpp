#include "../Headers/netinfo_get.h"


void Net_songs::init()
{
    manager_getinfo = new QNetworkAccessManager;
    manager_importinfo = new QNetworkAccessManager;
    manager_importinfo->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    // connect(manager_importinfo, SIGNAL(finished(QNetworkReply*)),this, SLOT(get_song_url(QNetworkReply*)));
}

void Net_songs::find(QString searchName)
{
    m_listResult.clear();
    QTextCodec *codec_ = QTextCodec::codecForName("utf-8");
    QString strInput = searchName;
    QByteArray bytecodec_ = codec_->fromUnicode(strInput);
    QByteArray byteencodec = bytecodec_.toPercentEncoding();
    QString str_name = QString(byteencodec);
    QNetworkReply* answer;
    QNetworkRequest request;
    QString strurl = QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json"
                               "&keyword=%1&page=1&pagesize=18").arg(str_name);
    qDebug()<<strurl;
    request.setUrl(QUrl(strurl));
    answer = manager_getinfo->get(request);
    QEventLoop eventloop;
    connect(manager_getinfo, SIGNAL(finished(QNetworkReply*)),&eventloop, SLOT(quit()));
    eventloop.exec();
    get_song_info(answer);
    answer->deleteLater();
}

void Net_songs::get_song_info(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QString result(bytes);
        parseSongInfo(result);
        emit get_songs_info_over();
    }
    get_net_path();
    reply->deleteLater();
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
                                    QString album_code;
                                    double filesize;
                                    QJsonObject object_first = value_first.toObject();
                                    if(object_first.contains("songname"))
                                    {
                                        QJsonValue filename = object_first.take("songname");
                                        songname = filename.toString();
                                    }else
                                    {
                                        songname = "未知";
                                    }
                                    if(object_first.contains("hash"))
                                    {
                                        QJsonValue filehash = object_first.take("hash");
                                        hash = filehash.toString();    
                                    }else
                                    {
                                        hash = "未知";
                                    }
                                    if(object_first.contains("album_id"))
                                    {
                                        QJsonValue album = object_first.take("album_id");
                                        album_code = album.toString();  
                                        // qDebug()<<singer;  
                                    }else
                                    {
                                        singername = "未知";
                                    }
                                    if(object_first.contains("singername"))
                                    {
                                        QJsonValue singer = object_first.take("singername");
                                        singername = singer.toString();  
                                        // qDebug()<<singer;  
                                    }else
                                    {
                                        singername = "未知";
                                    }
                                    if(object_first.contains("album_name"))
                                    {
                                        QJsonValue album = object_first.take("album_name");
                                        album_name = album.toString();    
                                    }else
                                    {
                                        album_name = "未知";
                                    }
                                    if(object_first.contains("filesize"))
                                    {
                                        QJsonValue value_filesize  = object_first.take("filesize");
                                        filesize = value_filesize.toDouble(); 
                                        filesize = round(filesize/1024/1024*100)/100;
                                    }else
                                    {
                                        filesize = 0;
                                    }   
                                    result_list.songName = songname;
                                    result_list.hash = hash;
                                    result_list.filesize = QString("%1").arg(filesize);
                                    result_list.album_id = album_code;
                                    result_list.filesize = result_list.filesize+"Mb";
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
    }else
    {
        qDebug() << "error ... ...";
    }
}

void Net_songs::get_net_path()
{   
    for(int i=0; i < m_listResult.length();i++)
    {
        QString hash = m_listResult.at(i).hash;
        QString id = m_listResult.at(i).album_id;
        QNetworkRequest request;
        request.setUrl(QUrl(QString("https://www.kugou.com/yy/index.php?r=play/getdata"
                                    "&hash=%1&album_id=%2&_=1497972864535").arg(hash).arg(id)));
        // request.setUrl(QUrl(QString("https://www.kugou.com/yy/index.php?r=play/getdata&hash=%1").arg(hash)));
        // qDebug()<<QString("https://www.kugou.com/yy/index.php?r=play/getdata&hash=%1").arg(hash);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Cookie","kg_mid=1001");
        // manager_importinfo->get(request);
        // qDebug()<<manager_importinfo;
        QNetworkReply* answer;
        answer = manager_importinfo->get(request);
        QEventLoop event2;
        connect(manager_importinfo, SIGNAL(finished(QNetworkReply*)),&event2, SLOT(quit()));
        event2.exec();
        int ret = 1;
        ret = get_song_url(answer);
        QEventLoop event3;
        if(!ret)
        {
            event3.quit();
        }else
        {
            event3.exec();
        }
    }
    std::cout<<"exit";
    emit get_timelength_over();
}

int Net_songs::get_song_url(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status_code;
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray byte = reply->readAll();
        std::cout<<byte.toStdString()<<std::endl;
        QString result(byte);
        parse_songs(result);
    }
    emit continue_songs();
    reply->deleteLater();
    return 0;
}

void Net_songs::parse_songs(QString json)
{
    QByteArray byte_array;
    QJsonParseError json_error;
    byte_array = json.toLatin1();
    QJsonDocument json_document = QJsonDocument::fromJson(byte_array, &json_error);
    // qDebug()<<json_error.error;
    if(json_error.error == QJsonParseError::NoError)
    {
        if(json_document.isObject())
        {
            QJsonObject rootobj = json_document.object();
            if(rootobj.contains("data"))
            {
                QJsonValue value_data = rootobj.take("data");
                if(value_data.isObject())
                {
                    QJsonObject first_obj = value_data.toObject();
                    QString play_path;
                    int time_length;
                    QString img;
                    if(first_obj.contains("play_url"))
                    {
                        QJsonValue play_url = first_obj.take("play_url");
                        play_path = play_url.toString();
                    }
                    if(first_obj.contains("timelength"))
                    {
                        QJsonValue time_ = first_obj.take("timelength");
                        time_length = time_.toInt();
                    }else
                    {
                        time_length = 0;
                    }
                    if(first_obj.contains("img"))
                    {
                        QJsonValue img_ = first_obj.take("img");
                        img = img_.toString();
                    }
                    findResult tmp;
                    if(first_obj.contains("hash"))
                    {
                        QJsonValue value_filename = first_obj.take("hash");
                        for(int i=0;i<m_listResult.length();i++)
                        {
                            if(m_listResult.at(i).hash == value_filename.toString())
                            {
                                time_length = time_length/1000;
                                tmp.songName =m_listResult.at(i).songName;
                                tmp.singername =m_listResult.at(i).singername;
                                tmp.album_name = m_listResult.at(i).album_name;
                                tmp.filesize = m_listResult.at(i).filesize;
                                tmp.playPath = play_path;
                                tmp.timelength = QString("%1:%2").arg(time_length/60).arg(time_length%60,2,10,QChar('0'));
                                tmp.image = img;
                                qDebug()<<tmp.timelength;
                                m_listResult.replace(i,tmp);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}