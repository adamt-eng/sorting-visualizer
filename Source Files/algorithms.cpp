#include "../Header Files/algorithms.h"
#include "Data Structures/vector.cpp"
#include "Data Structures/list.cpp"
#include <random>

Algorithms::Algorithms(MainWindow& m)
    : array(m.array), sortedElements(m.sortedElements), heapElements(m.heapElements),
    shouldReset(m.shouldReset), isAscending(m.isAscending),
    elementsCount(m.elementsCount),
    comparator(m.comparator),
    comparisonCount(m.comparisonCount), arrayAccessCount(m.arrayAccessCount),
    redBar1Index(m.redBar1Index), redBar2Index(m.redBar2Index),
    greenBarIndex(m.greenBarIndex), blueBarIndex(m.blueBarIndex), maxHeight(m.maxHeight)
{
    mainwindow = &m;
}

void Algorithms::bubbleSort()
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

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->wait();

            if (comparator(array[j], array[j + 1]))
            {
                std::swap(array[j], array[j + 1]);

                arrayAccessCount += 2;

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->playSound(j, j + 1);
                mainwindow->wait();
            }
        }

        sortedElements.push_back(elementsCount - i - 1);

        redBar1Index = -1;
        redBar2Index = -1;

        mainwindow->visualize();
    }
}

void Algorithms::mergeSort(int start, int end)
{
    if (shouldReset || start >= end) return;

    int mid = (start + end) / 2;

    mergeSort(start, mid); // First half sort
    mergeSort(mid + 1, end); // Second half sort

    redBar1Index = start;
    redBar2Index = end;
    blueBarIndex = mid;

    mainwindow->visualize();

    // Merge both halves
    Vector temp(end - start + 1,0); // end - start + 1 is the size of merged array

    int i = start, j = mid + 1, k = 0;

    redBar1Index = start;
    redBar2Index = end;
    blueBarIndex = mid;

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->wait();

    while (i <= mid && j <= end)
    {
        if (shouldReset) return;

        ++comparisonCount;
        arrayAccessCount += 3;

        mainwindow->visualize();
        mainwindow->playSound(i, j);

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

        mainwindow->visualize();
    }

    while (j <= end)
    {
        if (shouldReset) return;

        greenBarIndex = start + k;

        ++arrayAccessCount;

        temp[k++] = array[j++];

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();
    }

    for (int l = 0; l < temp.size(); ++l)
    {
        ++arrayAccessCount;

        array[start + l] = temp[l]; // Copy into original array so we can visualize it

        sortedElements.push_back(start + l);

        mainwindow->visualize();
        mainwindow->playSound(start + l, l);
        mainwindow->wait();
    }
}

void Algorithms::quickSort(int start, int end)
{
    if (shouldReset || start >= end) return;

    redBar1Index = start;
    redBar2Index = end;

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->playSound(start, end);
    mainwindow->wait();

    int pivot = array[end]; // We chose the end as the pivot index
    int i = start - 1;

    ++arrayAccessCount;

    mainwindow->visualize();

    for (int j = start; j < end; ++j)
    {
        if (shouldReset) return;

        blueBarIndex = j;

        ++arrayAccessCount;
        ++comparisonCount;

        mainwindow->visualize();

        if (comparator(pivot, array[j]))
        {
            ++i;
            std::swap(array[i], array[j]);
            arrayAccessCount += 2;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(i, j);
            mainwindow->wait();
        }
    }

    // After the loop, i + 1 points to the correct position for the pivot
    // So, swap between the elements in that position and the pivot index
    ++i;
    std::swap(array[i], array[end]);

    arrayAccessCount += 2;

    blueBarIndex = i;

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->playSound(i, end);
    mainwindow->wait();

    sortedElements.push_back(i);

    quickSort(start, i - 1);
    quickSort(i + 1, end);
}

