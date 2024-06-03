#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Include for isdigit and isalpha functions

#define MAX_STRING_LENGTH 30
#define range 256

/// Node structure represents a node in the doubly linked list
struct Node {
    char Data[MAX_STRING_LENGTH + 1];  // Data array to store a string (word) of maximum length
                                       // +1 for null terminator '\0'
    struct Node* Next;  // Pointer to the next node in the linked list
    struct Node* Prev;  // Pointer to the previous node in the linked list
};

/// DoublyLinkedList structure represents a doubly linked list
struct DoublyLinkedList {
    struct Node* Head;  // Pointer to the first node (head) of the linked list
    struct Node* Tail;  // Pointer to the last node (tail) of the linked list
};


/******************* Function Prototype ********************/
struct Node* createNode(const char* data);
struct Node* insert(struct Node* head, const char* data);
void freeList(struct DoublyLinkedList* list);
int isValidWord(const char* word);
void readFromFile(struct DoublyLinkedList* list, const char* filename);
void print_pefor_sorting(struct DoublyLinkedList* list);
void countSort(struct DoublyLinkedList* list, int digit);
int getMaxStringLength(struct DoublyLinkedList* list);
void radixSort(struct DoublyLinkedList* list);
void printSortedStrings(struct DoublyLinkedList* list);
void deleteNode(struct DoublyLinkedList* list, const char* data);
void saveToFile(struct DoublyLinkedList* list, const char* filename);


/// Function to create a new node
struct Node* createNode(const char* data) {
    // Allocate memory for the new node
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        printf("Memory allocation error\n");
        exit(0);
    }

    // Use strncpy to copy and truncate the data to MAX_STRING_LENGTH
    strncpy(new_node->Data, data, MAX_STRING_LENGTH);
    new_node->Data[MAX_STRING_LENGTH] = '\0'; // Ensure null-termination

    new_node->Next = NULL;
    new_node->Prev = NULL;

    return new_node;
}

/// Function to insert a new node to the end of the doubly linked list
struct Node* insert(struct Node* head, const char* data) {
    // Create a new node
    struct Node* new_node = createNode(data);

    // If the list is empty, the new node becomes the head
    if (head == NULL) {
        head = new_node;
    } else {
        // Traverse the list to find the last node
        struct Node* current = head;
        while (current->Next != NULL) {
            current = current->Next;
        }

        // Link the new node to the last node
        current->Next = new_node;
        new_node->Prev = current;
    }

    return head;
}


/// Function to free the memory allocated for the doubly linked list
void freeList(struct DoublyLinkedList* list) {
    struct Node* current = list->Head;
    struct Node* next;

    while (current != NULL) {
        next = current->Next;
        free(current);
        current = next;
    }

    list->Head = NULL;
    list->Tail = NULL;
}


/// isValidWord function checks the validity of a word based on the specified criteria
int isValidWord(const char* word) {
    size_t length = strlen(word);

    // Check if the line is longer than 30 characters
    if (length > MAX_STRING_LENGTH) {
        printf("Note: Line \"%s\" longer than 30 characters. Skipping.\n", word);
        return 0; // Invalid word
    }

    // Check if the word starts with a digit/number
    if (isdigit(word[0])) {
        printf("Note: Line \"%s\" starting with a number. Skipping.\n", word);
        return 0; // Invalid word
    }

    // Check if the word contains any special characters
    for (size_t i = 0; i < length; i++) {
        if (!isalnum(word[i]) && word[i] != '_') {
            printf("Note: Line \"%s\" containing special characters. Skipping.\n", word);
            return 0; // Invalid word
        }
    }

    return 1; // Valid word
}


