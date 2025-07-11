package src.MyTaskManager;

import java.util.ArrayList;
import src.MyPriorityQueue.*;

/**
 * The {@code MyTaskManager} class manages users and their associated tasks
 * with a priority queue based on user priorities and task IDs.
 */
public class MyTaskManager {
    private MyPriorityQueue<MyTask> tasks;
    private ArrayList<MyUser> users;
    
    /**
     * Constructs a {@code MyTaskManager} by reading user priorities from a configuration file.
     * @param configFile the path to the configuration file containing user priorities
     * <p><b>Time Complexity:</b> O(m) — adds m users.</p>
     */
    public MyTaskManager (String configFile) {
        users = new ArrayList<MyUser>();
        tasks = new MinHeap<MyTask>();
        addUser(configFile);
    }

    /**
     * Reads users and their priorities from the given configuration file and adds them to the user list.
     * @param configFile the path to the configuration file
     * <p><b>Time Complexity:</b> O(m) — where m is the number of users.</p>
     */
    private void addUser(String configFile) {
        java.util.Scanner scanner = null;
        try {
            scanner = new java.util.Scanner(new java.io.File(configFile));
            int i = 0;
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine().trim();
                if (line.isEmpty()) {
                    continue;
                }
                try {
                    int priority = Integer.parseInt(line);
                    if (priority < 0) {
                        System.err.println("Invalid priority: " + priority);
                        continue;
                    }
                    MyUser user = new MyUser(i, priority);
                    users.add(user);
                    i++;
                } catch (NumberFormatException e) {
                    System.err.println("Invalid line: " + line);
                }
            }
        } catch (java.io.FileNotFoundException e) {
            System.err.println("Config file not found - " + e.getMessage());
        } finally {
            if (scanner != null) {
                scanner.close();
            }
        }
    }
    
    /**
     * Executes and prints all tasks in order of their priority.
     * <p><b>Time Complexity:</b> O(n log n) — where n is the number of tasks.</p>
     */
    public void execute () {
        while (!tasks.isEmpty()) {
            MyTask task = tasks.poll(); // remove and return the highest priority task
            System.out.println (task.toString()); // prints the task
        }
    }

    /**
     * Creates a new task associated with a specific user.
     * @param taskId the ID to assign to the new task
     * @param userId the ID of the user for whom the task is created
     * @throws IllegalArgumentException if the {@code userId} is invalid
     * <p><b>Time Complexity:</b> O(log n) — due to adding the task to the priority queue.</p>
     */
    public void createTask (int taskId, int userId) { // creates a new task for a user
        if (userId >= users.size()) {
            throw new IllegalArgumentException("Invalid user ID: " + userId);
        };
        MyTask task = new MyTask(users.get(userId), taskId);
        tasks.add(task); // add new task to the heap
    }   
}