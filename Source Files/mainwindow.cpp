#include "../Header Files/mainwindow.h"
#include ".././ui_mainwindow.h"
#include "../Header Files/soundplayer.h"
#include "../Source Files/vector.cpp"

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

vector<int> sortedElements;

QColor backgroundColor = Qt::black;
QColor barColor = Qt::white;

int bar1Index = -1, bar2Index = -1, bar3Index = -1, bar4Index = -1;

vector<int> heapElements;
vector<QColor> heapLevelColors = { QColor::fromHsl(30, 150, 200), QColor::fromHsl(120, 150, 200), QColor::fromHsl(180, 150, 200), QColor::fromHsl(240, 150, 200), QColor::fromHsl(300, 150, 200) };

vector<int> array;
std::string sortingAlgorithm;
bool isAscending = true;
bool isContinuous;
bool stepTriggered;
int delayInMilliseconds;
bool shouldReset = false;
int elementsCount;
int maxHeight;

int comparisonCount = 0;
int arrayAccessCount = 0;

QString getAlgorithmComplexity(const std::string& algorithm)
{
    if (algorithm == "Bubble Sort") return "O(n^2)";
    if (algorithm == "Merge Sort") return "O(n log n)";
    if (algorithm == "Quick Sort") return "O(n log n)";
    if (algorithm == "Counting Sort") return "O(n + k)";
    if (algorithm == "Radix Sort") return "O(nk)";
    if (algorithm == "Selection Sort") return "O(n^2)";
    if (algorithm == "Insertion Sort") return "O(n^2)";
    if (algorithm == "Heap Sort") return "O(n log n)";
    if (algorithm == "Cocktail Sort") return "O(n^2)";
    if (algorithm == "Gnome Sort") return "O(n^2)";
    if (algorithm == "Bogo Sort") return "O((n-1)!)";
    return "N/A";
}

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

        comparisonCount = 0;
        arrayAccessCount = 0;

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
            if (choice != "No Sound")
            {
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
        }

        // Disable all controls to prevent modification
        ui->nextStepButton->setEnabled(!isContinuous);
        ui->soundComboBox->setEnabled(false);
        ui->continuousRadioButton->setEnabled(false);
        ui->stepByStepRadioButton->setEnabled(false);
        ui->comboBox->setEnabled(false);
        ui->elementsCount->setEnabled(false);
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
        else if (sortingAlgorithm == "Bogo Sort")
        {
            bogoSort();
        }

        if (!shouldReset)
        {
            for (int i = 0; i < elementsCount; ++i)
            {
                sortedElements.push_back(i);
            }
        }

        bar1Index = bar2Index = bar3Index = bar4Index = -1;

        visualize();

        sortedElements.clear();
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

    int textHeight = 20;
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    QString complexity = getAlgorithmComplexity(sortingAlgorithm);
    QString algorithmText = QString("Algorithm: %1").arg(QString::fromStdString(sortingAlgorithm));
    QString complexityText = QString("Complexity: %1").arg(complexity);

    if (isAscending)
    {
        painter.drawText(10, textHeight, algorithmText);
        painter.drawText(10, 2 * textHeight, complexityText);

        QString comparisonText = QString("Comparisons: %1").arg(comparisonCount);
        QString accessText = QString("Array Accesses: %1").arg(arrayAccessCount);

        painter.drawText(10, 3 * textHeight, comparisonText);
        painter.drawText(10, 4 * textHeight, accessText);
    }
    else
    {
        int algorithmTextWidth = painter.fontMetrics().horizontalAdvance(algorithmText);
        int complexityTextWidth = painter.fontMetrics().horizontalAdvance(complexityText);

        painter.drawText(width - algorithmTextWidth - 10, textHeight, algorithmText);
        painter.drawText(width - complexityTextWidth - 10, 2 * textHeight, complexityText);

        QString comparisonText = QString("Comparisons: %1").arg(comparisonCount);
        QString accessText = QString("Array Accesses: %1").arg(arrayAccessCount);

        int comparisonTextWidth = painter.fontMetrics().horizontalAdvance(comparisonText);
        int accessTextWidth = painter.fontMetrics().horizontalAdvance(accessText);

        painter.drawText(width - comparisonTextWidth - 10, 3 * textHeight, comparisonText);
        painter.drawText(width - accessTextWidth - 10, 4 * textHeight, accessText);
    }

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
        int barHeight = (array[i] * (height - 5 * textHeight)) / maxHeight;

        QColor currentColor = barColor;

        if (std::find(heapElements.begin(), heapElements.end(), i) != heapElements.end())
        {
            int level = static_cast<int>(std::log2(i + 1));
            currentColor = heapLevelColors[level % heapLevelColors.size()];
        }

        if (i == bar1Index || i == bar2Index) currentColor = Qt::red;
        else if (std::find(sortedElements.begin(), sortedElements.end(), i) != sortedElements.end() || i == bar3Index) currentColor = Qt::green;
        else if (i == bar4Index) currentColor = QColor(3, 181, 252);

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

