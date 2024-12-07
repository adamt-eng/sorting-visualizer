#include "../Header Files/mainwindow.h"
#include ".././ui_mainwindow.h"
#include "../Header Files/soundplayer.h"

#include <QRandomGenerator>
#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <QMessageBox>
#include <QRect>
#include <QKeyEvent>

#include <algorithm>
#include <random>

#include <SDL2/SDL.h>

// This boolean is used to avoid calling generateArray() again before the first visualization,
// as it was already called in the constructor of the UI
bool firstTry;

// UI Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(false); // Disables the resize icon for the window

    generateArray();
    firstTry = true;
}

// UI Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

SoundPlayer player;
bool variableSound;

QColor backgroundColor = Qt::black;
QColor barColor = Qt::white;

int bar1Index = -1, bar2Index = -1, bar3Index = -1, bar4Index = -1;

std::vector<int> heapElements;
std::vector<QColor> heapLevelColors = {  QColor::fromHsl(30, 150, 200), QColor::fromHsl(120, 150, 200), QColor::fromHsl(180, 150, 200), QColor::fromHsl(240, 150, 200), QColor::fromHsl(300, 150, 200)};

std::vector<int> array;
std::string sortingAlgorithm;
bool isAscending;
bool isContinuous;
bool stepTriggered;
int delayInMilliseconds;
bool shouldReset = false;
int elementsCount;
int maxHeight;

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

    for (int i = 1; i <= elementsCount; ++i) array.push_back(i);

    maxHeight = *std::max_element(array.begin(), array.end());

    // Shuffle the array randomly
    std::shuffle(array.begin(), array.end(), std::mt19937{std::random_device{}()});

    visualize();
}

// Start visualization button
void MainWindow::on_startButton_clicked()
{
    if (ui->startButton->text() == "Start")
    {
        heapElements.clear();

        ui->startButton->setText("Reset");

        shouldReset = false;

        bar1Index = bar2Index = bar3Index = bar4Index = -1;

        generateArray();

        // Read sorting method and order
        sortingAlgorithm = ui->comboBox->currentText().toStdString();
        isAscending = ui->ascendingRadioButton->isChecked();
        delayInMilliseconds = ui->delay->value();
        isContinuous = ui->continuousRadioButton->isChecked();

        std::string choice = ui->soundComboBox->currentText().toStdString();
        variableSound = (choice == "Variable");

        if (!variableSound)
        {
            bool wavLoaded;
            if (choice == "Beep")
            {
                wavLoaded = player.loadWAV(":/resources/sounds/beep.wav");
            }
            else if (choice == "Duck")
            {
                wavLoaded = player.loadWAV(":/resources/sounds/quack.wav");
            }
            else if (choice == "Cat")
            {
                wavLoaded = player.loadWAV(":/resources/sounds/meow.wav");
            }
            else if (choice == "Dog")
            {
                wavLoaded = player.loadWAV(":/resources/sounds/bark.wav");
            }

            if (!wavLoaded)
            {
                QMessageBox::warning(this, "Warning", "Failed to load audio file, sound will be set to variable.");
                variableSound = true;
            }
        }

        // Disable all controls to prevent modification
        ui->nextStepButton->setEnabled(!isContinuous);
        ui->soundComboBox->setEnabled(false);
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
                mergeSortAscending(0, elementsCount - 1);
            }
            else
            {
                mergeSortDescending(0, elementsCount - 1);
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

        player.stopSound();
        bar1Index = bar2Index = bar3Index = bar4Index = -1;
        visualize();
    }
    else
    {
        shouldReset = true;
    }

    // Re-enable controls
    ui->startButton->setText("Start");
    ui->soundComboBox->setEnabled(true);
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

        if (std::find(heapElements.begin(), heapElements.end(), i) != heapElements.end())
        {
            int level = static_cast<int>(std::log2(i + 1));
            currentColor = heapLevelColors[level % heapLevelColors.size()];
        }

        if (i == bar1Index || i == bar2Index) currentColor = Qt::red;
        if (i == bar3Index) currentColor = Qt::green;
        if (i == bar4Index) currentColor = QColor(3, 181, 252);

        painter.fillRect(QRectF(xPos, height - barHeight, barWidth, barHeight), currentColor);

        xPos += barWidth + gapWidth;
    }

    ui->textLabel->setPixmap(pixmap);
}

