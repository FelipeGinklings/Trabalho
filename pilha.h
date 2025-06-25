#ifndef STACK_HPP
#define STACK_HPP

template <typename TYPE>
struct NodeS {
    TYPE data;
    NodeS<TYPE> *next;
};

template <typename TYPE>
struct Stack {
    NodeS<TYPE> *first;
};

template <typename TYPE>
void initializeStack(Stack<TYPE> &s) {
    s.first = nullptr;
}

template <typename TYPE>
void addToTopStack(Stack<TYPE> &s, TYPE data) {
    auto newItem = new NodeS<TYPE>;
    newItem->data = data;
    newItem->next = s.first;
    s.first = newItem;
}

template <typename TYPE>
TYPE removeFromTopStack(Stack<TYPE> &s) {
    if (s.first != nullptr) {
        auto itemToRemove = s.first;
        TYPE data = itemToRemove->data;
        s.first = itemToRemove->next;

        delete itemToRemove;
        return data;
    }
    return TYPE{};  // Return default-constructed value when stack is empty
}

#endif  // STACK_HPP