void Algorithms::countingSort(int place)
{
    // Determine radix
    int max = (place == 0) ? (maxHeight + 1) : 10;

    // Initialize buckets as a linked-list based list
    Vector<List<int>> buckets(max);

    // Distribute elements into buckets
    for (int i = 0; i < elementsCount; ++i)
    {
        if (shouldReset) return;

        int item = array[i];
        int digit = place == 0 ? item : (item / place) % 10;

        // Insert the element into the corresponding bucket
        buckets[digit].insert(item);

        ++arrayAccessCount;
        blueBarIndex = i;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->playSound(i, i);
        mainwindow->wait();
    }

    blueBarIndex = -1;
    mainwindow->visualize();

    int index = 0;

    int start = isAscending ? 0 : max - 1;
    int end = isAscending ? max : -1;
    int step = isAscending ? 1 : -1;

    for (int bucket = start; bucket != end; bucket += step)
    {
        if (shouldReset) return;

        while (!buckets[bucket].empty())
        {
            array[index++] = buckets[bucket][0];
            buckets[bucket].eraseAt(0);

            ++arrayAccessCount;
            redBar1Index = index - 1;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(redBar1Index, redBar1Index);
            mainwindow->wait();

            // If counting sort (place == 0) or if radix sort and the current pass is the last one, mark element as green (sorted)
            if (place == 0 || place != 0 && maxHeight / (place * 10) <= 0) sortedElements.push_back(redBar1Index);
        }
    }

    redBar1Index = -1;
    mainwindow->visualize();
}

void Algorithms::radixSort()
{
    for (int place = 1; maxHeight / place > 0; place *= 10)
    {
        countingSort(place);
    }
}

void Algorithms::selectionSort()
{
    for (int i = 0; i < elementsCount - 1; ++i)
    {
        if (shouldReset) return;

        int currentIndex = i;

        redBar1Index = currentIndex;
        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();

        for (int j = i + 1; j < elementsCount; ++j)
        {
            if (shouldReset) return;

            redBar1Index = j;
            redBar2Index = currentIndex;

            ++comparisonCount;
            arrayAccessCount += 2;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(j, j);
            mainwindow->wait();

            if (comparator(array[currentIndex], array[j]))
            {
                currentIndex = j;
            }
        }

        if (currentIndex != i)
        {
            std::swap(array[currentIndex], array[i]);
            arrayAccessCount += 2;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->wait();
        }

        redBar1Index = -1;
        redBar2Index = -1;

        sortedElements.push_back(i);
        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();
    }
}

void Algorithms::cocktailSort()
{
    int start = 0;
    int end = elementsCount - 1;
    bool swapped = true;

    while (swapped)
    {
        mainwindow->waitForStep();

        swapped = false;

        // Forward pass
        for (int i = start; i < end; ++i)
        {
            if (shouldReset) return;

            redBar1Index = i;
            redBar2Index = i + 1;

            ++comparisonCount;
            arrayAccessCount += 2;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->wait();

            if (comparator(array[i], array[i + 1]))
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount += 2;

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->playSound(i, i + 1);
                mainwindow->wait();

                swapped = true;
            }
        }

        sortedElements.push_back(end);
        mainwindow->visualize();
        mainwindow->wait();

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

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->wait();

            if (comparator(array[i], array[i + 1]))
            {
                std::swap(array[i], array[i + 1]);
                arrayAccessCount += 2;

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->playSound(i, i + 1);
                mainwindow->wait();

                swapped = true;
            }
        }

        sortedElements.push_back(start);
        mainwindow->visualize();
        mainwindow->wait();

        ++start;
    }
}

void Algorithms::gnomeSort()
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

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->wait();

                std::swap(array[index], array[index - 1]);
                arrayAccessCount += 2;
                --index;

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->playSound(index + 1, index);
                mainwindow->wait();
            }
        }
    }

    sortedElements.push_back(elementsCount - 1);
    mainwindow->visualize();
    mainwindow->wait();
}

