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

#include <QEventLoop>
#include <QTimer>

extern void mysleep(size_t msec);


mcgf_fo::mcgf_fo()
{

}

mcgf_fo::~mcgf_fo()
{

}

bool mcgf_fo::open_with_check(QFile *filp)
{
    size_t timeout = 10;
    while ( filp->isOpen() && timeout-- )      // 等待已打开文件退出并设置超时
        mysleep(10);
    if ( filp->isOpen() ) {
        filp->close();
        return false;
    } else
        return true;
}

bool mcgf_fo::_openr(QString path, QString &rbuffer, QIODevice::OpenMode mode)
{

}

bool mcgf_fo::_openw(QString path, const QString &wbuffer, QIODevice::OpenMode mode)
{
    bool ret = false;
    QFile *filp = new QFile(path);

    ret = open_with_check(filp);
    if ( !ret )
        goto __out;

    ret = filp->open(mode);
    if ( !ret )
        goto __out;

    if ( filp->write(wbuffer.toUtf8()) )
        ret = true;
    else
        ret = false;

__out:
    delete filp;
    return ret;
}


bool mcgf_fo::open_write(QString path, const QString &wbuffer)
{
    return this->_openw(path, wbuffer, QIODevice::Text | QIODevice::WriteOnly);
}

bool mcgf_fo::open_write_bytes(QString path, const QString &wbuffer)
{
    return this->_openw(path, wbuffer);
}


