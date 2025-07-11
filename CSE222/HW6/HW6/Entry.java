public class Entry<K, V> {
    public K key;
    public V value;
    public boolean isDeleted; // indicates if the entry is deleted
    
    public Entry(K key, V value) { // constructor to initialize the entry
        this.key = key;
        this.value = value;
        this.isDeleted = false;
    }
}