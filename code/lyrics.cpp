#include "../Headers/lyrices.h"

void LC_classer::lyrics_net_show()
{
    if (pos == 0)
        return ;
    qint64 position = ((pos/60000)*60+(pos / 1000) % 60);
    QMap<int, QString>::iterator iter = lc_list[row].begin();
    while(iter != lc_list[row].end())
    {
        std::cout<<iter.value().toStdString();
        if(position-2<=iter.key()&& position>=iter.key())
        {
            if(iter.value()=="\r")
            {
                break;
            }
            int j=0;
            if(iter != lc_list[row].begin())
            {
                choose_ly_c.lyc3 = iter.value();
                j++;
                iter--;
            }else
            {
                break;
            }

            if(iter != lc_list[row].begin())
            {
                choose_ly_c.lyc2 = iter.value();
                j++;
                iter--;
            }

            if(iter != lc_list[row].begin())
            {
                choose_ly_c.lyc1 = iter.value();
                j++;
                iter--;
            }
            for(;j>0;j--)
            {
                iter++;
            }
            iter++;
            if(iter != lc_list[row].end())
            {
                choose_ly_c.lyc4 = iter.value();
                iter++;
            }else
            {
                choose_ly_c.lyc4 = "";
                return;
            }

            if(iter != lc_list[row].end())
            {
                choose_ly_c.lyc5 = iter.value();
                iter++;
            }else
            {
                choose_ly_c.lyc5 = "";
                return;
            }
            break;
        }
        iter++;

    }
   emit show_lc();
}