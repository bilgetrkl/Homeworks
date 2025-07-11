public class GTUHashMap<K, V> {
 
    Entry<K, V>[] table; // hash table array
    private int size; // number of elements in the map
    private static final int CAPACITY = 16; // initial capacity of the hash table
    private static final double LOAD_FACTOR = 0.75; // threshold for resizing the table
    
    @SuppressWarnings("unchecked")
    public GTUHashMap() { // default constructor
        table = new Entry[nextPrime(CAPACITY)]; // initializes with a prime number
        size = 0;
    }
    
    public void put(K key, V value) { // inserts key value pair into the map
        if (key == null)
            throw new IllegalArgumentException("Key cannot be null");

        // if the load factor exceeds the threshold, resizes the table
        if ((double) size / table.length >= LOAD_FACTOR) resize();

        int index = getIndex(key);
        int startIndex = index;
        int i = 1;
        
        while (table[index] != null) { // if the index is occupied
            if (!table[index].isDeleted && table[index].key.equals(key)) { // if the index is occupied by the same key
                table[index].value = value; // updates the value
                return; 
            }
            
            // if the index is occupied by a different key, uses quadratic probing to find the next available index
            index = (startIndex + i*i) % table.length; 
            i++;
            
            if (index == startIndex) break;
        }
        
        table[index] = new Entry<>(key, value); // creates a new entry
        size++;
    }
    
    public V get(K key) { // retrieves the value associated with the key
        if (key == null) return null; 
        
        int index = getIndex(key);
        int startIndex = index;
        int i = 1;
        
        while (table[index] != null) { // if the index is occupied
            if (!table[index].isDeleted && table[index].key.equals(key)) { // if the index is occupied by the same key
                return table[index].value; // returns the value
            }

            // if the index is occupied by a different key, uses quadratic probing to find the next available index
            index = (startIndex + i*i) % table.length;
            i++;
            
            if (index == startIndex) break;
        }
        
        return null; // returns null if the key is not found
    }
    
    public void remove(K key) { // removes the key value pair from the map
        if (key == null) return;
        
        int index = getIndex(key);
        int startIndex = index;
        int i = 1;
        
        while (table[index] != null) { // if the index is occupied
            if (!table[index].isDeleted && table[index].key.equals(key)) { // if the index is occupied by the same key
                table[index].isDeleted = true; // marks the entry as deleted
                size--;
                return;
            }
            
            // if the index is occupied by a different key, uses quadratic probing to find the next available index
            index = (startIndex + i*i) % table.length;
            i++;
            
            if (index == startIndex) break;
        }
    }
    
    public boolean containsKey(K key) { // checks if the key is present in the map
        if (key == null) return false;
        
        int index = getIndex(key);
        int startIndex = index;
        int i = 1;
        
        while (table[index] != null) { // if the index is occupied
            if (!table[index].isDeleted && table[index].key.equals(key)) { // if the index is occupied by the same key
                return true;
            }
            
            // if the index is occupied by a different key, uses quadratic probing to find the next available index
            index = (startIndex + i*i) % table.length;
            i++;
            
            if (index == startIndex) break;
        }
        
        return false; // returns false if the key is not found
    }
    
    public int size() { // returns the size of the map
        return size;
    }
    
    private int getIndex(K key) { // calculates the index for the key
        int hash = key.hashCode();
        hash = hash ^ (hash >>> 16);
        return (hash & 0x7FFFFFFF) % table.length;
    }
    
    @SuppressWarnings("unchecked")
    private void resize() { // resizes the table
        Entry<K, V>[] oldTable = table; // stores the old table
        int newCapacity = nextPrime(oldTable.length * 2); // doubles the capacity and finds the next prime number
        table = new Entry[newCapacity];
        size = 0;
        
        for (Entry<K, V> entry : oldTable) { // rehashes the entries
            if (entry != null && !entry.isDeleted) 
                put(entry.key, entry.value); 
        }
    }
    
    private int nextPrime(int n) { // finds the next prime number
        if (n <= 2) return 2;
        
        if (n % 2 == 0) n++; // makes n odd
        
        while (!isPrime(n)) n += 2; // checks for the next prime number
        
        return n;
    }
    
    private boolean isPrime(int n) { // checks if a number is prime
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 3 == 0) return false;
        
        int i = 5;
        while (i * i <= n) { // checks for factors
            if (n % i == 0 || n % (i + 2) == 0) return false;
            i += 6;
        }
        
        return true;
    }
}