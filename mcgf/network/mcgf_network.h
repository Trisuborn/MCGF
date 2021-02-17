/************************************************
 * @file mcgf_network.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-17
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#ifndef MCGF_NETWORK_H
#define MCGF_NETWORK_H

#include <qt/inc/main_window.h>
#include <QNetworkAccessManager>

class mcgf_network : public QObject
{

    Q_OBJECT

public:
    mcgf_network();
    ~mcgf_network();

    void get_html(QString url = nullptr);

public slots:
    void network_reply_slot(QNetworkReply *reply);

private:
    QNetworkAccessManager *network_am;

};

#endif // MCGF_NETWORK_H
