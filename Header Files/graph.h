#ifndef GRAPH_H
#define GRAPH_H
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include "../Header Files/pair.h"

using namespace std;

class Graph {
public:
    // NODE CLASS
    class Node : public QGraphicsEllipseItem {
    private:
        int nodeNumber;
    public:
        Node(int x, int y, int radius = 20) : QGraphicsEllipseItem(x - radius, y - radius, 2 * radius, 2 * radius)
            , nodeNumber(nodeCounter++)   {
            setBrush(Qt::blue);

            // Number on top of node and its position
            QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(nodeNumber), this);
            textItem->setDefaultTextColor(Qt::white);
            textItem->setFont(QFont("Arial", 16));

            textItem->setPos(x - textItem->boundingRect().width() / 2,
                             y - textItem->boundingRect().height() / 2);
        }
        int getNodeNumber(){ return nodeNumber; }
    };

    // EDGE CLASS
    class Edge : public QGraphicsLineItem {
    public:
        Edge(QPointF start, QPointF end, int weight, Graph& graph) : QGraphicsLineItem(QLineF(start, end)) {
            setPen(QPen(Qt::black, 2));
            setZValue(-1);

            QPointF midPoint = QLineF(start, end).center();

            // Weight on top of edge and its position
            QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(weight), this);
            graph.weightLabels.push_back(textItem);
            textItem->setDefaultTextColor(Qt::red);  // Text color
            textItem->setFont(QFont("Arial", 14));  // Set the font (optional)

            textItem->setPos(midPoint.x() - textItem->boundingRect().width() / 2,
                             midPoint.y() - 10 - textItem->boundingRect().height() / 2);
        }
    };


    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<QGraphicsTextItem*> weightLabels;

    Graph();
    Graph(int vertices);

    void addNode(int x, int y, QGraphicsScene *scene);
    void changeWeightVisibility(bool check);
    void resetGraph(QGraphicsView *view);
    //void addEdge(int u, int v, int weight = 1); // weighted edge
    void BFS(int startNode, int goalNode, QGraphicsScene *scene);
    void DFS(int startNode, int goalNode, QGraphicsScene *scene);
    void Dijkstra(int startNode, int goalNode, QGraphicsScene *scene);
    void addEdge(Node* node1, Node* node2, int weight, QGraphicsScene *scene, bool &success);
    int getVertices();
    const std::vector<Pair<int, int>>& getAdjacencyList(int u) const;
    const std::vector<std::vector<Pair<int, int>>>& getAdjacencyListVector() const;


private:
    int static nodeCounter;
    int vertices_;
    std::vector<std::vector<Pair<int, int>>> adjList_;
};

#endif // GRAPH_H
