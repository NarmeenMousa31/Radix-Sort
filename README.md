# Radix-Sort

## Overview

This program reads strings from a file, stores them in a doubly linked list, and sorts them using Radix Sort. The sorted strings can be saved to an output file. The program supports various operations such as adding new words, deleting words, and displaying strings before and after sorting.

## Features

1. Load strings from an input file.
2. Print strings before sorting.
3. Sort strings using Radix Sort.
4. Print sorted strings.
5. Add a new word to the list and sort it.
6. Delete a word from the sorted strings.
7. Save the sorted strings to an output file.

## Implementation Details

- The program uses a doubly linked list to store strings.
- Radix Sort is implemented for sorting the strings.
- Input validation checks for string length, starting character, and special characters.
- The program provides a simple menu-based interface for user interaction.

## Functions

### `struct Node* createNode(const char* data)`
Creates a new node with the given data.

### `struct Node* insert(struct Node* head, const char* data)`
Inserts a new node at the end of the doubly linked list.

### `void freeList(struct DoublyLinkedList* list)`
Frees the memory allocated for the doubly linked list.

### `int isValidWord(const char* word)`
Checks the validity of a word based on specified criteria.

### `void readFromFile(struct DoublyLinkedList* list, const char* filename)`
Reads strings from a file and inserts valid ones into the doubly linked list.

### `void print_pefor_sorting(struct DoublyLinkedList* list)`
Prints the strings in the doubly linked list before sorting.

### `void countSort(struct DoublyLinkedList* list, int digit)`
Performs counting sort on the strings based on a specified digit.

### `int getMaxStringLength(struct DoublyLinkedList* list)`
Calculates and returns the maximum length of strings in the doubly linked list.

### `void radixSort(struct DoublyLinkedList* list)`
Performs radix sort on the doubly linked list of strings.

### `void printSortedStrings(struct DoublyLinkedList* list)`
Prints the sorted strings in the doubly linked list.

### `void deleteNode(struct DoublyLinkedList* list, const char* data)`
Deletes a node with specific data from the doubly linked list.

### `void saveToFile(struct DoublyLinkedList* list, const char* filename)`
Saves the sorted strings to an output file.


