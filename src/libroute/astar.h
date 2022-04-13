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
    static float distance(const Node* a, const Node* b);

    static float calculateLocalGoal(const Node* a, const Node* b);

    static bool isTurned(const Node* a, const Node* b);
    static bool isTurned(const Node* before, const Node* current, const Node* after);


    static QList<QLineF> makePath(Node* endNode);

};

#endif // ASTAR_H
