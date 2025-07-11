package Tests;

import DSA.Sorting.*;

import java.util.Arrays;
import java.util.Comparator;

public class SortingTests {
    public static void main(String[] args) {
        boolean allSortTestsPassed = true;

        if (!testSorter(new MyInsertSort(), "MyInsertSort")) { // insertion sort test
            allSortTestsPassed = false;
        }

        if (!testSorter(new MySelectSort(), "MySelectSort")) { // selection sort test
            allSortTestsPassed = false;
        }

        if (!testSorter(new MyQuickSort(), "MyQuickSort (Default)")) { // default quicksort test
            allSortTestsPassed = false;
        }

        if (!testSorter(new MyQuickSort(new MyInsertSort(), 10), "MyQuickSort (with MyInsertSort, limit 10)")) { // quicksort with insertion sort for small partitions
            allSortTestsPassed = false;
        }
        
        if (!testSorter(new MyQuickSort(new MySelectSort(), 15), "MyQuickSort (with MySelectSort, limit 15)")) { // quicksort with selection sort for small partitions
            allSortTestsPassed = false;
        }

        if (allSortTestsPassed) {
            System.out.println("\nAll Sorter tests passed.");
        } else {
            System.out.println("\nSome Sorter tests failed.");
        }
    }

    private static boolean testSorter(GTUSorter sorter, String sorterName) {
        System.out.println("\nTesting Sorter: " + sorterName);
        boolean passed = true;

        // First test case: Random order integer array
        Integer[] arr1 = {5, 1, 4, 2, 8, 3, 7, 6};
        Integer[] expected1 = {1, 2, 3, 4, 5, 6, 7, 8};
        sorter.sort(arr1, Comparator.naturalOrder());
        if (!Arrays.equals(arr1, expected1)) {
            System.out.println("FAILED: Random order integer array. Expected: " + Arrays.toString(expected1) + ", Got: " + Arrays.toString(arr1));
            passed = false;
        } else {
            System.out.println("PASSED: Random order integer array");
        }

        // Second test case: Already sorted integer array
        Integer[] arr2 = {50, 40, 30, 20, 10};
        Integer[] expected2 = {10, 20, 30, 40, 50};
        sorter.sort(arr2, Comparator.naturalOrder());
        if (!Arrays.equals(arr2, expected2)) {
            System.out.println("FAILED: Already sorted integer array. Expected: " + Arrays.toString(expected2) + ", Got: " + Arrays.toString(arr2));
            passed = false;
        } else {
            System.out.println("PASSED: Already sorted integer array.");
        }
        
        // Third test case: Reverse sorted integer array
        Integer[] arr3 = {11, 22, 33, 44, 55};
        Integer[] expected3 = {11, 22, 33, 44, 55};
        sorter.sort(arr3, Comparator.naturalOrder());
        if (!Arrays.equals(arr3, expected3)) {
            System.out.println("FAILED: Reverse sorted integer array. Expected: " + Arrays.toString(expected3) + ", Got: " + Arrays.toString(arr3));
            passed = false;
        } else {
            System.out.println("PASSED: Reverse sorted integer array.");
        }

        // Fourth test case: Integer array with duplicates
        Integer[] arr4 = {4, 1, 4, 2, 8, 2, 8, 1};
        Integer[] expected4 = {1, 1, 2, 2, 4, 4, 8, 8};
        sorter.sort(arr4, Comparator.naturalOrder());
        if (!Arrays.equals(arr4, expected4)) {
            System.out.println("FAILED: Integer array with duplicates. Expected: " + Arrays.toString(expected4) + ", Got: " + Arrays.toString(arr4));
            passed = false;
        } else {
            System.out.println("PASSED: Integer array with duplicates.");
        }

        // Fifth test case: Empty integer array
        Integer[] arr5 = {};
        Integer[] expected5 = {};
        sorter.sort(arr5, Comparator.naturalOrder());
        if (!Arrays.equals(arr5, expected5)) {
            System.out.println("FAILED: Empty integer array. Expected: " + Arrays.toString(expected5) + ", Got: " + Arrays.toString(arr5));
            passed = false;
        } else {
            System.out.println("PASSED: Empty integer array.");
        }

        // Sixth test case: Single element integer array
        Integer[] arr6 = {42};
        Integer[] expected6 = {42};
        sorter.sort(arr6, Comparator.naturalOrder());
        if (!Arrays.equals(arr6, expected6)) {
            System.out.println("FAILED: Single element integer array. Expected: " + Arrays.toString(expected6) + ", Got: " + Arrays.toString(arr6));
            passed = false;
        } else {
            System.out.println("PASSED: Single element integer array.");
        }

        // Seventh test case: String array
        String[] arr7 = {"banana", "apple", "orange", "grape", "cherry"};
        String[] expected7 = {"apple", "banana", "cherry", "grape", "orange"};
        sorter.sort(arr7, Comparator.naturalOrder());
        if (!Arrays.equals(arr7, expected7)) {
            System.out.println("FAILED: String array. Expected: " + Arrays.toString(expected7) + ", Got: " + Arrays.toString(arr7));
            passed = false;
        } else {
            System.out.println("PASSED: String array.");
        }
        
        // Eighth test case: Reverse order integer array with custom comparator
        Integer[] arr8 = {5, 1, 4, 2, 8};
        Integer[] expected8 = {8, 5, 4, 2, 1};
        sorter.sort(arr8, Comparator.reverseOrder());
        if (!Arrays.equals(arr8, expected8)) {
            System.out.println("FAILED: Reverse order integer array with custom comparator. Expected: " + Arrays.toString(expected8) + ", Got: " + Arrays.toString(arr8));
            passed = false;
        } else {
            System.out.println("PASSED: Reverse order integer array with custom comparator.");
        }

        // Ninth test case: String array with custom comparator (by length)
        String[] arr9 = {"apples", "ape", "banana", "kiwi"};
        String[] expected9 = {"ape", "kiwi", "apples", "banana"}; 
        sorter.sort(arr9, Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder()));
        if (!Arrays.equals(arr9, expected9)) {
            System.out.println("FAILED: String array with custom comparator. Expected: " + Arrays.toString(expected9) + ", Got: " + Arrays.toString(arr9));
            passed = false;
        } else {
            System.out.println("PASSED: String array with custom comparator.");
        }
        
        // Tenth test case: Integer array with all identical elements
        Integer[] arr10 = {7, 7, 7, 7, 7};
        Integer[] expected10 = {7, 7, 7, 7, 7};
        sorter.sort(arr10, Comparator.naturalOrder());
        if (!Arrays.equals(arr10, expected10)) {
            System.out.println("FAILED: Integer array with all identical elements. Expected: " + Arrays.toString(expected10) + ", Got: " + Arrays.toString(arr10));
            passed = false;
        } else {
            System.out.println("PASSED: Integer array with all identical elements.");
        }

        if (passed) {
            System.out.println("All tests for " + sorterName + " PASSED.");
        } else {
            System.out.println("Some tests for " + sorterName + " FAILED.");
        }
        return passed;
    }
}