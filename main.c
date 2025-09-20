#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char url[100];
    struct Node* prev;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;
Node* current = NULL;

// Create a new node with given URL
Node* createNode(const char* url) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->url, url);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Visit a new website (add to end)
void visitWebsite(const char* url) {
    Node* newNode = createNode(url);
    if (head == NULL) {
        head = tail = current = newNode;
    } else {
        // If current is not tail, remove forward history
        while (current != tail) {
            Node* temp = tail;
            tail = tail->prev;
            tail->next = NULL;
            free(temp);
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        current = newNode;
    }
    printf("Visited: %s\n", url);
}

// Display all history
void displayHistory() {
    if (head == NULL) {
        printf("History is empty.\n");
        return;
    }
    printf("Browser History:\n");
    Node* temp = head;
    while (temp != NULL) {
        if (temp == current)
            printf("-> %s (current)\n", temp->url);
        else
            printf("   %s\n", temp->url);
        temp = temp->next;
    }
}

// Navigate backward
void goBack() {
    if (current == NULL || current->prev == NULL) {
        printf("No previous website.\n");
    } else {
        current = current->prev;
        printf("Went back to: %s\n", current->url);
    }
}

// Navigate forward
void goForward() {
    if (current == NULL || current->next == NULL) {
        printf("No forward website.\n");
    } else {
        current = current->next;
        printf("Went forward to: %s\n", current->url);
    }
}

// Delete a website by URL
void deleteWebsite(const char* url) {
    Node* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->url, url) == 0) {
            if (temp->prev)
                temp->prev->next = temp->next;
            else
                head = temp->next;

            if (temp->next)
                temp->next->prev = temp->prev;
            else
                tail = temp->prev;

            // Adjust current pointer if needed
            if (current == temp) {
                if (temp->next)
                    current = temp->next;
                else if (temp->prev)
                    current = temp->prev;
                else
                    current = NULL;
            }

            free(temp);
            printf("Deleted website: %s\n", url);
            return;
        }
        temp = temp->next;
    }
    printf("Website not found: %s\n", url);
}

// Clear entire history
void clearHistory() {
    Node* temp = head;
    while (temp != NULL) {
        Node* next = temp->next;
        free(temp);
        temp = next;
    }
    head = tail = current = NULL;
    printf("Cleared all history.\n");
}

// Linear search for a website
Node* searchWebsite(const char* url) {
    Node* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->url, url) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Swap URLs of two nodes (helper for sorting)
void swapURLs(Node* a, Node* b) {
    char temp[100];
    strcpy(temp, a->url);
    strcpy(a->url, b->url);
    strcpy(b->url, temp);
}

// Bubble sort history alphabetically
void sortHistory() {
    if (head == NULL) {
        printf("History is empty, nothing to sort.\n");
        return;
    }
    int swapped;
    Node* ptr1;
    Node* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->url, ptr1->next->url) > 0) {
                swapURLs(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("History sorted alphabetically.\n");
}

void menu() {
    printf("\nBrowser History Manager\n");
    printf("1. Visit new website\n");
    printf("2. Display history\n");
    printf("3. Go back\n");
    printf("4. Go forward\n");
    printf("5. Delete a website\n");
    printf("6. Search a website\n");
    printf("7. Sort history alphabetically\n");
    printf("8. Clear history\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    char url[100];
    Node* found;

    while (1) {
        menu();
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter website URL to visit: ");
                fgets(url, sizeof(url), stdin);
                url[strcspn(url, "\n")] = 0; // remove newline
                visitWebsite(url);
                break;

            case 2:
                displayHistory();
                break;

            case 3:
                goBack();
                break;

            case 4:
                goForward();
                break;

            case 5:
                printf("Enter website URL to delete: ");
                fgets(url, sizeof(url), stdin);
                url[strcspn(url, "\n")] = 0;
                deleteWebsite(url);
                break;

            case 6:
                printf("Enter website URL to search: ");
                fgets(url, sizeof(url), stdin);
                url[strcspn(url, "\n")] = 0;
                found = searchWebsite(url);
                if (found)
                    printf("Website found: %s\n", found->url);
                else
                    printf("Website not found in history.\n");
                break;

            case 7:
                sortHistory();
                break;

            case 8:
                clearHistory();
                break;

            case 9:
                clearHistory();
                printf("Exiting... Bye!\n");
                exit(0);

            default:
                printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}
