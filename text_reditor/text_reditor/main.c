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
    int cursor_line;
    int cursor_symbol;
    char* clipboard;
};

struct TextBuffer* allocate(){
    struct TextBuffer* buffer = (struct TextBuffer*)malloc(sizeof(struct TextBuffer));
    buffer->capacity = 10;
    buffer->line_count = 0;
    buffer->lines = (char**)malloc(buffer->capacity*sizeof(char*));
    buffer->cursor_line = 0;
    buffer->cursor_symbol = 0;
    buffer->clipboard = NULL;
    return buffer;
}

void setCursor(struct TextBuffer* buffer){
    if (buffer->line_count == 0){
        printf("Error: Text is empty, cannot set cursos.\n");
        return;
    }
    int line;
    int symbol;
    printf(">>Enter line and symbol index to place cursor: ");
    if (scanf("%d %d", &line, &symbol) != 2){
        printf("*Error: Invalid input format.*\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    if (line < 0 || line >= buffer->line_count) {
        printf("*Error: No such line index.*\n");
        return;
    }

    int current_len = strlen(buffer->lines[line]);
    if (symbol < 0 || symbol > current_len) {
        printf("*Error: No such symbol index.*\n");
        return;
    }
    buffer->cursor_line = line;
    buffer->cursor_symbol = symbol;
    printf("*Cursor was set at [%d: %d]*\n", line, symbol);
}


char* readDynamicLine(FILE* stream){
    int capacity = 256;
    int length = 0;
    char* buffer = (char*)malloc(capacity * sizeof(char));
    if (buffer == NULL) return NULL;
    buffer[0] = '\0';
    while(fgets(buffer + length, capacity - length, stream) != NULL){
        length += strlen(buffer+length);
        if (length > 0 && buffer[length-1] == '\n'){
            buffer[length-1] = '\0';
            break;
        }
        capacity *= 2;
        char* new_buffer = (char*)realloc(buffer,capacity * sizeof(char));
        if (new_buffer == NULL){
            free(buffer);
            return NULL;
        }
        buffer = new_buffer;
    }
    if (length == 0 && buffer[0] == '\0'){
        free(buffer);
        return NULL;
    }
    return buffer;
}

void appendText(struct TextBuffer* buffer){
    printf(">Enter text to append: ");
    char* temp_buffer = readDynamicLine(stdin);
    if (temp_buffer != NULL){
        int length = strlen(temp_buffer);
        if (buffer->line_count == 0){
            if (buffer->line_count >= buffer->capacity){
                buffer->capacity *= 2;
                buffer->lines = (char**)realloc(buffer->lines, buffer->capacity * sizeof(char*));
            }
            char* new_str = (char*)malloc((length + 1) * sizeof(char));
            if (new_str != NULL) {
                strcpy(new_str, temp_buffer);
                buffer->lines[buffer->line_count] = new_str;
                buffer->line_count++;
                printf("*Your text was added*\n");
            }
        }
        else{
            int last_idx = buffer->line_count - 1;
            int old_len = strlen(buffer->lines[last_idx]);
            char* resized_line = (char*)realloc(buffer->lines[last_idx], (old_len + length + 1) * sizeof(char));
            if (resized_line != NULL) {
                buffer->lines[last_idx] = resized_line;
                strcat(buffer->lines[last_idx], temp_buffer);
                printf("*Your text was added*\n");
            }
        }
        free(temp_buffer);
    }
}

void printText(struct TextBuffer* buffer){
    if (buffer->line_count == 0){
        printf("*Text is empty*\n");
        return;
    }
    for (int i = 0; i<buffer->line_count; i++){
        printf("%s\n", buffer->lines[i]);
    }
}

void freeBuffer(struct TextBuffer* buffer){
    for (int i = 0; i < buffer->line_count; ++i){
        free(buffer->lines[i]);
    }
    if (buffer->clipboard != NULL){
        free(buffer->clipboard);
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
    printf("*New line is started*\n");
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
            fclose(file);
            printf("*Text has been saved successfully*\n");
        }
        else{
            printf("*Error: Couldn't open file.\n");
            return;
        }
        
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
            char* temp_buffer;
            while ((temp_buffer = readDynamicLine(file)) != NULL){
                if (buffer->line_count >= buffer->capacity){
                    buffer->capacity *= 2;
                    buffer->lines = (char**)realloc(buffer->lines, buffer->capacity * sizeof(char*));
                }
                temp_buffer = (char*)realloc(temp_buffer, (strlen(temp_buffer)+1) * sizeof(char));
                buffer->lines[buffer->line_count]=temp_buffer;
                buffer->line_count++;
            }
            fclose(file);
            printf("*Text has been loaded successfully*\n");
        }
        else{
            printf("*Error: Couldn't load from the file.\n");
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
        if (new_line != NULL){
            strncpy(new_line, buffer->lines[line_index], symbol_index);
            new_line[symbol_index] = '\0';
            strcat(new_line, temp_buffer);
            strcat(new_line, buffer->lines[line_index]+symbol_index);
            free(buffer->lines[line_index]);
            buffer->lines[line_index] = new_line;
            printf("*Text was inserted successfully*\n");
        }
        free(temp_buffer);
    }
}

