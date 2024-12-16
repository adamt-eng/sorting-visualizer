#include "../Header Files/secondwindow.h"
#include ".././ui_secondwindow.h"
#include "../Header Files/Data Structures/graph.h"
#include "../Source Files/Data Structures/pair.cpp"
#include <iostream>
#include <QMouseEvent>
#include <QInputDialog>
#include <QString>
#include <QWidget>
using namespace std;

Graph testGraph;

SecondWindow::SecondWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SecondWindow)
    , scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    // Link the scene to the QGraphicsView in the UI
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->weightInputLineEdit->setText("1");
    ui->messageUpdate->setText("");

    QWidget *mainWidget = this;  // 'this' is your main window
    mainWidget->setFocus(); // set focus on main window

    //connect(ui->pushButton, &QPushButton::clicked, this, &SecondWindow::printAdjacencyList);
    connect(ui->runButton, &QPushButton::clicked, this, &SecondWindow::runAlgorithm);

    ui->weightVisibilityCheckbox->setChecked(true); // Initially show all edges
    connect(ui->weightVisibilityCheckbox, &QCheckBox::checkStateChanged, this, [this]() {
        testGraph.changeWeightVisibility(ui->weightVisibilityCheckbox->isChecked());
    }); // checkbox

    connect(ui->resetButton, &QPushButton::clicked, this, [this]() {
        testGraph.resetGraph(ui->graphicsView);
    }); // reset button

}

void SecondWindow::runAlgorithm() {
    if (ui->startBox->text().toInt() < 0 || ui->startBox->text().toInt() >= testGraph.getVertices() || ui->startBox->text().isEmpty()) {
        ui->messageUpdate->setStyleSheet("color: red;");
        ui->messageUpdate->setText("Start node not found!");
        return;
    }
    else if(ui->endBox->text().toInt() < 0 || ui->endBox->text().toInt() >= testGraph.getVertices() || ui->endBox->text().isEmpty()){
        ui->messageUpdate->setStyleSheet("color: red;");
        ui->messageUpdate->setText("Goal node not found!");
        return;
    }
    else{
        ui->messageUpdate->setText("");
        QString selectedAlgorithm = ui->algorithmComboBox->currentText();

        if(selectedAlgorithm == "BFS") testGraph.BFS(ui->startBox->text().toInt(), ui->endBox->text().toInt(), scene);
        else if(selectedAlgorithm == "DFS") testGraph.DFS(ui->startBox->text().toInt(), ui->endBox->text().toInt(), scene);
        else testGraph.Dijkstra(ui->startBox->text().toInt(), ui->endBox->text().toInt(), scene);
    }
}

void SecondWindow::printAdjacencyList() {
    vector<std::vector<Pair<int, int>>> adjList = testGraph.getAdjacencyListVector();
    cout << "Adjacency List:" << endl;
    for (int i = 0; i < adjList.size(); i++) {
        for (auto it = adjList[i].begin(); it != adjList[i].end(); ++it) {
            cout << "[" << it->first << ", " << it->second << "] ";
        }
        cout << endl;
    }
}

void SecondWindow::mousePressEvent(QMouseEvent *event) // USER CAN CLICK THE SAME TWO NODES MORE THAN ONCE?! FIXED!
{
    // Check if the mouse press occurred within the QGraphicsView
    if (ui->graphicsView->underMouse()) {
        // Map the mouse event's position relative to the QGraphicsView
        QPoint viewPos = ui->graphicsView->mapFromParent(event->pos());
        // Then map the view-relative position to the scene coordinates
        QPointF scenePos = ui->graphicsView->mapToScene(viewPos);
        // SCENEPOS IS THE FINAL POS WE NEED

        // Flag to track if the clicked position is inside a node
        bool nodeClicked = false;

        // Variable to hold the node clicked during this event
        Graph::Node* clickedNode = nullptr;

        // Check if the click is inside any existing node
        for (auto node : testGraph.nodes) {
            if (node->contains(scenePos)) {
                clickedNode = node;
                nodeClicked = true;
                break;
            }
        }

        if (nodeClicked) {
            if (firstNodeClicked == nullptr) {
                // If it's the first click, store the clicked node
                firstNodeClicked = clickedNode;
                firstNodeClicked->setBrush(Qt::red);  // Optionally change color of the first node
            } else {
                // If it's the second click, create an edge between the two nodes
                if (firstNodeClicked != clickedNode) {
                    bool weightCheck; bool connectionCheck;
                    int weight = ui->weightInputLineEdit->text().toInt(&weightCheck);  // Get the weight from the line edit

                    if (weightCheck && weight > 0) {
                        // Use the weight from the input to add the edge
                        testGraph.addEdge(firstNodeClicked, clickedNode, weight, scene, connectionCheck);
                        if(connectionCheck == false){
                            ui->messageUpdate->setStyleSheet("color: red;");
                            ui->messageUpdate->setText("The two nodes are already connected!");
                        }

                        else {
                        // Reset node colors to blue
                        firstNodeClicked->setBrush(Qt::blue);
                        clickedNode->setBrush(Qt::blue);

                        ui->messageUpdate->setStyleSheet("color: green;");
                        ui->messageUpdate->setText("Nodes connected successfully!");

                        // Reset after the second click
                        firstNodeClicked = nullptr;
                        }
                    } else {
                        // Handle case if weight input is invalid
                        ui->messageUpdate->setStyleSheet("color: red;");
                        ui->messageUpdate->setText("Weight must be a positive integer!");
                    }
                }
            }
        } else {
            // If no node was clicked, add a new node at the clicked position
            testGraph.addNode(scenePos.x(), scenePos.y(), scene);
        }
    }
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

#include <QApplication>
#include "../Header Files/mainwindow.h"

void SecondWindow::on_runButton_2_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    this->hide();
}

