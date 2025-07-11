public class GTUHashSet<E> {
    private static final Object WORD = new Object();
    private GTUHashMap<E, Object> map;
    
    public GTUHashSet() { // default constructor
        map = new GTUHashMap<>(); 
    }
    
    public boolean add(E element) { // adds an element to the set
        boolean contains = contains(element);
        map.put(element, WORD); 
        return !contains;
    }
    
    public boolean remove(E element) { // removes an element from the set
        boolean contains = contains(element);
        map.remove(element); 
        return contains;
    }
    
    public boolean contains(E element) { // checks if the set contains an element
        return map.containsKey(element); 
    }
    
    public int size() { // returns the size of the set
        return map.size(); 
    }
}