#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "ui_main_window.h"

class main_window : public QMainWindow
{
    Q_OBJECT
public:
    main_window();
    ~main_window();

    void closeEvent(QCloseEvent *event);

    Ui::main_window *main_ui;



};
#endif // MAIN_WINDOW_H
