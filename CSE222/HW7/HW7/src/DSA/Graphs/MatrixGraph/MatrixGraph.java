package DSA.Graphs.MatrixGraph;

import java.util.Collection;
import java.util.Collections;

import DSA.Graphs.GTUGraph;

/**
 * Implementation of GTUGraph using an adjacency matrix representation.
 * Represents an undirected graph using an array of AdjacencyVect objects.
 * Space Complexity: O(n²)
 */
public class MatrixGraph implements GTUGraph {
    private AdjacencyVect[] matrix;
    private int graphSize;
    
    /**
     * Default constructor creates an empty graph
     */
    public MatrixGraph() {
        this.graphSize = 0;
        this.matrix = new AdjacencyVect[0];
    }
    
    /**
     * Creates a graph with a specific size
     * @param size Number of vertices in the graph
     */
    public MatrixGraph(int size) {
        reset(size);
    }
    
    /**
     * Sets an undirected edge between vertices id1 and id2.
     * Since this is an undirected graph, edges are bidirectional.
     * @param id1 First vertex ID
     * @param id2 Second vertex ID
     * @return True if the edge was set, false if invalid vertices or edge already exists
     * Time Complexity: O(1)
     */
    @Override
    public Boolean setEdge(int id1, int id2) {
        if (id1 < 0 || id1 >= graphSize || id2 < 0 || id2 >= graphSize) {
            return false;
        }
        
        boolean added1 = matrix[id1].add(id2);
        boolean added2 = matrix[id2].add(id1);
        
        return added1 || added2; // since is an undirected graph
    }
    
    /**
     * Checks if an edge exists between vertices id1 and id2
     * @param id1 First vertex ID
     * @param id2 Second vertex ID
     * @return True if the edge exists, false otherwise
     * Time Complexity: O(1)
     */
    @Override
    public Boolean getEdge(int id1, int id2) {
        if (id1 < 0 || id1 >= graphSize || id2 < 0 || id2 >= graphSize) {
            return false;
        }
        
        return matrix[id1].contains(id2);
    }
    
    /**
     * Gets a collection of neighbors for a vertex
     * @param id Vertex ID
     * @return Collection of neighbor vertex IDs
     * Time Complexity: O(1) to return the collection
     */
    @Override
    public Collection<Integer> getNeighbors(int id) {
        if (id < 0 || id >= graphSize) {
            return Collections.emptyList();
        }
        
        return matrix[id];
    }
    
    /**
     * Returns the number of vertices in the graph
     * @return Size of the graph
     * Time Complexity: O(1)
     */
    @Override
    public int size() {
        return graphSize;
    }
    
    /**
     * Resets the graph to a new size, clearing all edges
     * @param size New size for the graph
     * Time Complexity: O(n)
     */
    @Override
    public void reset(int size) {
        this.graphSize = size;
        this.matrix = new AdjacencyVect[size];
        
        for (int i = 0; i < size; i++) { // initializes the adjacency vectors for each vertex
            matrix[i] = new AdjacencyVect(size);
        }
    }
}