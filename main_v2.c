#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOTE_FILE "notes.txt" // File to store notes
#define TEMP_FILE "temp.txt"  // Temporary file for delete operations

// Function declarations
void clear_screen(void); // Clears the terminal screen
void add_note(void);     // Add a new note
void list_notes(void);   // List all notes
void delete_note(void);  // Delete a specific note
void pause(void);        // Wait for user to press Enter

int main(void)
{
    while (true) // Main menu loop
    {
        clear_screen(); // Clear screen for a fresh menu

        // Display menu header
        printf("===========================\n");
        printf("        NOTES APP\n");
        printf("===========================\n\n");

        // Display menu options
        printf("1. Add Note\n");
        printf("2. List Notes\n");
        printf("3. Delete Note\n");
        printf("4. Exit\n\n");

        string input = get_string("Enter your choice: "); // Get user input
        int choice = atoi(input);                         // Convert input string to integer

        // Call appropriate function based on user choice
        if (choice == 1)
        {
            add_note(); // Add a new note
        }
        else if (choice == 2)
        {
            list_notes(); // List all existing notes
        }
        else if (choice == 3)
        {
            delete_note(); // Delete a note
        }
        else if (choice == 4)
        {
            printf("Exiting program...\n"); // Exit message
            break;                          // Exit the loop and terminate program
        }
        else
        {
            // Invalid input handling
            printf("\nInvalid option. Please enter 1, 2, 3, or 4.\n");
            pause(); // Wait for user before re-displaying menu
        }
    }

    return 0; // End of program
}

// ------------------ Utility Functions ------------------

// Clears the terminal screen depending on OS
void clear_screen(void)
{
#ifdef _WIN32
    system("cls"); // Windows clear command
#else
    system("clear"); // Linux/macOS clear command
#endif
}

// Pauses execution until user presses Enter
void pause(void)
{
    printf("\nPress Enter to continue...");
    getchar(); // Wait for Enter
}

// ------------------ Add Note ------------------

// Adds a new note with timestamp
void add_note(void)
{
    string note = get_string("Enter your note: "); // Prompt user for note

    // Check if note is empty
    if (note == NULL || strlen(note) == 0)
    {
        printf("Note cannot be empty.\n");
        pause(); // Wait for user
        return;
    }

    // Remove trailing newline character
    note[strcspn(note, "\n")] = '\0';

    // Open file in append mode
    FILE *file = fopen(NOTE_FILE, "a");
    if (file == NULL)
    {
        printf("Error: Could not open notes file.\n");
        pause();
        return;
    }

    // Get current time for timestamp
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Write note with timestamp to file
    fprintf(file, "[%04d-%02d-%02d %02d:%02d] %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, note);

    fclose(file); // Close file after writing

    printf("Note added successfully!\n"); // Confirmation message
    pause();                              // Wait for user
}

// ------------------ List Notes ------------------

// Displays all notes with line numbers
void list_notes(void)
{
    FILE *file = fopen(NOTE_FILE, "r"); // Open file in read mode
    if (file == NULL)
    {
        printf("No notes found.\n"); // Handle empty file
        pause();
        return;
    }

    char line[512];  // Buffer to read each line
    int counter = 1; // Note numbering

    printf("\nYour Notes:\n");
    printf("---------------------------\n");

    // Read and display each note
    while (fgets(line, sizeof(line), file))
    {
        printf("%d. %s", counter, line);
        counter++;
    }

    // If file was empty
    if (counter == 1)
        printf("No notes available.\n");

    fclose(file); // Close file after reading
    pause();      // Wait for user
}

// ------------------ Delete Note ------------------

// Deletes a note specified by the user
void delete_note(void)
{
    FILE *file = fopen(NOTE_FILE, "r"); // Open notes file for reading
    if (file == NULL)
    {
        printf("No notes to delete.\n"); // Handle empty file
        pause();
        return;
    }

    char line[512]; // Buffer to store lines
    int counter = 1;

    printf("\nYour Notes:\n");
    printf("---------------------------\n");

    // Display all notes with numbering
    while (fgets(line, sizeof(line), file))
    {
        printf("%d. %s", counter, line);
        counter++;
    }

    // If no notes exist
    if (counter == 1)
    {
        printf("No notes available.\n");
        fclose(file);
        pause();
        return;
    }

    // Ask user which note to delete
    int delete_line = get_int("\nEnter the number of the note to delete: ");
    if (delete_line < 1 || delete_line >= counter)
    {
        printf("Invalid note number.\n");
        fclose(file);
        pause();
        return;
    }

    rewind(file); // Go back to start of file for re-reading

    // Open temporary file to store remaining notes
    FILE *temp = fopen(TEMP_FILE, "w");
    if (temp == NULL)
    {
        printf("Error accessing temporary file.\n");
        fclose(file);
        pause();
        return;
    }

    int current = 1;
    // Copy all notes except the one to delete
    while (fgets(line, sizeof(line), file))
    {
        if (current != delete_line)
            fputs(line, temp);
        current++;
    }

    fclose(file); // Close original file
    fclose(temp); // Close temp file

    // Replace original file with temp file
    remove(NOTE_FILE);
    rename(TEMP_FILE, NOTE_FILE);

    printf("Note deleted successfully.\n"); // Confirmation
    pause();                                // Wait for user
}
