package src.Main;

import java.util.Scanner;

import src.MyTaskManager.*;

/**
 * The {@code Main} class serves as the entry point of the application.
 * <p>
 * It reads user input to create tasks associated with user IDs and executes tasks
 * when the "execute" command is given.
 * </p>
 */
public class Main {

    /**
     * The main method initializes the task manager system and processes user input.
     * <p>
     * Users can input either a user ID (integer) to create tasks or type "execute" to run all created tasks.
     * </p>
     * @param args Command-line arguments, where args[0] must be the path to the configuration file.
     */
    public static void main(String[] args) {
        if (args.length == 0) {
            System.err.println("Usage: make run ARGS=\"<configFile>\"");
            System.exit(1);
        }
        String configFile = args[0]; 

        try {
            MyTaskManager system = new MyTaskManager(configFile); // initialize the task manager with the config file
            Scanner scanner = new Scanner(System.in);
            
            String input;
            int taskId = 0;
            while (scanner.hasNextLine()) {
                input = scanner.nextLine().trim();
                
                if (input.isEmpty()) {
                    continue;
                }
                
                if (input.equals("execute")) { // Check if it's the execute command
                    system.execute();
                    break;
                }

                try {
                    int userId = Integer.parseInt(input);
                    if (userId < 0) {
                        throw new IllegalArgumentException("User ID must be positive:" + userId);
                    }
                    system.createTask(taskId, userId); // creates a task with the given userId
                    taskId++;
                    
                } catch (NumberFormatException e) {
                    System.err.println("Invalid input. Please enter an integer value or 'execute'." );
                } catch (IllegalArgumentException e) {
                    System.err.println(e.getMessage());
                }
            }
            
            scanner.close();
                
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}