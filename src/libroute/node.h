#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <QtMath>
#include <QDebug>

struct Node
{
    Node(double x = 0.0, double y = 0.0) {
        this->x = x;
        this->y = y;
    }

    void reset()
    {
        visited = false;
        globalGoal = INFINITY;
        localGoal  = INFINITY;
        parent = nullptr;
    }

    bool visited = false;
    float globalGoal = INFINITY;
    float localGoal  = INFINITY;
    double x = 0.0;
    double y = 0.0;
    QVector<Node*> neighbours;
    Node* parent = nullptr;
};

#endif // NODE_H
