#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store user profiles
typedef struct UserProfile {
    char username[50];
    char name[50];
    char email[100];
    char dob[12];
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

// Function to add a user profile to the hash table
void addUser(UserProfile user, Node* hash_table[], unsigned int table_size) {
    unsigned int index = hash(user.username, table_size);

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->user = user;
    new_node->next = NULL;

    if (hash_table[index] == NULL) {
        hash_table[index] = new_node;
    } else {
        // Collision handling: chaining
        Node* current = hash_table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Function to find a user profile based on a username
UserProfile* findUser(char* username, Node* hash_table[], unsigned int table_size) {
    unsigned int index = hash(username, table_size);

    Node* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            return &current->user;
        }
        current = current->next;
    }

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

    do {
        printf("\nMenu:\n");
        printf("1. Add User\n");
        printf("2. Search Data\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add User
                UserProfile user;
                printf("Enter Username: ");
                scanf("%s", user.username);
                printf("Enter Name: ");
                scanf("%s", user.name);
                printf("Enter Email: ");
                scanf("%s", user.email);
                printf("Enter Date of Birth: ");
                scanf("%s", user.dob);

                addUser(user, hash_table, table_size);
                printf("User added successfully.\n");
                break;

            case 2:
                // Search Data
                printf("Enter Username to search: ");
                scanf("%s", search_username);
                UserProfile* found_user = findUser(search_username, hash_table, table_size);
                if (found_user != NULL) {
                    printf("User Found:\n");
                    printf("Username: %s\nName: %s\nEmail: %s\nDOB: %s\n",
                           found_user->username, found_user->name, found_user->email, found_user->dob);
                } else {
                    printf("User not found.\n");
                }
                break;

            case 3:
                // Exit
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 3);

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
