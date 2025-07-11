package DSA.Sorting;

import java.util.Comparator;

/**
 * Implementation of selection sort algorithm that extends GTUSorter.
 * Selection Sort is a sorting algorith that sorts an array by repeatedly selecting the smallest (or largest) 
 * element from the unsorted portion and swapping it with the first unsorted element.
 */
public class MySelectSort extends GTUSorter {
    
    /**
     * Default constructor
     */
    public MySelectSort() {}
    
    /**
     * Sorts elements in arr between start (inclusive) and end (exclusive) using selection sort.
     * The algorithm repeatedly finds the minimum element from the unsorted part
     * and puts it at the beginning of the unsorted part.
     * @param <T> Any comparable type
     * @param arr Array to be sorted
     * @param start First index (inclusive)
     * @param end Last index (exclusive)
     * @param comparator A comparator for type T to determine order
     * Time Complexity: O(n²) in all cases
     * Space Complexity: O(1)
     */
    @Override
    protected <T> void sort(T[] arr, int start, int end, Comparator<T> comparator) {
        for (int i = start; i < end - 1; i++) {
            // finds the minimum element in the unsorted part of the array
            int minIndex = i;
            
            for (int j = i + 1; j < end; j++) {
                if (comparator.compare(arr[j], arr[minIndex]) < 0) {
                    minIndex = j;
                }
            }
            
            // swaps the minimum element with the first element of the unsorted part
            if (minIndex != i) {
                T temp = arr[i];
                arr[i] = arr[minIndex];
                arr[minIndex] = temp;
            }
        }
    }
}