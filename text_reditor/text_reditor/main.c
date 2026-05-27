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
    printf("*New line is started\n*");
}

void saveToFile(struct TextBuffer* buffer){
    char filename[256];
    printf(">>Enter the file name for saving: ");
    if (scanf("%s", filename) == 1){
        while (getchar() != '\n');
        FILE* file = fopen(filename, "w");
        if (file != NULL){
            for (int i = 0; i<buffer->line_count; i++){
                fputs(buffer->lines[i], file);
                fputs("\n", file);
            }
        }
        else{
            printf("*Error: Couldn't open file.\n");
            return;
        }
        fclose(file);
        printf("*Text has been saved successfully*\n");
    }
}

void loadFromFile(struct TextBuffer* buffer){
    char filename[256];
    printf(">>Enter the file name for loading: ");
    if (scanf("%s", filename) == 1){
        while (getchar() != '\n');
        FILE* file = fopen(filename, "r");
        if (file != NULL){
            for (int i = 0; i<buffer->line_count; i++){
                free(buffer->lines[i]);
            }
            buffer->line_count = 0;
            char temp_buffer[256];
            while (fgets(temp_buffer, sizeof(temp_buffer), file) != NULL){
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
                for (int i = 0; i<buffer->line_count; i++){
                    printf(" %s\n", buffer->lines[i]);
                }
            }
            fclose(file);
            printf("*Text has been loaded successfully*\n");
        }
        else{
            printf("*Error: Couldn't load from the file.\n");
            return;
        }
    }
}

void insertText(struct TextBuffer* buffer){
    int line_index;
    int symbol_index;
    printf(">>Choose line and index: ");
    if (scanf("%d %d", &line_index, &symbol_index) != 2){
        printf("*Error: Invalid input format.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    if (line_index < 0 || line_index >= buffer->line_count){
        printf("Error: No such index.\n");
        return;
    }
    printf(">>Enter text to insert: ");
    char temp_buffer[256];
    if (fgets(temp_buffer, sizeof(temp_buffer), stdin) != NULL){
        int length = strlen(temp_buffer);
        if (length >0 && temp_buffer[length-1]=='\n'){
            temp_buffer[length-1] = '\0';
        }
        int old_len = strlen(buffer->lines[line_index]);
        if (symbol_index < 0 || symbol_index > old_len){
            printf("*Error: No such index.\n");
            return;
        }
        int new_len = strlen(temp_buffer);
        char* new_line = (char*)malloc((old_len+new_len+1) * sizeof(char));
        strncpy(new_line, buffer->lines[line_index], symbol_index);
        new_line[symbol_index] = '\0';
        strcat(new_line, temp_buffer);
        strcat(new_line, buffer->lines[line_index]+symbol_index);
        free(buffer->lines[line_index]);
        buffer->lines[line_index] = new_line;
        printf("*Text was inserted successfully*\n");
    }
}

void searchWord(struct TextBuffer* buffer){
    char search_term[256];
    printf(">>Enter text to search: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL){
        int length = strlen(search_term);
        if (length >0 && search_term[length-1]=='\n'){
            search_term[length-1] = '\0';
        }
        if (strlen(search_term) == 0) {
            printf("*Error: Search term was empty.\n");
            return;
        }
        int found_count = 0;
        for (int i = 0; i<buffer->line_count; i++){
            char* found_ptr = strstr(buffer->lines[i], search_term);
            if (found_ptr != NULL){
                int symbol_index = found_ptr-buffer->lines[i];
                printf(">>Text is present in this position: %d %d\n", i, symbol_index);
                found_count++;
            }
        }
        if (found_count == 0){
            printf("*Search term was not found*\n");
            return;
        }
    }
}

void clearConsole(){
    system("clear");
}

int main() {
    struct TextBuffer* textStorage = allocate();
    int command = 0;
    int running = 1;
    printf("--- Menu ---\n");
    printf("1. Append text\n2. Start new line\n3. Save to file\n4. Load from file\n");
    printf("5. Print current text\n6. Insert text by index\n7. Search by word\n8. Clear console\n9. Exit\n");
    
    while(running == 1){
        printf(">Choose the command: ");
        if (scanf("%d", &command) != 1){
            printf("///Error, type a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        switch(command){
            case 1:
                appendText(textStorage);
                break;
            case 2:
                insertNewLine(textStorage);
                break;
            case 3:
                saveToFile(textStorage);
                break;
            case 4:
                loadFromFile(textStorage);
                break;
            case 5:
                printText(textStorage);
                break;
            case 6:
                insertText(textStorage);
                break;
            case 7:
                searchWord(textStorage);
                break;
            case 8:
                clearConsole();
                break;
            case 9:
                printf("You have finished the process\n");
                running = 0;
                break;
            default:
                printf("Unknown command\n");
                break;
        }
    }
    freeBuffer(textStorage);
    return 0;
}
