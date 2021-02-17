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
#include <QThread>
#include <QMutex>

extern QMutex nw_qmutex;

class mcgf_nw_dl_th : public QThread
{
    Q_OBJECT
public:
    size_t dl_msec = 0;
    mcgf_nw_dl_th();
    void run();

public slots:
    void done_slot();

signals:
    void done_sig();

};

//class mcgf_nw_getname_th : public QThread
//{
//    Q_OBJECT
//public:
//    size_t dl_msec = 0;

//    mcgf_nw_getname_th();
//    ~mcgf_nw_getname_th();
//    void run();
//public slots:
//    void getname_done_slot();

//signals:
//    void getname_done_sig();

//};

class mcgf_network : public QObject
{
    Q_OBJECT
public:

    /* get html params */
    typedef struct {
        QNetworkReply *reply;
        QString file_name;
        QString save_path;
    }gh_params_t;

    mcgf_network();
    ~mcgf_network();
    bool get_html(QString url = nullptr, bool save = false, void *params = nullptr);
    size_t get_file_size(QString url = nullptr);
    QString get_filename(QString url = nullptr);
    bool save_file(QString url, QString save_path, size_t pack_size = 512);

public slots:
    void get_html_slot(QNetworkReply *reply = nullptr);
    void save_html_slot(gh_params_t *params = nullptr);
    void save_file_slot(QNetworkReply *reply);
    void get_file_size_slot(QNetworkReply *reply);

signals:
    void save_html_sig(gh_params_t *params);
    void save_file_sig();           // TODO: lack parameter

private:
    QNetworkAccessManager *network_am = nullptr;
    QNetworkRequest *network_req = nullptr;

    mcgf_nw_dl_th *dl_tid = nullptr;
//    mcgf_nw_getname_th * gf_tid = nullptr;

    bool save_flag = false;
    gh_params_t *args = nullptr;

    size_t file_size = 0;

};

#endif // MCGF_NETWORK_H
