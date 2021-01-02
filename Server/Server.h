#pragma once

#include <QtWidgets/QWidget>
#include "ui_Server.h"

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = Q_NULLPTR);

private:
    Ui::ServerClass ui;
};
