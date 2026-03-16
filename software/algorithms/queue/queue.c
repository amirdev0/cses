#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

struct queue {
    int *base;
    int *front;
    int *rear;
};

void enqueue(int value, struct queue *queue) {
    if (queue->rear + 1 == queue->front)
        return;
    
    *queue->rear = value;

    if (queue->rear + 1 > queue->base + SIZE) {
        if (queue->base == queue->front)
            return;
        else
            queue->rear = queue->base;
    }
    else
        queue->rear = queue->rear + 1;
}

int dequeue(struct queue *queue) {
    if (queue->front == queue->rear)
        return -1;

    int value = *queue->front;
    *queue->front = -1;

    if (queue->front + 1 > queue->base + SIZE)
        queue->front = queue->base;
    else
        queue->front = queue->front + 1;

    return value;
}

void draw(struct queue *queue) {
    int *iterator = queue->front;
    
    printf("front : ");
    while (iterator != queue->rear) {
        if (iterator + 1 > queue->base + SIZE + 1) {
            if (queue->base == queue->rear)
                break;
            iterator = queue->base;
        }

        printf("%d ", *iterator++);
    }
        
    printf(": rear\n");
}

