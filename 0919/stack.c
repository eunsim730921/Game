#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

void init(Stack *s) {
    s->top = NULL;
}

int isEmpty(Stack *s) {
    return s->top == NULL;
}

void push(Stack *s, int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    newNode->data = value;
    newNode->next = s->top;
    s->top = newNode;
}

int pop(Stack *s) {
    if (isEmpty(s)) return -1;
    Node *temp = s->top;
    int value = temp->data;
    s->top = s->top->next;
    free(temp);
    return value;
}

void printStack(Stack *s) {
    Node *curr = s->top;
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    Stack s;
    init(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    printStack(&s);

    printf("pop: %d\n", pop(&s));
    printf("pop: %d\n", pop(&s));
    printStack(&s);

    push(&s, 40);
    printStack(&s);

    printf("pop: %d\n", pop(&s));
    printf("pop: %d\n", pop(&s));
    printf("pop: %d\n", pop(&s));

    return 0;
}