void deleteLogic(struct TextBuffer* buffer, int number_of_symbols){
    int line_index = buffer->cursor_line;
    int symbol_index = buffer->cursor_symbol;
    if (line_index >= buffer->line_count){
        printf("*Error: Cursor is out of bounds*\n");
        return;
    }
    int old_len = strlen(buffer->lines[line_index]);
    if ((symbol_index + number_of_symbols) > old_len){
        printf("*Error: Cant delete unexising symbols*\n");
        return;
    }
    char* dest = buffer->lines[line_index] + symbol_index;
    char* src = dest + number_of_symbols;
    int bytes_to_move = old_len - (symbol_index + number_of_symbols) + 1;
    memmove(dest, src, bytes_to_move);
    int new_len = old_len - number_of_symbols;
    if (new_len == 0) {
        free(buffer->lines[line_index]);
        for (int i = line_index; i < buffer->line_count - 1; i++) {
            buffer->lines[i] = buffer->lines[i + 1];
        }
        buffer->line_count--;
        buffer->cursor_line = 0;
        buffer->cursor_symbol = 0;
        printf("*Text was deleted successfully*\n");
    }
    else{
        char* safe_buffer = (char*)realloc(buffer->lines[line_index], (new_len + 1) * sizeof(char));
        if (safe_buffer != NULL) {
            buffer->lines[line_index] = safe_buffer;
        }
    }
}
    
void delete(struct TextBuffer* buffer){
    if (buffer->line_count == 0) return;
    int number_of_symbols;
    printf(">>Choose number of symbols to delete: ");
    if (scanf("%d", &number_of_symbols) != 1){
        printf("*Error: Invalid input format*\n");
        while(getchar() != '\n');
        return;
    }
    deleteLogic(buffer, number_of_symbols);
    printf("*Text was deleted successfully*\n");
}

void copyLogic(struct TextBuffer* buffer, int num){
    int line_index = buffer->cursor_line;
        int symbol_index = buffer->cursor_symbol;
        int old_len = strlen(buffer->lines[line_index]);
        if (symbol_index+num > old_len){
            num = old_len - symbol_index;
        }
        if (buffer->clipboard != NULL){
            free(buffer->clipboard);
        }
        buffer->clipboard = (char*)malloc((num + 1)* sizeof(char));
        strncpy(buffer->clipboard, buffer->lines[line_index]+symbol_index, num);
        buffer->clipboard[num] = '\0';
}

void copy(struct TextBuffer* buffer){
    if (buffer->line_count == 0) return;
    int num;
    printf(">>Choose number of symbols to copy: ");
    if (scanf("%d", &num) != 1){
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    copyLogic(buffer, num);
    printf("*Text was copied*\n");
}

void cut(struct TextBuffer* buffer){
    if (buffer->line_count == 0) return;
    int num;
    printf(">>Chose number of symbols to cut: ");
    if (scanf("%d", &num) != 1){
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    copyLogic(buffer, num);
    deleteLogic(buffer, num);
    printf("*Text was cutted*\n");
}

void paste(struct TextBuffer* buffer){
    if (buffer->clipboard == NULL) {
        printf("*Error: Clipboard is empty*\n");
        return;
    }
    int line_index = buffer->cursor_line;
    int symbol_index = buffer->cursor_symbol;
    if (line_index >= buffer->line_count) {
        printf("*Error: Cursor is out of bounds*\n");
        return;
    }
    int old_len = strlen(buffer->lines[line_index]);
    int paste_len = strlen(buffer->clipboard);
    if (symbol_index > old_len) {
        symbol_index = old_len;
    }
    char* safe_buffer = (char*)realloc(buffer->lines[line_index], (old_len+paste_len+1)*sizeof(char));
    if (safe_buffer == NULL){
        printf("*Error: memory reallocation faild*\n");
    }
    buffer->lines[line_index] = safe_buffer;
    char* insert_p = buffer->lines[line_index] + symbol_index;
    int bytes_to_move = old_len - symbol_index + 1;
    memmove(insert_p+paste_len, insert_p, bytes_to_move);
    memcpy(insert_p, buffer->clipboard, paste_len);
    buffer->cursor_symbol += paste_len;
    printf("*Text was pasted*\n");
}

void searchWord(struct TextBuffer* buffer){
    char* search_term = readDynamicLine(stdin);
    printf(">>Enter text to search: ");
    if (search_term != NULL){
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
        free(search_term);
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
