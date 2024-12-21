// Standard Library Includes
#include <algorithm>
#include <random>
#include <functional>

// Qt Includes
#include <QThread>
#include <QPainter>
#include <QPixmap>
#include <QTime>
#include <QMessageBox>
#include <QRect>
#include <QKeyEvent>

// SDL Includes
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// Project Includes
#include ".././ui_mainwindow.h"
#include "../Header Files/mainwindow.h"
#include "../Header Files/algorithms.h"
#include "../Header Files/soundplayer.h"
#include "../Source Files/Data Structures/pair.cpp"

// Sound Player
SoundPlayer player;
bool variableSound; // Whether the user chose "Variable" sound or any other option

// Algorithms class contains the implementation of the sorting algorithms
Algorithms * algorithms;

// UI Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generateArray();

    setAlgorithmsComplexity(ui->algorithmComboBox);

    ui->textLabel->installEventFilter(this);

    on_horizontalSlider_valueChanged(10);

    algorithms = new Algorithms(*this);
}

// UI Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Function to set each algorithm's complexity as it's data in the comboBox for faster retrieval when visualizing
void MainWindow::setAlgorithmsComplexity(QComboBox *comboBox)
{
    Pair<QString, QString> algorithmComplexityPairs[] = {
        Pair<QString, QString>("Bubble Sort", "O(n²)"),
        Pair<QString, QString>("Merge Sort", "O(n log n)"),
        Pair<QString, QString>("Quick Sort", "O(n log n)"),
        Pair<QString, QString>("Counting Sort", "O(n + k)"),
        Pair<QString, QString>("Radix Sort", "O(nk)"),
        Pair<QString, QString>("Selection Sort", "O(n²)"),
        Pair<QString, QString>("Insertion Sort", "O(n²)"),
        Pair<QString, QString>("Heap Sort", "O(n log n)"),
        Pair<QString, QString>("Cocktail Sort", "O(n²)"),
        Pair<QString, QString>("Gnome Sort", "O(n²)"),
        Pair<QString, QString>("Bogo Sort", "O((n-1)!)"),
        Pair<QString, QString>("Shell Sort", "O(n²)")
    };

    constexpr int pairCount = sizeof(algorithmComplexityPairs) / sizeof(algorithmComplexityPairs[0]);

    for (int i = 0; i < comboBox->count(); ++i)
    {
        QString algorithm = comboBox->itemText(i);
        for (int j = 0; j < pairCount; ++j)
        {
            if (algorithmComplexityPairs[j].first == algorithm)
            {
                comboBox->setItemData(i, algorithmComplexityPairs[j].second);
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
        if (isSorted && QMessageBox::question(this, "Question", "Do you want to generate a new array?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            generateArray();
        }

        ui->pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
        ui->startButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStop));
        ui->startButton->setText("Reset");

        // Reset variables
        comparisonCount = 0;
        arrayAccessCount = 0;
        shouldReset = false;
        sortedElements.clear();

        // Read new configuration
        sortingAlgorithm = ui->algorithmComboBox->currentText().toStdString();
        isAscending = ui->ascendingRadioButton->isChecked();
        comparator = isAscending ? [](int a, int b) { return a > b; } : [](int a, int b) { return a < b; };
        delayInMilliseconds = ui->delaySpinBox->value();
        isContinuous = ui->continuousRadioButton->isChecked();

        std::string choice = ui->soundComboBox->currentText().toStdString();
        variableSound = (choice == "Variable");

        if (!variableSound)
        {
            bool wavLoaded;
            if (choice != "No Sound")
            {
                Pair<std::string, std::string> soundMappings[] = {
                    Pair<std::string, std::string>("Beep", ":/resources/sounds/beep.wav"),
                    Pair<std::string, std::string>("Duck", ":/resources/sounds/quack.wav"),
                    Pair<std::string, std::string>("Cat", ":/resources/sounds/meow.wav"),
                    Pair<std::string, std::string>("Dog", ":/resources/sounds/bark.wav")
                };

                constexpr int pairCount = sizeof(soundMappings) / sizeof(soundMappings[0]);

                for (int i = 0; i < pairCount; ++i)
                {
                    if (soundMappings[i].first == choice)
                    {
                        wavLoaded = player.loadWAV(soundMappings[i].second);
                    }
                }

                if (!wavLoaded)
                {
                    QMessageBox::warning(this, "Warning", "Failed to load audio file, 'Variable' sound option will be used instead.");
                    variableSound = true;
                }
            }
        }

        // Disable specific controls to prevent modification while sorting

        // Algorithm Settings
        ui->pauseButton->setEnabled(isContinuous); // Only enable pauseButton if sorting mode is continuous
        ui->algorithmComboBox->setEnabled(false);
        ui->ascendingRadioButton->setEnabled(false);
        ui->descendingRadioButton->setEnabled(false);

        // Array Settings
        ui->elementsCount->setEnabled(false);
        ui->generateArrayButton->setEnabled(false);

        // Step Control
        ui->nextStepButton->setEnabled(!isContinuous);
        ui->continuousRadioButton->setEnabled(false);
        ui->stepByStepRadioButton->setEnabled(false);

        // Animation Settings
        ui->soundComboBox->setEnabled(false);

        if (sortingAlgorithm == "Bubble Sort")
        {
            algorithms->bubbleSort();
        }
        else if (sortingAlgorithm == "Merge Sort")
        {
            algorithms->mergeSort(0, elementsCount - 1);
        }
        else if (sortingAlgorithm == "Quick Sort")
        {
            algorithms->quickSort(0, elementsCount - 1);
        }
        else if (sortingAlgorithm == "Counting Sort")
        {
            algorithms->countingSort(0);
        }
        else if (sortingAlgorithm == "Radix Sort")
        {
            algorithms->radixSort();
        }
        else if (sortingAlgorithm == "Selection Sort")
        {
            algorithms->selectionSort();
        }
        else if (sortingAlgorithm == "Insertion Sort")
        {
            algorithms->insertionSort();
        }
        else if (sortingAlgorithm == "Heap Sort")
        {
            algorithms->heapSort();
        }
        else if (sortingAlgorithm == "Cocktail Sort")
        {
            algorithms->cocktailSort();
        }
        else if (sortingAlgorithm == "Gnome Sort")
        {
            algorithms->gnomeSort();
        }
        else if (sortingAlgorithm == "Bogo Sort")
        {
            algorithms->bogoSort();
        }
        else if (sortingAlgorithm == "Shell Sort")
        {
            algorithms->shellSort();
        }

        if (!shouldReset)
        {
            for (int i = 0; i < elementsCount; ++i)
            {
                sortedElements.push_back(i);
            }

            isSorted = true;
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

    // Reset variables and update visualization
    redBar1Index = redBar2Index = greenBarIndex = blueBarIndex = -1;
    heapElements.clear();
    visualize();

    // Reset buttons text
    ui->pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
    ui->startButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
    ui->startButton->setText("Start");
    ui->pauseButton->setText("Pause");

    // Reset isPaused
    isPaused = false;

    // Re-enable controls

    // Algorithm Settings
    ui->pauseButton->setEnabled(false);
    ui->algorithmComboBox->setEnabled(true);
    ui->ascendingRadioButton->setEnabled(true);
    ui->descendingRadioButton->setEnabled(true);

    // Array Settings
    ui->elementsCount->setEnabled(true);
    ui->generateArrayButton->setEnabled(true);

    // Step Control
    ui->nextStepButton->setEnabled(false);
    ui->continuousRadioButton->setEnabled(true);
    ui->stepByStepRadioButton->setEnabled(true);

    // Animation Settings
    ui->soundComboBox->setEnabled(true);
}

// Event Handler for pauseButton
void MainWindow::on_pauseButton_clicked()
{
    isPaused = !isPaused;

    if (isPaused)
    {
        ui->pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        ui->pauseButton->setText("Resume");
        while (isPaused)
        {
            QCoreApplication::processEvents();
        }
    }
    else
    {
        ui->pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
        ui->pauseButton->setText("Pause");
    }
}

// Event Handler for generateArrayButton
void MainWindow::on_generateArrayButton_clicked()
{
    generateArray();
}

// Event Handler to call generateArray() everytime the user changes the elements count
void MainWindow::on_elementsCount_valueChanged(int arg1)
{
    generateArray();
}

// Event Handler for horizontalSlider
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // Set minimum value to 10 (yet allow 0 to disable the text completely)
    if (value > 0 && value < 10)
    {
        ui->horizontalSlider->setValue(10);
        return;
    }

    QFontMetrics fm(QFont("Arial", ui->horizontalSlider->sliderPosition(), QFont::Bold));

    int fullScreenButtonSection = 30 + ui->fullScreenButton->width();
    setMinimumWidth(fullScreenButtonSection + ui->textLabel->x() +  2 * (fm.horizontalAdvance(QString(ui->algorithmComboBox->currentText()))));

    visualize();
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
    Pair<QString, Pair<QColor, QColor>> themeColors[] = {
        Pair<QString, Pair<QColor, QColor>>("Default", Pair<QColor, QColor>(Qt::black, Qt::white)),
        Pair<QString, Pair<QColor, QColor>>("Night", Pair<QColor, QColor>(QColor(0, 31, 63), QColor(255, 220, 0))),
        Pair<QString, Pair<QColor, QColor>>("Retro", Pair<QColor, QColor>(QColor(243, 240, 224), QColor(0, 116, 217))),
        Pair<QString, Pair<QColor, QColor>>("Solar", Pair<QColor, QColor>(QColor(51, 51, 51), QColor(255, 133, 27))),
        Pair<QString, Pair<QColor, QColor>>("Tropical", Pair<QColor, QColor>(QColor(55, 163, 163), QColor(255, 255, 255))),
        Pair<QString, Pair<QColor, QColor>>("Amethyst", Pair<QColor, QColor>(QColor(237, 231, 246), QColor(96, 50, 168))),
        Pair<QString, Pair<QColor, QColor>>("Sunset", Pair<QColor, QColor>(QColor(44, 62, 80), QColor(241, 196, 15))),
        Pair<QString, Pair<QColor, QColor>>("Cyber", Pair<QColor, QColor>(QColor(128, 0, 128), QColor(0, 255, 255))),
        Pair<QString, Pair<QColor, QColor>>("Autumn", Pair<QColor, QColor>(QColor(90, 61, 49), QColor(255, 127, 80))),
        Pair<QString, Pair<QColor, QColor>>("Frosty", Pair<QColor, QColor>(QColor(50, 150, 200), QColor(220, 240, 255))),
        Pair<QString, Pair<QColor, QColor>>("Aurora", Pair<QColor, QColor>(QColor(12, 24, 48), QColor(168, 226, 140))),
        Pair<QString, Pair<QColor, QColor>>("Blossom", Pair<QColor, QColor>(QColor(230, 200, 255), QColor(255, 105, 180))),
        Pair<QString, Pair<QColor, QColor>>("Steel", Pair<QColor, QColor>(QColor(48, 57, 66), QColor(100, 149, 237))),
        Pair<QString, Pair<QColor, QColor>>("Crimson", Pair<QColor, QColor>(QColor(33, 33, 33), QColor(255, 0, 51))),
        Pair<QString, Pair<QColor, QColor>>("Inferno", Pair<QColor, QColor>(QColor(10, 10, 10), QColor(250, 50, 50))),
        Pair<QString, Pair<QColor, QColor>>("Whatsapp", Pair<QColor, QColor>(QColor(30, 36, 40), QColor(37, 211, 102))),
        Pair<QString, Pair<QColor, QColor>>("Nebula", Pair<QColor, QColor>(QColor(20, 20, 40), QColor(200, 100, 250))),
        Pair<QString, Pair<QColor, QColor>>("Minecraft", Pair<QColor, QColor>(QColor(80, 50, 30), QColor(50, 160, 80)))
    };

    constexpr int themeCount = sizeof(themeColors) / sizeof(themeColors[0]);

    for (int i = 0; i < themeCount; ++i)
    {
        if (themeColors[i].first == arg1)
        {
            backgroundColor = themeColors[i].second.first;
            barColor = themeColors[i].second.second;
        }
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

    isSorted = false;

    elementsCount = ui->elementsCount->value();

    for (int i = 1; i <= elementsCount; ++i) array.push_back(i);

    maxHeight = *std::max_element(array.begin(), array.end());

    // Shuffle the array randomly
    std::shuffle(array.begin(), array.end(), std::mt19937{std::random_device{}()});

    sortedElements.clear();

    sortingAlgorithm = "";

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

    int fontSize = ui->horizontalSlider->sliderPosition();
    int lineHeight = painter.fontMetrics().lineSpacing();

    if (fontSize != 0)
    {
        painter.setPen(barColor);
        painter.setFont(QFont("Arial", fontSize, QFont::Bold));

        lineHeight = std::max((double)lineHeight, painter.font().pointSizeF() * 1.9);

        bool algorithmSpecified = (sortingAlgorithm.length() != 0);

        if (shouldReset || !algorithmSpecified)
        {
            comparisonCount = arrayAccessCount = 0;
        }

        painter.drawText(fontSize - 1, 5 + lineHeight, QString("Algorithm: %1").arg(algorithmSpecified ? QString::fromStdString(sortingAlgorithm) : "N/A"));
        painter.drawText(fontSize - 1, 5 + lineHeight * 2, QString("Complexity: %1").arg(algorithmSpecified ? ui->algorithmComboBox->currentData().toString() : "N/A"));
        painter.drawText(fontSize - 1, 5 + lineHeight * 3, QString("Comparisons: %1").arg(comparisonCount));
        painter.drawText(fontSize - 1, 5 + lineHeight * 4, QString("Array Accesses: %1").arg(arrayAccessCount));
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
        int barHeight = (array[i] * (height - 6 * lineHeight)) / maxHeight;

        QColor currentColor = barColor;

        if (!heapElements.isEmpty() && std::find(heapElements.begin(), heapElements.end(), i) != heapElements.end())
        {
            int level = static_cast<int>(std::log2(i + 1));
            currentColor = heapLevelColors[level % 7];
        }

        if (i == redBar1Index || i == redBar2Index) currentColor = Qt::red;
        else if (i == blueBarIndex) currentColor = QColor(3, 181, 252);
        else if (std::find(sortedElements.begin(), sortedElements.end(), i) != sortedElements.end() || i == greenBarIndex) currentColor = Qt::green;

        painter.fillRect(QRectF(xPos, height - barHeight, barWidth, barHeight), currentColor);

        // Draw bar value on the bar at the bottom
        double barValueFontSize = std::min(barWidth * 0.8, barHeight * 1.0) / 1.5;

        if (barValueFontSize >= 5)
        {
            painter.setPen(backgroundColor);
            painter.setFont(QFont("Arial", barValueFontSize, QFont::Bold));
            painter.drawText(QRectF(xPos, height - barHeight, barWidth, barHeight), Qt::AlignCenter | Qt::AlignBottom, QString::number(array[i]));
        }

        xPos += barWidth + gapWidth;
    }

    ui->textLabel->setPixmap(pixmap);

    // Update fullScreenButton location
    QRect textLabelGeometry = ui->textLabel->geometry();

    int newButtonX = textLabelGeometry.right() - ui->fullScreenButton->width() - 15;
    int newButtonY = textLabelGeometry.top() + 15;

    ui->fullScreenButton->setGeometry(newButtonX, newButtonY, ui->fullScreenButton->width(), ui->fullScreenButton->height());
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
    static QRect originalTextLabelGeometry;
    static QRect preFullScreenGeometry;
    static bool wasMaximizedBeforeFullScreen = false;

    if (this->isFullScreen())
    {
        this->statusBar()->show();
        this->showNormal();

        if (wasMaximizedBeforeFullScreen)
        {
            this->showMaximized();
        }
        else
        {
            this->setGeometry(preFullScreenGeometry);
        }

        if (ui->startButton->text() == "Start")
        {
            QList<QWidget*> allWidgets = this->centralWidget()->findChildren<QWidget*>();
            for (auto widget : allWidgets)
            {
                widget->setEnabled(true);
            }

            ui->nextStepButton->setEnabled(false);
            ui->pauseButton->setEnabled(false);
        }
        else
        {
            ui->delaySpinBox->setEnabled(true);
            ui->themeComboBox->setEnabled(true);
            ui->invertThemeCheckBox->setEnabled(true);
            ui->startButton->setEnabled(true);
            ui->nextStepButton->setEnabled(!isContinuous);
            ui->pauseButton->setEnabled(isContinuous);
        }

        ui->horizontalSlider->setEnabled(true);
        ui->fullScreenButton->setEnabled(true);
        ui->switchButton->setEnabled(true);
        ui->fullScreenButton->show();

        ui->textLabel->setGeometry(originalTextLabelGeometry);
    }
    else
    {
        wasMaximizedBeforeFullScreen = this->isMaximized();
        if (!wasMaximizedBeforeFullScreen)
        {
            preFullScreenGeometry = this->geometry();
        }

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
                || qobject_cast<QPushButton*>(widget)
                || qobject_cast<QSlider*>(widget))
            {
                widget->setEnabled(false);
            }
        }

        ui->fullScreenButton->hide();
        ui->textLabel->setGeometry(this->centralWidget()->geometry());
    }

    visualize();
}

// Event Handler for keyboard shortcuts (Escape, F11, Right Arrow, and F keys are handled)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Escape && this->isFullScreen()) || (event->key() == Qt::Key_F11) || (event->key() == Qt::Key_F))
    {
        on_fullScreenButton_clicked();
    }
    else if (event->key() == Qt::Key_Right && !isContinuous && ui->startButton->text() == "Reset")
    {
        stepTriggered = true;
    }
    else if (event->key() == Qt::Key_Space && isContinuous && ui->startButton->text() == "Reset")
    {
        on_pauseButton_clicked();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

// Event filter to show fullScreenButton only when mouse cursor is in the upper 30% space of textLabel when in fullscreen
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (this->isFullScreen() && watched == ui->textLabel && event->type() == QEvent::MouseMove)
    {
        bool inUpperArea = static_cast<QMouseEvent *>(event)->pos().y() <= ui->textLabel->height() * 0.3;

        ui->fullScreenButton->setEnabled(inUpperArea);
        ui->fullScreenButton->setVisible(inUpperArea);

        return true;
    }

    return QMainWindow::eventFilter(watched, event);
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
    resizeTextLabel();
    QMainWindow::resizeEvent(event);
}

// Function to modify the maximum value of horizontalSlider depending on available space up to 30% of the pixmap's height
void MainWindow::updateHorizontalSlider()
{
    int maxHeightForText = ui->textLabel->pixmap().height() * 0.3;
    int fontSize = 1;

    while (true)
    {
        if (4 * (QFont("Arial", fontSize, QFont::Bold).pointSizeF() * 1.25) > maxHeightForText)
        {
            fontSize--;
            break;
        }

        fontSize++;
    }

    ui->horizontalSlider->setMaximum(fontSize);
}

// Function to resize textLabel to fit the centralWidget
void MainWindow::resizeTextLabel()
{
    QRect centralWidgetGeometry = ui->centralwidget->geometry();
    QRect textLabelGeometry = ui->textLabel->geometry();

    int newTextLabelWidth = centralWidgetGeometry.width() - textLabelGeometry.x() - 20;
    int newTextLabelHeight = centralWidgetGeometry.height() - textLabelGeometry.y() - 10;

    ui->textLabel->setGeometry(textLabelGeometry.x(), textLabelGeometry.y(), newTextLabelWidth, newTextLabelHeight);

    updateHorizontalSlider();

    visualize();
}

// Event Handler for calling updateHorizontalSlider() when window gets maximized
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange && Qt::WindowMaximized)
    {
        updateHorizontalSlider();
    }

    QMainWindow::changeEvent(event);
}

#include <QApplication>
#include "../Header Files/secondwindow.h"

// Event handler to switch to the graph visualizer window
void MainWindow::on_switchButton_clicked()
{
    shouldReset = true;

    SecondWindow *secondWindow = new SecondWindow();
    secondWindow->show();
    this->hide();
}
