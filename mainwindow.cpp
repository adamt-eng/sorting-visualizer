#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QRandomGenerator>
#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <QMessageBox>

#include <algorithm>
#include <random>

// UI Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    generateArray();
    firstTry = true;
    this->statusBar()->setSizeGripEnabled(false); // Disables the resize icon for the window
}

// UI Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

std::vector<int> array; // Array to be sorted
std::string sortingAlgorithm;
bool isAscending;
int delayInMilliseconds;
bool shouldReset = false;

// This boolean is used to avoid calling generateArray() again before the first visualization,
// as it was already called in the constructor of the UI
bool firstTry;

// Function to generate array
void MainWindow::generateArray()
{
    if (firstTry)
    {
        firstTry = false;
        return;
    }

    // Clear array
    array.clear();

    // Fill the array with values increasing by 1
    for (int i = 0; i < ui->elementsCount->value(); ++i)
    {
        array.push_back(i + 1);
    }

    // Shuffle the array randomly
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(array.begin(), array.end(), g);

    visualize();
}

// Start visualization button
void MainWindow::on_startButton_clicked()
{
    if (ui->startButton->text() == "Start")
    {
        shouldReset = false;

        ui->startButton->setText("Reset");

        generateArray();

        // Read sorting method and order
        sortingAlgorithm = ui->comboBox->currentText().toStdString();
        isAscending = ui->ascendingRadioButton->isChecked();
        delayInMilliseconds = ui->delay->value();

        // Disable all controls to prevent modification
        ui->comboBox->setEnabled(false);
        ui->elementsCount->setEnabled(false);
        ui->delay->setEnabled(false);
        ui->ascendingRadioButton->setEnabled(false);
        ui->descendingRadioButton->setEnabled(false);

        // Set the timer event based on user selection of sorting method and order
        if (sortingAlgorithm == "Bubble Sort")
        {
            if (isAscending)
            {
                bubbleSortAscending();
            }
            else
            {
                bubbleSortDescending();
            }
        }
        else
        {
            // Other algorithms here
            // Each algorithm has two functions: ascending & descending, this is to avoid checking the value of isAscending when sorting
        }
    }
    else
    {
        shouldReset = true;
    }

    // Re-enable controls
    ui->startButton->setText("Start");
    ui->comboBox->setEnabled(true);
    ui->elementsCount->setEnabled(true);
    ui->delay->setEnabled(true);
    ui->ascendingRadioButton->setEnabled(true);
    ui->descendingRadioButton->setEnabled(true);
}

// Visualization function
void MainWindow::visualize()
{
    int width = ui->textLabel->width();
    int height = ui->textLabel->height();

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::black);

    QPainter painter(&pixmap);

    int elementsCount = array.size();
    int barWidth = (width - elementsCount + 1) / elementsCount;
    int maxHeight = *std::max_element(array.begin(), array.end());
    int xPos = 0;

    for (int i = 0; i < elementsCount; ++i)
    {
        int currentBarWidth = barWidth + (i < (width - elementsCount + 1) % elementsCount ? 1 : 0);
        int barHeight = (array[i] * height) / maxHeight;

        painter.fillRect(xPos, height - barHeight, currentBarWidth, barHeight, Qt::white);

        xPos += currentBarWidth + 1;
    }

    ui->textLabel->setPixmap(pixmap);
}

// Function for adding wait time between the graph updates
void MainWindow::wait()
{
    QTime dieTime = QTime::currentTime().addMSecs(delayInMilliseconds);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

// Update graph in realtime when user changes the elements count
void MainWindow::on_elementsCount_valueChanged(int arg1)
{
    visualize();
    generateArray();
}

// Algorithms
// 1. Bubble Sort
void MainWindow::bubbleSortAscending()
{
    for (int i = 0; i < array.size() - 1; i++)
    {
        for (int j = 0; j < array.size() - i - 1; j++)
        {
            if (shouldReset) return;

            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);
            }

            visualize();
            wait();
        }
    }
}
void MainWindow::bubbleSortDescending()
{
    for (int i = 0; i < array.size() - 1; i++)
    {
        for (int j = 0; j < array.size() - i - 1; j++)
        {
            if (shouldReset) return;

            if (array[j] < array[j + 1])
            {
                std::swap(array[j], array[j + 1]);
            }

            visualize();
            wait();
        }
    }
}
