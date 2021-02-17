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

#ifdef __linux__
#include <windows.h>
#elif _WIN32
#include <windows.h>
#endif

void mysleep(size_t msec)
{
//    QTime dieTime = QTime::currentTime().addMSecs(msec);
//    while ( QTime::currentTime() < dieTime ) {
//        QApplication::processEvents(QEventLoop::AllEvents, 100);
//    }
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}


mcgf_nw_dl_th::mcgf_nw_dl_th()
{
    qDebug() << "mcgf_nw_dl_th";
    QObject::connect(this,
                     SIGNAL(done_sig()),
                     this,
                     SLOT(done_slot()));
}

void mcgf_nw_dl_th::run()
{
    QElapsedTimer qetime;

    qetime.start();

    for ( int cnt = 0; cnt < 11; cnt++ ) {
        qDebug() << cnt;
        mysleep(100);
    }

    dl_msec = qetime.elapsed();
    qDebug() << "msec" << dl_msec;
    emit done_sig();
}

void mcgf_nw_dl_th::done_slot()
{
    qDebug() << "done_slot";
    this->wait();
    this->quit();
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/******************************************************************
 * @brief Construct a new mcgf network::mcgf network object
 *******************************************************************/
mcgf_network::mcgf_network()
{
    qDebug() << "mcgf_network init.";
    this->network_am = new QNetworkAccessManager(this);
    this->network_req = new QNetworkRequest();

    this->dl_tid = new mcgf_nw_dl_th();
    this->dl_tid->start();
}

/******************************************************************
 * @brief Destroy the mcgf network::mcgf network object
 *******************************************************************/
mcgf_network::~mcgf_network()
{
    qDebug() << "mcgf_network delete.";
    delete this->network_am;
    delete this->network_req;
    delete this->dl_tid;
}

/******************************************************************
 * @brief
 *
 * @param url
 * @param save
 * @param params
 * @return true
 * @return false
 *******************************************************************/
bool mcgf_network::get_html(QString url, bool save, void *params)
{
    bool ret = true;

    if ( save == true ) {
        args = (gh_params_t *)(params);
        save_flag = true;
    }

    if ( url == nullptr ) {
        qDebug() << "url is null.";
        ret = false;
        goto __out;
    }

    qDebug() << "get html: " << url;
    this->network_am->get(QNetworkRequest(QUrl(url)));

    QObject::connect(
        this->network_am,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(get_html_slot(QNetworkReply *))
    );
    QObject::connect(
        this,
        SIGNAL(save_html_sig(gh_params_t *)),
        this,
        SLOT(save_html_slot(gh_params_t *))
    );

__out:
    return ret;
}

/******************************************************************
 * @brief
 *
 * @param reply
 *******************************************************************/
void mcgf_network::get_html_slot(QNetworkReply *reply)
{
    QObject::disconnect(
        this->network_am,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(get_html_slot(QNetworkReply *))
    );

    if ( reply ) {
        if ( save_flag == true ) {
            args->reply = reply;
            emit save_html_sig(args);
        }
    } else {
        // TODO: do something
    }
}


/******************************************************************
 * @brief
 *
 * @param params
 *******************************************************************/
void mcgf_network::save_html_slot(gh_params_t *params)
{
    if ( params->reply == nullptr ) {
        qDebug() << "Html hasn't any replies";
        QMessageBox::warning(nullptr, "Error", "Http get error!", QMessageBox::Ok, QMessageBox::Cancel);
        return;
    } else if ( (params->file_name == nullptr) || (params->save_path == nullptr) ) {
        qDebug() << "Save name and path error.";
        QMessageBox::warning(nullptr, "Error", "params error!", QMessageBox::Ok, QMessageBox::Cancel);
        return;
    }

    qDebug() << "Save html in: " << params->save_path;
    qDebug() << "Save html as: " << params->file_name;
    qDebug() << "Starting save...";

    QString path = params->save_path + params->file_name;
    mcgf_fo file;
    bool fflag = false;

    /* save html file to disk */
    fflag = file.openw(path, params->reply->readAll());
    if ( fflag ) {
        // TODO: do something
        qDebug() << "Save done.";
        QMessageBox::information(nullptr, "Successfully!", "Save done", QMessageBox::Ok, QMessageBox::Cancel);
    } else {
        QMessageBox::warning(nullptr, "Error", "Save html error", QMessageBox::Ok, QMessageBox::Cancel);
    }

    QObject::disconnect(
        this,
        SIGNAL(save_html_sig(gh_params_t *)),
        this,
        SLOT(save_html_slot(gh_params_t *))
    );

    return;
}

/******************************************************************
 * @brief get file size from url
 *
 * @param url
 * @return size_t
 *******************************************************************/
size_t mcgf_network::get_file_size(QString url)
{
    qDebug() << "get_file_size 1";

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

    qDebug() << "get_file_size 2";

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






