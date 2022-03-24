#ifndef ASTAR_H
#define ASTAR_H

#include <QList>

class Node;
class PointGraph;
class QLineF;

class AStar
{
public:
    static QList<QLineF> findPath(Node* startNode, Node* endNode);

    static void solveAStar(Node* startNode, Node* endNode);

private:
    static float distance(Node* a, Node* b);

    static float calculateLocalGoal(Node* a, Node* b);

    static QList<QLineF> makePath(Node* endNode);

};

#endif // ASTAR_H
