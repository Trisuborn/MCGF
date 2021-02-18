/************************************************
 * @file mcgf_network.cpp
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-17
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "mcgf_network.h"
#include "mcgf/file/mcgf_file.h"

#include <QMessageBox>
#include <QDebug>
#include <QNetworkReply>
#include <QFileInfo>

#include <QElapsedTimer>
#include <QTimer>
#include <QThread>
#include <QMutex>

/* 网络类管理块 */
mcgf_nw_manage mcgf_nw_mb;

void mysleep(size_t msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}


void mcgf_nw_manage::send_nw_all_abort_emit()
{
    emit nw_all_abort_sig();
}

mcgf_nw_manage::mcgf_nw_manage()
{
    qDebug() << __func__;
    connect(this, SIGNAL(nw_all_abort_sig()), this, SLOT(mcgf_nw_abort_all_slot()));

}

mcgf_nw_manage::~mcgf_nw_manage()
{
    qDebug() << __func__;

    // TODO:

}

bool mcgf_nw_manage::mcgf_nw_register(mcgf_network *obj)
{
    uint8_t i;
    for ( i = 0; i < MCGF_MAX_NETWORK_NUM; i++ ) {
        if ( mcgf_nw_stack[i] == nullptr ) {
            mcgf_nw_cnt += 1;
            mcgf_nw_stack[i] = obj;
            qDebug() << __func__ << "register in" << i;
            return true;
        }
    }

    return false;
}

bool mcgf_nw_manage::mcgf_nw_unregister(mcgf_network *obj)
{
    uint8_t i;
    for ( i = 0; i < MCGF_MAX_NETWORK_NUM; i++ ) {
        if ( mcgf_nw_stack[i] == nullptr )
            continue;
        if ( mcgf_nw_stack[i]->nw_reply == nullptr )
            continue;
        if ( obj == mcgf_nw_stack[i] ) {
            if ( mcgf_nw_stack[i]->nw_reply->isRunning() )
                mcgf_nw_stack[i]->nw_reply->abort();
            mcgf_nw_stack[i] = nullptr;
            return true;
        }
    }
    return false;
}

bool mcgf_nw_manage::mcgf_nw_has_busy()
{
    uint8_t i;
    for ( i = 0; i < MCGF_MAX_NETWORK_NUM; i++ ) {
        if ( mcgf_nw_stack[i] == nullptr )
            continue;
        if ( mcgf_nw_stack[i]->nw_reply == nullptr )
            continue;
        if ( mcgf_nw_stack[i]->nw_reply->isRunning() )
            return true;
    }
    return false;
}

void mcgf_nw_manage::mcgf_nw_abort_all_slot()
{
    uint8_t i;
    for ( i = 0; i < MCGF_MAX_NETWORK_NUM; i++ ) {
        if ( mcgf_nw_stack[i] == nullptr )
            continue;
        if ( mcgf_nw_stack[i]->nw_reply == nullptr )
            continue;
        if ( mcgf_nw_stack[i]->nw_reply->isRunning() )
            mcgf_nw_stack[i]->reply_abort_emit();
    }
}


/******************************************************************************/
/******************************************************************************/

/******************************************************************
 * @brief Construct a new mcgf network::mcgf network object
 *******************************************************************/
mcgf_network::mcgf_network()
{
    qDebug() << "mcgf_network create.";
    this->network_am = new QNetworkAccessManager(this);
    this->network_req = new QNetworkRequest();

    connect(this, SIGNAL(reply_abort_sig()), this, SLOT(reply_abort_slot()));
}

/******************************************************************
 * @brief Destroy the mcgf network::mcgf network object
 *******************************************************************/
mcgf_network::~mcgf_network()
{
    qDebug() << "mcgf_network delete.";
    delete this->network_am;
    delete this->network_req;
}

/******************************************************************
 * @brief wait reply
 *
 * @param timeout unit: 1 jiffy
 * @return true
 * @return false
 *******************************************************************/
bool mcgf_network::wait_reply(size_t timeout)
{
    if ( !timeout )
        return false;

    do {
        if ( nw_reply == nullptr )
            return false;
        if ( nw_reply->isOpen() && nw_reply->isFinished() )
            return true;
        mysleep(10);
    } while ( --timeout );

    nw_reply->abort();

    return false;
}

void mcgf_network::check_path(QString *path)
{
    if ( (path->endsWith("\\") || path->endsWith("/")) == false ) {
        if ( !path->endsWith("\\") )
            *path += '\\';
        else
            *path += '/';
    }
}

