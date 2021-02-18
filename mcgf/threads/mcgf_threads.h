/******************************************************************
 * @file mcgf_threads.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-18
 *
 * @copyright Copyright (c) 2021
 *
 *******************************************************************/
#ifndef MCGF_THREADS_H
#define MCGF_THREADS_H

#include <mcgf/network/mcgf_network.h>

#include <QThread>
#include <QMutex>
#include <QString>

/******************************************************************
 * @brief HTML 线程
 *******************************************************************/
class dl_html : public QObject
{
    Q_OBJECT
public:
    dl_html(QString url, QString file_name, QString save_path);
    ~dl_html();

public slots:
    void download_start();
    void download_abort();

signals:
    void download_done(size_t fsize = 0);

private:
    mcgf_network *nw = nullptr;
    QString url = nullptr;
    QString file_name = nullptr;
    QString save_path = nullptr;
};

class dl_html_ctl : public QObject
{
    Q_OBJECT
public:
    dl_html_ctl(QString url, QString file_name, QString save_path);
    ~dl_html_ctl(void);

    void start(void);
    void stop(void);

public slots:

signals:
    void start_sig();
    void stop_sig();

private:
    QThread *dl_thread = nullptr;
    dl_html *html = nullptr;
};



#endif // MCGF_THREADS_H
