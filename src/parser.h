struct request_line {
    char* method;
    char* URI;
    char* version;
};

int is_CTL(char byte);
int is_separator(char byte);
int is_token_char(char byte);
int LWS(char* byte, int size, char** next_char_pointer);
int CRLF(char* buffer, int buffer_size);
int separate_token_request_line(char* buffer, int buffer_size, char** element);
struct request_line* parse_request_line(char* buffer, int buffer_size);

typedef struct header {
    char* field_name;
    char* field_value; // TODO: Implement RFC 2616 4.2 specification
} header_t;