/// readFromFile function reads strings from a file and inserts valid ones into a doubly linked list
void readFromFile(struct DoublyLinkedList* list, const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    char buffer[MAX_STRING_LENGTH + 2];
    char invalidWord[MAX_STRING_LENGTH + 2];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t length = strlen(buffer);

        // Check if the line exceeds the maximum allowed length
        if (length > MAX_STRING_LENGTH && buffer[length - 1] != '\n') {
            // Discard the entire line if it exceeds the maximum allowed length
            strncpy(invalidWord, buffer, MAX_STRING_LENGTH);
            invalidWord[MAX_STRING_LENGTH] = '\0'; // Null-terminate the invalid word
            printf("Note: There are strings longer than 30 characters. Skipping.\n");
            while (fgets(buffer, sizeof(buffer), file) != NULL && buffer[strlen(buffer) - 1] != '\n') {
                // Continue reading until a newline character is encountered
            }
            continue;
        }

        // Remove the newline character if it exists
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
            length--;
        }

        if (isValidWord(buffer)) {
            list->Head = insert(list->Head, buffer);
        }
    }

    fclose(file);

    printf("File has been successfully read.\n");
}


/// Function to print strings before sorting
void print_pefor_sorting(struct DoublyLinkedList* list) {
    if (list->Head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    printf("Strings before sorting:\n");
    struct Node* current = list->Head;
    while (current != NULL) {
        // Print without an additional newline
        printf("%s", current->Data);

        // Move to the next node
        current = current->Next;

        // Print a newline if there is another node
        if (current != NULL) {
            printf("\n");
        }
    }
}


/// countSort function performs counting sort on the strings based on a specified digit
void countSort(struct DoublyLinkedList* list, int digit) {
    struct Node* output = NULL;
    struct Node* buckets[range];

    // Initialize the buckets
    for (int i = 0; i < range; i++) {
        buckets[i] = NULL;
    }

    /// Distribute strings into buckets based on the current digit
    struct Node* current = list->Head;
    while (current != NULL) {
        char currentChar = current->Data[digit];
        int index;

        // Determine the index based on the type of character
        if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z')) {
            // Letters
            index = currentChar - 'A';
        } else if (currentChar >= '0' && currentChar <= '9') {
            // Numbers
            index = currentChar - '0' + 52;  // Adjust the index for numbers
        } else {
            // Other characters
            index = currentChar;
        }

        //printf("Processing node: %s, Index: %d\n", current->Data, index);

        struct Node* next = current->Next;

        /// Remove the current node from the original list
        if (current->Prev != NULL) {
            current->Prev->Next = current->Next;
        } else {
            list->Head = current->Next;
        }

        if (current->Next != NULL) {
            current->Next->Prev = current->Prev;
        } else {
            list->Tail = current->Prev;
        }

        current->Next = NULL;
        current->Prev = NULL;

        /// Add the current node to the corresponding bucket
        if (buckets[index] == NULL) {
            buckets[index] = current;
        } else {
            struct Node* last = buckets[index];
            while (last->Next != NULL) {
                last = last->Next;
            }
            last->Next = current;
            current->Prev = last;
        }

        current = next;
    }

    /// Collect strings from buckets and update the original list
    list->Head = NULL;
    list->Tail = NULL;
    for (int i = 0; i < range; i++) {
        struct Node* bucket = buckets[i];
        while (bucket != NULL) {
            if (list->Head == NULL) {
                list->Head = bucket;
                list->Tail = bucket;
            } else {
                list->Tail->Next = bucket;
                bucket->Prev = list->Tail;
                list->Tail = bucket;
            }
            bucket = bucket->Next;
        }
    }
}


/// getMaxStringLength function calculates and returns the maximum length of strings in the DoublyLinkedList
int getMaxStringLength(struct DoublyLinkedList* list) {
    int maxLength = 0;
    struct Node* current = list->Head;

    // Iterate through the linked list
    while (current != NULL) {
        // Update maxLength if the length of the current string is greater
        if (strlen(current->Data) > maxLength) {
            maxLength = strlen(current->Data);
        }

        // Move to the next node
        current = current->Next;
    }

    // Return the calculated maximum string length
    return maxLength;
}


/// radixSort function performs radix sort on the DoublyLinkedList of strings
void radixSort(struct DoublyLinkedList* list) {
    int maxLength = getMaxStringLength(list);

    // Perform radix sort for each digit position (starting from the least significant digit)
    for (int digit = maxLength - 1; digit >= 0; digit--) {
        countSort(list, digit);
    }
}

