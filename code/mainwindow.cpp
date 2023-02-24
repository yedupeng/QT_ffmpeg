#include "../Headers/mainwindow.h"

qint64 duration = 240;
int number = 1;

Mainwindow::Mainwindow(QWidget *parent) : QWidget(parent),ui(new Ui::Form)
{
    ui->setupUi(this);
    net->moveToThread(pThread);
    mp3_player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist;
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu); 
    ui->radioButton->setChecked(1);
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    net->init();

    QTimer *timer = new QTimer(this);
    menu = new QMenu(this);
    m = new QMenu(this);

    QAction* action1 = new QAction("暂停", this);
    QAction* action2 = new QAction("删除歌曲", this);
    QAction* action3 = new QAction("添加进歌单", this);
    QAction* action4 = new QAction("我的歌单一", this);
    QAction* action5 = new QAction("我的歌单二", this);
    QAction* action6 = new QAction("我喜欢的音乐", this);
    QList<QAction*> action_list;
    action_list.append(action4);
    action_list.append(action5);
    action_list.append(action6);

    menu->addAction(action1);
    menu->addAction(action2);

    m->addAction(action4);
    m->addAction(action5);
    m->addAction(action6);
    m->setTitle("添加进歌单");

    menu->addMenu(m);
    menu->addSeparator();

    // menu->addAction(action4);
    // menu->addAction(action5);
    // system("chcp 65001");

    ui->verticalSlider->setMaximum(100);
    ui->verticalSlider->setMinimum(0);
    ui->verticalSlider->setPageStep(10);
    ui->verticalSlider->setSingleStep(10);
    ui->verticalSlider->setSliderPosition(50);
    ui->horizontalSlider->setRange(0,100);
    
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btn_down ,SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->btn_aut, SIGNAL(clicked()), this, SLOT(btn_chage_img()));
    connect(ui->btn_file,SIGNAL(clicked()),this,SLOT(btn_chage_page_local()));
    connect(ui->btn_favor,SIGNAL(clicked()),this,SLOT(btn_chage_page_favor()));
    connect(ui->btn_my,SIGNAL(clicked()),this,SLOT(btn_chage_page_my1())); 
    connect(ui->btn_my2,SIGNAL(clicked()),this,SLOT(btn_chage_page_my2())); 
    connect(ui->btn_rec,SIGNAL(clicked()),this,SLOT(btn_chage_page_rec())); 
    connect(ui->btn_search2,SIGNAL(clicked()),this,SLOT(btn_chage_import_songs())); 
    connect(ui->btn_import,SIGNAL(clicked()),this,SLOT(btn_chage_import_song())); 
    connect(ui->btn_play_all,SIGNAL(clicked()),this,SLOT(play_all())); 
    connect(ui->btn_start,SIGNAL(clicked()),this,SLOT(stop())); 
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(play_model_change()));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(volum_change()));
    connect(ui->sound,SIGNAL(clicked()),this,SLOT(btn_close_volum()));
    connect(mp3_player,SIGNAL(positionChanged(qint64)),this,SLOT(change_position(qint64)));
    connect(mp3_player,SIGNAL(durationChanged(qint64)),this,SLOT(change_duration(qint64)));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(seekChange(int)));
    connect(ui->btn_next,SIGNAL(clicked()),this,SLOT(next_song()));
    connect(ui->btn_last,SIGNAL(clicked()),this,SLOT(last_song()));
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(show_meau(const QPoint&)));
    connect(menu,SIGNAL(triggered(QAction *)),this,SLOT(select_action(QAction *)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(get_row(int,int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(update_red()));
    connect(ui->btn_search,SIGNAL(clicked()),this,SLOT(get_search_song()));
    connect(net, &Net_songs::get_songs_info_over, this, &Mainwindow::add_table);
    connect(net, &Net_songs::get_timelength_over, this, &Mainwindow::add_item,Qt::DirectConnection);
    timer->start(3000);
}

Mainwindow::~Mainwindow()
{
}

// 点击切换btn_aut头像
void Mainwindow::btn_chage_img()
{
    num++;
    File = "QPushButton{border-image: url(:/head/head/";
    if(num == 10)
    {
        num = 1;
    }
    File = File + QString::number(num, 10)+".png)}";
    // printf("%s\n",File.toStdString().data());
    ui->btn_aut->setStyleSheet(File);
}

void Mainwindow::btn_chage_img2()
{
    QString path = "QPushButton{border-image: url(:/head/head/";
    int i = number % 10;
    path = path + QString::number(i, 10)+".png)}";
    // printf("%s\n",File.toStdString().data());
    ui->pushButton_17->setStyleSheet(path);
}

void Mainwindow::btn_chage_page_local()
{
    ui->stack_1->setCurrentIndex(2);
}

void Mainwindow::btn_chage_page_favor()
{
    ui->stack_1->setCurrentIndex(1);
}

void Mainwindow::btn_chage_page_my1()
{
    ui->stack_1->setCurrentIndex(3);
}

void Mainwindow::btn_chage_page_my2()
{
    ui->stack_1->setCurrentIndex(4);
}

void Mainwindow::btn_chage_page_rec()
{
    ui->stack_1->setCurrentIndex(0);
}

void Mainwindow::btn_chage_import_songs()
{
    QStringList  path_list;
    QString selectedDir = QFileDialog::getExistingDirectory(this,"请选择本地歌单","",QFileDialog::ShowDirsOnly);
    std::string path = selectedDir.toStdString();
	intptr_t  hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.mp3").c_str(), &fileinfo)) != -1) {
		do {
            path_list.append(QString::fromLocal8Bit(QByteArray::fromStdString(p.assign(path).append("/").append(fileinfo.name))));
            
		} while (_findnext(hFile, &fileinfo) == 0);      
		_findclose(hFile);
	}
    list_songs = path_list;
    ui->lineEdit_2->setText(QString::fromLocal8Bit(QByteArray::fromStdString(path)));
    for(int i=0; i<path_list.size(); i++)
    {
        QString path = QDir::toNativeSeparators(path_list.at(i));
        if(path != nullptr)
        {
            get_songs(path);
            playlist->addMedia(QUrl::fromLocalFile(path));
            // play_list->addMedia(QUrl(path));
        }
    }
    
}

void Mainwindow::btn_chage_import_song()
{
    QStringList  path_list;
    path_list = QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"), "", tr("(*.mp3)"));
    // list_songs.append(path_list);
    list_songs.append(path_list);
    for (int i = 0; i < path_list.size(); ++i)
    {
        QString path = QDir::toNativeSeparators(path_list.at(i));
        if (!path.isEmpty())
        {
            //m_musicPlayList->addMedia(QUrl::fromLocalFile(path));
            get_songs(path);
            playlist->addMedia(QUrl::fromLocalFile(path));
        }
    }
}

void Mainwindow::get_songs(QString file)
{
    int ret = 0;
    av_register_all();
    // std::cout<<file.toLocal8Bit().data()<<std::endl;
    AVFormatContext *cnt_avf = nullptr;
    ret = avformat_open_input(&cnt_avf, file.toLocal8Bit().data(), nullptr, nullptr);
    if(ret<0)
    {
        std::cout<<"open the file of sings error"<<std::endl;
        return;
    }
    ret = avformat_find_stream_info(cnt_avf, nullptr);
    if(ret<0)
    {
        std::cout<<"get the information error"<<std::endl;
        return;
    }
    q_map.clear();
    AVDictionaryEntry* tag = nullptr;
    while(tag = av_dict_get(cnt_avf->metadata, "", tag, 2))
    {
        QString key_string = tag->key;
        QString value_string = QString::fromUtf8(tag->value);
        q_map.insert(key_string, value_string);
    }
    av_dump_format(cnt_avf, 0, file.toLocal8Bit().data(), 0);
    int duration_s = cnt_avf->duration/1000000;
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(q_map.value("title","未知")));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(q_map.value("artist","未知")));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(q_map.value("album","未知")));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%1:%2").arg(duration_s/60).arg(duration_s%60,2,10,QChar('0'))));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QDir::toNativeSeparators(file)));
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    avformat_close_input(&cnt_avf);
    avformat_free_context(cnt_avf);
}

