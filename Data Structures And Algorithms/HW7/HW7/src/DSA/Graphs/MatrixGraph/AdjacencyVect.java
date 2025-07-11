package DSA.Graphs.MatrixGraph;

import java.util.Collection;
import java.util.Iterator;

/**
 * Represents a boolean vector for a vertex's adjacency information in the adjacency matrix.
 * This class implements the Collection interface and stores a fixed-size boolean array
 * where true at index i indicates an edge to vertex i.
 * Space Complexity: O(n)
 */
public class AdjacencyVect implements Collection<Integer> {
    
    private boolean[] adjVect;
    private int size;
    private int capacity;
    
    /**
     * Constructor to create a new adjacency vector with specified capacity
     * @param capacity the maximum number of vertices
     */
    public AdjacencyVect(int capacity) {
        this.adjVect = new boolean[capacity];
        this.capacity = capacity;
        this.size = 0;
    }
    
    /**
     * Adds an edge to the vertex with index e
     * @param index index of the vertex to add as a neighbor
     * @return true if the edge was added, false if the index is invalid or it already existed
     * Time Complexity: O(1)
     */
    @Override
    public boolean add(Integer index) {
        if (index == null || index  < 0 || index >= capacity)
            return false;
        
        if (adjVect[index]) // edge already exists
            return false; 
        
        adjVect[index] = true;
        size++;
        return true;
    }
    
    /**
     * Adds all vertices in the collection as neighbors
     * @param collect collection of vertex indices to add
     * @return true if at least one edge was added, false otherwise
     * Time Complexity: O(n) where n is the size of the collection
     */
    @Override
    public boolean addAll(Collection<? extends Integer> collect) {
        boolean modified = false;
        for (Integer vertex : collect) {
            if (add(vertex))
                modified = true;
        }
        return modified;
    }
    
    /**
     * Removes all edges from this vector
     * Time Complexity: O(n)
     */
    @Override
    public void clear() {
        for (int i = 0; i < capacity; i++) {
            adjVect[i] = false;
        }
        size = 0;
    }
    
    /**
     * Checks if there is an edge to the specified vertex
     * @param index the vertex index to check
     * @return true if an edge exists, false otherwise
     * Time Complexity: O(1)
     */
    @Override
    public boolean contains(Object index) {
        if (index instanceof Integer) {
            int vertex = ((Integer) index).intValue();
            if (vertex < 0 || vertex >= capacity)
                return false;
            return adjVect[vertex];
        }
        return false;
    }
    
    /**
     * Checks if edges exist to all vertices in the collection
     * @param collect collection of vertex indices to check
     * @return true if all edges exist, false otherwise
     * Time Complexity: O(n) where n is the size of the collection
     */
    @Override
    public boolean containsAll(Collection<?> collect) {
        for (Object obj : collect) {
            if (!contains(obj))
                return false;
        }
        return true;
    }
    
    /**
     * Checks if there are no edges in this vector
     * @return true if no edges exist, false otherwise
     * Time Complexity: O(1)
     */
    @Override
    public boolean isEmpty() {
        return size == 0;
    }
    
    /**
     * Returns an iterator over the vertices that have edges
     * @return an iterator over vertex indices with edges
     * Time Complexity: O(1) for creation, O(n) per traversal
     */
    @Override
    public Iterator<Integer> iterator() {
        return new AdjacencyVectIterator();
    }
    
    /**
     * Removes an edge to the specified vertex
     * @param obj the vertex index to remove the edge to
     * @return true if the edge was removed, false if it didn't exist
     * Time Complexity: O(1)
     */
    @Override
    public boolean remove(Object obj) {
        if (!(obj instanceof Integer)) {
            return false;
        }
        
        Integer vertex = (Integer) obj;
        if (vertex < 0 || vertex >= capacity || !adjVect[vertex]) {
            return false;
        }
        
        adjVect[vertex] = false;
        size--;
        return true;
    }
    
    /**
     * Removes all edges to vertices in the specified collection
     * @param collect collection of vertex indices to remove edges to
     * @return true if at least one edge was removed, false otherwise
     * Time Complexity: O(n) where m is the size of the collection
     */
    @Override
    public boolean removeAll(Collection<?> collect) {
        boolean modified = false;
        for (Object obj : collect) {
            if (remove(obj))
                modified = true;
        }
        return modified;
    }
    
    /**
     * Retains only edges to vertices that are in the specified collection
     * @param collect collection of vertex indices to retain edges to
     * @return true if this vector was modified, false otherwise
     * Time Complexity: O(n) where n is the capacity
     */
    @Override
    public boolean retainAll(Collection<?> collect) {
        boolean modified = false;
        for (int i = 0; i < capacity; i++) {
            if (adjVect[i] && !collect.contains(i)) {
                adjVect[i] = false;
                size--;
                modified = true;
            }
        }
        return modified;
    }
    
    /**
     * Returns the number of edges in this vector
     * @return the number of edges
     * Time Complexity: O(1)
     */
    @Override
    public int size() {
        return size;
    }
    
    /**
     * Returns an array containing all vertices with edges
     * @return an array containing vertices with edges
     * Time Complexity: O(n)
     */
    @Override
    public Object[] toArray() {
        Object[] result = new Object[size];
        int index = 0;
        
        for (int i = 0; i < capacity; i++) {
            if (adjVect[i])
                result[index++] = i;
        }
        
        return result;
    }
    
    /**
     * Returns an array containing all vertices with edges, using the provided array if possible
     * @param arr the array into which the elements are to be stored
     * @return an array containing vertices with edges
     * Time Complexity: O(n)
     */
    @Override
    @SuppressWarnings("unchecked")
    public <T> T[] toArray(T[] arr) { // Checks if provided array has sufficient size
        if (arr.length < size) { // creates a new array of the same type with correct size
            arr = (T[]) java.lang.reflect.Array.newInstance(arr.getClass().getComponentType(), size);
        } else if (arr.length > size) { // if array is larger, set first element after our data to null
            arr[size] = null;
        }
        
        int index = 0;
        for (int i = 0; i < capacity; i++) {
            if (adjVect[i]) {
                arr[index++] = (T) Integer.valueOf(i);
            }
        }
        
        return arr;
    }  

    private class AdjacencyVectIterator implements Iterator<Integer> {
        private int currentIndex = -1;
        private int nextIndex = -1;
        
        /**
         * Constructor that finds the first vertex with an edge
         */
        public AdjacencyVectIterator() {
            findNext();
        }
        
        /**
         * Finds the next vertex with an edge
         * Time Complexity: O(n)
         */
        private void findNext() {
            nextIndex = -1;
            for (int i = currentIndex + 1; i < capacity; i++) {
                if (adjVect[i]) {
                    nextIndex = i;
                    break;
                }
            }
        }
        
        /**
         * Checks if there are more vertices with edges
         * @return true if there are more vertices with edges, false otherwise
         * Time Complexity: O(1)
         */
        @Override
        public boolean hasNext() {
            return nextIndex != -1;
        }
        
        /**
         * Returns the next vertex with an edge
         * @return the next vertex index with an edge, or -1 if there are no more vertices
         * Time Complexity: O(n) worst case, amortized O(1)
         */
        @Override
        public Integer next() {
            if (!hasNext()) {
                System.err.println("Error: No more elements in iterator");
                return -1;
            }
            currentIndex = nextIndex;
            findNext();
            return currentIndex;
        }
    }
}