/******************************************************************
 * @brief get file size from url
 *
 * @param url
 * @return size_t
 *******************************************************************/
size_t mcgf_network::get_file_size(QString url)
{
    busy_flag = true;

    network_req->setUrl(url);
    nw_reply = network_am->head(*network_req);

    file_size = 0;

    QObject::connect(
        network_am,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(get_file_size_slot(QNetworkReply *))
    );

    /* wait file size be get. */
    if ( wait_reply(100 * 60) == false ) {
        qDebug() << __func__ << "wait reply error";
        file_size = 0;
    }

    busy_flag = false;

    nw_reply->abort();
    return file_size;
}

/******************************************************************
 * @brief get_file_size_slot
 *
 * @param reply
 *******************************************************************/
void mcgf_network::get_file_size_slot(QNetworkReply *reply)
{
    QString raw_size = nullptr;

    nw_reply = reply;

    if ( nw_reply->isOpen() && nw_reply->isFinished() ) {
        if ( reply->hasRawHeader("Content-Length") ) {
            raw_size = nw_reply->rawHeader("Content-Length");
            file_size = raw_size.toUInt();
        } else {
            qDebug() << __func__ << "wait reply error";
        }
    }
}

/************************************************
 * @brief get_filename from url
 *
 * @param url
 * @return QString
 *************************************************/
QString mcgf_network::get_filename(QString url)
{
    if ( url == nullptr )
        return url;
    QStringList qstr_list = url.split('/');
    return qstr_list[qstr_list.count() - 1];
}

/******************************************************************
 * @brief save file from url
 *
 * @param url       url download
 * @param save_path save path
 * @param pack_size size one fetch
 * @return true     ok
 * @return false    error
 *******************************************************************/
bool mcgf_network::save_file(QString url, QString save_path, size_t pack_size)
{
    QString filename = nullptr;
    size_t fsize = 0;
    QString path = nullptr;

    filename = get_filename(url);
    check_path(&save_path);
    path = save_path + filename;

    fsize = get_file_size(url);
    if ( !fsize ) {
        qDebug() << "File size get error. stop download";
//        QMessageBox::warning(nullptr, "Error", "Get file size error. stop download", QMessageBox::Ok);
        return false;
    } else
        file_size = fsize;

    /* 根据数据包的长度计算传输的次数和余数 */
    size_t tt = 0;  // transmitted times
    size_t ts = 0;  // transmitted surplus

    if ( fsize <= pack_size )
        tt = 1;
    else {
        tt = fsize / pack_size;
        ts = fsize % pack_size;
    }

    /* 开始传输 */
    qDebug() << "file name:" << filename;
    qDebug() << "file size:" << file_size;
    qDebug() << "tt:" << tt;
    qDebug() << "ts:" << ts;


    network_req->setUrl(QUrl(url));
    network_am->get(*network_req);


    mcgf_fo *file;

    nw_reply->abort();
    return true;
}

void mcgf_network::save_file_slot(QNetworkReply *reply)
{

}




/******************************************************************
 * @brief get html via url
 *
 * @param url url
 * @param file_name file name to save
 * @param save_path path to save
 * @param timeout timeout(unit: jiffy)
 * @return true
 * @return false
 *******************************************************************/
bool mcgf_network::get_html(QString url, QString file_name, QString save_path, size_t timeout)
{
    if ( url.isNull() ) {
        qDebug() << "url error";
        return false;
    }
    if ( save_path.isNull() )
        save_path = "./";
    if ( file_name.isNull() )
        file_name = get_filename(url);

    check_path(&save_path);

    QString path = save_path + file_name;
    QFileInfo finfo(path);
    if ( finfo.isDir() )
        path += get_filename(url);

    /* connect signal */
    connect(network_am, SIGNAL(finished(QNetworkReply *)), this, SLOT(html_reply_slot(QNetworkReply *)));
    network_req->setUrl(QUrl(url));
    nw_reply = network_am->get(*network_req);

    if ( wait_reply(timeout) == false ) {
        qDebug() << __func__ << "wait reply error";
        return false;
    }

    mcgf_fo *file = new mcgf_fo;
    save_done_flag = file->open_write(path, nw_reply->readAll());
    delete file;

    if ( save_done_flag ) {
        qDebug() << "Html file save done";
        return true;
    } else {
        qDebug() << "Html file save error";
        return false;
    }
}

void mcgf_network::html_reply_slot(QNetworkReply *reply)
{
    nw_reply = reply;
}

void mcgf_network::reply_abort_emit()
{
    emit reply_abort_sig();
}

void mcgf_network::reply_abort_slot()
{
    nw_reply->abort();
}





