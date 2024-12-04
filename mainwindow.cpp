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
                bubbleSortDescending();
            }
        }
        else if (sortingAlgorithm == "Merge Sort")
        {
            if (isAscending)
            {
                mergeSortAscending(0, elementsCount);
            }
            else
            {
                mergeSortDescending(0, elementsCount);
            }
        }
        else if (sortingAlgorithm == "Quick Sort")
        {
            if (isAscending)
            {
                quickSortAscending(0, elementsCount - 1);
            }
            else
            {
                quickSortDescending(0, elementsCount - 1);
            }
        }
        else if (sortingAlgorithm == "Counting Sort")
        {
            countingSort(0);
        }
        else if (sortingAlgorithm == "Radix Sort")
        {
            radixSort();
        }
        else if (sortingAlgorithm == "Selection Sort")
        {
            if (isAscending)
            {
                selectionSortAscending();
            }
            else
            {
                selectionSortDescending();
            }
        }
        else if (sortingAlgorithm == "Insertion Sort")
        {

        }
        else if (sortingAlgorithm == "Heap Sort")
        {

        }
        else if (sortingAlgorithm == "Cocktail Sort")
        {
            if (isAscending)
            {
                cocktailSortAscending();
            }
            else
            {
                cocktailSortDescending();
            }
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

// Algorithms
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

void MainWindow::bubbleSortDescending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        waitForStep();

        for (int j = 0; j < elementsCount - i - 1; j++)
        {
            if (shouldReset) return;

            if (array[j] < array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                visualize();
                wait();
            }
        }
    }
}

void MainWindow::mergeAscending(int start, int mid, int end)
{
    std::vector<int> temp(end - start + 1); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        if (array[i] <= array[j])
        {
            temp[k] = array[i];
            i++;
            k++;
        }
        else
        {
            temp[k] = array[j];
            j++;
            k++;
        }
    }

    while (i <= mid)
    {
        if (shouldReset) return;

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        waitForStep();

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        visualize();
        wait();
    }
}

void MainWindow::mergeDescending(int start, int mid, int end)
{
    std::vector<int> temp(end - start + 1); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        if (array[i] >= array[j])
        {
            temp[k] = array[i];
            i++;
            k++;
        }
        else
        {
            temp[k] = array[j];
            j++;
            k++;
        }
    }

    while (i <= mid)
    {
        if (shouldReset) return;

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        waitForStep();

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        visualize();
        wait();
    }
}

void MainWindow::mergeSortAscending(int start, int end)
{
    if (shouldReset) return;

    if (start >= end) return;

    int mid = (start + end) / 2;

    mergeSortAscending(start, mid); // First half sort
    mergeSortAscending(mid + 1, end); // Second half sort
    mergeAscending(start, mid, end); // Merge both halves
}

void MainWindow::mergeSortDescending(int start, int end)
{
    if (shouldReset) return;

    if (start >= end) return;

    int mid = (start + end) / 2;

    mergeSortDescending(start, mid); // First half sort
    mergeSortDescending(mid + 1, end); // Second half sort
    mergeDescending(start, mid, end); // Merge both halves
}

