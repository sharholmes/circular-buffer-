//
//  circbuf.cpp
//  project 1
//
//  Created by Sharlotte Holmes on 1/18/24.
//
//  What I learned:
//  I honed my debugging skills, delving into function intricacies for a better grasp of their roles.
//  I also learned the imporatnce of buffers, especially for slower processors, ensuring uninterrupted data
//  reception by the device.
//
//  Problems I overcame:
//  I encountered an extra "-" being added by my examine function. I successfully resolved this issue by revisiting
//  and performing the actual calculations in my shrink fucntion. However, I faced another obstacle where my
//  flush function was dropping the initial 8 characters of the string and repeating only the first 6 characters,
//  leading to the only test failure.
//

#include "circbuf.hpp"
#include "iostream"
using std::ceil;
using std::copy;

// implement the hpp file

//CircBuf (size_t reserve = 0);
/*takes size_t parameter reverse initializes a constant CHUNK to 8
    creates a buffer with size of (reverse + CHUNK)
    initializes head & tail to 0
*/
CircBuf:: CircBuf(size_t reverse) {
    read = 0;
    write = 0;
    current_size = 0;
    current_capacity = ceil((float(reverse) / CHUNK)) * CHUNK;
    char_array = new char[current_capacity];
}
 
//~CircBuf();
/*empty destructor*/
CircBuf::~CircBuf(){
    delete [] char_array;
}

//size_t size();
/*
 returns size of circular buffer
 */
size_t CircBuf::size() {
    return current_size;
}

//size_t capacity();
/*
 returns the capacity of circular buffer
 */
size_t CircBuf::capacity() {
    return current_capacity;
}

//void insert (char);
/*
// inserts a single character into circular buffer
// */
void CircBuf::insert(char input_char) {
    if(current_size + 1 > current_capacity) {
        expand(0);
    }
    char_array[write] = input_char;
    write++;
    current_size++;
}

//void insert (const char*, size_t sz);
/*
 inserts an array of characters into circular buffer
 */
void CircBuf::insert(const char *input_char, size_t sz) {
    size_t new_size = current_size + sz;
    if (current_capacity < new_size) {
        size_t new_capacity_blocks = ceil(float(new_size) / CHUNK);
        expand(new_capacity_blocks);
    }
    
    size_t end_of_array = current_capacity;
    if (write + sz > end_of_array) {
        size_t left_half = end_of_array - write;
        size_t right_half = sz - left_half;
        
        std::copy(
            input_char,
            input_char + left_half,
            char_array + write
        );
        
        std::copy(
             input_char,
             input_char + right_half,
             char_array + read
        );
        write = right_half;
    }else{
        std::copy(input_char, input_char + sz, char_array + write);
        write += sz;
        if(write > current_capacity - 1) {
            write = write - current_capacity;
        }
    }
    current_size = new_size;
}

//void insert (const string&);
/*
 inserts a string into circular buffer
 */
void CircBuf::insert(const std::string &str) {
    size_t sz = str.length();
    size_t new_size = current_size + sz;
    if (current_capacity < new_size) {
        size_t new_capacity_blocks = ceil(float(new_size) / CHUNK);
        expand(new_capacity_blocks);
    }
    size_t end_of_array = current_capacity;
    if (write + sz >= end_of_array) {
        size_t left_half = end_of_array - write;
        size_t right_half = sz - left_half;
        
        std::copy (
                   str.begin(),
                   str.begin() + right_half,
                   char_array + write
                   );
        std::copy (
                   str.begin() + left_half,
                   str.end(),
                   char_array
                   );
        write = right_half;
    }else{
        std::copy(
                  str.begin(),
                  str.end(),
                  char_array + write
                  );
        write += sz;
        if (write > current_capacity - 1) {
            write = write - current_capacity;
        }
    }current_size = new_size;
}

//char get();
/*
 retrieves a single character from circular buffer
 */
char CircBuf::get() {
    if(current_size == 0) {
        return '\0';
    }
    char return_char = char_array[read];
    read++;
    if(read >= current_capacity) {
        read = 0;
    }
    current_size--;
    return return_char;
}

//string get(size_t);
/*
 retrieves a specified number of characters as a string from circular buffer
 */
std::string CircBuf::get(size_t sz) {
        if(current_size <= 0) {
            return "";
        }
        string result = "";
        size_t i = 0;
        while (i < sz && i < current_size) {
            result += char_array[read];
            if(read == current_capacity -1) {
                read = 0;
            }else{
                read++;
            }
            i++;
        }
    if(sz >= current_size) {
        current_size = 0;
    }else{
        current_size -= sz;
    }
    return result;
}

//string flush();
/*
 retrieves all characters from circular buffer
 shrinks the buffer
 returns the retrieved characters as a string
 */
std::string CircBuf::flush() {
    std::string result = get(current_size);
    current_size = 0;
    current_capacity = 0;
    read = 0;
    write = 0;
    shrink();
    return result;
}

//string examine();
/*
 returns a string containing the content of the circular buffer w/o removing characters
 */
std::string CircBuf::examine() {
    //case 1: empty [-,-,-,-]
    std::string result = "[";
    if((read == write) && (current_size == 0)) {
        for(size_t i = 0; i < current_capacity; i++) {
            result += "";
            current_capacity = 0;
        }
    //case 2: full [x,x,x,x]
    }else if(current_size == current_capacity) {
        for(size_t i = 0; i < current_capacity; i++) {
            result += char_array[i];
        }
    //case 3: not wrapped [x,x,-,-]
    }else if(read < write)
        for(size_t i = 0; i < current_capacity; i++) {
            if(i >= read && i < write) {
                result += char_array[i];
            }else{
                result += "-";
            }
    //case 4: wrapped [x,-,-,x]
    }else if (read > write) {
        for(size_t i = 0; i < current_capacity; i++) {
            if(i < write || i >= read){
                result += char_array[i];
            }else{
                result += "-";
            }
        }
}
    result += "]";
    return result;
}

//void shrink();
/*
 shrinks the circular buffer by resizing it to the current size + a constant CHUNK
 */
void CircBuf::shrink() {
    size_t new_size = current_capacity - (write - 2);
    //size_t new_size = current_size + CHUNK;

    if (new_size <= current_capacity) {
        char* new_char_array = new char[new_size];

        size_t new_write = 0;
        while (read != write) {
            new_char_array[new_write] = char_array[read];
            read = (read + 1) % current_capacity;
            new_write = (new_write + 1) % new_size;
        }

        delete[] char_array;
        char_array = new_char_array;
        current_capacity = new_size;
        read = 0;
        write = new_write;
    }
}

//void expand;
/*
 expands the capaticy of the array
 */

void CircBuf::expand(size_t num_chunks) {
    size_t new_capacity = current_capacity + CHUNK;
    char* new_char_array = new char[new_capacity];
    
    size_t new_write = 0;
    while (read != write) {
        new_char_array[new_write] = char_array[read];
        read = (read + 1) % current_capacity;
        new_write = (new_write + 1) % new_capacity;
    }
    
    delete [] char_array;
    char_array = new_char_array;
    current_capacity = new_capacity;
    read = 0;
    write = new_write;
}
