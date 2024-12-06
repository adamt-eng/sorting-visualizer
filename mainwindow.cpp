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

QColor backgroundColor = Qt::black;
QColor barColor = Qt::white;

int bar1Index = -1, bar2Index = -1, bar3Index = -1, bar4Index = -1;

std::vector<int> array;
std::string sortingAlgorithm;
bool isAscending;
bool isContinuous;
bool stepTriggered;
int delayInMilliseconds;
bool shouldReset = false;
int elementsCount;
int maxHeight;

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

    maxHeight = *std::max_element(array.begin(), array.end());

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

        bar1Index = bar2Index = bar3Index = bar4Index = -1;

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
            if (isAscending)
            {
                insertionSortAscending();
            }
            else
            {
                insertionSortDescending();
            }
        }
        else if (sortingAlgorithm == "Heap Sort")
        {
            if (isAscending)
            {
                heapSortAscending();
            }
            else
            {
                heapSortDescending();
            }
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
        else if (sortingAlgorithm == "Gnome Sort")
        {
            if (isAscending)
            {
                gnomeSortAscending();
            }
            else
            {
                gnomeSortDescending();
            }
        }
    }
    else
    {
        shouldReset = true;
    }

    bar1Index = bar2Index = bar3Index = bar4Index = -1;
    visualize();

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

    double xPos = 0;

    for (int i = 0; i < elementsCount; ++i)
    {
        int barHeight = (array[i] * height) / maxHeight;

        QColor currentColor = barColor;
        if (i == bar1Index || i == bar2Index) currentColor = Qt::red;
        if (i == bar3Index) currentColor = Qt::green;
        if (i == bar4Index) currentColor = QColor(3, 181, 252);

        painter.fillRect(QRectF(xPos, height - barHeight, barWidth, barHeight), currentColor);

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
void MainWindow::on_invertThemeCheckBox_checkStateChanged(const Qt::CheckState &arg1)
{
    QColor temp = barColor;
    barColor = backgroundColor;
    backgroundColor = temp;
    visualize();
}
void MainWindow::on_themeComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Default")
    {
        backgroundColor = Qt::black;
        barColor = Qt::white;
    }
    else if (arg1 == "Night Glow")
    {
        backgroundColor = QColor(0, 31, 63);
        barColor = QColor(255, 220, 0);
    }
    else if (arg1 == "Retro Pop")
    {
        backgroundColor = QColor(243, 240, 224);
        barColor = QColor(0, 116, 217);
    }
    else if (arg1 == "Solar Burst")
    {
        backgroundColor = QColor(51, 51, 51);
        barColor = QColor(255, 133, 27);
    }
    else if (arg1 == "Zen Garden")
    {
        backgroundColor = QColor(61, 153, 112);
        barColor = QColor(221, 221, 221);
    }
    else if (arg1 == "Tropical Drift")
    {
        backgroundColor = QColor(55, 163, 163);
        barColor = QColor(255, 255, 255);
    }
    else if (arg1 == "Amethyst")
    {
        backgroundColor = QColor(237, 231, 246);
        barColor = QColor(96, 50, 168);
    }
    else if (arg1 == "Bold Sunset")
    {
        backgroundColor = QColor(44, 62, 80);
        barColor = QColor(241, 196, 15);
    }
    else if (arg1 == "Cyber")
    {
        backgroundColor = QColor(128, 0, 128);
        barColor = QColor(0, 255, 255);
    }
    else if (arg1 == "Autumn")
    {
        backgroundColor = QColor(90, 61, 49);
        barColor = QColor(255, 127, 80);
    }
    else if (arg1 == "Frosty Chill")
    {
        backgroundColor = QColor(50, 150, 200);
        barColor = QColor(220, 240, 255);
    }

    if (ui->invertThemeCheckBox->isChecked())
    {
        on_invertThemeCheckBox_checkStateChanged(ui->invertThemeCheckBox->checkState());
    }
    else
    {
        visualize();
    }
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
        for (int j = 0; j < elementsCount - i - 1; j++)
        {
            if (shouldReset) return;

            bar1Index = j;
            bar2Index = j + 1;

            waitForStep();
            visualize();
            wait();

            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                waitForStep();
                visualize();
                wait();
            }
        }

        bar1Index = -1;
        bar2Index = -1;

        visualize();
    }
}

