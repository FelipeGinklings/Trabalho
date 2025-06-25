#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename TYPE>
struct Node {
    TYPE data;
    Node<TYPE> *next;
};

template <typename TYPE>
struct Queue {
    Node<TYPE> *front;
};

template <typename TYPE>
void initializeQueue(Queue<TYPE> &q) {
    q.front = nullptr;
}

template <typename TYPE>
void addToQueue(Queue<TYPE> &q, TYPE data) {
    auto newItem = new Node<TYPE>;
    newItem->data = data;
    newItem->next = nullptr;
    if (q.front == nullptr) {
        q.front = newItem;
    } else {
        Node<TYPE> *aux = q.front;
        while (aux->next != nullptr) {
            aux = aux->next;
        }
        aux->next = newItem;
    }
}

template <typename TYPE>
TYPE dequeue(Queue<TYPE> &q) {
    if (q.front != nullptr) {
        Node<TYPE> *removed = q.front;
        TYPE data = removed->data;
        q.front = removed->next;

        delete removed;
        return data;
    }
}

#endif  // QUEUE_HPP
