//
//  main.c
//  text_reditor
//
//  Created by Maria Goncharuk on 25.05.2026.
//

#include <stdlib.h>
#include <stdio.h>

struct TextBuffer{
    char** lines;
    int line_count;
    int capacity;
};

struct TextBuffer* allocate(){
    struct TextBuffer* buffer = (struct TextBuffer*)malloc(sizeof(struct TextBuffer));
    buffer->capacity = 10;
    buffer->line_count = 0;
    buffer->lines = (char**)malloc(buffer->capacity*sizeof(char*));
    return buffer;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return EXIT_SUCCESS;
}
