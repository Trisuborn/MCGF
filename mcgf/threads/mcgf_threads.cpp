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


/******************************************************************
 * @brief mcgf_thread
 *******************************************************************/
void mcgf_thread::run()
{

}

void mcgf_thread::done_slot()
{
    qDebug() << "mcgf_thread::done_slot";
    this->wait();
    this->quit();
}


/******************************************************************
 * @brief mcgf_dl_html_th
 *******************************************************************/
mcgf_dl_html_th::mcgf_dl_html_th()
{
    method = 1;
    connect(this, SIGNAL(done_sig()), this, SLOT(done_slot()));
}

mcgf_dl_html_th::mcgf_dl_html_th(QString iurl, QString file_name, QString save_path)
{
    method = 2;

    url   = iurl.isNull() ? local_url : iurl;
    fname = file_name.isNull() ? (nw->get_filename(url)) : file_name;
    sp    = save_path.isNull() ? def_path : save_path;

    connect(this, SIGNAL(done_sig()), this, SLOT(done_slot()));

}

mcgf_dl_html_th::~mcgf_dl_html_th()
{
    disconnect(this, SIGNAL(done_sig()), this, SLOT(done_slot()));
}


void mcgf_dl_html_th::run()
{
    mcgf_network *nw = new mcgf_network;
    nw->get_html(url, fname, sp, (100*60));
    emit done_sig();
}

void mcgf_dl_html_th::done_slot()
{
    delete nw;
    this->wait();
    this->quit();
}
