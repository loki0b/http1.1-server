#define DEL 0x7F
#define SP  0x20
#define HT  0x09
#define CR  0x0D
#define LF  0x0A

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