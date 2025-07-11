package DSA.Sorting;

import java.util.Comparator;
import java.util.Random;

/**
 * Implementation of quicksort algorithm that extends GTUSorter.
 * Uses random pivot selection and can switch to a different sorting algorithm
 * for small partitions to improve efficiency.
 */
public class MyQuickSort extends GTUSorter {
    private Random random;
    private GTUSorter subSorter;
    private int subSortLimit;

    /**
     * Constructor with custom sorter for small partitions.
     * @param subSorter The sorter to use for partitions smaller than subSortLimit
     * @param subSortLimit The size limit below which the alternative sorter is used
     */
    public MyQuickSort(GTUSorter subSorter, int subSortLimit) {
        random = new Random();
        this.subSorter = subSorter;
        this.subSortLimit = subSortLimit;
    }

    /**
     * Default constructor. Uses quicksort for all partitions.
     */
    public MyQuickSort() {
        random = new Random();
        this.subSorter = null;
        this.subSortLimit = 0;
    }

    /**
     * Sorts elements in arr between start (inclusive) and end (exclusive) using quicksort.
     * Uses random pivot selection and may use an alternative sorter for small partitions.
     * @param <T> Any comparable type
     * @param arr Array to be sorted
     * @param start First index (inclusive)
     * @param end Last index (exclusive)
     * @param comparator A comparator for type T to determine order
     * Time Complexity: O(n log n) best case and average case, O(n²) worst case
     * Space Complexity: O(log n) for recursion stack
     */
    @Override
    protected <T> void sort(T[] arr, int start, int end, Comparator<T> comparator) {
        int length = end - start;
        
        // use alternative sorter for small partitions if available
        if (subSorter != null && length <= subSortLimit && length > 1) {
            subSorter.sort(arr, start, end, comparator);
            return;
        }

        if (length <= 1)
            return;
        
        int pivotIndex = partition(arr, start, end, comparator);
        
        // Recursively sort the partitions
        sort(arr, start, pivotIndex, comparator);
        sort(arr, pivotIndex + 1, end, comparator);
    }
    
    /**
     * Partitions the array around a randomly selected pivot.
     * @param <T> Any comparable type
     * @param arr Array to be partitioned
     * @param start First index (inclusive)
     * @param end Last index (exclusive)
     * @param comparator A comparator for type T to determine order
     * @return The final position of the pivot
     */
    private <T> int partition(T[] arr, int start, int end, Comparator<T> comparator) {
        // select a random pivot and swap it with the first element
        int randomPivotIndex = start + random.nextInt(end - start);
        swap(arr, start, randomPivotIndex);
        
        T pivot = arr[start];
        int i = start + 1;
        
        // move elements smaller than pivot to the left side
        for (int j = start + 1; j < end; j++) {
            if (comparator.compare(arr[j], pivot) < 0) {
                swap(arr, i, j);
                i++;
            }
        }
        
        // put pivot in its final position
        swap(arr, start, i - 1);
        return i - 1;
    }
    
    /**
     * Swaps two elements in the array.
     * @param <T> Any type
     * @param arr Array containing elements to swap
     * @param i Index of first element
     * @param j Index of second element
     */
    private <T> void swap(T[] arr, int i, int j) {
        T temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}