// Update delay as user changes it
void MainWindow::on_delay_textChanged(const QString &arg1)
{
    delayInMilliseconds = ui->delay->value();
}
void MainWindow::on_delay_valueChanged(int arg1)
{
    delayInMilliseconds = ui->delay->value();
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
    if (ui->soundComboBox->currentText() == "No Sound") return;

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

            comparisonCount++;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            wait();

            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                arrayAccessCount += 2;

                waitForStep();
                visualize();
                playSound(j, j + 1);
                wait();
            }
        }

        sortedElements.push_back(elementsCount - i - 1);

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

            comparisonCount++;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            wait();

            if (array[j] < array[j + 1])
            {
                std::swap(array[j], array[j + 1]);

                arrayAccessCount += 2;

                waitForStep();
                visualize();
                playSound(j, j + 1);
                wait();
            }
        }

        sortedElements.push_back(elementsCount - i - 1);

        bar1Index = -1;
        bar2Index = -1;

        visualize();
    }
}

void MainWindow::mergeAscending(int start, int mid, int end)
{
    vector<int> temp(end - start + 1,0); // end - start + 1 is the size of merged array

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

        comparisonCount++;
        arrayAccessCount+=3;

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

        arrayAccessCount++;

        temp[k++] = array[i++];

        waitForStep();
        visualize();
        wait();
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        arrayAccessCount++;

        temp[k++] = array[j++];

        waitForStep();
        visualize();
        wait();
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar3Index = start + l;

        arrayAccessCount++;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        if (start == 0 && end == elementsCount - 1)
        {
            sortedElements.push_back(start + l);
        }

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
    vector<int> temp(end - start + 1,0); // end - start + 1 is the size of merged array

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

        comparisonCount++;
        arrayAccessCount+=3;

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
        arrayAccessCount++;

        temp[k++] = array[i++];

        waitForStep();
        visualize();
        wait();
    }

    while (j <= end)
    {
        if (shouldReset) return;

        bar3Index = start + k;

        arrayAccessCount++;

        temp[k++] = array[j++];

        waitForStep();
        visualize();
        wait();
    }

    for (int l = 0; l < temp.size(); l++)
    {
        bar3Index = start + l;

        arrayAccessCount++;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        if (start == 0 && end == elementsCount - 1)
        {
            sortedElements.push_back(start + l);
        }

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

    arrayAccessCount++;

    waitForStep();
    visualize();
    wait();

    for (int j = start; j < end; j++)
    {
        if (shouldReset) return -1;

        bar4Index = j;

        arrayAccessCount++;
        comparisonCount++;

        waitForStep();
        visualize();
        wait();

        if (array[j] < pivot)
        {
            i++;
            std::swap(array[i], array[j]);
            arrayAccessCount+=2;

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

    arrayAccessCount+=2;

    bar4Index = i;

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

    sortedElements.push_back(pivotIndex);

    quickSortAscending(start, pivotIndex - 1);
    quickSortAscending(pivotIndex + 1, end);

    visualize();
}

int MainWindow::partitionDescending(int start, int end)
{
    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;

    arrayAccessCount++;

    waitForStep();
    visualize();
    wait();

    for (int j = start; j < end; j++)
    {
        if (shouldReset) return -1;

        bar4Index = j;

        arrayAccessCount++;
        comparisonCount++;

        waitForStep();
        visualize();
        wait();

        if (array[j] > pivot)
        {
            i++;
            std::swap(array[i], array[j]);
            arrayAccessCount+=2;

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

    arrayAccessCount+=2;

    bar4Index = i;

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

    sortedElements.push_back(pivotIndex);

    quickSortDescending(start, pivotIndex - 1);
    quickSortDescending(pivotIndex + 1, end);
}

void MainWindow::countingSort(int place)
{
    int max = (place == 0) ? (*std::max_element(array.begin(), array.end()) + 1) : 10;

    vector<int> count(max, 0);
    vector<int> output(elementsCount);

    // Build the count array
    for (int i = 0; i < elementsCount; ++i)
    {
        int index = (place == 0) ? array[i] : (array[i] / place) % 10;
        count[index]++;
        arrayAccessCount++;
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
            int element = array[i];
            int index = (place == 0) ? element : (element / place) % 10;
            output[count[index] - 1] = element;
            count[index]--;

            arrayAccessCount++;

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
            int element = array[i];
            int index = (place == 0) ? element : (element / place) % 10;
            output[elementsCount - count[index]] = element;
            count[index]--;

            arrayAccessCount++;

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

        arrayAccessCount++;

        bar1Index = i;

        if (place == 0)
        {
            sortedElements.push_back(i);
        }

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

    for (int i = 0; i < elementsCount; ++i)
    {
        sortedElements.push_back(i);

        bar1Index = i;

        waitForStep();
        visualize();
        playSound(i, i);
        wait();
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

            comparisonCount++;
            arrayAccessCount+=2;

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
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            playSound(currentMinIndex, i);
            wait();
        }

        sortedElements.push_back(i);
        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

    sortedElements.push_back(elementsCount - 1);
    waitForStep();
    visualize();
    playSound(elementsCount - 1, elementsCount - 1);
    wait();
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

            comparisonCount++;
            arrayAccessCount+=2;

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
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            playSound(currentMaxIndex, i);
            wait();
        }

        sortedElements.push_back(i);
        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

    sortedElements.push_back(elementsCount - 1);
    waitForStep();
    visualize();
    playSound(elementsCount - 1, elementsCount - 1);
    wait();
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

            comparisonCount++;
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            wait();

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount+=2;

                waitForStep();
                visualize();
                playSound(i, i + 1);
                wait();

                swapped = true;
            }
        }

        sortedElements.push_back(end);
        visualize();
        wait();

        if (!swapped) break;

        swapped = false;
        --end;

        // Backward pass
        for (int i = end - 1; i >= start; --i)
        {
            if (shouldReset) return;

            bar1Index = i;
            bar2Index = i + 1;

            comparisonCount++;
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            wait();

            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount+=2;

                waitForStep();
                visualize();
                playSound(i, i + 1);
                wait();

                swapped = true;
            }
        }

        sortedElements.push_back(start);
        visualize();
        wait();

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

            comparisonCount++;
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            wait();

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount+=2;

                waitForStep();
                visualize();
                playSound(i, i + 1);
                wait();

                swapped = true;
            }
        }

        // sortedElements.push_back(end);
        // visualize();
        // wait();

        if (!swapped) break;

        swapped = false;
        --end;

        // Backward pass
        for (int i = end - 1; i >= start; --i)
        {
            if (shouldReset) return;

            bar1Index = i;
            bar2Index = i + 1;

            comparisonCount++;
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            wait();

            if (array[i] < array[i + 1])
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount+=2;

                waitForStep();
                visualize();
                playSound(i, i + 1);
                wait();

                swapped = true;
            }
        }

        // sortedElements.push_back(start);
        // visualize();
        // wait();

        ++start;
    }
}

