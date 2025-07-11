package Tests;

import DSA.Graphs.MatrixGraph.AdjacencyVect;

import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;

public class AdjacencyVectTests {
    public static void main(String[] args) {
        testBasicOperations();
        testIterator();
        testCollectionOperations();
        testArrayOperations();
        testEdgeCases();
    }
    
    private static void testBasicOperations() {
        System.out.println("\nTesting Basic Operations:");
        boolean passed = true;

        AdjacencyVect vect = new AdjacencyVect(10);

        // add() and contains()
        if (!vect.add(2) || !vect.contains(2)) {
            System.out.println("FAILED: add or contains failed");
            passed = false;
        } else {
            System.out.println("PASSED: add and contains");
        }

        // add duplicate
        if (vect.add(2)) {
            System.out.println("FAILED: duplicate add should return false");
            passed = false;
        } else {
            System.out.println("PASSED: duplicate add");
        }

        // remove()
        if (!vect.remove(2) || vect.contains(2)) {
            System.out.println("FAILED: remove failed");
            passed = false;
        } else {
            System.out.println("PASSED: remove");
        }

        // remove non-existent
        if (vect.remove(5)) {
            System.out.println("FAILED: remove non-existent should return false");
            passed = false;
        } else {
            System.out.println("PASSED: remove non-existent");
        }

        // clear()
        vect.add(1);
        vect.add(3);
        vect.clear();
        if (!vect.isEmpty()) {
            System.out.println("FAILED: clear or isEmpty failed");
            passed = false;
        } else {
            System.out.println("PASSED: clear and isEmpty");
        }

        // add after clear
        if (!vect.add(4) || !vect.contains(4)) {
            System.out.println("FAILED: add after clear failed");
            passed = false;
        } else {
            System.out.println("PASSED: add after clear");
        }
        
        // size()
        vect.clear();
        if (vect.size() != 0) {
            System.out.println("FAILED: size after clear should be 0");
            passed = false;
        } else {
            vect.add(1);
            vect.add(3);
            vect.add(5);
            if (vect.size() != 3) {
                System.out.println("FAILED: size should be 3 after adding 3 elements");
                passed = false;
            } else {
                System.out.println("PASSED: size works correctly");
            }
        }

        // boundary check
        if (vect.add(-1) || vect.add(10)) {
            System.out.println("FAILED: add out-of-bounds should return false");
            passed = false;
        } else {
            System.out.println("PASSED: add out-of-bounds rejected");
        }
        
        printTestResult(passed);
    }
    
