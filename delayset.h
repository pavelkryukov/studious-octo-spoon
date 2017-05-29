/*  
 * delayset.h
 *
 * Set with delayed erase template
 *
 * For FreeCell Solver
 *
 * Copyright (C) Pavel Kryukov, 2011
*/

#ifndef DELAYSET_H
#define DELAYSET_H

#include <set>
#include <queue>

// T - typename
// D - delay iteration
template<typename T, int D>
class delayset : public std::set<T> {
private:
    // Queue of entries to delete
    typename std::queue<T> queue;
public:
    delayset() : std::set<T>() {}

    // Erase with delay
    void delayErase(const T & key) {

        // If queue is full, erase head
        if (queue.size() == D) {
            this->erase(queue.front());
            queue.pop();
        }

        // New key is not erased, but pushing to queue.
        queue.push(key);
    }

    void cleanQ() {
        while (!queue.empty()) {
            this->erase(queue.front());
            queue.pop();
        }
    }
};

#endif