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

    /* get html params */
    typedef struct {
        QNetworkReply *reply;
        QString *file_name;
        QString *save_path;
    }gh_params_t;

    mcgf_network();
    ~mcgf_network();
    bool get_html(QString url = nullptr, bool save = false, void *params = nullptr);

public slots:
    void network_reply_slot(QNetworkReply *reply = nullptr);
    bool save_html(gh_params_t *params = nullptr);

signals:
    void save_html_sig(gh_params_t *params);

private:
    QNetworkAccessManager *network_am;

};

#endif // MCGF_NETWORK_H
