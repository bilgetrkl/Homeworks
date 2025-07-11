package Tests;

import DSA.Graphs.MatrixGraph.MatrixGraph;
import java.util.Collection;
import java.util.Iterator;

/**
 * Test class for testing MatrixGraph functionality
 */
public class MatrixGraphTests {
    public static void main(String[] args) {
        testSetEdge();
        testGetEdge();
        testGetNeighbors();
        testSize();
        testReset();
    }
    
    private static void testSetEdge() {
        System.out.println("\nTesting setEdge Method:");
        boolean passed = true;
        
        MatrixGraph graph = new MatrixGraph(5);
        
        // test valid edge
        if (!graph.setEdge(0, 1)) {
            System.out.println("FAILED: setEdge should return true for valid edge (0,1)");
            passed = false;
        } else {
            System.out.println("PASSED: setEdge returns true for valid edge (0,1)");
        }
        
        // test duplicate edge
        if (graph.setEdge(0, 1)) {
            System.out.println("FAILED: setEdge should return false for duplicate edge (0,1)");
            passed = false;
        } else {
            System.out.println("PASSED: setEdge returns false for duplicate edge (0,1)");
        }
        
        // test reverse edge
        if (graph.setEdge(1, 0)) {
            System.out.println("FAILED: setEdge should return false for reverse edge (1,0) when (0,1) exists");
            passed = false;
        } else {
            System.out.println("PASSED: setEdge returns false for reverse edge (1,0) when (0,1) exists");
        }
        
        // test invalid edges
        if (graph.setEdge(-1, 2) || graph.setEdge(0, 10) || graph.setEdge(10, 0)) {
            System.out.println("FAILED: setEdge should return false for invalid vertex IDs");
            passed = false;
        } else {
            System.out.println("PASSED: setEdge returns false for invalid vertex IDs");
        }
        
        printTestResult(passed);
    }
    
    private static void testGetEdge() {
        System.out.println("\nTesting getEdge Method:");
        boolean passed = true;
        
        MatrixGraph graph = new MatrixGraph(5);
        graph.setEdge(0, 1);
        graph.setEdge(1, 2);
        graph.setEdge(2, 3);
        
        // test existing edges
        if (!graph.getEdge(0, 1)) {
            System.out.println("FAILED: getEdge should return true for existing edge (0,1)");
            passed = false;
        } else {
            System.out.println("PASSED: getEdge returns true for existing edge (0,1)");
        }
        
        // test reverse direction
        if (!graph.getEdge(1, 0)) {
            System.out.println("FAILED: getEdge should return true for reverse edge (1,0) in undirected graph");
            passed = false;
        } else {
            System.out.println("PASSED: getEdge returns true for reverse edge (1,0) in undirected graph");
        }
        
        // test non-existing edges
        if (graph.getEdge(0, 2) || graph.getEdge(0, 3)) {
            System.out.println("FAILED: getEdge should return false for non-existing edges");
            passed = false;
        } else {
            System.out.println("PASSED: getEdge returns false for non-existing edges");
        }
        
        // test invalid edges
        if (graph.getEdge(-1, 0) || graph.getEdge(0, 10) || graph.getEdge(10, 0)) {
            System.out.println("FAILED: getEdge should return false for invalid vertex IDs");
            passed = false;
        } else {
            System.out.println("PASSED: getEdge returns false for invalid vertex IDs");
        }
        
        printTestResult(passed);
    }
    
