package DSA.Sorting;

import java.util.Comparator;

/**
 * Implementation of insertion sort algorithm that extends GTUSorter.
 * Insertion sort works by iterating through an array and inserting each element
 * into its correct position in the sorted portion of the array.
 */
public class MyInsertSort extends GTUSorter {
    
    /**
     * Default constructor
     */
    public MyInsertSort() {}

    /**
     * Sorts elements in arr between start (inclusive) and end (exclusive) using insertion sort.
     * The algorithm maintains a sorted region and inserts each new element into its correct position.
     * @param <T> Any comparable type
     * @param arr Array to be sorted
     * @param start First index (inclusive)
     * @param end Last index (exclusive)
     * @param comparator A comparator for type T to determine order
     * Time Complexity: O(n) best case, O(n²) average and worst case
     * Space Complexity: O(1)
     */

    @Override
    protected <T> void sort(T[] arr, int start, int end, Comparator<T> comparator) {
        for (int i = start + 1; i < end; i++) {
            T key = arr[i];
            int j = i - 1;
            
            // move elements that are greater than key to one position ahead
            while (j >= start && comparator.compare(arr[j], key) > 0) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
}