void MainWindow::gnomeSortAscending()
{
    int index = 0;
    while (index < elementsCount)
    {
        if (shouldReset) return;

        if (index == 0)
        {
            index++;
        }
        else
        {
            comparisonCount++;
            arrayAccessCount+=2;

            if (array[index] >= array[index - 1])
            {
                index++;
            }
            else
            {
                bar1Index = index;
                bar2Index = index - 1;

                sortedElements.push_back(index - 1);

                waitForStep();
                visualize();
                wait();

                std::swap(array[index], array[index - 1]);
                arrayAccessCount+=2;
                index--;

                waitForStep();
                visualize();
                playSound(index + 1, index);
                wait();
            }
        }
    }

    sortedElements.push_back(elementsCount - 1);
    visualize();
    wait();
}
void MainWindow::gnomeSortDescending()
{
    int index = 0;
    while (index < elementsCount)
    {
        if (shouldReset) return;

        if (index == 0)
        {
            index++;
        }
        else
        {
            comparisonCount++;
            arrayAccessCount+=2;

            if (array[index] <= array[index - 1])
            {
                index++;
            }
            else
            {
                bar1Index = index;
                bar2Index = index - 1;

                sortedElements.push_back(index - 1);

                waitForStep();
                visualize();
                wait();

                std::swap(array[index], array[index - 1]);
                arrayAccessCount+=2;
                index--;

                waitForStep();
                visualize();
                playSound(index + 1, index);
                wait();
            }
        }
    }

    sortedElements.push_back(elementsCount - 1);
    visualize();
    wait();
}