    private static void testGetNeighbors() {
        System.out.println("\nTesting getNeighbors Method:");
        boolean passed = true;
        
        MatrixGraph graph = new MatrixGraph(5);
        graph.setEdge(0, 1);
        graph.setEdge(0, 2);
        graph.setEdge(0, 3);
        graph.setEdge(1, 2);
        
        // test vertex with multiple neighbors
        Collection<Integer> neighbors = graph.getNeighbors(0);
        if (neighbors.size() != 3 || !neighbors.contains(1) || !neighbors.contains(2) || !neighbors.contains(3)) {
            System.out.println("FAILED: getNeighbors for vertex 0 should return {1, 2, 3}");
            passed = false;
        } else {
            System.out.println("PASSED: getNeighbors for vertex 0 returns correct neighbors");
        }
        
        // test vertex with no neighbors
        neighbors = graph.getNeighbors(4);
        if (neighbors.size() != 0) {
            System.out.println("FAILED: getNeighbors for vertex 4 should return empty collection");
            passed = false;
        } else {
            System.out.println("PASSED: getNeighbors for vertex 4 returns empty collection");
        }
        
        // test invalid vertex
        neighbors = graph.getNeighbors(-1);
        if (neighbors.size() != 0) {
            System.out.println("FAILED: getNeighbors for invalid vertex should return empty collection");
            passed = false;
        } else {
            System.out.println("PASSED: getNeighbors for invalid vertex returns empty collection");
        }
        
        // test that neighbors are returned in ascending order 
        graph = new MatrixGraph(10);
        graph.setEdge(5, 8);
        graph.setEdge(5, 2);
        graph.setEdge(5, 9);
        graph.setEdge(5, 0);
        
        neighbors = graph.getNeighbors(5);
        if (neighbors.size() != 4) {
            System.out.println("FAILED: getNeighbors for vertex 5 should return 4 neighbors");
            passed = false;
        } else {
            Iterator<Integer> it = neighbors.iterator();
            Integer[] expected = {0, 2, 8, 9};
            int i = 0;
            boolean correctOrder = true;
            
            while (it.hasNext()) {
                if (i >= expected.length || !it.next().equals(expected[i++])) {
                    correctOrder = false;
                    break;
                }
            }
            
            if (correctOrder) {
                System.out.println("PASSED: getNeighbors returns neighbors in ascending order");
            } else {
                System.out.println("FAILED: getNeighbors should return neighbors in ascending order");
                passed = false;
            }
        }
        
        printTestResult(passed);
    }
    
    private static void testSize() {
        System.out.println("\nTesting size Method:");
        boolean passed = true;
        
        // test empty graph
        MatrixGraph emptyGraph = new MatrixGraph();
        if (emptyGraph.size() != 0) {
            System.out.println("FAILED: Empty graph should have size 0");
            passed = false;
        } else {
            System.out.println("PASSED: Empty graph has size 0");
        }
        
        // test with specific size
        int testSize = 10;
        MatrixGraph graph = new MatrixGraph(testSize);
        if (graph.size() != testSize) {
            System.out.println("FAILED: Graph should have size " + testSize);
            passed = false;
        } else {
            System.out.println("PASSED: Graph has correct size");
        }
        
        // test that size doesn't change when adding edges
        graph.setEdge(0, 1);
        graph.setEdge(1, 2);
        if (graph.size() != testSize) {
            System.out.println("FAILED: Size should not change after adding edges");
            passed = false;
        } else {
            System.out.println("PASSED: Size remains correct after adding edges");
        }
        
        printTestResult(passed);
    }
    
    private static void testReset() {
        System.out.println("\nTesting reset Method:");
        boolean passed = true;
        
        MatrixGraph graph = new MatrixGraph(5);
        graph.setEdge(0, 1);
        graph.setEdge(1, 2);
        graph.setEdge(2, 3);
        
        // reset with same size
        graph.reset(5);
        if (graph.getEdge(0, 1) || graph.getEdge(1, 2) || graph.getEdge(2, 3)) {
            System.out.println("FAILED: Reset should clear all edges");
            passed = false;
        } else {
            System.out.println("PASSED: Reset clears all edges");
        }
        
        // reset with new size
        graph.reset(8);
        if (graph.size() != 8) {
            System.out.println("FAILED: Reset should set graph to new size");
            passed = false;
        } else {
            System.out.println("PASSED: Reset sets graph to new size");
        }
        
        // test that new edges can be added after reset
        if (!graph.setEdge(6, 7)) {
            System.out.println("FAILED: Should be able to add edges after reset");
            passed = false;
        } else {
            System.out.println("PASSED: Can add edges after reset");
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