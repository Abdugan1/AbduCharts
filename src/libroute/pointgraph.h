#ifndef POINTGRAPH_H
#define POINTGRAPH_H

#include <QMap>

class Node;
class QPointF;

class PointGraph
{
public:
    explicit PointGraph();
    ~PointGraph();

    void add(const QPointF& point);

    void connect(const QPointF& a, const QPointF& b);

    Node *get(const QPointF& point) const;

    QMap<double, QMap<double, Node *> > nodes() const;

private:
    QMap<double, QMap<double, Node*>> nodes_;
};

#endif // POINTGRAPH_H
