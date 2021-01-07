#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitDrawActions();

private slots:
    void on_actionLine_toggled(bool arg1);

    void on_actionCircle_toggled(bool arg1);

//    void on_actionPolygon_toggled(bool arg1);

    void on_actionAliasing_toggled(bool arg1);

private:
    Ui::MainWindow *ui;
    ImageWidget *imagewidget;
};
#endif // MAINWINDOW_H
