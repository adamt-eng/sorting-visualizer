#include "../Header Files/graph.h"
#include "../Source Files/pair.cpp"
#include <queue>
#include <stack>
#include <QThread>
#include <QGraphicsView>
#include <QCoreApplication>

int Graph::nodeCounter = 0;

Graph::Graph() : vertices_(0), adjList_(0) {}

Graph::Graph(int vertices) : vertices_(vertices), adjList_(vertices) {}

void Graph::resetGraph(QGraphicsView *view) {
    QGraphicsScene *scene = view->scene();
    // Remove all nodes from the scene
    for (auto& node : nodes) {
        scene->removeItem(node);
        delete node;
    }
    nodes.clear();

    // Remove all weight labels from the scene
    for (auto& label : weightLabels) {
        scene->removeItem(label);
        delete label;
    }
    weightLabels.clear();

    // Remove all edges from the scene
    for (auto& edge : edges) {
        scene->removeItem(edge);
        delete edge;
    }
    edges.clear();

    // Clear adjacency list
    adjList_.clear();

    vertices_ = 0;
    nodeCounter = 0;
}


void Graph::addNode(int x, int y, QGraphicsScene *scene){
    Node *newNode = new Node(x, y);

    nodes.push_back(newNode);
    adjList_.resize(nodes.size()); // Resize vector to fit the number of nodes

    scene->addItem(newNode);
    vertices_++;
}

void Graph::addEdge(Node* node1, Node* node2, int weight, QGraphicsScene *scene, bool &success) {

    for (auto& i : adjList_[node1->getNodeNumber()]) {
        if (i.first == node2->getNodeNumber()) {
            success = false;
            return;
        }
    }

    success = true;
    Edge *newEdge = new Edge(node1->rect().center(), node2->rect().center(), weight, *this);
    edges.push_back(newEdge);

    scene->addItem(newEdge);
    adjList_[node1->getNodeNumber()].emplace_back(node2->getNodeNumber(), weight);
    adjList_[node2->getNodeNumber()].emplace_back(node1->getNodeNumber(), weight); // For undirected graph
}

void Graph::changeWeightVisibility(bool check){
    for (auto weightLabel : weightLabels) {
        weightLabel->setVisible(check);
    }
}

void Graph::BFS(int startNode, int goalNode, QGraphicsScene *scene) {
    bool endflag = 0;

    // Create a visited vector to keep track of visited nodes
    std::vector<bool> visited(vertices_, false);
    std::queue<int> q;

    // Mark the start node as visited and change its color to green
    visited[startNode] = true;
    q.push(startNode);
    nodes[startNode]->setBrush(Qt::green);  // Color start node as green

    // Mark the goal node as red initially so it's visible from the start
    nodes[goalNode]->setBrush(Qt::red);  // Color goal node as red

    // Process events to ensure UI updates immediately
    QCoreApplication::processEvents();
    QThread::msleep(1000);  // Wait for 1 second before visiting next node

    // Traverse the graph using BFS
    while (!q.empty()) {
        int node = q.front();

        nodes[node]->setBrush(Qt::yellow);
        QCoreApplication::processEvents();
        QThread::msleep(1000);
        if(nodes[node]->getNodeNumber() == goalNode){
            break;
        }

        q.pop();

        // Explore the neighbors of the current node
        for (const auto& neighbor : adjList_[node]) {
            int neighborNode = neighbor.first;

            if (!visited[neighborNode]) {
                visited[neighborNode] = true;
                q.push(neighborNode);
            }
        }
    }

    // Optionally, reset node colors to blue after traversal if the goal was reached
    for (auto& node : nodes) {
        node->setBrush(Qt::blue);
    }
}

void Graph::DFS(int startNode, int goalNode, QGraphicsScene *scene) {
    bool endflag = 0;

    // Create a visited vector to keep track of visited nodes
    std::vector<bool> visited(vertices_, false);
    std::stack<int> stack;

    // Mark the start node as visited and change its color to green
    visited[startNode] = true;
    stack.push(startNode);
    nodes[startNode]->setBrush(Qt::green);  // Color start node as green

    // Mark the goal node as red initially so it's visible from the start
    nodes[goalNode]->setBrush(Qt::red);  // Color goal node as red

    // Process events to ensure UI updates immediately
    QCoreApplication::processEvents();
    QThread::msleep(1000);  // Wait for 1 second before visiting next node

    // Traverse the graph using BFS
    while (!stack.empty()) {

        int node = stack.top();

        nodes[node]->setBrush(Qt::yellow);
        QCoreApplication::processEvents();
        QThread::msleep(1000);
        if(nodes[node]->getNodeNumber() == goalNode){
            break;
        }

        stack.pop();

        // Explore the neighbors of the current node
        for (auto it = adjList_[node].rbegin(); it != adjList_[node].rend(); ++it) {
            int neighborNode = it->first;

            if (!visited[neighborNode]) {
                visited[neighborNode] = true;
                stack.push(neighborNode);
            }
        }
    }

    // Optionally, reset node colors to blue after traversal if the goal was reached
    for (auto& node : nodes) {
        node->setBrush(Qt::blue);
    }
}

void Graph::Dijkstra(int startNode, int goalNode, QGraphicsScene *scene) {
    // Priority queue to select the node with the smallest distance
    using NodeDistPair = Pair<int, int>; // {distance, node}
    std::priority_queue<NodeDistPair, std::vector<NodeDistPair>, std::greater<>> pq;

    // Distance vector, initialized to infinity
    std::vector<int> dist(vertices_, INT_MAX);
    dist[startNode] = 0;

    // Visited vector
    std::vector<bool> visited(vertices_, false);

    // Push the start node into the priority queue
    pq.push({0, startNode});
    nodes[startNode]->setBrush(Qt::green); // Color start node as green

    // Mark the goal node as red initially
    nodes[goalNode]->setBrush(Qt::red); // Color goal node as red

    // Process events to ensure UI updates immediately
    QCoreApplication::processEvents();

    while (!pq.empty()) {
        int currentNode = pq.top().second;
        pq.pop();

        // Skip if the node is already visited
        if (visited[currentNode]) continue;
        visited[currentNode] = true;

        // Color the current node as yellow
        nodes[currentNode]->setBrush(Qt::yellow);
        QCoreApplication::processEvents();
        QThread::msleep(1000); // Wait for 1 second for visualization

        // If we reach the goal node, terminate
        if (currentNode == goalNode) break;

        // Explore neighbors
        for (const auto& neighbor : adjList_[currentNode]) {
            int neighborNode = neighbor.first;
            int weight = neighbor.second;

            // Relaxation step
            if (dist[currentNode] + weight < dist[neighborNode]) {
                dist[neighborNode] = dist[currentNode] + weight;
                pq.push({dist[neighborNode], neighborNode});
            }
        }
    }

    // Optionally, reset node colors to blue after traversal if the goal was reached
    for (auto& node : nodes) {
        node->setBrush(Qt::blue);
    }
}


int Graph::getVertices(){
    return vertices_;
}

const std::vector<Pair<int, int>>& Graph::getAdjacencyList(int u) const {
    return adjList_[u];
}

const std::vector<std::vector<Pair<int, int>>>& Graph::getAdjacencyListVector() const{
    return adjList_;
}