#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "RT_Network.h"

class NetworkView : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkView(QWidget *parent = nullptr);
    
    void setNetwork(TNetwork *network);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    
private:
    TNetwork *m_network;
    
    void drawCell(QPainter &painter, const QString &name, const QPointF &pos);
    void drawConnection(QPainter &painter, const QPointF &from, const QPointF &to);
};

#endif // NETWORKVIEW_H
