#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename TYPE>
struct Item {
    TYPE data;
    Item<TYPE> *next;
};

template <typename TYPE>
struct Queue {
    Item<TYPE> *front;
};

template <typename TYPE>
void initializeQueue(Queue<TYPE> &q) {
    q.front = nullptr;
}

template <typename TYPE>
void addToQueue(Queue<TYPE> &q, TYPE data) {
    auto newItem = new Item<TYPE>;
    newItem->data = data;
    newItem->next = nullptr;
    if (q.front == nullptr) {
        q.front = newItem;
    } else {
        Item<TYPE> *aux = q.front;
        while (aux->next != nullptr) {
            aux = aux->next;
        }
        aux->next = newItem;
    }
}

template <typename TYPE>
TYPE dequeue(Queue<TYPE> &q) {
    if (q.front != nullptr) {
        Item<TYPE> *removed = q.front;
        TYPE data = removed->data;
        q.front = removed->next;

        delete removed;
        return data;
    }
}

#endif  // QUEUE_HPP
