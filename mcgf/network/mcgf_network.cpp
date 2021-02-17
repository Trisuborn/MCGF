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
#include <fstream>

#include <QDebug>
#include <QNetworkReply>


mcgf_network::mcgf_network()
{
    qDebug() << "mcgf_network init.";
    this->network_am = new QNetworkAccessManager(this);
    QObject::connect(this->network_am, SIGNAL(finished(QNetworkReply*)),this ,SLOT(network_reply_slot(QNetworkReply*)));
}

mcgf_network::~mcgf_network()
{
    qDebug() << "mcgf_network delete.";
    delete this->network_am;
}

void mcgf_network::get_html(QString url)
{
    if (url == nullptr) {
        qDebug() << "url is null.";
    } else {
        qDebug() << "get html: " << url;
        this->network_am->get(QNetworkRequest(QUrl(url)));
    }
}

void mcgf_network::network_reply_slot(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}




