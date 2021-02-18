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
QString url2 = "https://zhuanlan.zhihu.com/p/53270619";
QString url3 = "https://files.minecraftforge.net/maven/net/minecraftforge/forge/1.12.1-14.22.1.2481/forge-1.12.1-14.22.1.2481-installer-win.exe";

QString cur_path = "F:\\work\\NoEmbed\\cpp\\qt\\MCGF\\";
QString file_name1 = "test1.html";




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;
    main_win.show();



    dl_html_ctl dhc(url1, 0, cur_path);
    dl_html_ctl dhc2(url2, file_name1, cur_path);
    dl_html_ctl dhc3(url1, file_name1, cur_path);
    dl_html_ctl dhc4(url2, file_name1, cur_path);
    dl_html_ctl dhc5(url1, file_name1, cur_path);
    dl_html_ctl dhc6(url1, file_name1, cur_path);
    dl_html_ctl dhc7(url3, file_name1, cur_path);
    dl_html_ctl dhc8(url3, file_name1, cur_path);

    dhc.start();
    dhc2.start();
    dhc3.start();
    dhc4.start();
    dhc5.start();
    dhc6.start();
    dhc7.start();
    dhc8.start();

//    mcgf_network nw;
//    nw.save_file(url3, cur_path);

//    nw.nw_reply->abort();

    return app.exec();
}
