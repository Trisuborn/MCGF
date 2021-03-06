#include <qt/inc/main_window.h>
#include <mcgf/network/mcgf_network.h>
#include "ui_main_window.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QMutex>

main_window::main_window() // : main_ui(new Ui::main_window)
{
    this->main_ui = new Ui::main_window;
    this->main_ui->setupUi(this);
    this->main_ui->progress_bar->setValue(25);
}

main_window::~main_window()
{
    delete main_ui;
}

void main_window::closeEvent(QCloseEvent *event)
{
    QMessageBox msg;
    QMessageBox::StandardButton msg_ret;

    /* 检查是否有network的进程正在运行 */
    if ( mcgf_nw_mb.mcgf_nw_has_busy() ) {
        qDebug() << "network is busy.";
        msg_ret = msg.question(
            nullptr,
            "Quit", "Network is running, exit?",
            msg.Yes | msg.No
        );
        if ( msg_ret == msg.Yes ) {
            // TODO:
            mcgf_nw_mb.send_nw_all_abort_emit();
            event->accept();
        } else
            event->ignore();
        return;
    }

    msg_ret = msg.question(
        nullptr,
        "Quit", "Are you sure you want to exit? ",
        msg.Yes | msg.No
    );
    if ( msg_ret == msg.Yes )
        event->accept();
    else
        event->ignore();

}