void MainWindow::bubbleSortDescending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        for (int j = 0; j < elementsCount - i - 1; j++)
        {
            if (shouldReset) return;

            bar1Index = j;
            bar2Index = j + 1;

            waitForStep();
            visualize();
            wait();

            if (array[j] < array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                waitForStep();
                visualize();
                wait();
            }
        }

        bar1Index = -1;
        bar2Index = -1;

        visualize();
    }
}

void MainWindow::mergeAscending(int start, int mid, int end)
{
    std::vector<int> temp(end - start + 1); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    bar1Index = start;
    bar2Index = end;
    bar3Index = mid;

    waitForStep();
    visualize();
    wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        if (array[i] <= array[j])
        {
            temp[k++] = array[i++];
        }
        else
        {
            temp[k++] = array[j++];
        }
    }

    while (i <= mid)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar4Index = start + l;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        waitForStep();
        visualize();
        wait();
    }
}
void MainWindow::mergeSortAscending(int start, int end)
{
    if (shouldReset || start >= end) return;

    int mid = (start + end) / 2;

    mergeSortAscending(start, mid); // First half sort
    mergeSortAscending(mid + 1, end); // Second half sort

    bar1Index = start;
    bar2Index = end;
    bar3Index = mid;

    waitForStep();
    visualize();
    wait();

    mergeAscending(start, mid, end); // Merge both halves
}

void MainWindow::mergeDescending(int start, int mid, int end)
{
    std::vector<int> temp(end - start + 1); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    bar1Index = start;
    bar2Index = end;
    bar3Index = mid;

    waitForStep();
    visualize();
    wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        if (array[i] >= array[j])
        {
            temp[k++] = array[i++];
        }
        else
        {
            temp[k++] = array[j++];
        }
    }

    while (i <= mid)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar4Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar4Index = start + l;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        waitForStep();
        visualize();
        wait();
    }
}
void MainWindow::mergeSortDescending(int start, int end)
{
    if (shouldReset || start >= end) return;

    int mid = (start + end) / 2;

    mergeSortDescending(start, mid); // First half sort
    mergeSortDescending(mid + 1, end); // Second half sort

    bar1Index = start;
    bar2Index = end;
    bar3Index = mid;

    waitForStep();
    visualize();
    wait();

    mergeDescending(start, mid, end); // Merge both halves
}

int MainWindow::partitionAscending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;
    int temp; // For swapping purposes

    for (int j = start; j < end; j++)
    {
        if (shouldReset) return -1;

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
        if (shouldReset) return -1;

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

    visualize();
    wait();

    return i;
}

void MainWindow::quickSortAscending(int start, int end)
{
    if (shouldReset || start >= end) return;

    int pivotIndex = partitionAscending(start, end);

    if (pivotIndex == -1) return;

    quickSortAscending(start, pivotIndex - 1);
    quickSortAscending(pivotIndex + 1, end);
}
void MainWindow::quickSortDescending(int start, int end)
{
    if (shouldReset || start >= end) return;

    int pivotIndex = partitionDescending(start, end);

    if (pivotIndex == -1) return;

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

            bar4Index = i;

            waitForStep();
            visualize();
            wait();
        }
    }
    else
    {
        for (int i = 0; i < elementsCount; ++i)
        {
            int index = (place == 0) ? array[i] : (array[i] / place) % 10;
            output[elementsCount - count[index]] = array[i];
            count[index]--;

            bar4Index = i;

            waitForStep();
            visualize();
            wait();
        }
    }

    bar4Index = -1;
    visualize();

    // Copy back the sorted array and visualize each step
    for (int i = 0; i < elementsCount; ++i)
    {
        if (shouldReset) return;

        array[i] = output[i];

        bar1Index = i;

        waitForStep();
        visualize();
        wait();
    }

    bar1Index = -1;
    visualize();
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
        if (shouldReset) return;

        int currentMinIndex = i;

        bar1Index = currentMinIndex;
        waitForStep();
        visualize();
        wait();

        for (int j = i + 1; j < elementsCount; j++)
        {
            bar1Index = j;
            bar2Index = currentMinIndex;

            waitForStep();
            visualize();
            wait();

            if (array[j] < array[currentMinIndex])
            {
                currentMinIndex = j;
            }
        }

        if (currentMinIndex != i)
        {
            std::swap(array[currentMinIndex], array[i]);

            waitForStep();
            visualize();
            wait();
        }
    }
}
void MainWindow::selectionSortDescending()
{
    for (int i = 0; i < elementsCount - 1; i++)
    {
        if (shouldReset) return;

        int currentMaxIndex = i;

        bar1Index = currentMaxIndex;
        waitForStep();
        visualize();
        wait();

        for (int j = i + 1; j < elementsCount; j++)
        {
            bar1Index = j;
            bar2Index = currentMaxIndex;

            waitForStep();
            visualize();
            wait();

            if (array[j] > array[currentMaxIndex])
            {
                currentMaxIndex = j;
            }
        }

        if (currentMaxIndex != i)
        {
            std::swap(array[currentMaxIndex], array[i]);

            waitForStep();
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

            bar1Index = i;
            bar2Index = i + 1;

            waitForStep();
            visualize();
            wait();

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                waitForStep();
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

            bar1Index = i;
            bar2Index = i + 1;

            waitForStep();
            visualize();
            wait();

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                waitForStep();
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

            bar1Index = i;
            bar2Index = i + 1;

            waitForStep();
            visualize();
            wait();

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                waitForStep();
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

            bar1Index = i;
            bar2Index = i + 1;

            waitForStep();
            visualize();
            wait();

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);

                waitForStep();
                visualize();
                wait();

                swapped = true;
            }
        }

        ++start;
    }
}

