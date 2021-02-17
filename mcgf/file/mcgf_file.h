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

class qt_file_operation : public QObject
{
    Q_OBJECT
public:
    bool openw(QString path, const char *wbuffer);
    bool opena(QString path, const char *wbuffer);
    bool openr(QString path, char *rbuffer);
    bool openrw(QString path, char *buffer);
private:
};

#endif // MCGF_FILE_H
