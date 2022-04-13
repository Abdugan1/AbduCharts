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
    startNode->setLocalGoal(0.0f);
    startNode->setGlobalGoal(heuristic(startNode, endNode));

    QList<Node*> notTestedNodes;
    notTestedNodes.push_back(startNode);

    while (!notTestedNodes.empty() && currentNode != endNode) {

        // Sort untesded nodes by global goal, so lowest is first
        std::sort(notTestedNodes.begin(), notTestedNodes.end(), [](const Node* lhs, const Node* rhs) {
            return lhs->globalGoal() < rhs->globalGoal();
        });

        // Front of notTestedNodes is potentially the lowest distance node.
        // Out list may also contain that have been visited, so ditch these..
        while (!notTestedNodes.empty() && notTestedNodes.front()->visited())
            notTestedNodes.pop_front();

        // ... or abort because there are no valid nodes to test
        if (notTestedNodes.empty())
            break;

        currentNode = notTestedNodes.front();
        currentNode->setVisited(true); // Only explore a node once

        for (auto neighbourNode : currentNode->neighbours()) {
            if (!neighbourNode->visited())
                notTestedNodes.push_back(neighbourNode);

            float possiblyLowerGoal = calculateLocalGoal(currentNode, neighbourNode);

            if (possiblyLowerGoal < neighbourNode->localGoal()) {
                neighbourNode->setParent(currentNode);
                neighbourNode->setLocalGoal(possiblyLowerGoal);
                neighbourNode->setGlobalGoal(neighbourNode->localGoal() + heuristic(neighbourNode, endNode));
            }
        }
    }
}

float AStar::distance(const Node *a, const Node *b)
{
    return qSqrt((a->x() - b->x()) * (a->x() - b->x()) + (a->y() - b->y()) * (a->y() - b->y()));
}

float AStar::calculateLocalGoal(const Node *a, const Node *b)
{
    float localGoal = a->localGoal() + distance(a, b);

    // Avoid segmenting
    localGoal += StepPenalty * isTurned(a, b);

    return localGoal;
}

bool AStar::isTurned(const Node *a, const Node *b)
{
    return isTurned(a->parent(), a, b);
}

bool AStar::isTurned(const Node *before, const Node *current, const Node *after)
{
    bool turned = false;
    if (before) {
        if (current->x() == before->x()) {
            turned = (after->x() != current->x());
        } else {
            turned = (after->y() != current->y());
        }
    }
    return turned;
}

QList<QLineF> AStar::makePath(Node *endNode)
{
    QList<QLineF> path;
    if (endNode != nullptr) {
        Node* current = endNode;
        Node* before  = endNode;
        while (auto after = current->parent()) {

            if (isTurned(before, current, after)) {
                if (QPointF(before->x(), before->y()) != QPointF(current->x(), current->y()))
                    path.push_back({QPointF(before->x(), before->y()), QPointF(current->x(), current->y())});
                before = current;
            }
            current = after;
        }

        // last connection
        path.push_back({QPointF(before->x(), before->y()), QPointF(current->x(), current->y())});
    }
    return path;
}
