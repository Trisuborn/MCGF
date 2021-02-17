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

bool qt_file_operation::openw(QString path, const char *wbuffer)
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
}
