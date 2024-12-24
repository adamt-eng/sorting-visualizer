#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include "Data Structures/graph.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui {
class SecondWindow;
}
QT_END_NAMESPACE

class SecondWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();
    void mousePressEvent(QMouseEvent *event) override;  // Override to handle mouse clicks
    void printAdjacencyList();
    void runAlgorithm();

private slots:
    void on_runButton_2_clicked();

private:
    Ui::SecondWindow *ui;
    QGraphicsScene *scene;  // Add a scene as a member
    Graph::Node* firstNodeClicked = nullptr;  // Initialize first node as nullptr (keep track of red node)

};
#endif // SECONDWINDOW_H
