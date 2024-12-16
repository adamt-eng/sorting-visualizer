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

private:
    MainWindow *mainwindow;

    // Array to be sorted
    gui::vector<int>& array;

    // Visual Elements
    gui::vector<int>& sortedElements; // Elements to be marked green

    // Heap Sort Specific Variables
    gui::vector<int>& heapElements;

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
