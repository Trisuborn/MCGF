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
#include <QTimer>

#define MCGF_MAX_NETWORK_NUM 50

class mcgf_network;
class mcgf_nw_manage;

/******************************************************************
 * @brief MCGF 网络类
 *******************************************************************/
class mcgf_network : public QObject
{
    Q_OBJECT
public:
    bool abort_flag = false;
    bool busy_flag = false;
    bool save_done_flag = false;
    size_t file_size = 0;

    QNetworkReply *nw_reply = nullptr;

    mcgf_network();
    ~mcgf_network();

    bool wait_reply(size_t timeout);
    void check_path(QString *path);

    bool get_html(QString url, QString file_name, QString save_path, size_t timeout);
    size_t get_file_size(QString url = nullptr);
    QString get_filename(QString url = nullptr);
    bool save_file(QString url, QString save_path, size_t pack_size = 512);

    void reply_abort_emit();

public slots:
    void html_reply_slot(QNetworkReply *reply = nullptr);
    void save_file_slot(QNetworkReply *reply = nullptr);
    void get_file_size_slot(QNetworkReply *reply = nullptr);

    void reply_abort_slot();

signals:
    void save_html_sig();
    void save_file_sig();           // TODO: lack parameter

    void reply_abort_sig();

private:
    QNetworkAccessManager *network_am = nullptr;
    QNetworkRequest *network_req = nullptr;

public:

};


/******************************************************************
 * @brief MCGF 网络管理类
 *******************************************************************/
class mcgf_nw_manage : public QObject
{
    Q_OBJECT
public:
    uint8_t mcgf_nw_cnt = 0;
    mcgf_network *mcgf_nw_stack[MCGF_MAX_NETWORK_NUM] = {nullptr};

    mcgf_nw_manage();
    ~mcgf_nw_manage();

    bool mcgf_nw_register(mcgf_network *obj);
    bool mcgf_nw_unregister(mcgf_network *obj);

    bool mcgf_nw_has_busy(void);

    void send_nw_all_abort_emit();

public slots:
    void mcgf_nw_abort_all_slot(void);

signals:
    void nw_all_abort_sig();

private:

};


extern mcgf_nw_manage mcgf_nw_mb;

#endif // MCGF_NETWORK_H
