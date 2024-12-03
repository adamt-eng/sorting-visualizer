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
    void swap();
    void bubbleSortAscending();
    void bubbleSortDescending();
    void selectionSortAscending(std::vector<int>& array, int size);
    void wait();
    void revisualize();
    void waitForStep();
};
#endif // MAINWINDOW_H
