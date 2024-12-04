#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_elementsCount_valueChanged(int arg1);
    void on_elementsCount_textChanged(const QString &arg1);
    void on_nextStepButton_clicked();
    void on_themeComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    bool firstTry;
    void generateArray();
    void visualize();

    void bubbleSortAscending();
    void bubbleSortDescending();

    void mergeAscending(int start, int mid, int end);
    void mergeDescending(int start, int mid, int end);
    void mergeSortAscending(int start, int end);
    void mergeSortDescending(int start, int end);

    int partitionAscending(int start, int end);
    int partitionDescending(int start, int end);
    void quickSortAscending(int start, int end);
    void quickSortDescending(int start, int end);

    void countingSort(int place);

    void radixSort();

    void selectionSortAscending();
    void selectionSortDescending();

    void cocktailSortAscending();
    void cocktailSortDescending();

    void wait();
    void revisualize();
    void waitForStep();
};
#endif // MAINWINDOW_H
