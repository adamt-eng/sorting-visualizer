// Standard Library Includes
#include <algorithm>
#include <random>
#include <functional>

// Qt Includes
#include <QRandomGenerator>
#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <QMessageBox>
#include <QRect>
#include <QKeyEvent>

// SDL Includes
#include <SDL2/SDL.h>

// Project Includes
#include ".././ui_mainwindow.h"
#include "../Header Files/mainwindow.h"
#include "../Header Files/soundplayer.h"
#include "../Source Files/vector.cpp"

// Array to be sorted
vector<int> array;

// Flags and Status Variables
bool stepTriggered;
bool shouldReset;

// Sorting Configuration
bool isAscending = true; // Sort ascendingly or descendingly
bool isContinuous; // Continuous or step-by-step sorting
int delayInMilliseconds;
int elementsCount;
int maxHeight; // Biggest element in the array (currently always equals to elementsCount but this is kept as we might change it)
std::string sortingAlgorithm;
std::function<bool(int, int)> comparator;

// Sorting Statistics
int comparisonCount = 0;
int arrayAccessCount = 0;

// Visual Elements
int redBar1Index = -1, redBar2Index = -1, greenBarIndex = -1, blueBarIndex = -1;
QColor backgroundColor = Qt::black, barColor = Qt::white;
vector<int> sortedElements; // Elements to be marked green
QRect originalGeometry, originalTextLabelGeometry;

// Sound Player
SoundPlayer player;
bool variableSound; // Whether the user chose "Variable" sound or any other option

// Heap Sort Specific Variables
vector<int> heapElements;
vector<QColor> heapLevelColors = { QColor::fromHsl(0, 250, 230), QColor::fromHsl(30, 250, 230), QColor::fromHsl(60, 250, 230), QColor::fromHsl(120, 250, 230), QColor::fromHsl(180, 250, 230), QColor::fromHsl(240, 250, 230), QColor::fromHsl(300, 250, 230) };

// UI Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generateArray();

    setAlgorithmsComplexity(ui->algorithmComboBox);
}

// UI Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Function to set each algorithm's complexity as it's data in the comboBox for faster retrieval when visualizing
void MainWindow::setAlgorithmsComplexity(QComboBox *comboBox)
{
    vector<QString> algorithms =
        {
        "Bubble Sort",
        "Merge Sort",
        "Quick Sort",
        "Counting Sort",
        "Radix Sort",
        "Selection Sort",
        "Insertion Sort",
        "Heap Sort",
        "Cocktail Sort",
        "Gnome Sort",
        "Bogo Sort"
    };

    vector<QString> complexities = {
        "O(n^2)",
        "O(n log n)",
        "O(n log n)",
        "O(n + k)",
        "O(nk)",
        "O(n^2)",
        "O(n^2)",
        "O(n log n)",
        "O(n^2)",
        "O(n^2)",
        "O((n-1)!)"
    };

    for (int i = 0; i < comboBox->count(); ++i)
    {
        QString algorithm = comboBox->itemText(i);
        for (size_t j = 0; j < algorithms.size(); ++j)
        {
            if (algorithms[j] == algorithm)
            {
                comboBox->setItemData(i, complexities[j]);
                break;
            }
        }
    }
}

