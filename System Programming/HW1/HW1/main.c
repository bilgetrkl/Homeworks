#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#ifndef DT_REG
#define DT_REG 8
#endif
#include <sys/wait.h>

char *timeFunc () {
    time_t now = time (NULL);
    char *time_str = ctime (&now);
    time_str [strlen (time_str) - 1] = '\0';
    return time_str;
}
void operationLog (const char* operation) {
    int fd = open ("log.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
    if (fd == -1) {
        write (2, "Error: Cannot open log file\n", 25);
        exit (1);
    }
    write (fd, timeFunc (), strlen (timeFunc ()));
    write (fd, " ", 1);
    write (fd, operation, strlen (operation));
    write (fd, "\n", 1);
    close (fd);
}
void helpAndUsage () {
    write (1, "Usage: ./fileManager <command> [arguments]\n", 41);
    write (1, "Commands:\n", 10);
    write (1, "createDir \"folderName\"\t\t\t\t-Create a new directory.\n", 52);
    write (1, "createFile \"fileName\"\t\t\t\t-Create a new file.\n", 46);
    write (1, "listDir \"folderName\"\t\t\t\t-List all files in a directory.\n", 57);
    write (1, "listFilesByExtension \"folderName\" \".extension\"\t-List files with specific extension.\n", 84);
    write (1, "readFile \"fileName\"\t\t\t\t-Read a file's content.\n", 48);
    write (1, "appendToFile \"fileName\" \"new content\"\t\t-Append content to a file.\n", 66);
    write (1, "deleteFile \"fileName\"\t\t\t\t-Delete a file.\n", 42);
    write (1, "deleteDir \"folderName\"\t\t\t\t-Delete an empty directory.\n", 54);
    write (1, "showLogs\t\t\t\t\t-Display operation logs.\n", 39);
    exit (0);
}
void createDir (const char* dirname) {
    if (mkdir (dirname, 0755) == -1) {
        if (errno == EEXIST) {
            char error_msg [256] = "Error: Directory '";
            strcat (error_msg, dirname);
            strcat (error_msg, "' already exists.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        char error_msg [256] = "Error: Cannot create directory '";
        strcat (error_msg, dirname);
        strcat (error_msg, "'");
        write (2, error_msg, strlen (error_msg));
        write (2, "\n", 1);
        exit (1);
    }
    char success_msg [256] = "Directory '";
    strcat (success_msg, dirname);
    strcat (success_msg, "' created successfully.");
    write (2, success_msg, strlen (success_msg));
    write (2, "\n", 1);
    operationLog (success_msg);
    exit (0);
}
void createFile (const char* filename) {
    int fd = open (filename, O_CREAT | O_EXCL | O_RDWR, 0644);
    if (fd == -1) {
        if (errno == EEXIST) {
            char error_msg [256] = "Error: File '";
            strcat (error_msg, filename);
            strcat (error_msg, "' already exists.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        char error_msg [256] = "Error: Cannot create file '";
        strcat (error_msg, filename);
        strcat (error_msg, "'");
        write (2, error_msg, strlen (error_msg));
        write (2, "\n", 1);
        exit (1);
    }
    write (fd, timeFunc (), strlen (timeFunc ()));
    write (fd, "\n", 1);
    close (fd);
    char success_msg [256] = "File '";
    strcat (success_msg, filename);
    strcat (success_msg, "' created successfully.");
    write (2, success_msg, strlen (success_msg));
    write (2, "\n", 1);
    operationLog (success_msg);
    exit (0);
} 
void listDir (const char* dirname) {
    int pid = fork ();
    if (pid == -1) {
        write (2, "Error: Cannot create child process\n", 30);
        exit (1);
    }
    if (pid == 0) {
        DIR *dir = opendir (dirname);  
        if (dir == NULL) {
            char error_msg [256] = "Error: Directory '";
            strcat (error_msg, dirname);
            strcat (error_msg, "' not found.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        } 
        struct dirent *entry;
        while ((entry = readdir (dir)) != NULL) {
            write (1, entry -> d_name, strlen (entry -> d_name));
            write (1, "\n", 1);
        }
        char success_msg [256] = "Directory '";
        strcat (success_msg, dirname);
        strcat (success_msg, "' listed successfully.");
        write (2, success_msg, strlen (success_msg));
        write (2, "\n", 1);
        operationLog (success_msg);
        closedir (dir);
        exit (0);
    }
    int status = 0;
    waitpid (pid, &status, 0);
    exit (status);
}
void listFilesByExtension (const char* dirname, const char* extension) {
    int pid = fork ();
    if (pid == -1) {
        write (2, "Error: creating child process\n", 30);
        exit (1);
    }
    if (pid == 0) {
        DIR *dir = opendir (dirname);
        if (dir == NULL) {
            char error_msg [256] = "Error: Directory '";
            strcat (error_msg, dirname);
            strcat (error_msg, "' not found.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        struct dirent *entry;
        int count = -1;
        while ((entry = readdir (dir)) != NULL) {
            if (entry -> d_type == DT_REG) {
                char *file_extension = strrchr (entry -> d_name, '.');
                if (file_extension != NULL && strcmp (file_extension, extension) == 0) { 
                    write (1, entry -> d_name, strlen (entry -> d_name));
                    write (1, "\n", 1);
                    ++count;
                }
            }
        }
        if (count == -1) { 
            char error_msg [256] = "Error: No files with extension '";
            strcat (error_msg, extension);
            strcat (error_msg, "' found in directory '");
            strcat (error_msg, dirname);
            strcat (error_msg, "'.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            closedir (dir);
            exit (1);
        }
        char success_msg [256] = "Files with extension '";
        strcat (success_msg, extension);
        strcat (success_msg, "' in directory '");
        strcat (success_msg, dirname);
        strcat (success_msg, "' listed successfully.");
        write (2, success_msg, strlen (success_msg));
        write (2, "\n", 1);
        operationLog (success_msg);
        closedir (dir);
        exit (0);
    }
    int status = 0;
    waitpid (pid, &status, 0);
    exit (status);
}
void readFile (const char* filename) {
    int fd = open (filename, O_RDWR);
    if (fd == -1) {
        if (errno == ENOENT) {
            char error_msg [256] = "Error: File '";
            strcat (error_msg, filename);
            strcat (error_msg, "' not found.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        write (2, "Error: Cannot open file.\n", 25);
        exit (1);
    }
    char file_content [1024];
    size_t read_chars;
    while ((read_chars = read (fd, file_content, sizeof (file_content))) > 0) {
        write (1, file_content, read_chars);
    }
    write (1, "\n", 1);
    close (fd);
    
    char success_msg [256] = "File '";
    strcat (success_msg, filename);
    strcat (success_msg, "' read successfully.");
    write (2, success_msg, strlen (success_msg));
    write (2, "\n", 1);
    operationLog (success_msg);
    exit (0);
}
void appendToFile (const char* filename, const char* content) {
    int fd = open (filename, O_RDWR | O_APPEND);
    if (fd == -1) {
        if (errno == ENOENT) {
            char error_msg [256] = "Error: File '";
            strcat (error_msg, filename);
            strcat (error_msg, "' not found.");
            write (2, error_msg, strlen(error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        } 
        else if (errno == EACCES || errno == EROFS) {    
            char error_msg [256] = "Error: Cannot write to \"";
            strcat (error_msg, filename);
            strcat (error_msg, "\". File is locked or read-only.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        write (2, "Error: Cannot open file.\n", 25);
        exit (1);
    } 
    struct flock fl;
    fl.l_type = F_WRLCK;  
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;        
    fl.l_len = 0;          
    fl.l_pid = getpid ();  
    
    if (fcntl (fd, F_SETLK, &fl) == -1) {
        char error_msg [256] = "Error: Cannot write to \"";
        strcat (error_msg, filename);
        strcat (error_msg, "\". File is locked or read-only.");
        write (2, error_msg, strlen (error_msg));
        write (2, "\n", 1);
        operationLog (error_msg);
        close (fd);
        exit (1);
    }
    write (fd, content, strlen (content));
    write (fd, "\n", 1);
    fl.l_type = F_UNLCK;
    fcntl (fd, F_SETLK, &fl);
    close (fd);
    
    char success_msg [256] = "'";
    strcat (success_msg, content);
    strcat (success_msg, "' appended to file '");
    strcat (success_msg, filename);
    strcat (success_msg, "' successfully.");
    write (2, success_msg, strlen(success_msg));
    write (2, "\n", 1);
    operationLog (success_msg);
    exit (0);
}
void deleteFile (const char* filename) {
    int pid = fork ();
    if (pid == -1) {
        write (2, "Error: Cannot create child process\n", 30);
        exit (1);
    }
    if (pid == 0) {
        int status = unlink (filename);
        if (status == -1) {
            if (errno == ENOENT) {
                char error_msg [256] = "Error: File '";
                strcat (error_msg, filename);
                strcat (error_msg, "' not found.");
                write (2, error_msg, strlen (error_msg));
                write (2, "\n", 1);
                operationLog (error_msg);
                exit (1);
            }
                char error_msg [256] = "Error: Cannot delete file '";
                strcat (error_msg, filename);
                strcat (error_msg, "'.");
                write (2, error_msg, strlen (error_msg));
                write (2, "\n", 1);
                operationLog (error_msg);
                exit (1);
        }
        char success_msg [256] = "File '";
        strcat (success_msg, filename);
        strcat (success_msg, "' deleted successfully.");
        write (2, success_msg, strlen (success_msg));
        write (2, "\n", 1);
        operationLog (success_msg);
        exit (0);
    }
    int status = 0;
    waitpid (pid, &status, 0);
    exit (status);
}
void deleteDir (const char* dirname) {
    int pid = fork ();
    if (pid == -1) {
        write (2, "Error: Cannot create child process\n", 30);
        exit (1);
    }
    if (pid == 0) {
        if (rmdir (dirname) == -1) {
            if (errno == ENOENT) {
                char error_msg [256] = "Error: Directory '";
                strcat (error_msg, dirname);
                strcat (error_msg, "' not found.");
                write (2, error_msg, strlen (error_msg));
                write (2, "\n", 1);
                operationLog (error_msg);
                exit (1);
            }
            if (errno == ENOTEMPTY) {
                char error_msg [256] = "Error: Directory '";
                strcat (error_msg, dirname);
                strcat (error_msg, "' is not empty.");
                write (2, error_msg, strlen (error_msg));
                write (2, "\n", 1);
                operationLog (error_msg);
                exit (1);
            }
            char error_msg [256] = "Error: Cannot delete directory '";
            strcat (error_msg, dirname);
            strcat (error_msg, "'.");
            write (2, error_msg, strlen (error_msg));
            write (2, "\n", 1);
            operationLog (error_msg);
            exit (1);
        }
        char success_msg [256] = "Directory '";
        strcat (success_msg, dirname);
        strcat (success_msg, "' deleted successfully.");
        write (2, success_msg, strlen (success_msg));
        write (2, "\n", 1);
        operationLog (success_msg);
        exit (0);
    }
    int status = 0;
    waitpid (pid, &status, 0);
    exit (status);
}
void showLogs () {
    int fd = open ("log.txt", O_RDONLY);
    if (fd == -1) {
        write (2, "Error: Cannot open log.txt.\n", 28);
        exit (1);
    }
    char logs [1024];
    size_t read_chars;
    while ((read_chars = read (fd, logs, sizeof (logs))) > 0) {
        write (1, logs, read_chars);
    }
    write (1, "\n", 1);
    close (fd);
    exit (0);
}
int main (int argc, char *argv []) {
    if (argc < 2) {
        helpAndUsage ();
        return 0;
    }

    if (strcmp (argv [1], "createDir") == 0) {
        if (argc != 3) {
            write (2, "Usage: createDir \"folderName\"\n", 31);
            return 1;
        }
        createDir (argv [2]);
    }
    if (strcmp (argv [1], "createFile") == 0) {
        if (argc != 3) {
            write (2, "Usage: createFile \"fileName\"\n", 30);
            return 1;
        }
        createFile (argv [2]);
    }
    if (strcmp (argv [1], "listDir") == 0) {
        if (argc != 3) {
            write (2, "Usage: listDir \"folderName\"\n", 29);
            return 1;
        }
        listDir (argv [2]);
    }
    if (strcmp (argv [1], "listFilesByExtension") == 0) {
        if (argc != 4) {
            write (2, "Usage: listFilesByExtension \"folder_name\" \".extension\"\n", 56);
            return 1;
        }
        listFilesByExtension (argv [2], argv [3]);
    }
    if (strcmp (argv [1], "readFile") == 0) {
        if (argc != 3) {
            write (2, "Usage: readFile \"fileName\"\n", 28);
            return 1;
        }
        readFile (argv [2]);
    }
    if (strcmp (argv [1], "appendToFile") == 0) {
        if (argc != 4) {
            write (2, "Usage: appendToFile \"fileName\" \"new content\"\n", 46);
            return 1;
        }
        appendToFile (argv [2], argv [3]);
    }
    if (strcmp (argv [1], "deleteFile") == 0) {
        if (argc != 3) {
            write (2, "Usage: deleteFile \"fileName\"\n", 30);
            return 1;
        }
        deleteFile (argv [2]);
    }
    if (strcmp (argv [1], "deleteDir") == 0) {
        if (argc != 3) {
            write (2, "Usage: deleteDir \"folderName\"\n", 31);
            return 1;
        }
        deleteDir (argv [2]);
    }
    if (strcmp (argv [1], "showLogs") == 0) {
        if (argc != 2) {
            write (2, "Usage: showLogs\n", 16);
            return 1;
        }
        showLogs ();
    }
}