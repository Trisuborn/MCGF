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

QString url = "http://files.minecraftforge.net/maven/net/minecraftforge/forge/index_1.12.1.html";
QString cur_path = "F:\\work\\NoEmbed\\cpp\\qt\\MCGF\\";
QString file_name = "test.html";
QString fpath = cur_path + file_name;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;

    mcgf_network *mcgf_netw = new mcgf_network;
    mcgf_network::gh_params_t gh;
    gh.file_name = file_name;
    gh.save_path = cur_path;
    mcgf_netw->get_html(url, true, &gh);

    main_win.show();
    return app.exec();
}