// Event Handler for startButton
void MainWindow::on_startButton_clicked()
{
    if (ui->startButton->text() == "Start")
    {
        ui->startButton->setText("Reset");

        shouldReset = false;

        comparisonCount = 0;
        arrayAccessCount = 0;

        redBar1Index = redBar2Index = greenBarIndex = blueBarIndex = -1;

        sortedElements.clear();

        // Read sorting method and order
        sortingAlgorithm = ui->algorithmComboBox->currentText().toStdString();
        isAscending = ui->ascendingRadioButton->isChecked();
        comparator = isAscending ? [](int a, int b) { return a > b; }
                                 : [](int a, int b) { return a < b; };
        delayInMilliseconds = ui->delaySpinBox->value();
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
        ui->generateArrayButton->setEnabled(false);
        ui->soundComboBox->setEnabled(false);
        ui->continuousRadioButton->setEnabled(false);
        ui->stepByStepRadioButton->setEnabled(false);
        ui->algorithmComboBox->setEnabled(false);
        ui->elementsCount->setEnabled(false);
        ui->ascendingRadioButton->setEnabled(false);
        ui->descendingRadioButton->setEnabled(false);

        // Set the timer event based on user selection of sorting method and order
        if (sortingAlgorithm == "Bubble Sort")
        {
            bubbleSort();
        }
        else if (sortingAlgorithm == "Merge Sort")
        {
            mergeSort(0, elementsCount - 1);
        }
        else if (sortingAlgorithm == "Quick Sort")
        {
            quickSort(0, elementsCount - 1);
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
            selectionSort();
        }
        else if (sortingAlgorithm == "Insertion Sort")
        {
            insertionSort();
        }
        else if (sortingAlgorithm == "Heap Sort")
        {
            heapSort();
        }
        else if (sortingAlgorithm == "Cocktail Sort")
        {
            cocktailSort();
        }
        else if (sortingAlgorithm == "Gnome Sort")
        {
            gnomeSort();
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
    }
    else
    {
        shouldReset = true;
    }

    if (shouldReset)
    {
        generateArray();
    }

    redBar1Index = redBar2Index = greenBarIndex = blueBarIndex = -1;
    heapElements.clear();

    visualize();

    // Re-enable controls
    ui->startButton->setText("Start");
    ui->generateArrayButton->setEnabled(true);
    ui->soundComboBox->setEnabled(true);
    ui->algorithmComboBox->setEnabled(true);
    ui->elementsCount->setEnabled(true);
    ui->nextStepButton->setEnabled(false);
    ui->continuousRadioButton->setEnabled(true);
    ui->stepByStepRadioButton->setEnabled(true);
    ui->ascendingRadioButton->setEnabled(true);
    ui->descendingRadioButton->setEnabled(true);
}

// Event Handler for generateArrayButton
void MainWindow::on_generateArrayButton_clicked()
{
    generateArray();
}

// Update delayInMilliseconds as user changes it, this allows the user to change the delaySpinBox while sorting is already happening
void MainWindow::on_delaySpinBox_valueChanged(int arg1)
{
    delayInMilliseconds = ui->delaySpinBox->value();
}

// Event Handler for invertThemeCheckBox, this function swaps the barColor and backgroundColor to invert the theme colors
void MainWindow::on_invertThemeCheckBox_checkStateChanged(const Qt::CheckState &arg1)
{
    std::swap(barColor, backgroundColor);
    visualize();
}

// Event Handler for themeComboBox
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

// This function is called within the sorting algorithms to wait for user input to change the state of stepTriggered
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

// Event Handler for nextStepButton
void MainWindow::on_nextStepButton_clicked()
{
    stepTriggered = true;
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

// Function to generate array
void MainWindow::generateArray()
{
    // Clear array
    array.clear();

    elementsCount = ui->elementsCount->value();

    for (int i = 1; i <= elementsCount; ++i) array.push_back(i);

    maxHeight = *std::max_element(array.begin(), array.end());

    // Shuffle the array randomly
    std::shuffle(array.begin(), array.end(), std::mt19937{std::random_device{}()});

    sortedElements.clear();

    visualize();
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
    painter.setPen(barColor);
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    QString complexity = ui->algorithmComboBox->currentData().toString();
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

        if (!heapElements.isEmpty() && std::find(heapElements.begin(), heapElements.end(), i) != heapElements.end())
        {
            int level = static_cast<int>(std::log2(i + 1));
            currentColor = heapLevelColors[level % heapLevelColors.size()];
        }

        if (i == redBar1Index || i == redBar2Index) currentColor = Qt::red;
        else if (i == blueBarIndex) currentColor = QColor(3, 181, 252);
        else if (std::find(sortedElements.begin(), sortedElements.end(), i) != sortedElements.end() || i == greenBarIndex) currentColor = Qt::green;

        painter.fillRect(QRectF(xPos, height - barHeight, barWidth, barHeight), currentColor);

        xPos += barWidth + gapWidth;
    }

    ui->textLabel->setPixmap(pixmap);
}

// Function to handle playing sounds, whether that's a WAV file or a frequency with ADSR
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

// Event Handler for fullScreenButton
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

// Event Handler for keyboard shortcuts (Escape, F11, Right Arrow, and F keys are handled)
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
            ui->delaySpinBox->setEnabled(true);
            ui->themeComboBox->setEnabled(true);
            ui->invertThemeCheckBox->setEnabled(true);
            ui->fullScreenButton->setEnabled(true);
            ui->startButton->setEnabled(true);
            ui->nextStepButton->setEnabled(!isContinuous);
        }

        ui->textLabel->setGeometry(originalTextLabelGeometry);

        visualize();
    }
    else if ((event->key() == Qt::Key_F || event->key() == Qt::Key_F11) && !this->isFullScreen())
    {
        on_fullScreenButton_clicked();
    }
    else if (event->key() == Qt::Key_Right && !isContinuous && ui->startButton->text() == "Reset")
    {
        stepTriggered = true;
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

// Event Handler for the close event, this is to ensure that the program has stopped all processes before attempting to close
// Resetting handles this already so it's used
void MainWindow::closeEvent(QCloseEvent *event)
{
    shouldReset = true;
    event->accept();
}

// Event Handler for window resize event, this makes sure that new window size is fully utilized for a better visualization
void MainWindow::resizeEvent(QResizeEvent *event)
{
    static int originalRightMargin = 0;
    static int originalBottomMargin = 0;
    static bool marginsInitialized = false;

    if (!marginsInitialized)
    {
        QRect centralWidgetGeometry = ui->centralwidget->geometry();
        QRect textLabelGeometry = ui->textLabel->geometry();

        originalRightMargin = centralWidgetGeometry.width() - textLabelGeometry.right();
        originalBottomMargin = centralWidgetGeometry.height() - textLabelGeometry.bottom();
        marginsInitialized = true;
    }

    QRect currentGeometry = ui->textLabel->geometry();
    QRect centralWidgetGeometry = ui->centralwidget->geometry();

    int newWidth = centralWidgetGeometry.width() - currentGeometry.x() - originalRightMargin;
    int newHeight = centralWidgetGeometry.height() - currentGeometry.y() - originalBottomMargin;

    ui->textLabel->setGeometry(currentGeometry.x(), currentGeometry.y(), newWidth, newHeight);

    QMainWindow::resizeEvent(event);

    visualize();
}

// Algorithms
void MainWindow::bubbleSort()
{
    for (int i = 0; i < elementsCount - 1; ++i)
    {
        for (int j = 0; j < elementsCount - i - 1; ++j)
        {
            if (shouldReset) return;

            redBar1Index = j;
            redBar2Index = j + 1;

            ++comparisonCount;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            wait();

            if (comparator(array[j], array[j + 1]))
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

        redBar1Index = -1;
        redBar2Index = -1;

        visualize();
    }
}

void MainWindow::mergeSort(int start, int end)
{
    if (shouldReset || start >= end) return;

    int mid = (start + end) / 2;

    mergeSort(start, mid); // First half sort
    mergeSort(mid + 1, end); // Second half sort

    redBar1Index = start;
    redBar2Index = end;
    blueBarIndex = mid;

    visualize();

    // Merge both halves
    vector<int> temp(end - start + 1,0); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    redBar1Index = start;
    redBar2Index = end;
    blueBarIndex = mid;

    waitForStep();
    visualize();
    wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        ++comparisonCount;
        arrayAccessCount += 3;

        visualize();
        playSound(i, j);

        if (comparator(array[j], array[i]))
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

        ++arrayAccessCount;

        temp[k++] = array[i++];

        visualize();
    }

    while (j <= end)
    {
        if (shouldReset) return;

        greenBarIndex = start + k;

        ++arrayAccessCount;

        temp[k++] = array[j++];

        waitForStep();
        visualize();
        wait();
    }

    for (int l = 0; l < temp.size(); ++l)
    {
        ++arrayAccessCount;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        sortedElements.push_back(start + l);

        visualize();
        playSound(start + l, l);
        wait();
    }
}

void MainWindow::quickSort(int start, int end)
{
    if (shouldReset || start >= end) return;

    redBar1Index = start;
    redBar2Index = end;

    waitForStep();
    visualize();
    playSound(start, end);
    wait();

    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;

    ++arrayAccessCount;

    waitForStep();
    visualize();
    wait();

    for (int j = start; j < end; ++j)
    {
        if (shouldReset) return;

        blueBarIndex = j;

        ++arrayAccessCount;
        ++comparisonCount;

        waitForStep();
        visualize();
        wait();

        if (comparator(pivot, array[j]))
        {
            ++i;
            std::swap(array[i], array[j]);
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            playSound(i, j);
            wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    ++i;
    std::swap(array[i], array[end]);

    arrayAccessCount += 2;

    blueBarIndex = i;

    waitForStep();
    visualize();
    playSound(i, end);
    wait();

    sortedElements.push_back(i);

    quickSort(start, i - 1);
    quickSort(i + 1, end);
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
        ++count[index];
        ++arrayAccessCount;
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
            --count[index];

            ++arrayAccessCount;

            blueBarIndex = i;

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
            --count[index];

            ++arrayAccessCount;

            blueBarIndex = i;

            waitForStep();
            visualize();
            playSound(i, i);
            wait();
        }
    }

    blueBarIndex = -1;
    visualize();

    // Copy back the sorted array and visualize each step
    for (int i = 0; i < elementsCount; ++i)
    {
        if (shouldReset) return;

        array[i] = output[i];

        ++arrayAccessCount;

        redBar1Index = i;

        if (place == 0)
        {
            sortedElements.push_back(i);
        }

        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

    redBar1Index = -1;
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

        redBar1Index = i;

        waitForStep();
        visualize();
        playSound(i, i);
        wait();
    }

}

void MainWindow::selectionSort()
{
    for (int i = 0; i < elementsCount - 1; ++i)
    {
        if (shouldReset) return;

        int currentIndex = i;

        redBar1Index = currentIndex;
        waitForStep();
        visualize();
        wait();

        for (int j = i + 1; j < elementsCount; ++j)
        {
            if (shouldReset) return;

            redBar1Index = j;
            redBar2Index = currentIndex;

            ++comparisonCount;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            playSound(j, j);
            wait();

            if (comparator(array[currentIndex], array[j]))
            {
                currentIndex = j;
            }
        }

        if (currentIndex != i)
        {
            std::swap(array[currentIndex], array[i]);
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            playSound(currentIndex, i);
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

void MainWindow::cocktailSort()
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

            redBar1Index = i;
            redBar2Index = i + 1;

            ++comparisonCount;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            wait();

            if (comparator(array[i], array[i + 1]))
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount += 2;

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

            redBar1Index = i;
            redBar2Index = i + 1;

            ++comparisonCount;
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            wait();

            if (comparator(array[i], array[i + 1]))
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount += 2;

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

void MainWindow::gnomeSort()
{
    int index = 0;
    while (index < elementsCount)
    {
        if (shouldReset) return;

        if (index == 0)
        {
            ++index;
        }
        else
        {
            ++comparisonCount;
            arrayAccessCount += 2;

            if (comparator(array[index], array[index - 1]))
            {
                ++index;
            }
            else
            {
                redBar1Index = index;
                redBar2Index = index - 1;

                sortedElements.push_back(index - 1);

                waitForStep();
                visualize();
                wait();

                std::swap(array[index], array[index - 1]);
                arrayAccessCount += 2;
                --index;

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

void MainWindow::insertionSort()
{
    for (int i = 1; i < elementsCount; ++i)
    {
        if (shouldReset) return;

        waitForStep();

        greenBarIndex = i;
        redBar1Index = -1;
        redBar2Index = -1;
        blueBarIndex = -1;

        waitForStep();
        visualize();
        wait();

        int index = i;

        while (index > 0 && comparator(array[index - 1], array[index]))
        {
            if (shouldReset) return;

            arrayAccessCount += 2;
            ++comparisonCount;

            redBar1Index = index - 1;
            redBar2Index = index;
            greenBarIndex = i;

            waitForStep();
            visualize();
            wait();

            std::swap(array[index], array[index - 1]);
            arrayAccessCount += 2;

            waitForStep();
            visualize();
            playSound(index, index - 1);
            wait();

            --index;
        }

        for (int j = 0; j <= i; ++j)
        {
            if (std::find(sortedElements.begin(), sortedElements.end(), j) == sortedElements.end())
            {
                sortedElements.push_back(j);
            }
        }

        greenBarIndex = -1;
        redBar1Index = -1;
        redBar2Index = -1;
        blueBarIndex = index;

        waitForStep();
        visualize();
        wait();
    }
}

void MainWindow::bogoSort()
{
    bool sorted = false;
    int shuffleSeed = 0; // Seed to vary shuffle deterministically

    while (!sorted)
    {
        if (shouldReset) return;

        // Check if the array is sorted
        sorted = true;
        for (int i = 0; i < elementsCount - 1; ++i)
        {
            ++comparisonCount; // Increment comparison count
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
            // Shuffle using a deterministic seed
            std::mt19937 rng(shuffleSeed++);
            std::shuffle(array.begin(), array.end(), rng);

            arrayAccessCount += elementsCount; // Each shuffle accesses all elements

            redBar1Index = -1;
            redBar2Index = -1;

            waitForStep();
            visualize();
            playSound(0, elementsCount - 1);
            wait();
        }
    }
}

void MainWindow::heapify(int n, int i)
{
    int extreme = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    redBar1Index = redBar2Index = blueBarIndex = -1;
    visualize();

    if (left < n)
    {
        ++comparisonCount;
        arrayAccessCount += 2;

        if (std::find(heapElements.begin(), heapElements.end(), left) == heapElements.end())
        {
            heapElements.push_back(left);
        }

        if (comparator(array[left], array[extreme]))
        {
            extreme = left;
        }
    }

    if (right < n)
    {
        ++comparisonCount;
        arrayAccessCount += 2;

        if (std::find(heapElements.begin(), heapElements.end(), right) == heapElements.end())
        {
            heapElements.push_back(right);
        }

        if (comparator(array[right], array[extreme]))
        {
            extreme = right;
        }
    }

    blueBarIndex = extreme;

    waitForStep();
    visualize();
    wait();

    blueBarIndex = -1;

    if (std::find(heapElements.begin(), heapElements.end(), i) == heapElements.end())
    {
        heapElements.push_back(i);
    }

    if (extreme != i)
    {
        redBar1Index = i;
        redBar2Index = extreme;

        waitForStep();
        visualize();
        wait();

        std::swap(array[i], array[extreme]);
        arrayAccessCount += 2;

        waitForStep();
        visualize();
        playSound(i, extreme);
        wait();

        heapify(n, extreme);
    }
}
void MainWindow::pop(int n)
{
    if (n == 0) return;

    std::swap(array[0], array[n - 1]);
    arrayAccessCount += 2;

    waitForStep();
    visualize();
    wait();

    heapify(n - 1, 0);

    waitForStep();
    visualize();
    wait();
}
void MainWindow::buildHeap()
{
    for (int i = elementsCount / 2 - 1; i >= 0; --i)
    {
        if (shouldReset) return;

        heapify(elementsCount, i);
    }
}
void MainWindow::heapSort()
{
    buildHeap();

    redBar1Index = redBar2Index = blueBarIndex = -1;

    waitForStep();
    visualize();
    wait();

    for (int i = 0; i < elementsCount - 1; ++i)
    {
        if (shouldReset) return;

        pop(elementsCount - i);

        sortedElements.push_back(elementsCount - i - 1);

        waitForStep();
        visualize();
        wait();
    }

    sortedElements.push_back(0);
    visualize();
}
