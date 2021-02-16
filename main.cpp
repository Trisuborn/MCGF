#include <qt/inc/main_window.h>
#include <QApplication>
#include <QtNetwork>

class mcgf_network
{
public:
    mcgf_network(main_window *mw);
    ~mcgf_network();
private:

};

mcgf_network::mcgf_network(main_window *mw)
{
    mw->main_ui->progress_bar->setValue(99);
}

mcgf_network::~mcgf_network()
{

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    main_window main_win;

    mcgf_network mcgf_netw = mcgf_network(&main_win);



    main_win.show();
    return app.exec();
}
