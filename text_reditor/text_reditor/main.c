//
//  main.c
//  text_reditor
//
//  Created by Maria Goncharuk on 25.05.2026.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void appendText(struct TextBuffer* buffer){
    printf(">Enter text to append: ");
    char temp_buffer[256];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) != NULL){
        int length = strlen(temp_buffer);
        if (length >0 && temp_buffer[length-1]=='\n'){
            temp_buffer[length-1] = '\0';
        }
        if (buffer->line_count >= buffer->capacity){
            buffer->capacity *= 2;
            buffer->lines = (char**)realloc(buffer->lines, buffer->capacity * sizeof(char*));
        }
        int clean_len = strlen(temp_buffer);
        char* new_str = (char*)malloc((clean_len+1)* sizeof(char));
        strcpy(new_str, temp_buffer);
        buffer->lines[buffer->line_count]=new_str;
        buffer->line_count++;
        printf("*Your text was added*\n");
    }
}

void printText(struct TextBuffer* buffer){
    if (buffer->line_count == 0){
        printf("*Text is empty*\n");
        return;
    }
    for (int i = 0; i<buffer->line_count; i++){
        printf(" %s\n", buffer->lines[i]);
    }
}

void freeBuffer(struct TextBuffer* buffer){
    for (int i = 0; i < buffer->line_count; ++i){
        free(buffer->lines[i]);
    }
    free (buffer->lines);
    free(buffer);
    printf("*Memory was freed\n*");
}

void insertNewLine(struct TextBuffer* buffer){
    if (buffer->line_count >= buffer->capacity){
        buffer->capacity *= 2;
        buffer->lines = (char**)realloc(buffer->lines, buffer->capacity * sizeof(char*));
    }
    char* new_str = (char*)malloc(1*sizeof(char));
    new_str[0] = '\0';
    buffer->lines[buffer->line_count] = new_str;
    buffer->line_count++;
    printf("*New line is started*");
}

int main() {
    struct TextBuffer* textStorage = allocate();
    int command = 0;
    int running = 1;
    
    while(running == 1){
        printf(">Choose the command: ");
        if (scanf("%d", &command) != 1){
            printf("///Error, type a number.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch(command){
            case 1:
                appendText(textStorage);
                break;
            case 2:
                // і так далі
                break;
            case 3:
                // тут виклик функції
                break;
            case 4:
                // і так далі
                break;
            case 5:
                printText(textStorage);
                break;
            case 6:
                // і так далі
                break;
            case 7:
                // тут виклик функції
                break;
            case 8:
                printf("You have finished the process");
                running = 0;
                break;
            default:
                printf("Unknown command");
                break;
        }
    }
    freeBuffer(textStorage);
    return 0;
}
