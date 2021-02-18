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

/* mcgf file operation */
class mcgf_fo : public QObject
{
    Q_OBJECT
public:
    bool file_busy_flag = false;

    mcgf_fo();
    ~mcgf_fo();

    bool open_with_check(QFile *file);

    bool openw(QString path, const char *wbuffer);
    bool opena(QString path, const char *wbuffer);
    bool openr(QString path, char *rbuffer);
    bool openrw(QString path, char *buffer);


};

#endif // MCGF_FILE_H
