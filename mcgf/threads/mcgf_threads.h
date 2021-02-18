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
 * @brief mcgf_thread
 *******************************************************************/
class mcgf_thread : public QThread
{
    Q_OBJECT
public:
    size_t th_run_time = 0;
    virtual void run();

public slots:
    virtual void done_slot();

signals:
    void done_sig();
};

/******************************************************************
 * @brief mcgf_dl_html_th
 *******************************************************************/
class mcgf_dl_html_th : public mcgf_thread
{
    Q_OBJECT
public:
    mcgf_dl_html_th();
    mcgf_dl_html_th(QString iurl, QString file_name, QString save_path);
    ~mcgf_dl_html_th();
    void run();

public slots:
    void done_slot();

private:
    int method = -1;
    mcgf_network *nw = nullptr;
    QString url = nullptr;
    QString fname = nullptr;
    QString sp = nullptr;
};

#endif // MCGF_THREADS_H
