//
//  circbuf.hpp
//  project 1
//
//  Created by Sharlotte Holmes on 1/18/24.
//
//  What I learned:
//
//
//  Problems I overcame:
//
//

#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <string>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cmath>
using std::string;

class CircBuf {
private:
    const size_t CHUNK {8};
    // Insert your private stuff here
    char *char_array;
    size_t current_size;
    size_t current_capacity;
    size_t read;
    size_t write;
    
public:
    // Number of elements you want it to be able to hold to start with.
    CircBuf (size_t reserve = 0);
    
    ~CircBuf();
    size_t size();
    size_t capacity();
    
    void insert (char);
    void insert (const char*, size_t sz);
    void insert (const string&);
    char get();
    string get(size_t);
    
    // Returns a string with all the characters, AND shrinks the buffer to zero.
    string flush();
    
    string examine();
    
    // Reduces the unused space in the buffer.
    void shrink();
    
    void expand(size_t num_chunks);
    //relate to size increase
};

#endif
