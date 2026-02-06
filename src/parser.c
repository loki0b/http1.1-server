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

int LWS(char* line, int size, char** next_char_pointer) {
    for (int i = 0; i < size - 2; i++) {
        if (line[i] == CR && line[i + 1] == LF) {
            if (line[i + 2] == SP || line[i + 2] == HT) {
                while ((size - i > 0) && (line[i] == SP || line[i] == HT)) i++;
                *next_char_pointer = &line[i];
                // TODO: Otimizar consumo de memória reduzindo o tamanho da request removendo os SP e HT.

                return 0;
            }
        }

        *next_char_pointer = &line[i];
        return 0;
    }
}