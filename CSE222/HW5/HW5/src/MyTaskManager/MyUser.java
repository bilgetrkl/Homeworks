package src.MyTaskManager;

/**
 * The {@code MyUser} class represents a user with an ID and a priority.
 * <p>Lower priority values indicate higher importance.</p>
 */
class MyUser {
    private Integer id;
    private Integer priority; // lower means higher priority

    /**
     * Constructs a {@code MyUser} with the specified ID and priority.
     * @param id the unique identifier for the user
     * @param priority the priority value of the user (lower value means higher priority)
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public MyUser(Integer id, Integer priority) { 
        this.id = id;
        this.priority = priority;
    }

    /**
     * Returns the ID of the user.
     * @return the user's ID
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public Integer getID() { // get user ID
        return this.id;
    }

    /**
     * Returns the priority of the user.
     * @return the user's priority
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public Integer getPriority() { // get user priority
        return this.priority;
    }
}