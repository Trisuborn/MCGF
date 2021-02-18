/******************************************************************
 * @file mcgf_threads.cpp
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-18
 *
 * @copyright Copyright (c) 2021
 *
 *******************************************************************/
#include "mcgf_threads.h"

#include <QElapsedTimer>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>


QString local_url = "./404.html";
QString def_path  = "./";

extern void mysleep(size_t msec);

QMutex qmutex_nw;

/******************************************************************
 * @brief HTML
 *******************************************************************/
dl_html::dl_html(QString url, QString file_name, QString save_path)
{
    this->url         = url;
    this->file_name   = file_name;
    this->save_path   = save_path;
}

dl_html::~dl_html()
{

}

void dl_html::download_start()
{
    nw = new mcgf_network;

    qmutex_nw.lock();
    mcgf_nw_mb.mcgf_nw_register(nw);
    qmutex_nw.unlock();

    url         = url.isNull() ? local_url : url;
    file_name   = file_name.isNull() ? (nw->get_filename(url)) : file_name;
    save_path   = save_path.isNull() ? def_path : save_path;

    qDebug() << nw->get_html(url, file_name, save_path, (100 * 60));

    qmutex_nw.lock();
    mcgf_nw_mb.mcgf_nw_unregister(nw);
    qmutex_nw.unlock();

    delete nw;

}

void dl_html::download_abort()
{
    qDebug() << __func__;
}

dl_html_ctl::dl_html_ctl(QString url, QString file_name, QString save_path)
{

    dl_thread = new QThread;
    html = new dl_html(url, file_name, save_path);

    /* start and stop */
    connect(this, SIGNAL(start_sig()), html, SLOT(download_start()));
    connect(this, SIGNAL(stop_sig()), html, SLOT(download_abort()));

    /* delete later */
    connect(dl_thread, SIGNAL(finished()), html, SLOT(deleteLater()));

    /* run thread */
    html->moveToThread(dl_thread);
    dl_thread->start();
}

dl_html_ctl::~dl_html_ctl()
{
    dl_thread->quit();
    dl_thread->wait();

    delete dl_thread;
}

void dl_html_ctl::start()
{
    emit start_sig();
}
void dl_html_ctl::stop()
{
    emit stop_sig();
}


