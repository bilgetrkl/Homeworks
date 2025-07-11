package src.MyTaskManager;

/**
 * The {@code MyTask} class represents a task assigned to a user.
 * <p>
 * Each task has an associated {@code MyUser} and a unique task ID.
 * Tasks can be compared based on their user's priority and their task ID.
 * </p>
 */
class MyTask implements Comparable<MyTask> {
    private MyUser user;
    private Integer id;

    /**
     * Constructs a new {@code MyTask} with the specified user and task ID.
     * @param user the user associated with the task
     * @param id the unique identifier of the task
     */
    public MyTask(MyUser user, Integer id) {
        this.user = user;
        this.id = id;
    }

    /**
     * Returns a string representation of the task.
     * @return a string in the format "Task {taskId} User {userId}"
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public String toString() { // returns a string representation of the task
        return "Task " + this.id + " User " + this.user.getID();
    } 

    /**
     * Returns the ID of the task.
     * @return the task ID
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public int getTaskId() { // returns the task id
        return this.id;
    }

    /**
     * Compares this task with another task for ordering.
     * <p>
     * Tasks are compared first by the priority of their associated users,
     * and if priorities are equal, by their task IDs.
     * </p>
     * @param task the other task to be compared
     * @return 1 if this task should come after the other task, 
     *         -1 if this task should come before, 
     *         0 if they are considered equal
     * <p><b>Time Complexity:</b> O(1)</p>
     */
    public int compareTo(MyTask task) { // compares two tasks
        
        if (this.user.getPriority() < task.user.getPriority()) { // this task is more important
            return 1;
        }
        else if (this.user.getPriority() > task.user.getPriority()) { // the other task is more important
            return -1;
        }
        else { // both tasks are equally important
            if (this.id < task.id) {
                return 1;
            } else if (this.id > task.id) {
                return -1;
            } else {
                return 0;
            }
        }
    }
}