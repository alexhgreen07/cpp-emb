#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <stdlib.h>

template <typename T> class list {
public:
  struct node {
    node() : next(NULL) {}

    template <typename... ARGS>
    node(ARGS &&... args) : value(args...), next(NULL) {}

    T value;
    node *next;
  };

  struct iterator {
    iterator(node *currentEntry) : currentEntry(currentEntry) {}

    T &operator*() { return currentEntry->value; }

    iterator &operator++() {
      currentEntry = currentEntry->next;
      return *this;
    }

    bool operator==(const iterator &rhs) {
      return (this->currentEntry == rhs.currentEntry);
    }

    bool operator!=(const iterator &rhs) { return !(*this == rhs); }

    node *currentEntry;
  };

  list() : head(NULL), tail(NULL), length(0) {}

  node *getHead() { return head; }

  node *getTail() { return tail; }

  void append(node &newEntry) {
    if (length == 0) {
      head = &newEntry;
      tail = &newEntry;
    } else {
      tail->next = &newEntry;
      tail = &newEntry;
      newEntry.next = NULL;
    }

    length++;
  }

  void insert(node &newEntry, size_t index = 0) {
    if (index <= length) {
      if (length == 0) {
        head = &newEntry;
        tail = &newEntry;
      } else if (index == length) {
        tail->next = &newEntry;
        tail = &newEntry;
        tail->next = NULL;
      } else {
        size_t count = 0;
        iterator previous(head);

        for (auto iter = begin(); iter != end(); ++iter) {
          if (count == index) {
            if (iter.currentEntry == head) {
              head = &newEntry;
            } else {
              previous.currentEntry->next = &newEntry;
            }

            newEntry.next = iter.currentEntry;

            break;
          }

          count++;
          previous = iter;
        }
      }

      length++;
    }
  }

  node *remove(node &nodeToRemove) {
    node *removed = NULL;
    node *current = head;
    node *previous = NULL;

    while (current != NULL) {
      if (current == &nodeToRemove) {
        if (current == head) {
          head = head->next;
          current->next = NULL;
        } else {
          unlink(previous);
        }

        removed = current;
        length--;

        break;
      }

      previous = current;
      current = current->next;
    }

    return removed;
  }

  node *remove(size_t index = 0) {
    node *removed = NULL;

    if (index < length) {
      if (length == 1) {
        removed = head;

        head = NULL;
        tail = NULL;
      } else {
        size_t count = 0;
        iterator previous(head);

        for (auto iter = begin(); iter != end(); ++iter) {
          if (count == index) {
            if (iter.currentEntry == head) {
              head = head->next;
              iter.currentEntry->next = NULL;
            } else {
              unlink(previous.currentEntry);
            }

            removed = iter.currentEntry;

            break;
          }

          count++;
          previous = iter;
        }
      }

      length--;
    }

    return removed;
  }

  T &get(size_t index) {
    size_t count = 0;
    for (auto &val : *this) {
      if (count == index) {
        return val;
      }
      count++;
    }

    return tail->value;
  }

  size_t getLength() { return length; }

  iterator begin() { return iterator(head); }

  iterator end() { return iterator(NULL); }

protected:
  node *head;
  node *tail;
  size_t length;

  void unlink(node *previous) {
    node *nodeToUnlink = previous->next;

    if (nodeToUnlink == tail) {
      tail = previous;
    }

    previous->next = nodeToUnlink->next;
    nodeToUnlink->next = NULL;
  }
};

#endif
