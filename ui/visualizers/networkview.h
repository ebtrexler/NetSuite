#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include "RT_Network.h"

class NetworkView : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkView(QWidget *parent = nullptr);
    void setNetwork(TNetwork *network);

signals:
    void cellMoved(const std::wstring &name, int x, int y);
    void requestAddCell(int x, int y);
    void requestAddSynapse(const std::wstring &from, const std::wstring &to);
    void requestAddElectrode(const std::wstring &cellName);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    TNetwork *m_network;

    // Coordinate transform
    double m_scale = 1, m_offX = 0, m_offY = 0;
    double m_minX = 0, m_minY = 0;
    static constexpr double CellRadius = 25;
    static constexpr double Margin = 50;

    void computeTransform();
    QPointF toScreen(double cx, double cy);
    QPointF fromScreen(const QPointF &sp);
    std::wstring cellAt(const QPointF &screenPos);

    // Drag state
    bool m_dragging = false;
    bool m_transformLocked = false;
    std::wstring m_dragCell;
    QPoint m_dragStart;

    void drawCell(QPainter &p, const QString &name, const QPointF &pos, bool hasElectrode);
    void drawSynapse(QPainter &p, const QPointF &from, const QPointF &to);
    void drawElectrode(QPainter &p, const QPointF &cellPos);
};

#endif
