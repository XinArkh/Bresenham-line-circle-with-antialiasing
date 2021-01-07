#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imagewidget = new ImageWidget(this);
    setCentralWidget(imagewidget);

    InitDrawActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitDrawActions()
{
    QActionGroup *drawActionGroup = new QActionGroup(this);
    drawActionGroup->addAction(ui->actionLine);
    drawActionGroup->addAction(ui->actionCircle);
    drawActionGroup->addAction(ui->actionPolygon);
    ui->actionLine->setChecked(true);
}

void MainWindow::on_actionLine_toggled(bool arg1)
{
    if (arg1)
    {
        imagewidget->setDrawType(QString("line"));
    }
}

void MainWindow::on_actionCircle_toggled(bool arg1)
{
    if (arg1)
    {
        imagewidget->setDrawType(QString("circle"));
    }
}

//void MainWindow::on_actionPolygon_toggled(bool arg1)
//{
//    if (arg1)
//    {
//        imagewidget->setDrawType(QString("polygon"));
//    }
//}

void MainWindow::on_actionAliasing_toggled(bool arg1)
{
    imagewidget->setAntiAliasing(arg1);
}
