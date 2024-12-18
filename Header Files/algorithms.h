#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "../Header Files/mainwindow.h"

class Algorithms
{
public:
    Algorithms(MainWindow& mainwindow);

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
    void shellSort();

private:
    MainWindow *mainwindow;

    // Array to be sorted
    gui::List<int>& array;

    // Visual Elements
    gui::List<int>& sortedElements; // Elements to be marked green

    // Heap Sort Specific Variables
    gui::List<int>& heapElements;

    // Flags and Status Variables
    bool& shouldReset;

    // Sorting Configuration
    bool& isAscending; // Sort ascendingly or descendingly
    int& elementsCount;
    std::function<bool(int, int)>& comparator;

    // Sorting Statistics
    int& comparisonCount, & arrayAccessCount;

    // Visual Elements
    int& redBar1Index, & redBar2Index, & greenBarIndex, & blueBarIndex;
};

#endif // ALGORITHMS_H
