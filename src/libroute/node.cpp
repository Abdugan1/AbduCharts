#include "node.h"

Node::Node(double x, double y) {
    x_ = x;
    y_ = y;
}

void Node::reset()
{
    visited_ = false;
    globalGoal_ = INFINITY;
    localGoal_  = INFINITY;
//    neighbours_.clear();
    parent_ = nullptr;
}

Node *Node::parent() const
{
    return parent_;
}

void Node::setParent(Node *parent)
{
    parent_ = parent;
}

QVector<Node *> Node::neighbours() const
{
    return neighbours_;
}

void Node::setNeighbours(const QVector<Node *> &neighbours)
{
    neighbours_ = neighbours;
}

void Node::appendNeighbour(Node *node)
{
    neighbours_.append(node);
}

double Node::y() const
{
    return y_;
}

void Node::setY(double y)
{
    y_ = y;
}

double Node::x() const
{
    return x_;
}

void Node::setX(double x)
{
    x_ = x;
}

float Node::localGoal() const
{
    return localGoal_;
}

void Node::setLocalGoal(float localGoal)
{
    localGoal_ = localGoal;
}

float Node::globalGoal() const
{
    return globalGoal_;
}

void Node::setGlobalGoal(float globalGoal)
{
    globalGoal_ = globalGoal;
}

bool Node::visited() const
{
    return visited_;
}

void Node::setVisited(bool visited)
{
    visited_ = visited;
}
