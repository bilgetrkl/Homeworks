package src.MyPriorityQueue;

import java.util.ArrayList;

/**
 * The {@code MinHeap} class implements a minimum heap (priority queue) structure.
 * <p>
 * Elements with higher priority (as determined by {@code compareTo}) are promoted to the top of the heap.
 * </p>
 * @param <T> the type of elements in the heap, which must implement {@code Comparable<T>}
 */
public class MinHeap <T extends Comparable<T>> implements MyPriorityQueue<T> {
    
    private ArrayList<T> heap;

    /**
     * Adds a task to the heap while maintaining the heap property.
     * @param task the task to be added
     * <p><b>Time Complexity:</b> O(log n).</p>
     */
    @Override
    public void add(T task) { // adds a task to the heap
        if (heap == null) { 
            heap = new ArrayList<>();
        }
        heap.add(task); 
        int child = heap.size() - 1;
        int parent = (child - 1) / 2;
    
        while (child > 0 && heap.get(child).compareTo(heap.get(parent)) == 1) { // if child has higher priority than parent
            // swaps child and parent
            T temp = heap.get(parent); 
            heap.set(parent, heap.get(child));
            heap.set(child, temp);
    
            child = parent;
            parent = (child - 1) / 2;
        }
    }

    /**
     * Removes and returns the task with the highest priority from the heap.
     * @return the task with the highest priority, or {@code null} if the heap is empty
     * <p><b>Time Complexity:</b> O(log n)- best case is O(1)</p>
     */
    @Override
    public T poll() { // removes and returns the task with the highest priority
        if (isEmpty()) { return null;}
    
        T result = heap.get(0);
    
        if (heap.size() == 1) { // best case
            heap.remove(0);
            return result;
        }
    
        heap.set(0, heap.remove(heap.size() - 1)); // remove last element and set it as root
    
        // sorts queue by highest priority
        int parent = 0;
        while (true) {
            int leftChild = 2 * parent + 1;
            if (leftChild >= heap.size()) break;
    
            int rightChild = leftChild + 1;

            // determines which child has higher priority
            int cmpChild = leftChild; 
    
            if (rightChild < heap.size() && heap.get(rightChild).compareTo(heap.get(leftChild)) == 1) { // if right child has higher priority than left child
                cmpChild = rightChild;
            }
    
            if (heap.get(cmpChild).compareTo(heap.get(parent)) == 1) { // if child has higher priority than parent
                // swaps child and parent
                T temp = heap.get(parent);
                heap.set(parent, heap.get(cmpChild));
                heap.set(cmpChild, temp);

                parent = cmpChild;
            } else {
                break;
            }
        }
    
        return result; // returns the highest priority task
    }
    
    /**
     * Checks whether the heap is empty.
     * @return {@code true} if the heap is empty, otherwise {@code false}
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    @Override
    public Boolean isEmpty() { // returns true if the heap is empty
        return heap == null || heap.isEmpty();
    }
}
