#include "astar.h"
#include "node.h"
#include "pointgraph.h"

#include <QLineF>

const int StepPenalty = 1;

QList<QLineF> AStar::findPath(Node *startNode, Node *endNode)
{
    solveAStar(startNode, endNode);

    return makePath(endNode);
}

void AStar::solveAStar(Node *startNode, Node *endNode)
{
    auto heuristic = [](Node* a, Node* b) {
        float h = distance(a, b);
        return h;
    };

    // Setup starting conditions
    Node* currentNode  = startNode;
    startNode->localGoal = 0.0f;
    startNode->globalGoal = heuristic(startNode, endNode);

    QList<Node*> notTestedNodes;
    notTestedNodes.push_back(startNode);

    while (!notTestedNodes.empty() && currentNode != endNode) {

        // Sort untesded nodes by global goal, so lowest is first
        std::sort(notTestedNodes.begin(), notTestedNodes.end(), [](const Node* lhs, const Node* rhs) {
            return lhs->globalGoal < rhs->globalGoal;
        });

        // Front of notTestedNodes is potentially the lowest distance node.
        // Out list may also contain that have been visited, so ditch these..
        while (!notTestedNodes.empty() && notTestedNodes.front()->visited)
            notTestedNodes.pop_front();

        // ... or abort because there are no valid nodes to test
        if (notTestedNodes.empty())
            break;

        currentNode = notTestedNodes.front();
        currentNode->visited = true; // Only explore a node once

        for (auto neighbourNode : qAsConst(currentNode->neighbours)) {
            if (!neighbourNode->visited)
                notTestedNodes.push_back(neighbourNode);

            float possiblyLowerGoal = calculateLocalGoal(currentNode, neighbourNode);

            if (possiblyLowerGoal < neighbourNode->localGoal) {
                neighbourNode->parent = currentNode;
                neighbourNode->localGoal = possiblyLowerGoal;
                neighbourNode->globalGoal = neighbourNode->localGoal + heuristic(neighbourNode, endNode);
            }
        }
    }
}

float AStar::distance(Node *a, Node *b)
{
    return qSqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

float AStar::calculateLocalGoal(Node *a, Node *b)
{
    float localGoal = a->localGoal + distance(a, b);

    // Avoid segmenting
    if (a->parent) {
        bool turned = false;
        if (a->x == a->parent->x) {
            turned = (b->x != a->x);
        } else {
            turned = (b->y != a->y);
        }
        localGoal += StepPenalty * turned;
    }

    return localGoal;
}

QList<QLineF> AStar::makePath(Node *endNode)
{
    QList<QLineF> path;
    if (endNode != nullptr) {
        Node* p = endNode;
        while (p->parent != nullptr) {
            path.push_back(QLineF{QPointF(p->x, p->y), QPointF(p->parent->x, p->parent->y)});
            p = p->parent;
        }
    }
    return path;
}
