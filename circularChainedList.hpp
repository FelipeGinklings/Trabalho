#ifndef CIRCULAR_CHAINED_LIST_H_INCLUDED
#define CIRCULAR_CHAINED_LIST_H_INCLUDED

#include <vector>

using namespace std;

template <typename Type>
struct NodeC {
    Type data;
    NodeC *next;
};

template <typename Type>
struct CircularLinkedList {
    NodeC<Type> *start, *end;
    size_t size;

    CircularLinkedList() : start(nullptr), end(nullptr), size(0) {}

    bool pushFront(Type data) {
        auto *newItem = new NodeC<Type>;
        newItem->data = data;
        this->size += 1;
        if (this->end == nullptr) {
            this->end = newItem;
            this->start = newItem;
        } else {
            newItem->next = this->start;
            this->start = newItem;
        }
        return true;
    }

    bool pushBack(Type data) {
        auto *newItem = new NodeC<Type>;
        newItem->data = data;
        newItem->next = this->start;
        this->size += 1;
        if (this->end == nullptr) {
            this->end = newItem;
            this->start = newItem;
        } else {
            this->end->next = newItem;
            this->end = newItem;
        }
        return true;
    }

    vector<Type> convertToVector() {
        vector<Type> newVector;
        NodeC<Type> *nav = this->start;
        while (nav != this->end) {
            newVector.push_back(nav->data);
            nav = nav->next;
        }
        newVector.push_back(this->end->data);
        return newVector;
    }
};

#endif
