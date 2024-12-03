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
#include <unordered_map>

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

QColor backgroundColor = Qt::black;
QColor barColor = Qt::white;

std::vector<int> array;
std::string sortingAlgorithm;
bool isAscending;
bool isContinuous;
bool stepTriggered;
int delayInMilliseconds;
bool shouldReset = false;
int elementsCount;

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

    elementsCount = ui->elementsCount->value();

    // Fill the array with values increasing by 1
    for (int i = 0; i < elementsCount; ++i)
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
        isContinuous = ui->continuousRadioButton->isChecked();

        // Disable all controls to prevent modification
        ui->nextStepButton->setEnabled(!isContinuous);
        ui->continuousRadioButton->setEnabled(false);
        ui->stepByStepRadioButton->setEnabled(false);
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
                // bubbleSortDescending();
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
    ui->nextStepButton->setEnabled(false);
    ui->continuousRadioButton->setEnabled(true);
    ui->stepByStepRadioButton->setEnabled(true);
    ui->ascendingRadioButton->setEnabled(true);
    ui->descendingRadioButton->setEnabled(true);
}

// Visualization function
void MainWindow::visualize()
{
    int width = ui->textLabel->width();
    int height = ui->textLabel->height();

    QPixmap pixmap(width, height);
    pixmap.fill(backgroundColor);

    QPainter painter(&pixmap);

    double gapWidth = 1.0;
    double barWidth = (width - (elementsCount - 1) * gapWidth) / elementsCount;

    // If barWidth is less than 1, make it 1 and reduce gapWidth
    if (barWidth < 1.0)
    {
        barWidth = 1.0;
        gapWidth = (width - elementsCount * barWidth) / (elementsCount - 1);
    }

    int maxHeight = *std::max_element(array.begin(), array.end());
    double xPos = 0;

    for (int i = 0; i < elementsCount; ++i)
    {
        int barHeight = (array[i] * height) / maxHeight;

        painter.fillRect(QRectF(xPos, height - barHeight, barWidth, barHeight), barColor);

        xPos += barWidth + gapWidth;
    }

    ui->textLabel->setPixmap(pixmap);
}

// Function for adding wait time between the graph updates
void MainWindow::wait()
{
    if (!isContinuous) return;
    QTime dieTime = QTime::currentTime().addMSecs(delayInMilliseconds);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

// Update graph in realtime when user changes the elements count
bool beingUpdated;
void MainWindow::revisualize()
{
    if (!beingUpdated)
    {
        beingUpdated = true;
        visualize();
        generateArray();
        beingUpdated = false;
    }
}
void MainWindow::on_elementsCount_valueChanged(int arg1)
{
    revisualize();
}
void MainWindow::on_elementsCount_textChanged(const QString &arg1)
{
    revisualize();
}

// Themes
void MainWindow::on_themeComboBox_currentTextChanged(const QString &arg1)
{
    static const std::unordered_map<std::string, std::pair<QColor, QColor>> themes =
        {
            { "Default", { Qt::black, Qt::white }},
            { "Night Glow", { QColor(0, 31, 63), QColor(255, 220, 0) }},
            { "Retro Pop", { QColor(243, 240, 224), QColor(0, 116, 217) }},
            { "Solar Burst", { QColor(51, 51, 51), QColor(255, 133, 27) }},
            { "Zen Garden", { QColor(61, 153, 112), QColor(221, 221, 221) }},
            { "Tropical Drift", { QColor(57, 204, 204), QColor(255, 255, 255) }},
            { "Monotone", { QColor(0, 0, 0), QColor(170, 170, 170) }},
            { "Bold Sunset", { QColor(44, 62, 80), QColor(241, 196, 15) }},
            { "Cyber", { QColor(128, 0, 128), QColor(0, 255, 255) }},
            { "Autumn", { QColor(90, 61, 49), QColor(255, 127, 80) }},
            { "Frosty Chill", { QColor(50, 150, 200), QColor(220, 240, 255) }}
        };

    std::string choice = arg1.toStdString();
    auto it = themes.find(choice);

    if (it != themes.end())
    {
        backgroundColor = it->second.first;
        barColor = it->second.second;
    }

    visualize();
}

// Next step trigger
void MainWindow::on_nextStepButton_clicked()
{
    stepTriggered = true;
}
void MainWindow::waitForStep()
{
    if (!isContinuous)
    {
        stepTriggered = false;
        while (!stepTriggered)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
}


//swap function

void swap(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
}

// Algorithms
// 1. Bubble Sort
void MainWindow::bubbleSortAscending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        waitForStep();

        for (int j = 0; j < elementsCount - i - 1; j++)
        {
            if (shouldReset) return;

            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                visualize();
                wait();
            }
        }
    }
}



// 2.Selection Sort



void MainWindow::selectionSortAscending(std::vector<int>& array, int size)
{
    int currentMin;
    int currentMinIndex;
    for (int i=0;i<size-1;i++)
    {
        currentMin=array[i];
        currentMinIndex=i;

        for(int j=i+1;j<size;j++)
        {
            if (currentMin>array[j])
            {
                currentMin=array[j];
                currentMinIndex=j;

            }
            visualize();
            wait();

        }

        if (currentMinIndex!=i)
        {
            //array[currentMinIndex]=array[i];
           // array[i]=currentMin;

            std::swap(array[currentMinIndex], array[i]);


            visualize();
            wait();
        }

    }
}
