// Delay function to wait the time specified by the user in delayInMilliseconds
void MainWindow::wait()
{
    if (!isContinuous) return;
    QTime dieTime = QTime::currentTime().addMSecs(delayInMilliseconds);
    while (QTime::currentTime() < dieTime)
    {
        if (shouldReset) return; // For if the user uses a high delay but tries to reset
        else QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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

// Theme control
void MainWindow::on_invertThemeCheckBox_checkStateChanged(const Qt::CheckState &arg1)
{
    std::swap(barColor, backgroundColor);
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

// Sound control
void MainWindow::playSound(int i, int j)
{
    if (variableSound)
    {
        float minFrequency = 120.0f;
        float maxFrequency = 1212.0f;

        float value1 = static_cast<float>(array[i]) / static_cast<float>(elementsCount);
        float value2 = static_cast<float>(array[j]) / static_cast<float>(elementsCount);

        float frequency = minFrequency + ((value1 + value2) / 2.0f) * (maxFrequency - minFrequency);

        float totalDuration = delayInMilliseconds / 1000.0f;

        float attackTime = 0.10f * totalDuration;
        float decayTime = 0.15f * totalDuration;
        float sustainLevel = 0.6f;
        float sustainDuration = 0.50f * totalDuration;
        float releaseTime = 0.25f * totalDuration;

        player.playFrequencyWithEnvelope(frequency, attackTime, decayTime, sustainLevel, sustainDuration, releaseTime);
    }
    else
    {
        player.playSound();
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
                playSound(j, j + 1);
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
                playSound(j, j + 1);
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
    bar4Index = mid;

    waitForStep();
    visualize();
    wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        waitForStep();
        visualize();
        playSound(i, j);
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

        bar3Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar3Index = start + l;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        waitForStep();
        visualize();
        playSound(start + l, l);
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
    bar4Index = mid;

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
    bar4Index = mid;

    waitForStep();
    visualize();
    wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        waitForStep();
        visualize();
        playSound(i, j);
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

        bar3Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[i++];
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        waitForStep();
        visualize();
        wait();

        temp[k++] = array[j++];
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar3Index = start + l;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        waitForStep();
        visualize();
        playSound(start + l, l);
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
    bar4Index = mid;

    waitForStep();
    visualize();
    wait();

    mergeDescending(start, mid, end); // Merge both halves
}

int MainWindow::partitionAscending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;

    bar4Index = end;

    waitForStep();
    visualize();
    wait();

    for (int j = start; j < end; j++)
    {
        if (shouldReset) return -1;

        bar3Index = j;

        waitForStep();
        visualize();
        wait();

        if (array[j] < pivot)
        {
            i++;
            std::swap(array[i], array[j]);

            waitForStep();
            visualize();
            playSound(i, j);
            wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    i++;
    std::swap(array[i], array[end]);

    bar3Index = i;

    waitForStep();
    visualize();
    playSound(i, end);
    wait();

    return i;
}
void MainWindow::quickSortAscending(int start, int end)
{
    if (shouldReset || start >= end) return;

    bar1Index = start;
    bar2Index = end;

    waitForStep();
    visualize();
    playSound(start, end);
    wait();

    int pivotIndex = partitionAscending(start, end);
    if (pivotIndex == -1) return;

    quickSortAscending(start, pivotIndex - 1);
    quickSortAscending(pivotIndex + 1, end);
}

int MainWindow::partitionDescending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;

    bar4Index = end;

    waitForStep();
    visualize();
    wait();

    for (int j = start; j < end; j++)
    {
        if (shouldReset) return -1;

        bar3Index = j;

        waitForStep();
        visualize();
        wait();

        if (array[j] > pivot)
        {
            i++;
            std::swap(array[i], array[j]);

            waitForStep();
            visualize();
            playSound(i, j);
            wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    i++;
    std::swap(array[i], array[end]);

    bar3Index = i;

    waitForStep();
    visualize();
    playSound(i, end);
    wait();

    return i;
}
void MainWindow::quickSortDescending(int start, int end)
{
    if (shouldReset || start >= end) return;

    bar1Index = start;
    bar2Index = end;

    waitForStep();
    visualize();
    playSound(start, end);
    wait();

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
            playSound(i, i);
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
            playSound(i, i);
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
        playSound(i, i);
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
            if (shouldReset) return;

            bar1Index = j;
            bar2Index = currentMinIndex;

            waitForStep();
            visualize();
            playSound(j, j);
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
            playSound(currentMinIndex, i);
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
            if (shouldReset) return;

            bar1Index = j;
            bar2Index = currentMaxIndex;

            waitForStep();
            visualize();
            playSound(j, j);
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
            playSound(currentMaxIndex, i);
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
                playSound(i, i + 1);
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
                playSound(i, i + 1);
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
                playSound(i, i + 1);
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
                playSound(i, i + 1);
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
            playSound(index + 1, index);
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
            playSound(index + 1, index);
            wait();
        }
    }
}

void MainWindow::heapifyMax(std::vector<int>& heap, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    bar1Index = i;
    waitForStep();
    visualize();
    wait();

    if (std::find(heapElements.begin(), heapElements.end(), i) == heapElements.end())
    {
        heapElements.push_back(i);
        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

    if (left < n)
    {
        if (std::find(heapElements.begin(), heapElements.end(), left) == heapElements.end())
        {
            heapElements.push_back(left);
            waitForStep();
            visualize();
            playSound(left, left);
            wait();
        }

        if (heap[left] > heap[largest])
        {
            largest = left;
        }
    }

    if (right < n)
    {
        if (std::find(heapElements.begin(), heapElements.end(), right) == heapElements.end())
        {
            heapElements.push_back(right);
            waitForStep();
            visualize();
            playSound(right, right);
            wait();
        }

        if (heap[right] > heap[largest])
        {
            largest = right;
        }
    }

    if (largest != i)
    {
        std::swap(heap[i], heap[largest]);

        waitForStep();
        visualize();
        playSound(i, largest);
        wait();

        heapifyMax(heap, n, largest);
    }
}


void MainWindow::buildMaxdHeap(std::vector<int>& heap)
{
    int n = heap.size();
    heapElements.clear();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        if (shouldReset) return;

        heapifyMax(heap, n, i);

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::popMax(std::vector<int>& heap, int n)
{
    if (n == 0) return;

    std::swap(heap[0], heap[n - 1]);

    waitForStep();
    visualize();
    wait();

    heapifyMax(heap, n - 1, 0);

    waitForStep();
    visualize();
    wait();
}

void MainWindow::heapSortAscending()
{
    buildMaxdHeap(array);
    waitForStep();
    visualize();
    wait();

    for (int i = 0; i < array.size()-1;i++)
    {
        if (shouldReset) return;

        popMax(array, array.size()-i);

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::heapifyMin(std::vector<int>& heap, int n, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    bar1Index = i;
    waitForStep();
    visualize();
    wait();

    if (std::find(heapElements.begin(), heapElements.end(), i) == heapElements.end())
    {
        heapElements.push_back(i);
        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

    if (left < n)
    {
        if (std::find(heapElements.begin(), heapElements.end(), left) == heapElements.end())
        {
            heapElements.push_back(left);
            waitForStep();
            visualize();
            playSound(left, left);
            wait();
        }

        if (heap[left] < heap[smallest])
        {
            smallest = left;
        }
    }

    if (right < n)
    {
        if (std::find(heapElements.begin(), heapElements.end(), right) == heapElements.end())
        {
            heapElements.push_back(right);
            waitForStep();
            visualize();
            playSound(right, right);
            wait();
        }

        if (heap[right] < heap[smallest])
        {
            smallest = right;
        }
    }

    if (smallest != i)
    {
        std::swap(heap[i], heap[smallest]);

        waitForStep();
        visualize();
        playSound(i, smallest);
        wait();

        heapifyMin(heap, n, smallest);
    }
}
void MainWindow::buildMindHeap(std::vector<int>& heap)
{
    int n = heap.size();
    heapElements.clear();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        if (shouldReset) return;

        heapifyMin(heap, n, i);

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::popMin(std::vector<int>& heap, int n)
{
    if (n == 0) return;

    std::swap(heap[0], heap[n - 1]);

    waitForStep();
    visualize();
    wait();

    heapifyMin(heap, n - 1, 0);

    waitForStep();
    visualize();
    wait();
}

void MainWindow::heapSortDescending()
{
    buildMindHeap(array);
    waitForStep();
    visualize();
    wait();

    for (int i = 0; i < array.size() - 1; i++)
    {
        if (shouldReset) return;

        popMin(array, array.size() - i);

        waitForStep();
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
            playSound(index, index - 1);
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
            playSound(index, index - 1);
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

QRect originalGeometry, originalTextLabelGeometry;
void MainWindow::on_fullScreenButton_clicked()
{
    originalGeometry = this->geometry();
    originalTextLabelGeometry = ui->textLabel->geometry();

    this->statusBar()->hide();
    this->showFullScreen();

    QList<QWidget*> allWidgets = this->centralWidget()->findChildren<QWidget*>();
    for (auto widget : allWidgets)
    {
        if (widget != ui->textLabel)
        {
            widget->hide();
        }
    }

    ui->textLabel->setGeometry(this->centralWidget()->geometry());
    ui->textLabel->setPixmap(ui->textLabel->pixmap().scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    visualize();
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && this->isFullScreen())
    {
        this->statusBar()->show();
        this->showNormal();
        this->setGeometry(originalGeometry);

        QList<QWidget*> allWidgets = this->centralWidget()->findChildren<QWidget*>();
        for (auto widget : allWidgets)
        {
            widget->show();
        }

        ui->textLabel->setGeometry(originalTextLabelGeometry);

        visualize();

        QMessageBox* messageBox = new QMessageBox(this);
        messageBox->setWindowTitle("Notification");
        messageBox->setText("Fullscreen mode exited");
        messageBox->setStandardButtons(QMessageBox::Ok);
        messageBox->setModal(true);
        messageBox->show();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

