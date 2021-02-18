/************************************************
 * @file mcgf_file.cpp
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2021-02-17
 * 
 * @copyright Copyright (c) 2021
 * 
 *************************************************/
#include "mcgf_file.h"

extern void mysleep(size_t msec);


mcgf_fo::mcgf_fo()
{

}

mcgf_fo::~mcgf_fo()
{

}

bool mcgf_fo::open_with_check(QFile *file)
{
    size_t timeout = 10;
    while (file->isOpen() && timeout--)      // 等待已打开文件退出并设置超时
        mysleep(10);
    if (file->isOpen()){
        file->close();
        return false;
    } else
        return true;
}

bool mcgf_fo::openw(QString path, const char *wbuffer)
{
    bool fflag = false;
    qint64 wsize = 0;
    QFile file(path);

    file_busy_flag = true;

    open_with_check(&file);
    fflag = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!fflag)
        return fflag;
    else {
        wsize = file.write(wbuffer);
        if (!wsize)
            return false;
        else
            return true;
    }
    file.close();

    file_busy_flag = false;
}

bool mcgf_fo::opena(QString path, const char *wbuffer)
{

}

bool mcgf_fo::openr(QString path, char *rbuffer)
{

}

bool mcgf_fo::openrw(QString path, char *buffer)
{

}


