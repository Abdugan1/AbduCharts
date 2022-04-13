#include "pointgraph.h"
#include "node.h"

#include <QPointF>

PointGraph::PointGraph()
{

}

PointGraph::~PointGraph()
{
    for (auto& a : nodes_)
        qDeleteAll(a);
    nodes_.clear();
}

void PointGraph::add(const QPointF &point)
{
    double x = point.x();
    double y = point.y();
    Node *node = new Node(x, y);
    nodes_[x][y] = node;
}

void PointGraph::connect(const QPointF &a, const QPointF &b)
{
//    Q_ASSERT(nodes_.contains(a) && nodes_.contains(b));

    Node* nodeA = nodes_[a.x()][a.y()];
    Node* nodeB = nodes_[b.x()][b.y()];

    nodeA->appendNeighbour(nodeB);
}

Node *PointGraph::get(const QPointF &point) const
{
    return nodes_[point.x()][point.y()];
}

QMap<double, QMap<double, Node *> > PointGraph::nodes() const
{
    return nodes_;
}