void Mainwindow::play_all()
{
    QTableWidgetItem* item;
    item = ui->tableWidget->item(number, 1);
    ui->label_singer->setText(item->text());
    item = ui->tableWidget->item(number, 0);
    ui->label_sing->setText(item->text());

    QString model = ui->comboBox->currentText();
    if(model == "单曲循环")
    {
        std::cout<<"进入单曲循环"<<std::endl;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else if(model == "列表循环")
    {
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        std::cout<<"进入列表循环"<<std::endl;
    }
    mp3_player->setPlaylist(playlist);
    mp3_player->play();
}


void Mainwindow::stop()
{
    if(flag)
    {
        mp3_player->pause();
        qDebug()<<mp3_player->mediaStatus();
    }else
    {
        mp3_player->play();
        qDebug()<<mp3_player->mediaStatus();
    }
    flag = 1-flag;
}

void Mainwindow::play_model_change()
{
    QString model = ui->comboBox->currentText();
    if(model == "单曲循环")
    {
        std::cout<<"进入单曲循环"<<std::endl;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else if(model == "列表循环")
    {
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        std::cout<<"进入列表循环"<<std::endl;
    }
}

void Mainwindow::volum_change()
{
    int pos = ui->verticalSlider->sliderPosition();
    mp3_player->setVolume(pos);
    // std::cout<<"当前音量"<<pos<<std::endl;
}

void Mainwindow::btn_close_volum()
{
    if(flag_volum)
    {
        mp3_player->setVolume(0);
        std::cout<<"静音"<<std::endl;
    }else
    {
        mp3_player->setVolume(50);
    }
    flag_volum = 1-flag_volum;
}

void Mainwindow::change_position(qint64 pos)
{
    ui->horizontalSlider->setValue(pos*100/duration);
    QTime moveTime(0,(pos/60000) % 60,(pos / 1000) % 60);//设置时间
    ui->label_time1 ->setText(moveTime.toString("mm:ss"));
}

void Mainwindow::change_duration(qint64 tim)
{
    btn_chage_img2();
    number++;
    duration = tim;
    QTime Time(0,(tim % (1000 * 60 * 60)) / (1000 * 60),(tim % (1000 * 60)) / 1000);
    ui->label_time2->setText(Time.toString("mm:ss"));
    QTableWidgetItem* item;

    int row = playlist->currentIndex();
    item = ui->tableWidget->item(row, 1);
    ui->label_singer->setText(item->text());
    item = ui->tableWidget->item(row, 0);
    ui->label_sing->setText(item->text());
}

void Mainwindow::seekChange(int pos)
{
    mp3_player -> setPosition(pos*duration/100);
    int moved = pos * 1000;
    QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);
    ui->label_time1 ->setText(moveTime.toString("mm:ss"));
}

void Mainwindow::next_song()
{
    int row = playlist->currentIndex();
    int num_all = playlist->mediaCount();
    row++;
    if(row >= num_all)
    {
        row = 0;
    }
    QTableWidgetItem* item;
    item = ui->tableWidget->item(row, 1);
    ui->label_singer->setText(item->text());
    item = ui->tableWidget->item(row, 0);
    ui->label_sing->setText(item->text());
    playlist->setCurrentIndex(row);
    std::cout<<"row:"<<row<<std::endl;
    mp3_player->play();
}

void Mainwindow::last_song()
{
    int row = playlist->currentIndex();
    row--;
    if(row<0)
    {
        row = 0;
    }
    QTableWidgetItem* item;
    item = ui->tableWidget->item(row, 1);
    ui->label_singer->setText(item->text());
    item = ui->tableWidget->item(row, 0);
    ui->label_sing->setText(item->text());
    playlist->setCurrentIndex(row);
    mp3_player->play();
}

void Mainwindow::show_meau(const QPoint& pos)
{
    selectedItem = ui->tableWidget->itemAt(pos);
    if (nullptr == selectedItem)
	{
		return;
	}
    menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void Mainwindow::select_action(QAction *action)
{
    // qDebug()<<action->text();
    QString selection = action->text();
    if(selection == "暂停")
    {
        mp3_player->stop();
    }else if(selection == "删除歌曲")
    {
        if (nullptr == selectedItem)
        {
            return;
        }else
        {
            if(selectedItem2 == selectedItem)
            {
                ui->tableWidget->removeRow(row_index);
                playlist->removeMedia(row_index);
                qDebug()<<selectedItem;
            }
        }
    }else if(selection == "我的歌单一")
    {
        qDebug()<<selection;
    }else if(selection == "我的歌单二")
    {
        qDebug()<<selection;
    }else if(selection == "我喜欢的音乐")
    {
        qDebug()<<selection;
    }
}

void Mainwindow::get_row(int row, int col)
{
    row_index = row;
    selectedItem2 = ui->tableWidget->item(row ,col);;
}

void Mainwindow::update_red()
{
    if(rad_num == 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        ui->radioButton_2->setChecked(1);
        ui->radioButton->setChecked(0);
        rad_num++;
    }else if(rad_num == 1)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        ui->radioButton_3->setChecked(1);
        ui->radioButton_2->setChecked(0);  
        rad_num++;
    }else if(rad_num == 2)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_4);
        ui->radioButton_4->setChecked(1);
        ui->radioButton_3->setChecked(0); 
        rad_num++;    
    }else if(rad_num == 3)
    {        
        ui->stackedWidget->setCurrentWidget(ui->page_1);
        ui->radioButton->setChecked(1);
        ui->radioButton_4->setChecked(0);   
        rad_num = 0;
    }
}

