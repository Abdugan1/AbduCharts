#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QtMath>
#include <QDebug>

class Node
{
public:
    explicit Node(double x = 0.0, double y = 0.0);

    void reset();

    bool visited() const;
    void setVisited(bool visited);

    float globalGoal() const;
    void setGlobalGoal(float globalGoal);

    float localGoal() const;
    void setLocalGoal(float localGoal);

    double x() const;
    void setX(double x);

    double y() const;
    void setY(double y);

    QVector<Node *> neighbours() const;
    void setNeighbours(const QVector<Node *> &neighbours);
    void appendNeighbour(Node* node);

    Node *parent() const;
    void setParent(Node *parent);

private:
    bool visited_ = false;
    float globalGoal_ = INFINITY;
    float localGoal_  = INFINITY;
    double x_ = 0.0;
    double y_ = 0.0;
    QVector<Node*> neighbours_;
    Node* parent_ = nullptr;
};

#endif // NODE_H
