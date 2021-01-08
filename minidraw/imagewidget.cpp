#include "imagewidget.h"
#include <cmath>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    drawType = "line";
    antiAliasingOpen = false;
}

void ImageWidget::setDrawType(QString type)
{
    drawType = type;
}

void ImageWidget::setAntiAliasing(bool ok)
{
    if (ok) {
        antiAliasingOpen = true;
    }
    else
    {
        antiAliasingOpen = false;
    }
}


void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    startPnt = endPnt = event->pos();
    update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    endPnt = event->pos();
    update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseMoveEvent(event);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    image = new QImage(this->size(), QImage::Format_Grayscale8);        // initialize painting image
    image->fill(Qt::white);

    QPainter painterWidget(this);                                       // painter for operating widget
    QPainter painterImage(image);                                       // painter for operating image

    if (drawType == "line") {
        drawLine(painterImage);
    }
    else if (drawType == "circle") {
        drawCircle(painterImage);
    }
//    else if (drawType == "polygon") {
//        drawPolygon(painterImage);
//    }

    if (antiAliasingOpen) {                                             // antialiasing
        renderAntiAliasing(painterImage);
    }

    QVector<QPoint>().swap(pntSet);                                     // clear point storage safely
    painterWidget.drawImage(0, 0, *image);                              // visualize painting image
}

void ImageWidget::drawLine(QPainter &painter)
{
    // Bresenham Algorithm for drawing line
    // Reference: https://www.cnblogs.com/pheye/archive/2010/08/14/1799803.html
    int x0, y0, x1, y1;                                                 // start point and end point
    x0 = startPnt.x();
    y0 = startPnt.y();
    x1 = endPnt.x();
    y1 = endPnt.y();

    int dx, dy, ux, uy;
    dx = x1 - x0;                                                       // dx and dy: step size
    dy = y1 - y0;
    ux = 2 * (dx > 0) - 1;                                              // us and uy: direction
    uy = 2 * (dy > 0) - 1;

    int x = x0, y = y0, eps = 0;                                        // initialize x and y, eps is cumulative error
    dx = dx >= 0? dx: -dx;                                              // make x and y positive values
    dy = dy >= 0? dy: -dy;

    if (dx >= dy)                                                       // -1 < k <= 1, iterate x
    {
        for (x = x0; x != x1; x += ux) {
            drawPoint(x, y, painter);
            eps += dy;
            if (2 * eps >= dx) {
                y += uy;
                eps -= dx;
            }
        }
    }
    else                                                                // k > 1 or k < -1, iterate y
    {
        for (y = y0; y != y1; y += uy) {
            drawPoint(x, y, painter);
            eps += dx;
            if (2 * eps >= dy) {
                x += ux;
                eps -= dy;
            }
        }
    }
}

void ImageWidget::drawCircle(QPainter &painter)
{
    // Bresenham Algorithm for drawing circle
    // Reference: https://blog.csdn.net/gaoxiaoba/article/details/52154852
    int R= round(sqrt(pow(endPnt.x() - startPnt.x(), 2) + pow(endPnt.y() - startPnt.y(), 2)));
    int x = startPnt.x(), y = startPnt.y();
    int dx = 0, dy = R;
    int d = 3 - 2 * R;
    while (dx <= dy) {                                                  // draw eight mirror points in each loop
        drawPoint(x + dx, y + dy, painter);
        drawPoint(x + dx, y - dy, painter);
        drawPoint(x - dx, y + dy, painter);
        drawPoint(x - dx, y - dy, painter);
        drawPoint(x + dy, y + dx, painter);
        drawPoint(x + dy, y - dx, painter);
        drawPoint(x - dy, y + dx, painter);
        drawPoint(x - dy, y - dx, painter);

        if (d < 0) {                                                    // update d
            d += 4 * dx + 6;
        }
        else {
            d += 4 * (dx - dy) + 10;
            dy--;
        }
        dx++;
    }
}

//void ImageWidget::drawPolygon(QPainter &painter)
//{

//}

void ImageWidget::drawPoint(int x, int y, QPainter &painter)
{
    painter.setPen(Qt::black);                                          // set pen to black before drawing
    painter.drawPoint(x, y);                                            // draw target point
    pntSet.push_back(QPoint(x, y));                                     // store target point
}

void ImageWidget::renderAntiAliasing(QPainter &painter)
{
    int xmax, ymax;
    xmax = this->width();
    ymax = this->height();

    for (int i = 0; i < pntSet.size(); ++i) {
        int x, y;
        x = pntSet[i].x();                                              // take out the point in storage
        y = pntSet[i].y();

        if (x >= 0 + 1 && x < xmax - 1 && y >= 0 + 1 && y < ymax - 1) { // ensure target point inside window-1 (1px for possible rendering)
            int dx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};                 // position map in a 3x3 pixel matrix
            int dy[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
            float wi[9] = {1., 2., 1., 2., 4., 2., 1., 2., 1.};         // weight map

            for (int i = 0; i < 9; ++i) {                               // render around target point (3x3 pixels)
                    int c0, c1, c;
                    c0 = qGray(image->pixel(x + dx[i], y + dy[i]));     // current color
                    c1 = round(255 * (1 - wi[i] / wi[4]));              // new color
                    c = (255 - c0) + (255 - c1);                        // overlaye two colors in blackness
                    c = c <= 255? c: 255;                               // clip blackness to no more than 255
                    c = 255 - c;                                        // overlayed color in whiteness
                    painter.setPen(QColor(c, c, c));
                    painter.drawPoint(x + dx[i], y + dy[i]);            // draw color to pixel
//                    qDebug()<<1 - wi[i] / wi[4]<<c;
            }
        }
    }
}
