package src.MyPriorityQueue;

/**
 * The {@code MyPriorityQueue} interface defines the operations of a priority queue.
 * @param <T> the type of elements held in the priority queue, must implement {@code Comparable<T>}
 */
public interface MyPriorityQueue <T extends Comparable<T>> {

    /**
     * Adds an element to the priority queue.
     * @param t the element to add
     */
    void add(T t); // adds an element to the queue

    /**
     * Removes and returns the element with the highest priority from the queue.
     * @return the highest priority element, or {@code null} if the queue is empty
     */
    T poll(); // removes and returns the highest priority element

        /**
     * Checks if the priority queue is empty.
     * @return {@code true} if the queue is empty, otherwise {@code false}
     */
    Boolean isEmpty(); // checks if the queue is empty
}