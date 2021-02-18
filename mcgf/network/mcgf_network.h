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
    bool nw_reply_flag = false;
    bool save_done_flag = false;

    mcgf_network();
    ~mcgf_network();

    bool wait_reply(size_t timeout);

    bool get_html(QString url, QString file_name, QString save_path, size_t timeout);
    size_t get_file_size(QString url = nullptr);
    QString get_filename(QString url = nullptr);
    bool save_file(QString url, QString save_path, size_t pack_size = 512);

public slots:
    void html_reply_slot(QNetworkReply *reply = nullptr);
    void save_file_slot(QNetworkReply *reply = nullptr);
    void get_file_size_slot(QNetworkReply *reply = nullptr);

signals:
    void save_html_sig();
    void save_file_sig();           // TODO: lack parameter

private:
    QNetworkReply *nw_reply = nullptr;
    QNetworkAccessManager *network_am = nullptr;
    QNetworkRequest *network_req = nullptr;
    size_t file_size = 0;

};

#endif // MCGF_NETWORK_H
