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

// TODO: open check

bool mcgf_fo::openw(QString path, const char *wbuffer)
{
    bool fflag = false;
    qint64 wsize = 0;
    QFile file(path);
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


