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

void mysleep(size_t msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
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
    while ( !nw_reply_flag && timeout-- )
        mysleep(10);
    return nw_reply_flag;
}

/******************************************************************
 * @brief get file size from url
 *
 * @param url
 * @return size_t
 *******************************************************************/
size_t mcgf_network::get_file_size(QString url)
{
    network_req->setUrl(url);
    network_am->head(*network_req);

    file_size = 0;

    QObject::connect(
        network_am,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(get_file_size_slot(QNetworkReply *))
    );

    /* wait file size be get. */
    while ( file_size == 0 )
        mysleep(100);

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

    if ( reply->hasRawHeader("Content-Length") ) {
        raw_size = reply->rawHeader("Content-Length");
        file_size = raw_size.toUInt();
    } else {
        QMessageBox::warning(nullptr, "Error", "Get file size error", QMessageBox::Ok, QMessageBox::Cancel);
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
    QMessageBox::StandardButton msg_ret;

    /* Judge the save path is legal or not */
    QFileInfo finfo(save_path);
    if ( finfo.exists() ) {
        if ( finfo.isFile() == false ) {
            qDebug() << "Save path is directory.";
            QMessageBox::warning(nullptr, "warning", "Save path is directory.", QMessageBox::Ok);
            return false;
        } else {
            qDebug() << "File is exist in save path. Delete it?";
            msg_ret = QMessageBox::question(
                nullptr,
                "question", "File is exist in save path. Delete it?",
                QMessageBox::Yes | QMessageBox::Cancel
            );
            if ( msg_ret == QMessageBox::Yes ) {
                QFile file(save_path);
                if ( file.isOpen() )
                    file.close();
                file.remove();
            } else
                return false;
        }
    }

    QString filename = nullptr;
    size_t fsize = 0;

    filename = get_filename(url);
    fsize = get_file_size(url);
    if ( !fsize ) {
        qDebug() << "File size get error. stop download";
        QMessageBox::warning(nullptr, "Error", "Get file size error. stop download", QMessageBox::Ok);
        return false;
    }

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


    return true;
}

void mcgf_network::save_file_slot(QNetworkReply *reply)
{
//    if (reply)
}





bool mcgf_network::get_html(QString url, QString file_name, QString save_path, size_t timeout)
{
    if ( url.isNull() ) {
        qDebug() << "url error";
        QMessageBox::critical(nullptr, "Error", "url error.", QMessageBox::Ok);
        return false;
    }
    if ( save_path.isNull() )
        save_path = "./";
    if ( file_name.isNull() )
        file_name = get_filename(url);

    if ( (save_path.endsWith("\\") || save_path.endsWith("/")) == false ) {
        if ( !save_path.endsWith("\\") )
            save_path += '\\';
        else
            save_path += '/';
    }

    QString path = save_path + file_name;
    QFileInfo finfo(path);
    if ( finfo.isDir() )
        path += get_filename(url);

    /* connect signal */
    connect(network_am, SIGNAL(finished(QNetworkReply *)), this, SLOT(html_reply_slot(QNetworkReply *)));
    network_req->setUrl(QUrl(url));
    network_am->get(*network_req);

    if ( wait_reply(timeout) == false ) {
        qDebug() << "wait reply error";
        QMessageBox::critical(nullptr, "Error", "html wait reply error", QMessageBox::Ok);
        return false;
    }

    mcgf_fo *file = new mcgf_fo;
    save_done_flag = file->openw(path, nw_reply->readAll());
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
    nw_reply_flag = true;
    nw_reply = reply;
}


