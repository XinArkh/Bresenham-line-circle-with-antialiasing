#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QVector>
#include <QString>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void setDrawType(QString type);                                 // set draw type flag to line or circle
    void setAntiAliasing(bool ok);                                  // set antialiasing flag

protected:                                                          // rewrite mouse and paint events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void drawLine(QPainter &painter);                               // draw line with Bresenham Algorithm
    void drawCircle(QPainter &painter);                             // draw circle with Bresenham Algorithm
//    void drawPolygon(QPainter &painter);
    void drawPoint(int x, int y, QPainter &painter);                // draw single point
    void renderAntiAliasing(QPainter &painter);                     // render antialiasing mode

private:
    QImage *image;                                                  // painting image
    QPoint startPnt, endPnt;                                        // start and end points
    QVector<QPoint> pntSet;                                         // store the target points
    QString drawType;                                               // draw type flag, line or circle
    bool antiAliasingOpen;                                          // antialiasing flag
};

#endif // IMAGEWIDGET_H
