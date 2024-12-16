#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void visualize();
    void playSound(int i, int j);
    void wait();
    void waitForStep();

    void bubbleSort();

    void mergeSort(int start, int end);

    void quickSort(int start, int end);

    void countingSort(int place);

    void radixSort();

    void selectionSort();

    void cocktailSort();

    void gnomeSort();

    void insertionSort();

    void bogoSort();

    void heapify(int n, int i);
    void pop(int n);
    void buildHeap();
    void heapSort();
};
#endif // MAINWINDOW_H
