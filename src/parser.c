#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "data_structures.h"

#define DEL 0x7F
#define SP  0x20
#define HT  0x09
#define CR  0x0D
#define LF  0x0A
#define HEADER_SEPARATOR 0x3A

int is_CTL(char byte) {
    if ((byte - 31 <= 0) || (byte == DEL)) return 1;

    return 0;
}

// (, ), <, >, @, ,, ;, :, \, ", /, [, ], ?, =, {, }, SP (32), HT (9).
int is_separator(char byte) {
    if (byte == '(' ||
        byte == ')' ||
        byte == '<' ||
        byte == '>' ||
        byte == '@' ||
        byte == ',' ||
        byte == ';' ||
        byte == ':' ||
        byte == '\\'||
        byte == '"' ||
        byte == '/' ||
        byte == '[' ||
        byte == ']' ||
        byte == '?' ||
        byte == '=' ||
        byte == '{' ||
        byte == '}' ||
        byte == SP  ||
        byte == HT) return 1;

    return 0;
}

int is_token_char(char byte) {
    if (byte >= 0 && byte < 127 && !is_CTL(byte) && !is_separator(byte)) return 1;

    return 0;
}

// TODO: Otimizar consumo de memória reduzindo o tamanho da request removendo os SP e HT.
// Return a pointer to next char that is not LWS or SP/HT
int LWS(char* byte, int size, char** next_char_pointer) {
    int i = 0;
    if ( i < (size - 2) && byte[i] == CR && byte[i + 1] == LF) {
        if (byte[i + 2] == SP || byte[i + 2] == HT) i += 2;
    
        else return 0; // End of line
    }   
    
    while ((i < size) && (byte[i] == SP || byte[i] == HT)) i++;
    *next_char_pointer = &byte[i];
    
    if (i > 0) return 1; //LWS
    else return 0;
}

// Search for a CRLF.
// Return index of LF or -1 if not match
int CRLF(char* buffer, int buffer_size) {
    int i = 0;
    while (i < (buffer_size - 1)) {
        if (buffer[i] == CR && buffer[i + 1] == LF) {
            i++;
            return i;
        }
        i++;
    }

    return -1;
}

int separate_token_request_line(char* buffer, int buffer_size, char** element) {
    int offset = 0;

    while (offset < buffer_size) {
        if (offset < (buffer_size - 1) && buffer[offset] == CR && buffer[offset + 1] == LF) {
            offset += 2;
            break;
        }

        else if (buffer[offset] == SP) {
            offset++;
            break;
        }

        offset++;
    }
    
    char* token = (char*) malloc((sizeof(char) * offset) + 1);
    //if (token == NULL) return NULL;

    for (int j = 0; j < offset; j++) token[j] = buffer[j];
    token[offset] = '\0';
    *element = token;

    return offset;
}

struct request_line* parse_request_line(char* buffer, int buffer_size) {
    int reqli = CRLF(buffer, buffer_size);
    if (reqli == -1); // Do something
    
    struct request_line* rl = (struct request_line*) malloc(sizeof(struct request_line));
    if (rl == NULL) return NULL;

    int offset = 0;
    offset += separate_token_request_line(buffer, buffer_size, &rl->method);
    offset += separate_token_request_line(&buffer[offset], buffer_size - offset, &rl->URI);
    offset += separate_token_request_line(&buffer[offset], buffer_size - offset, &rl->version);

    return rl;
}

// TODO: Test
int parse_headers(char* buffer, int buffer_size, struct linked_list* headers) {
    if (headers == NULL || buffer == NULL) return -1;
    if (buffer_size <= 0) return 0;

    header_t* h;

    h = (header_t*) malloc(sizeof(header_t));
    if (h == NULL) return -1;

    int i = 0;
    while (i < buffer_size && buffer[i] != HEADER_SEPARATOR) i++;
    char* field_name = (char*) malloc((sizeof(char) * i) + 1);
    for (int j = 0; j < i; j++) field_name[j] = buffer[j];
    field_name[i] = '\0';
    h->field_name = field_name;

    insert(headers, h);
}