void Mainwindow::get_search_song()
{
    ui->stack_1->setCurrentIndex(5);
    QString text = ui->lineEdit->text();
    while(ui->tableWidget_7->rowCount())
    {
        ui->tableWidget_7->removeRow(0);
    }
    net->find(text);
    ui->label_54->setText(text);
}

void Mainwindow::add_table()
{
    std::cout<<1<<std::endl;
    ui->tableWidget_7->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_7->setContextMenuPolicy(Qt::CustomContextMenu); 

    if(!net->m_listResult.isEmpty())
    {
        ui->label_17->setStyleSheet("QLabel {border-image: url(:/background/background/20.png);}");
        const int tablerow = net->m_listResult.count();
        for(int i=0;i<tablerow;i++)
        {
            net->info = net->m_listResult.at(i);
            int rownum = ui->tableWidget_7->rowCount();
            ui->tableWidget_7->insertRow(rownum);
            ui->tableWidget_7->setItem(rownum, 0, new QTableWidgetItem(net->info.songName));
            ui->tableWidget_7->setItem(rownum, 1, new QTableWidgetItem(net->info.singername));
            ui->tableWidget_7->setItem(rownum, 2, new QTableWidgetItem(net->info.album_name));
            ui->tableWidget_7->setItem(rownum, 3, new QTableWidgetItem(net->info.filesize));
            ui->tableWidget_7->setItem(rownum, 4, new QTableWidgetItem(net->info.hash));
            ui->tableWidget_7->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            qDebug()<<net->info.songName.toStdString().c_str()<<net->info.singername.toStdString().c_str();
        }
        ui->label_56->setText(QString("%1").arg(tablerow));
    }else
    {
        ui->label_17->setStyleSheet("QLabel {border-image: url(:/background/pushubottom/question.png);}");
        ui->label_56->setText("0");
    }
}

void Mainwindow::add_item()
{
    const int tablerow = net->m_listResult.count();
    std::cout<<tablerow<<std::endl;
    for(int i =0; i<tablerow; i++)
    {
        qDebug()<<net->m_listResult.at(i).timelength;
        ui->tableWidget_7->setItem(i, 5, new QTableWidgetItem(net->m_listResult.at(i).timelength));
    }
}