#include <qt/inc/main_window.h>
#include "ui_main_window.h"

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

