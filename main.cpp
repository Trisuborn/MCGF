/************************************************
 * @file main.cpp
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-02-17
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include <QApplication>
#include <QtNetwork>

#include <qt/inc/main_window.h>
#include <mcgf/network/mcgf_network.h>

QString url = "https://www.baidu.com/";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;

    mcgf_network *mcgf_netw = new mcgf_network;
    mcgf_netw->get_html(url);

    main_win.show();
    return app.exec();
}