    private static void testIterator() {
        System.out.println("\nTesting Iterator Functionality:");
        boolean passed = true;
        
        // basic iterator test - ascending order
        AdjacencyVect vect = new AdjacencyVect(10);
        vect.add(5);
        vect.add(2);
        vect.add(8);
        int[] expected = {2, 5, 8};
        int i = 0;
        for (Iterator<Integer> it = vect.iterator(); it.hasNext(); ) {
            int val = it.next();
            if (val != expected[i]) {
                System.out.println("FAILED: iterator order incorrect at index " + i +  ", expected " + expected[i] + " but got " + val);
                passed = false;
                break;
            }
            i++;
        }
        if (i == expected.length) {
            System.out.println("PASSED: iterator returns vertices in ascending order");
        }

        // empty iterator test
        vect = new AdjacencyVect(10);
        if (vect.iterator().hasNext()) {
            System.out.println("FAILED: empty iterator should have no next");
            passed = false;
        } else {
            System.out.println("PASSED: empty iterator has no elements");
        }
        
        // test handling of exhausted iterator (no elements left)
        vect = new AdjacencyVect(10);
        vect.add(3);
        Iterator<Integer> it = vect.iterator();
        it.next(); // Consume the only element
        try {
            Integer result = it.next(); // should return a default value (-1) or throw a runtime exception
            if (result == -1) {
                System.out.println("PASSED: iterator.next() returns -1 when exhausted");
            } else {
                System.out.println("FAILED: iterator.next() should return -1 when exhausted, got: " + result);
                passed = false;
            }
        } catch (RuntimeException e) {
            System.out.println("PASSED: iterator.next() throws RuntimeException when exhausted");
        }
        
        // sparse iterator test - only some elements are true
        vect = new AdjacencyVect(20);
        vect.add(3);
        vect.add(9);
        vect.add(17);
        
        expected = new int[]{3, 9, 17};
        i = 0;
        for (Iterator<Integer> iter = vect.iterator(); iter.hasNext(); ) {
            int val = iter.next();
            if (val != expected[i]) {
                System.out.println("FAILED: iterator sparse test - expected " + expected[i] + " but got " + val);
                passed = false;
                break;
            }
            i++;
        }
        if (i == expected.length) {
            System.out.println("PASSED: iterator works correctly with sparse elements");
        }
        
        // testing iterator after modification
        vect = new AdjacencyVect(10);
        vect.add(1);
        vect.add(3);
        vect.add(5);
        
        it = vect.iterator();
        it.next(); 
        vect.add(2);
        
        if (it.next() == 3 && it.next() == 5 && !it.hasNext()) {
            System.out.println("PASSED: iterator continues correctly after modification");
        } else {
            System.out.println("FAILED: iterator behavior incorrect after modification");
            passed = false;
        }
        
        // bulk insert (adding lots of elements) and iterate
        vect = new AdjacencyVect(100);
        for (int j = 0; j < 100; j += 2) {
            vect.add(j);
        }
        
        boolean correctOrder = true;
        int index = 0;
        for (Iterator<Integer> iter = vect.iterator(); iter.hasNext(); ) {
            int val = iter.next();
            if (val != index) {
                correctOrder = false;
                break;
            }
            index += 2;
        }
        
        if (correctOrder && index == 100) {
            System.out.println("PASSED: bulk insert and iteration works correctly");
        } else {
            System.out.println("FAILED: bulk insert and iteration yields incorrect results");
            passed = false;
        }
        
        printTestResult(passed);
    }
    
    private static void testCollectionOperations() {
        System.out.println("\nTesting Collection Operations:");
        boolean passed = true;
        
        // Test addAll
        AdjacencyVect vect = new AdjacencyVect(10);
        Integer[] toAddArray = {1, 3, 5, 7};
        Collection<Integer> toAdd = Arrays.asList(toAddArray);
        
        if (vect.addAll(toAdd) && vect.size() == 4) {
            boolean allPresent = true;
            for (Integer num : toAdd) {
                if (!vect.contains(num)) {
                    allPresent = false;
                    break;
                }
            }
            if (allPresent) {
                System.out.println("PASSED: addAll works correctly");
            } else {
                System.out.println("FAILED: addAll didn't add all elements");
                passed = false;
            }
        } else {
            System.out.println("FAILED: addAll didn't return true or incorrect size");
            passed = false;
        }
        
        // test containsAll
        vect = new AdjacencyVect(10);
        vect.add(1);
        vect.add(3);
        vect.add(5);
        
        Integer[] containsArray = {1, 3};
        if (vect.containsAll(Arrays.asList(containsArray))) {
            System.out.println("PASSED: containsAll returns true for contained subset");
        } else {
            System.out.println("FAILED: containsAll failed for contained subset");
            passed = false;
        }
        
        Integer[] notContainsArray = {1, 3, 7};
        if (!vect.containsAll(Arrays.asList(notContainsArray))) {
            System.out.println("PASSED: containsAll returns false when not all elements are present");
        } else {
            System.out.println("FAILED: containsAll returned true when not all elements are present");
            passed = false;
        }
        
        // test removeAll
        vect = new AdjacencyVect(10);
        vect.add(1);
        vect.add(3);
        vect.add(5);
        vect.add(7);
        
        Integer[] removeArray = {1, 5, 9};
        if (vect.removeAll(Arrays.asList(removeArray))) {
            if (!vect.contains(1) && vect.contains(3) && !vect.contains(5) && vect.contains(7) && vect.size() == 2) {
                System.out.println("PASSED: removeAll works correctly");
            } else {
                System.out.println("FAILED: removeAll didn't remove correct elements");
                passed = false;
            }
        } else {
            System.out.println("FAILED: removeAll should return true when elements are removed");
            passed = false;
        }
        
        // test retainAll
        vect = new AdjacencyVect(10);
        vect.add(1);
        vect.add(3);
        vect.add(5);
        vect.add(7);
        
        Integer[] retainArray = {3, 7, 9};
        if (vect.retainAll(Arrays.asList(retainArray))) {
            if (!vect.contains(1) && vect.contains(3) && !vect.contains(5) && vect.contains(7) && vect.size() == 2) {
                System.out.println("PASSED: retainAll works correctly");
            } else {
                System.out.println("FAILED: retainAll didn't retain correct elements");
                passed = false;
            }
        } else {
            System.out.println("FAILED: retainAll should return true when collection is modified");
            passed = false;
        }
        
        printTestResult(passed);
    }
    
