/************************************************
 * @file mcgf_file.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-17
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#ifndef MCGF_FILE_H
#define MCGF_FILE_H

#include <QFile>
#include <QFileInfo>

/* mcgf file operation */
class mcgf_fo : public QObject
{
    Q_OBJECT
public:
    bool file_busy_flag = false;

    mcgf_fo();
    ~mcgf_fo();

    bool open_with_check(QFile *filp);

    bool _openw(QString path, const QString &wbuffer, QIODevice::OpenMode mode = QIODevice::WriteOnly);
    bool _openr(QString path, QString &rbuffer, QIODevice::OpenMode mode = QIODevice::WriteOnly);

    bool open_write(QString path, const QString &wbuffer);
    bool open_write_bytes(QString path, const QString &wbuffer);

private:


};

#endif // MCGF_FILE_H
