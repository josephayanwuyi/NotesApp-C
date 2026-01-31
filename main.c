#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOTE_FILE "notes.txt"
#define TEMP_FILE "temp.txt"

// Function declarations
void clear_screen(void);
void add_note(void);
void list_notes(void);
void delete_note(void);
void pause(void);

int main(void)
{
    while (true)
    {
        clear_screen();

        printf("===========================\n");
        printf("        NOTES APP\n");
        printf("===========================\n\n");
        printf("1. Add Note\n");
        printf("2. List Notes\n");
        printf("3. Delete Note\n");
        printf("4. Exit\n\n");

        string input = get_string("Enter your choice: ");
        int choice = atoi(input); // safe conversion

        if (choice == 1)
        {
            add_note();
        }
        else if (choice == 2)
        {
            list_notes();
        }
        else if (choice == 3)
        {
            delete_note();
        }
        else if (choice == 4)
        {
            printf("Exiting program...\n");
            break;
        }
        else
        {
            printf("\nInvalid option. Please enter 1, 2, 3, or 4.\n");
            pause();
        }
    }

    return 0;
}

// ------------------ Utility Functions ------------------

void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause(void)
{
    printf("\nPress Enter to continue...");
    getchar();
}

// ------------------ Add Note ------------------

void add_note(void)
{
    string note = get_string("Enter your note: ");

    if (note == NULL || strlen(note) == 0)
    {
        printf("Note cannot be empty.\n");
        pause();
        return;
    }

    // Remove trailing newline if present
    note[strcspn(note, "\n")] = '\0';

    FILE *file = fopen(NOTE_FILE, "a");
    if (file == NULL)
    {
        printf("Error: Could not open notes file.\n");
        pause();
        return;
    }

    // Add timestamp
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "[%04d-%02d-%02d %02d:%02d] %s\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, note);

    fclose(file);

    printf("Note added successfully!\n");
    pause();
}

// ------------------ List Notes ------------------

void list_notes(void)
{
    FILE *file = fopen(NOTE_FILE, "r");
    if (file == NULL)
    {
        printf("No notes found.\n");
        pause();
        return;
    }

    char line[512];
    int counter = 1;

    printf("\nYour Notes:\n");
    printf("---------------------------\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("%d. %s", counter, line);
        counter++;
    }

    if (counter == 1)
        printf("No notes available.\n");

    fclose(file);
    pause();
}

// ------------------ Delete Note ------------------

void delete_note(void)
{
    FILE *file = fopen(NOTE_FILE, "r");
    if (file == NULL)
    {
        printf("No notes to delete.\n");
        pause();
        return;
    }

    char line[512];
    int counter = 1;

    printf("\nYour Notes:\n");
    printf("---------------------------\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("%d. %s", counter, line);
        counter++;
    }

    if (counter == 1)
    {
        printf("No notes available.\n");
        fclose(file);
        pause();
        return;
    }

    int delete_line = get_int("\nEnter the number of the note to delete: ");
    if (delete_line < 1 || delete_line >= counter)
    {
        printf("Invalid note number.\n");
        fclose(file);
        pause();
        return;
    }

    rewind(file);

    FILE *temp = fopen(TEMP_FILE, "w");
    if (temp == NULL)
    {
        printf("Error accessing temporary file.\n");
        fclose(file);
        pause();
        return;
    }

    int current = 1;
    while (fgets(line, sizeof(line), file))
    {
        if (current != delete_line)
            fputs(line, temp);
        current++;
    }

    fclose(file);
    fclose(temp);

    remove(NOTE_FILE);
    rename(TEMP_FILE, NOTE_FILE);

    printf("Note deleted successfully.\n");
    pause();
}