///Function to print sorted string
void printSortedStrings(struct DoublyLinkedList* list) {
    if (list->Head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    printf("Sorted strings:\n");
    struct Node* current = list->Head;
    while (current != NULL) {
        printf("%s\n", current->Data);
        current = current->Next;
    }
}

/// Function to delete a node with a specific data from the list
void deleteNode(struct DoublyLinkedList* list, const char* data) {
    struct Node* current = list->Head;

    // Search for the node with the specified data
    while (current != NULL) {
        if (strcmp(current->Data, data) == 0) {
            // Node found, update pointers to remove it from the list
            if (current->Prev != NULL) {
                current->Prev->Next = current->Next;
            } else {
                list->Head = current->Next;
            }

            if (current->Next != NULL) {
                current->Next->Prev = current->Prev;
            } else {
                list->Tail = current->Prev;
            }

            // Free the memory of the deleted node
            free(current);

            printf("Word \"%s\" deleted from the list.\n", data);
            return;
        }

        current = current->Next;
    }

    // Word not found in the list
    printf("Word \"%s\" not found in the list.\n", data);
}


/// Function to save the sorted strings to an output file
void saveToFile(struct DoublyLinkedList* list, const char* filename) {
    // Attempt to open the file for writing
    FILE* file = fopen(filename, "w");

    // Check if the file opening was successful
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    // Write each string to the file
    struct Node* current = list->Head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->Data);
        current = current->Next;
    }

    // Close the file after writing
    fclose(file);

    printf("Sorted strings have been successfully saved to %s.\n", filename);
}



int main() {
    // Create an empty doubly linked list
    struct DoublyLinkedList myList;
    myList.Head = NULL;
    myList.Tail = NULL;

    char outputFilename[100];
    char newWord[MAX_STRING_LENGTH + 1];
    char wordToDelete[MAX_STRING_LENGTH + 1];
    int choice;
    char filename[100];

    do {
        printf("\n1. Load strings from input file\n");
        printf("2. Print strings before sorting\n");
        printf("3. Sort strings using Radix Sort\n");
        printf("4. Print sorted strings\n");
        printf("5. Add a new word to the list (and sort it)\n");
        printf("6. Delete a word from the sorted strings\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n");
        printf("-----------------------------------------------\n");
        printf("Enter your choice: \n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // Implementation to load strings from an input file
            printf("Enter the filename: ");
            scanf("%s", filename);
            readFromFile(&myList, filename);
            break;

        case 2:
            // Implementation to print strings before sorting
            print_pefor_sorting(&myList);
            break;

        case 3:
            // Implementation to sort strings using Radix Sort
            radixSort(&myList);
            printf("Strings sorted using Radix Sort.\n");

            break;
        case 4:
            // Implementation to print sorted strings
            printSortedStrings(&myList);
            break;
        case 5:
            // Implementation to add a new word to the list and sort it
            printf("Enter the new word to add (30 characters or less): ");
            scanf("%s", newWord);

            // Check if the new word is valid using the isValidWord function
            if (isValidWord(newWord))
            {
                // Insert the new word and sort the list
                myList.Head = insert(myList.Head, newWord);
                radixSort(&myList);
                printf("Word \"%s\" added to the list and sorted.\n", newWord);
                printSortedStrings(&myList);
            }
            break;

        case 6:
            // Implementation to delete a word from the sorted strings
            printf("Enter the word to delete: ");
            scanf("%s", wordToDelete);
            deleteNode(&myList, wordToDelete);
            printSortedStrings(&myList);

            break;

        case 7:
            // Implementation to save to output file
            printf("Enter the output filename: ");
            scanf("%s", outputFilename);
            saveToFile(&myList, outputFilename);
            break;

        case 8:
            // Implementation to exit
            printf("Exiting the program.\n");
            // Free the memory allocated for the doubly linked list before exiting
            freeList(&myList);
            exit(0);
            break;

        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 8);

    // Free the memory allocated for the doubly linked list before exiting
    freeList(&myList);

    return 0;
}