void MainWindow::heapifyMax(int n, int i)
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

        comparisonCount++;
        arrayAccessCount+=2;

        if (array[left] > array[largest])
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

        comparisonCount++;
        arrayAccessCount+=2;

        if (array[right] > array[largest])
        {
            largest = right;
        }
    }

    if (largest != i)
    {
        std::swap(array[i], array[largest]);
        arrayAccessCount += 2;

        waitForStep();
        visualize();
        playSound(i, largest);
        wait();

        heapifyMax(n, largest);
    }
}

void MainWindow::buildMaxdHeap()
{
    int n = array.size();
    heapElements.clear();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        if (shouldReset) return;

        heapifyMax(n, i);

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::popMax(int n)
{
    if (n == 0) return;

    std::swap(array[0], array[n - 1]);
    arrayAccessCount+=2;

    waitForStep();
    visualize();
    wait();

    heapifyMax(n - 1, 0);

    waitForStep();
    visualize();
    wait();
}

void MainWindow::heapSortAscending()
{
    buildMaxdHeap();
    waitForStep();
    visualize();
    wait();

    for (int i = 0; i < array.size()-1;i++)
    {
        if (shouldReset) return;

        popMax(array.size() - i);

        sortedElements.push_back(array.size() - i - 1);

        waitForStep();
        visualize();
        wait();
    }

    sortedElements.push_back(0);
    visualize();
    wait();
}

void MainWindow::heapifyMin(int n, int i)
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

        comparisonCount++;
        arrayAccessCount+=2;

        if (array[left] < array[smallest])
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

        comparisonCount++;
        arrayAccessCount+=2;

        if (array[right] < array[smallest])
        {
            smallest = right;
        }
    }

    if (smallest != i)
    {
        std::swap(array[i], array[smallest]);
        arrayAccessCount += 2;
        waitForStep();
        visualize();
        playSound(i, smallest);
        wait();

        heapifyMin(n, smallest);
    }
}
void MainWindow::buildMindHeap()
{
    int n = array.size();
    heapElements.clear();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        if (shouldReset) return;

        heapifyMin(n, i);

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::popMin(int n)
{
    if (n == 0) return;

    std::swap(array[0], array[n - 1]);
    arrayAccessCount+=2;

    waitForStep();
    visualize();
    wait();

    heapifyMin(n - 1, 0);

    waitForStep();
    visualize();
    wait();
}

void MainWindow::heapSortDescending()
{
    buildMindHeap();
    waitForStep();
    visualize();
    wait();

    for (int i = 0; i < array.size() - 1; i++)
    {
        if (shouldReset) return;

        popMin(array.size() - i);

        sortedElements.push_back(array.size() - i - 1);

        waitForStep();
        visualize();
        wait();
    }

    sortedElements.push_back(0);
    visualize();
    wait();
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

            arrayAccessCount+=2;
            comparisonCount++;

            bar1Index = index - 1;
            bar2Index = index;
            bar3Index = i;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            playSound(index, index - 1);
            wait();

            index--;
        }

        for (int j = 0; j <= i; j++)
        {
            if (std::find(sortedElements.begin(), sortedElements.end(), j) == sortedElements.end())
            {
                sortedElements.push_back(j);
            }
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

            arrayAccessCount+=2;
            comparisonCount++;

            bar1Index = index - 1;
            bar2Index = index;
            bar3Index = i;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);
            arrayAccessCount+=2;

            waitForStep();
            visualize();
            playSound(index, index - 1);
            wait();

            index--;
        }

        for (int j = 0; j <= i; j++)
        {
            if (std::find(sortedElements.begin(), sortedElements.end(), j) == sortedElements.end())
            {
                sortedElements.push_back(j);
            }
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

void MainWindow::bogoSort()
{
    bool sorted = false;

    while (!sorted)
    {
        if (shouldReset) return;

        // Check if the array is sorted
        sorted = true;
        for (int i = 0; i < elementsCount - 1; i++)
        {
            comparisonCount++; // Increment comparison count
            arrayAccessCount += 2; // Access array[i] and array[i + 1]

            if (array[i] > array[i + 1])
            {
                sorted = false;
                break;
            }
        }

        // If not sorted, shuffle the array
        if (!sorted)
        {
            std::shuffle(array.begin(), array.end(), std::mt19937{std::random_device{}()});
            arrayAccessCount += elementsCount; // Each shuffle accesses all elements

            bar1Index = -1;
            bar2Index = -1;

            waitForStep();
            visualize();
            playSound(0, elementsCount - 1);
            wait();
        }
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
        if (qobject_cast<QRadioButton*>(widget)
            || qobject_cast<QSpinBox*>(widget)
            || qobject_cast<QComboBox*>(widget)
            || qobject_cast<QCheckBox*>(widget)
            || qobject_cast<QPushButton*>(widget))
        {
            widget->setEnabled(false);
        }
    }

    ui->textLabel->setGeometry(this->centralWidget()->geometry());

    visualize();
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Escape || event->key() == Qt::Key_F11 || event->key() == Qt::Key_F) && this->isFullScreen())
    {
        this->statusBar()->show();
        this->showNormal();
        this->setGeometry(originalGeometry);

        if (ui->startButton->text() == "Start")
        {
            QList<QWidget*> allWidgets = this->centralWidget()->findChildren<QWidget*>();
            for (auto widget : allWidgets)
            {
                widget->setEnabled(true);
            }

            ui->nextStepButton->setEnabled(false);
        }
        else
        {
            ui->delay->setEnabled(true);
            ui->themeComboBox->setEnabled(true);
            ui->invertThemeCheckBox->setEnabled(true);
            ui->fullScreenButton->setEnabled(true);
            ui->startButton->setEnabled(true);
            ui->nextStepButton->setEnabled(!isContinuous);
        }

        ui->textLabel->setGeometry(originalTextLabelGeometry);

        visualize();

        QMessageBox::information(this, "Notification", "Fullscreen mode exited");
    }
    else if ((event->key() == Qt::Key_F || event->key() == Qt::Key_F11) && !this->isFullScreen())
    {
        on_fullScreenButton_clicked();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    shouldReset = true;
    event->accept();
}
