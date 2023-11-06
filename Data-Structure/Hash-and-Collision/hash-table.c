#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// User profiles are stored in a hash table
typedef struct UserProfile {
    char username[50];
    char name[50];
    char email[100];
    char dob[12];
    char bio[200];
    char website[100];
} UserProfile;

// Structure for nodes in the linked list (chaining)
typedef struct Node {
    UserProfile user;
    struct Node* next;
} Node;

// Function to calculate the hash of a username
unsigned int hash(char* username, unsigned int table_size) {
    unsigned int hash_val = 0;
    for (int i = 0; username[i] != '\0'; i++) {
        hash_val = hash_val * 31 + username[i];
    }
    return hash_val % table_size;
}

// Function to add a user profile to the hash table while checking for existing usernames
void addUser(UserProfile user, Node* hash_table[], unsigned int table_size) {
    unsigned int index = hash(user.username, table_size);

    // Check if the username already exists in the hash bucket
    Node* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->user.username, user.username) == 0) {
            printf("Username '%s' already exists. User not added.\n", user.username);
            return;
        }
        current = current->next;
    }

    // If the username does not exist, add the new user
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->user = user;
    new_node->next = NULL;

    if (hash_table[index] == NULL) {
        hash_table[index] = new_node;
    } else {
        // Collision handling: chaining
        current = hash_table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    printf("User '%s' added successfully.\n", user.username);
}

// Function to display all user data in a table format
void displayAllUsers(Node* hash_table[], unsigned int table_size) {
    printf("All User Data:\n");
    printf("+---------------------------------------------------------------+\n");
    printf("|  Username  |     Name     |     Email     |     DOB     |  Website  |\n");
    printf("+---------------------------------------------------------------+\n");

    for (unsigned int i = 0; i < table_size; i++) {
        Node* current = hash_table[i];
        while (current != NULL) {
            UserProfile user = current->user;
            printf("| %-10s | %-12s | %-14s | %-10s | %-12s |\n", user.username, user.name, user.email, user.dob, user.website);
            current = current->next;
        }
    }

    printf("+---------------------------------------------------------------+\n");
}
// Function to find a user profile based on a username and measure the search time
UserProfile* findUserWithTiming(char* username, Node* hash_table[], unsigned int table_size, clock_t *search_time) {
    unsigned int index = hash(username, table_size);

    clock_t start_time = clock(); // Record the start time

    Node* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            clock_t end_time = clock(); // Record the end time
            *search_time = end_time - start_time; // Calculate the search time
            return &(current->user);
        }
        current = current->next;
    }

    *search_time = -1; // Set search time to -1 if user not found
    return NULL; // User not found
}

int main() {
    unsigned int table_size = 100;
    Node* hash_table[table_size];

    for (unsigned int i = 0; i < table_size; i++) {
        hash_table[i] = NULL;
    }

    int choice;
    char search_username[50];
    clock_t search_time;
    UserProfile user;

    do {
        printf("\nMenu:\n");
        printf("1. Add User\n");
        printf("2. Search Data\n");
        printf("3. Display All Data\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add User
                printf("Enter Username: ");
                scanf("%s", user.username);
                printf("Enter Name: ");
                scanf(" %[^\n]s", user.name);
                printf("Enter Email: ");
                scanf("%s", user.email);
                printf("Enter Date of Birth: ");
                scanf("%s", user.dob);
                printf("Enter Bio: ");
                getchar(); // Consume newline character left in the buffer
                fgets(user.bio, sizeof(user.bio), stdin); // Allow input with spaces
                printf("Enter Website: ");
                scanf("%s", user.website); // Website without spaces

                addUser(user, hash_table, table_size);
                break;

            case 2:
                // Search Data
                printf("Enter Username to search: ");
                scanf("%s", search_username);
                UserProfile* found_user = findUserWithTiming(search_username, hash_table, table_size, &search_time);
                if (found_user != NULL) {
                    printf("User Found:\n");
                    printf("Username: %s\nName: %s\nEmail: %s\nDOB: %s\nBio: %s\nWebsite: %s\n",
                           found_user->username, found_user->name, found_user->email, found_user->dob, found_user->bio, found_user->website);
                    if (search_time != -1) {
                        printf("Search Time: %f seconds\n", (double)search_time / CLOCKS_PER_SEC);
                    }
                } else {
                    printf("User not found.\n");
                }
                break;


            case 3:
                // Display All Data
                displayAllUsers(hash_table, table_size);
                break;

            case 4:
                // Exit
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 4);

    // Freeing memory allocated for chaining
    for (unsigned int i = 0; i < table_size; i++) {
        Node* current = hash_table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}
