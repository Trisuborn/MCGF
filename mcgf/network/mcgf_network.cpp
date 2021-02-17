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

#include <QDebug>
#include <QNetworkReply>

static bool save_flag = false;

mcgf_network::gh_params_t *args = nullptr;

void Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while ( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

mcgf_network::mcgf_network()
{
    qDebug() << "mcgf_network init.";
    this->network_am = new QNetworkAccessManager(this);
}

mcgf_network::~mcgf_network()
{
    qDebug() << "mcgf_network delete.";
    delete this->network_am;
}

bool mcgf_network::get_html(QString url, bool save, void *params)
{
    QNetworkReply *reply = nullptr;

    if ( save == true ) {
        args = (gh_params_t *)(params);
        save_flag = true;
    }

    if ( url == nullptr ) {
        qDebug() << "url is null.";
    } else {
        qDebug() << "get html: " << url;
        reply = this->network_am->get(QNetworkRequest(QUrl(url)));
        if ( reply ) {
            QObject::connect(
                this->network_am,
                SIGNAL(finished(QNetworkReply *)),
                this,
                SLOT(network_reply_slot(QNetworkReply *))
            );
            QObject::connect(
                this,
                SIGNAL(save_html_sig(gh_params_t *)),
                this,
                SLOT(save_html(gh_params_t *))
            );
            return true;
        } else
            return false;
    }
    return false;
}

void mcgf_network::network_reply_slot(QNetworkReply *reply)
{
    if ( reply ) {
        qDebug() << reply->read(50);
        if (save_flag == true) {
            emit save_html_sig(args);
        }
    } else {

    }
    QObject::disconnect(
        this->network_am,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(network_reply_slot(QNetworkReply *))
    );
}

bool mcgf_network::save_html(gh_params_t *params)
{
    if ( params->reply == nullptr ) {
        // TODO: using ui to alert user.
        qDebug() << "Html hasn't any replies";
        return false;
    } else if ( !(params->file_name) || !(params->save_path) ) {
        qDebug() << "Save name and path error.";
        return false;
    }

    qDebug() << "Save html in: " << *params->save_path;
    qDebug() << "Save html as: " << *params->file_name;

    QObject::disconnect(
        this,
        SIGNAL(save_html_sig(gh_params_t *)),
        this,
        SLOT(save_html(gh_params_t *))
    );

    return true;
}