void MainWindow::gnomeSortAscending()
{
    int index = 0;
    while (index < elementsCount)
    {
        if (shouldReset) return;

        if (index == 0 || array[index] >= array[index - 1])
        {
            index++;
        }
        else
        {
            bar1Index = index;
            bar2Index = index - 1;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);
            index--;

            waitForStep();
            visualize();
            wait();
        }
    }
}
void MainWindow::gnomeSortDescending()
{
    int index = 0;
    while (index < elementsCount)
    {
        if (shouldReset) return;

        if (index == 0 || array[index] <= array[index - 1])
        {
            index++;
        }
        else
        {
            bar1Index = index;
            bar2Index = index - 1;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);
            index--;

            waitForStep();
            visualize();
            wait();
        }
    }
}

void MainWindow::heapSortAscending()
{
    std::make_heap(array.begin(), array.end());

    visualize();
    wait();

    for(int i = 0; i < array.size()-1;i++)
    {
        if (shouldReset) return;

        std::pop_heap(array.begin(), array.end()-i);

        visualize();
        wait();
    }
}
void MainWindow::heapSortDescending()
{
    std::make_heap(array.begin(), array.end(), std::greater<int>());

    visualize();
    wait();

    for(int i = 0; i < array.size()-1;i++)
    {
        if (shouldReset) return;

        std::pop_heap(array.begin(), array.end()-i, std::greater<int>());

        visualize();
        wait();
    }
}

void MainWindow::insertionSortAscending()
{
    for (int i = 1; i < elementsCount; i++)
    {
        if (shouldReset) return;

        waitForStep();

        bar3Index = i;
        bar1Index = -1;
        bar2Index = -1;
        bar4Index = -1;

        waitForStep();
        visualize();
        wait();

        int index = i;

        while (index > 0 && array[index - 1] > array[index])
        {
            if (shouldReset) return;

            bar1Index = index - 1;
            bar2Index = index;
            bar3Index = i;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);

            waitForStep();
            visualize();
            wait();

            index--;
        }

        bar3Index = -1;
        bar1Index = -1;
        bar2Index = -1;
        bar4Index = index;

        waitForStep();
        visualize();
        wait();
    }
}
void MainWindow::insertionSortDescending()
{
    for (int i = 1; i < elementsCount; i++)
    {
        if (shouldReset) return;

        waitForStep();

        bar3Index = i;
        bar1Index = -1;
        bar2Index = -1;
        bar4Index = -1;

        waitForStep();
        visualize();
        wait();

        int index = i;

        while (index > 0 && array[index - 1] < array[index])
        {
            if (shouldReset) return;

            bar1Index = index - 1;
            bar2Index = index;
            bar3Index = i;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);

            waitForStep();
            visualize();
            wait();

            index--;
        }

        bar3Index = -1;
        bar1Index = -1;
        bar2Index = -1;
        bar4Index = index;

        waitForStep();
        visualize();
        wait();
    }
}
