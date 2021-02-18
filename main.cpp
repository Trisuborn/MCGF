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
#include <mcgf/file/mcgf_file.h>
#include <mcgf/threads/mcgf_threads.h>

QString url1 = "https://www.cnblogs.com/ybqjymy/p/12881760.html";
QString url2 = "https://blog.csdn.net/qq_40194498/article/details/80063960";
QString url3 = "https://files.minecraftforge.net/maven/net/minecraftforge/forge/1.12.1-14.22.1.2481/forge-1.12.1-14.22.1.2481-installer-win.exe";

QString cur_path = "F:\\work\\NoEmbed\\cpp\\qt\\MCGF\\";
QString file_name1 = "test1.html";
QString file_name2 = "test2.html";
QString file_name3 = "test3.html";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;
    main_win.show();

    mcgf_dl_html_th dl_html_th1(url1, 0, cur_path);
    mcgf_dl_html_th dl_html_th2(url2, 0, cur_path);

    dl_html_th1.start();
    dl_html_th2.start();

    return app.exec();
}