void Algorithms::insertionSort()
{
    for (int i = 1; i < elementsCount; ++i)
    {
        if (shouldReset) return;

        int index = i;

        while (index > 0 && comparator(array[index - 1], array[index]))
        {
            if (shouldReset) return;

            arrayAccessCount += 2;
            ++comparisonCount;

            redBar1Index = index - 1;
            redBar2Index = index;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->wait();

            std::swap(array[index], array[index - 1]);
            arrayAccessCount += 2;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(index, index - 1);
            mainwindow->wait();

            --index;
        }

        for (int j = 0; j <= i; ++j)
        {
            if (std::find(sortedElements.begin(), sortedElements.end(), j) == sortedElements.end())
            {
                sortedElements.push_back(j);
            }
        }

        redBar1Index = -1;
        redBar2Index = -1;
        blueBarIndex = index;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();

        blueBarIndex = -1;
    }
}

void Algorithms::bogoSort()
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
            ++comparisonCount;
            arrayAccessCount += 2;

            if (comparator(array[i], array[i + 1]))
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

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(0, elementsCount - 1);
            mainwindow->wait();
        }
    }
}

void Algorithms::heapify(int n, int i)
{
    int extreme = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    redBar1Index = redBar2Index = blueBarIndex = -1;
    mainwindow->visualize();

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

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->wait();

    blueBarIndex = -1;

    if (std::find(heapElements.begin(), heapElements.end(), i) == heapElements.end())
    {
        heapElements.push_back(i);
    }

    if (extreme != i)
    {
        redBar1Index = i;
        redBar2Index = extreme;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();

        std::swap(array[i], array[extreme]);
        arrayAccessCount += 2;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->playSound(i, extreme);
        mainwindow->wait();

        heapify(n, extreme);
    }
}
void Algorithms::pop(int n)
{
    if (n == 0) return;

    std::swap(array[0], array[n - 1]);
    arrayAccessCount += 2;

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->wait();

    heapify(n - 1, 0);

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->wait();
}
void Algorithms::buildHeap()
{
    for (int i = elementsCount / 2 - 1; i >= 0; --i)
    {
        if (shouldReset) return;

        heapify(elementsCount, i);
    }
}
void Algorithms::heapSort()
{
    buildHeap();

    redBar1Index = redBar2Index = blueBarIndex = -1;

    mainwindow->waitForStep();
    mainwindow->visualize();
    mainwindow->wait();

    for (int i = 0; i < elementsCount - 1; ++i)
    {
        if (shouldReset) return;

        pop(elementsCount - i);

        sortedElements.push_back(elementsCount - i - 1);

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();
    }

    sortedElements.push_back(0);
    mainwindow->visualize();
}

void Algorithms::shellSort()
{
    for (int gap = elementsCount / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < elementsCount; ++i)
        {
            if (shouldReset) return;

            int j = i;

            redBar1Index = j;
            redBar2Index = j - gap;

            mainwindow->waitForStep();
            mainwindow->visualize();
            mainwindow->playSound(redBar1Index, redBar2Index);
            mainwindow->wait();

            while (j >= gap)
            {
                arrayAccessCount += 2;
                ++comparisonCount;

                mainwindow->waitForStep();
                mainwindow->visualize();
                mainwindow->wait();

                if (comparator(array[j - gap], array[j]))
                {
                    redBar1Index = j - gap;
                    redBar2Index = j;

                    std::swap(array[j], array[j - gap]);

                    j -= gap;

                    arrayAccessCount += 2;

                    redBar1Index = j - gap;
                    redBar2Index = j;

                    mainwindow->waitForStep();
                    mainwindow->visualize();
                    mainwindow->wait();
                }
                else
                {
                    break;
                }
            }
        }
    }
}

void Algorithms::stalinSort()
{
    int lastValidIndex = 0;

    sortedElements.push_back(0);

    for (int i = 1; i < elementsCount;)
    {
        if (shouldReset) return;

        redBar1Index = i;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->playSound(redBar1Index, 0);
        mainwindow->wait();

        ++comparisonCount;
        arrayAccessCount += 2;

        if (comparator(array[i], array[lastValidIndex]))
        {
            lastValidIndex = i;
            sortedElements.push_back(i);
            ++i;
        }
        else
        {
            array.erase(i);
            --elementsCount;
        }

        redBar1Index = -1;
        greenBarIndex = -1;

        mainwindow->waitForStep();
        mainwindow->visualize();
        mainwindow->wait();
    }
}
