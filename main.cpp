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

QString url1 = "https://www.cnblogs.com/lifan3a/articles/7691974.html";
QString url2 = "https://www.cnblogs.com/lifan3a/articles/7691974.html";
QString url3 = "https://files.minecraftforge.net/maven/net/minecraftforge/forge/1.12.1-14.22.1.2481/forge-1.12.1-14.22.1.2481-installer-win.exe";

QString cur_path = "F:\\work\\NoEmbed\\cpp\\qt\\MCGF\\";
QString file_name = "test.html";
QString fpath = cur_path + file_name;



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;

    main_win.show();


    mcgf_network *mcgf_netw = new mcgf_network;
//    mcgf_network::gh_params_t gh;
//    gh.file_name = file_name;
//    gh.save_path = cur_path;
//    mcgf_netw->get_html(url1, true, &gh);

    qDebug() << mcgf_netw->get_filename(url3);

    qDebug() << "file size:" << mcgf_netw->get_file_size(url3);

//    delete mcgf_netw;

    return app.exec();
}
