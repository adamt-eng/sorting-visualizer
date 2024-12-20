#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Data Structures/vector.h"
#include <QMainWindow>
#include <qcombobox.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void changeEvent(QEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void visualize();
    void playSound(int i, int j);
    void wait();
    void waitForStep();

    // Array to be sorted
    Vector<int> array;

    // Flags and Status Variables
    bool stepTriggered;
    bool isPaused;

    // Sorting Configuration
    bool isSorted;
    bool isContinuous; // Continuous or step-by-step sorting
    int delayInMilliseconds;
    std::string sortingAlgorithm;

    // Visual Elements
    QColor backgroundColor = Qt::black, barColor = Qt::white;
    int originalRightMarginTextLabel, originalBottomMarginTextLabel;
    Vector<int> sortedElements; // Elements to be marked green

    // Heap Sort Specific Variables
    QColor heapLevelColors[7] = {
        QColor::fromHsl(0, 250, 230),
        QColor::fromHsl(30, 250, 230),
        QColor::fromHsl(60, 250, 230),
        QColor::fromHsl(120, 250, 230),
        QColor::fromHsl(180, 250, 230),
        QColor::fromHsl(240, 250, 230),
        QColor::fromHsl(300, 250, 230)
    };
    Vector<int> heapElements;

    // Flags and Status Variables
    bool shouldReset;

    // Sorting Configuration
    bool isAscending = true; // Sort ascendingly or descendingly
    int elementsCount;
    int maxHeight; // Biggest element in the array (currently always equals to elementsCount but this is kept as we might change it)
    std::function<bool(int, int)> comparator;

    // Sorting Statistics
    int comparisonCount, arrayAccessCount = 0;

    // Visual Elements
    int redBar1Index = -1, redBar2Index = -1, greenBarIndex = -1, blueBarIndex = -1;

private slots:
    void on_startButton_clicked();
    void on_nextStepButton_clicked();
    void on_themeComboBox_currentTextChanged(const QString &arg1);
    void on_invertThemeCheckBox_checkStateChanged(const Qt::CheckState &arg1);
    void on_fullScreenButton_clicked();
    void on_delaySpinBox_valueChanged(int arg1);
    void on_generateArrayButton_clicked();
    void on_pauseButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_switchButton_clicked();
    void on_elementsCount_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    void resizeTextLabel();
    void updateHorizontalSlider();
    void setAlgorithmsComplexity(QComboBox *comboBox);
    void generateArray();
};
#endif // MAINWINDOW_H
