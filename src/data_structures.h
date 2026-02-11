struct node {
    void* element;
    struct node* next;
};

struct linked_list {
    struct node* head;
    struct node* tail;
    int length;
};

struct node* create_node(struct node* next, void* element);
struct linked_list* create_linked_list();
int insert(struct linked_list* list, void* element);