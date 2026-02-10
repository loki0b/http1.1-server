#include <stdlib.h>

#define DEL 0x7F
#define SP  0x20
#define HT  0x09
#define CR  0x0D
#define LF  0x0A

struct request_line {
    char* method;
    char* URI;
    char* version;
};

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
    int i = 0;
    while (i < buffer_size && buffer[i] != SP) i++;
    char* token = (char*) malloc((sizeof(char) * i) + 1);
    //if (token == NULL) return NULL;

    for (int j = 0; j < i + 1; j++) token[j] = buffer[j];
    token[i + 1] = '\0';
    *element = token;

    return i + 1;
}

struct request_line* parse_request_line(char* buffer, int buffer_size) {
    int reqli = CRLF(buffer, buffer_size);
    if (reqli == -1); // Do something
    
    struct request_line* rl = (struct request_line*) malloc(sizeof(struct request_line));
    if (rl == NULL) return NULL;

    int ret = separate_token_request_line(buffer, buffer_size, &rl->method);
    ret = separate_token_request_line(&buffer[ret], buffer_size - ret, &rl->URI);
    ret = separate_token_request_line(&buffer[ret], buffer_size - ret, &rl->version);

    return rl;
} 