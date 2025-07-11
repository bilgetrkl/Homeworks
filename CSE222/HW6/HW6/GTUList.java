public class GTUList<E> implements Iterable<E> {
    private Object[] elements;
    private int size;
    private static final int CAPACITY = 10;
     
    public GTUList() { // default constructor
        elements = new Object[CAPACITY];
        size = 0;
    }

    public GTUList(int initialCapacity) { // constructor with initial capacity
        if (initialCapacity < 0) {
            throw new IllegalArgumentException("Illegal Capacity cannot be negative: " + initialCapacity);
        }
        elements = new Object[initialCapacity];
        size = 0;
    }
    
    public boolean add(E element) { // adds an element to the end of the list
        ensureCapacity(size + 1);
        elements[size++] = element;
        return true;
    }

    @Override
    public String toString() { // returns a string representation of the list
        if (size == 0) return "[]";
        
        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (int i = 0; i < size; i++) {
            sb.append(elements[i]);
            if (i < size - 1)
                sb.append(", ");
        }
        sb.append(']');
        return sb.toString();
    }
    
    private void ensureCapacity(int minCapacity) { // checks if the current capacity is sufficient
        if (minCapacity > elements.length) { // increases capacity if needed
            int newCapacity = Math.max(elements.length * 2, minCapacity);
            Object[] newElements = new Object[newCapacity];
            System.arraycopy(elements, 0, newElements, 0, size);
            elements = newElements;
        }
    }
    
    // Iterator implementation for foreach support
    @Override
    public java.util.Iterator<E> iterator() { // returns an iterator for the list
        return new Iterator();
    }
    
    private class Iterator implements java.util.Iterator<E> { // inner class for iterator
        private int cursor = 0;
        
        @Override
        public boolean hasNext() { // checks if there are more elements to iterate
            return cursor < size;
        }
        
        @Override
        @SuppressWarnings("unchecked")
        public E next() { // returns the next element in the iteration
            if (cursor >= size) {
                throw new java.util.NoSuchElementException();
            }

            // The cast to type E is necessary due to Java's type erasure.
            // At runtime, the array is just Object[], but since we only store elements of type E, this cast is guaranteed to be safe.
            return (E) elements[cursor++]; 
        }
    }
}