int MainWindow::partitionAscending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;
    int temp; // For swapping purposes

    for (int j = start; j < end; j++)
    {
        if (array[j] < pivot)
        {
            waitForStep();

            i++;
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            visualize();
            wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    i++;
    temp = array[i];
    array[i] = array[end];
    array[end] = temp;

    visualize();
    wait();

    return i;
}

int MainWindow::partitionDescending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;
    int temp; // For swapping purposes

    for (int j = start; j < end; j++)
    {
        if (array[j] > pivot)
        {
            waitForStep();

            i++;
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            visualize();
            wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    i++;
    temp = array[i];
    array[i] = array[end];
    array[end] = temp;

    return i;
}

void MainWindow::quickSortAscending(int start, int end)
{
    if (shouldReset) return;

    if (start >= end) return;

    int pivotIndex = partitionAscending(start, end);

    quickSortAscending(start, pivotIndex - 1);
    quickSortAscending(pivotIndex + 1, end);
}

void MainWindow::quickSortDescending(int start, int end)
{
    if (shouldReset) return;

    if (start >= end) return;

    int pivotIndex = partitionDescending(start, end);

    quickSortDescending(start, pivotIndex - 1);
    quickSortDescending(pivotIndex + 1, end);
}

void MainWindow::countingSort(int place)
{
    int max = (place == 0) ? (*std::max_element(array.begin(), array.end()) + 1) : 10;

    std::vector<int> count(max, 0);
    std::vector<int> output(elementsCount);

    // Build the count array
    for (int i = 0; i < elementsCount; ++i)
    {
        int index = (place == 0) ? array[i] : (array[i] / place) % 10;
        count[index]++;
    }

    // Build cumulative count
    for (int i = 1; i < max; ++i)
    {
        count[i] += count[i - 1];
    }

    // Build the output array
    if (isAscending)
    {
        for (int i = elementsCount - 1; i >= 0; --i)
        {
            int index = (place == 0) ? array[i] : (array[i] / place) % 10;
            output[count[index] - 1] = array[i];
            count[index]--;
        }
    }
    else
    {
        for (int i = 0; i < elementsCount; ++i)
        {
            int index = (place == 0) ? array[i] : (array[i] / place) % 10;
            output[elementsCount - count[index]] = array[i];
            count[index]--;
        }
    }

    // Copy back the sorted array and visualize each step
    for (int i = 0; i < elementsCount; ++i)
    {
        array[i] = output[i];
        visualize();
        wait();
    }
}

void MainWindow::radixSort()
{
    int max = *std::max_element(array.begin(), array.end());

    for (int i = 1; max / i > 0; i *= 10)
    {
        countingSort(i);
    }
}


void MainWindow::selectionSortAscending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        waitForStep();

        int currentMinIndex = i;

        for (int j = i + 1; j < elementsCount; j++)
        {
            if (array[j] < array[currentMinIndex])
            {
                currentMinIndex = j;
            }

        }

        if (currentMinIndex != i)
        {
            std::swap(array[currentMinIndex], array[i]);

            visualize();
            wait();
        }
    }
}

void MainWindow::selectionSortDescending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        waitForStep();

        int currentMaxIndex = i;

        for (int j = i + 1; j < elementsCount; j++)
        {
            if (array[j] > array[currentMaxIndex])
            {
                currentMaxIndex = j;
            }

        }

        if (currentMaxIndex != i)
        {
            std::swap(array[currentMaxIndex], array[i]);

            visualize();
            wait();
        }
    }
}

void MainWindow::cocktailSortAscending()
{
    int start = 0;
    int end = elementsCount - 1;
    bool swapped = true;

    while (swapped)
    {
        waitForStep();

        swapped = false;

        // Forward pass
        for (int i = start; i < end; ++i)
        {
            if (shouldReset) return;

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                visualize();
                wait();

                swapped = true;
            }
        }

        if (!swapped) break;

        swapped = false;
        --end;

        // Backward pass
        for (int i = end - 1; i >= start; --i)
        {
            if (shouldReset) return;

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                visualize();
                wait();

                swapped = true;
            }
        }

        ++start;
    }
}

void MainWindow::cocktailSortDescending()
{
    int start = 0;
    int end = elementsCount - 1;
    bool swapped = true;

    while (swapped)
    {
        waitForStep();

        swapped = false;

        // Forward pass
        for (int i = start; i < end; ++i)
        {
            if (shouldReset) return;

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                visualize();
                wait();

                swapped = true;
            }
        }

        if (!swapped) break;

        swapped = false;
        --end;

        // Backward pass
        for (int i = end - 1; i >= start; --i)
        {
            if (shouldReset) return;

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                visualize();
                wait();

                swapped = true;
            }
        }

        ++start;
    }
}