    private static void testArrayOperations() {
        System.out.println("\nTesting Array Operations:");
        boolean passed = true;
        
        // test toArray()
        AdjacencyVect vect = new AdjacencyVect(10);
        vect.add(2);
        vect.add(4);
        vect.add(6);
        
        Object[] arr = vect.toArray();
        if (arr.length == 3 && ((Integer)arr[0]).equals(2) && 
            ((Integer)arr[1]).equals(4) && ((Integer)arr[2]).equals(6)) {
            System.out.println("PASSED: toArray() works correctly");
        } else {
            System.out.println("FAILED: toArray() returned incorrect result");
            passed = false;
        }
        
        // test toArray(T[] a) with array smaller than collection
        Integer[] smallArr = new Integer[1];
        Integer[] resultArr = vect.toArray(smallArr);
        
        if (resultArr != smallArr && resultArr.length == 3 && 
            resultArr[0].equals(2) && resultArr[1].equals(4) && resultArr[2].equals(6)) {
            System.out.println("PASSED: toArray(T[] a) works with smaller array");
        } else {
            System.out.println("FAILED: toArray(T[] a) with smaller array");
            passed = false;
        }
        
        // Test toArray(T[] a) with array larger than collection
        Integer[] largeArr = new Integer[5];
        for (int i = 0; i < largeArr.length; i++) {
            largeArr[i] = 99;
        }
        
        resultArr = vect.toArray(largeArr);
        if (resultArr == largeArr && resultArr[0].equals(2) && 
            resultArr[1].equals(4) && resultArr[2].equals(6) && 
            resultArr[3] == null && resultArr[4].equals(99)) {
            System.out.println("PASSED: toArray(T[] a) works with larger array");
        } else {
            System.out.println("FAILED: toArray(T[] a) with larger array");
            passed = false;
        }
        
        printTestResult(passed);
    }
    
    private static void testEdgeCases() {
        System.out.println("\nTesting Edge Cases:");
        boolean passed = true;
        
        // test with null value
        AdjacencyVect vect = new AdjacencyVect(10);
        if (vect.add(null)) {
            System.out.println("FAILED: add should reject null values");
            passed = false;
        } else {
            System.out.println("PASSED: add correctly rejected null");
        }
        
        // test zero capacity
        try {
            vect = new AdjacencyVect(0);
            if (!vect.add(0) && vect.size() == 0) {
                System.out.println("PASSED: zero capacity vector behaves correctly");
            } else {
                System.out.println("FAILED: zero capacity vector should reject all adds");
                passed = false;
            }
        } catch (Exception e) {
            System.out.println("NOTE: zero capacity vector threw exception: " + e.getMessage());
        }
        
        printTestResult(passed);
    }
    
    private static void printTestResult(boolean passed) {
        if (passed) {
            System.out.println("ALL TESTS IN THIS PART PASSED.");
        } else {
            System.out.println("SOME TESTS IN THIS PART FAILED.");
        }
    }
}