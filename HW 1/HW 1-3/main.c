#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define Max 700

struct LinkNode{
    int size;
    struct LinkNode *next;
    struct LinkNode *rev;
    int array[Max];
    int sorted[Max];
};
typedef struct LinkNode LinkNode;

void printNode(LinkNode *head);
void printsortNode(LinkNode *head);

void initNode(LinkNode **node){
    (*node)->next = NULL;
    (*node)->rev = (LinkNode *)malloc(sizeof(LinkNode));
    (*node)->rev->next = NULL;
    (*node)->rev->rev = *node;
    return;
}
LinkNode *addNode(LinkNode **node){
    LinkNode *newNode;
    newNode = (LinkNode *)malloc(sizeof(LinkNode));
    initNode(&newNode);
    (*node)->next = newNode;
    (*node)->rev->next = newNode->rev;
    return newNode;
}
/* quicksort */
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}
int Partition(LinkNode **node, int front, int end){
    int pivot = (*node)->sorted[end];
    int i = front -1;
    for (int j = front; j < end; j++) {
        if ((*node)->sorted[j] < pivot) {
            i++;
            swap(&((*node)->sorted[i]), &((*node)->sorted[j]));
        }
    }
    i++;
    swap(&((*node)->sorted[i]), &((*node)->sorted[end]));
    return i;
}
void QuickSort(LinkNode **node, int front, int end){
    if (front < end) {
        int pivot = Partition(node, front, end);
        QuickSort(node, front, pivot - 1);
        QuickSort(node, pivot + 1, end);
    }
    return;
}
void copytoSort(LinkNode **node){
    int len = (*node)->size;
    for (int i = 0; i < len; i++){
        (*node)->sorted[i] = (*node)->array[i];
    }
    return;
}
void Sort(LinkNode **node){
    copytoSort(node);
    QuickSort(node, 0, (*node)->size - 1);
    return;
}
void SortRev(LinkNode **node){
    LinkNode *ptr = *node;
    int size = ptr->size;
    for (int i = 0; i < size; i++)
        ptr->rev->sorted[i] = ptr->sorted[i];
    return;
}
/* function of insert */
bool full(LinkNode *node){
    return (node->size == Max);
}
void movehalf(LinkNode **node){
    LinkNode *from = *node;
    LinkNode *to = from->next;
    for (int i = 0; i < Max / 2; i++){
        to->array[i]  = from->array[i + Max / 2];
        to->rev->array[i] = from->rev->array[i];
        from->rev->array[i] = from->rev->array[i + Max / 2];
    }
    to->size = from->size = Max / 2;
    to->rev->size = from->rev->size = Max / 2;
    Sort(&to);
    Sort(&from);
    Sort(&(to->rev));
    Sort(&(from->rev));
    return;
}
void splitnode(LinkNode **node){
    LinkNode *newNode;
    newNode = (LinkNode *)malloc(sizeof(LinkNode));
    initNode(&newNode);
    LinkNode *ptr = *node;
    if (ptr->next == NULL){
        ptr->next = newNode;
        ptr->rev->next = newNode->rev;
    }else{
        newNode->next = ptr->next;
        ptr->next = newNode;
        newNode->rev->next = ptr->rev->next;
        ptr->rev->next = newNode->rev;
    }
    return;
}
void insert(int index, int x, LinkNode **head){
    LinkNode *ptr = *head;
    while (index > ptr->size + 1){
        index -= ptr->size;
        ptr = ptr->next;
    }
    if (full(ptr)){
        splitnode(&ptr);
        movehalf(&ptr);
    }
    if (index > ptr->size + 1){
        index -= ptr->size;
        ptr = ptr->next;
    }
    for (int i = ptr->size; i >= index; i--)
        ptr->array[i] = ptr->array[i - 1];
    ptr->array[index - 1] = x;
    int key = 0;
    if (x > ptr->sorted[ptr->size - 1])
        key = ptr->size;
    else{
        for (int i = 0; i < ptr->size; i++)
            if (ptr->sorted[i] >= x){
                key = i;
                break;
            }
    }
    for (int i = ptr->size; i > key; i--){
        ptr->sorted[i] = ptr->sorted[i - 1];
        ptr->rev->sorted[i] = ptr->sorted[i - 1];
    }
    ptr->sorted[key] = x;
    ptr->rev->sorted[key] = x;
    ptr->size++;
    for (int i = ptr->rev->size; i > ptr->rev->size - index + 1; i--)
        ptr->rev->array[i] = ptr->rev->array[i - 1];
    ptr->rev->array[ptr->rev->size - index + 1] = x;
    ptr->rev->size++;
    return;
}
/* function of delete */
void merge(LinkNode **node){
    LinkNode *from = *node;
    LinkNode *to = from->next;
    if (to == NULL)
        return;
    for (int i = from->size; i < from->size + to->size; i++)
        from->array[i] = to->array[i - from->size];
    from->size += to->size;
    from->next = to->next;
    free(to);
    for (int i = 0; i < from->size; i++)
        from->rev->array[i] = from->array[from->size - i - 1];
    from->rev->size = from->size;
    LinkNode *tmp = from->rev->next;
    from->rev->next = from->rev->next->next;
    free(tmp);
    return;
}
void Delete(int index, LinkNode **head){
    LinkNode *ptr = *head;
    while (index > ptr->size){
        index -= ptr->size;
        ptr = ptr->next;
    }
    int key = ptr->array[index - 1];
    for (int i = index; i < ptr->size; i++)
        ptr->array[i - 1] = ptr->array[i];
    for (int i = 0; i < ptr->size; i++){
        if (ptr->sorted[i] == key){
            key = i;
            break;
        }
    }
    for (int i = key + 1; i < ptr->size; i++){
        ptr->sorted[i - 1] = ptr->sorted[i];
        ptr->rev->sorted[i - 1] = ptr->sorted[i];
    }
    ptr->size--;
    for (int i = ptr->rev->size - index + 1; i <= ptr->rev->size; i++)
        ptr->rev->array[i - 1] = ptr->rev->array[i];
    ptr->rev->size--;
    if ((ptr->size < Max / 2) && (ptr->next != NULL) && 
        (ptr->size + ptr->next->size <= Max)){
            merge(&ptr);
            Sort(&ptr);
            Sort(&(ptr->rev));
    }
    return;
}
/* function of reverse */
void reverse(int l, int r, LinkNode **head){
    LinkNode *node_l, *node_r;
    LinkNode *ptr = *head;
    int leftNode = 0, rightNode = 0;
    while (l > ptr->size){
        l -= ptr->size;
        leftNode++;
        ptr = ptr->next;
    }
    node_l = ptr;
    ptr = *head;
    while (r > ptr->size){
        r -= ptr->size;
        rightNode++;
        ptr = ptr->next;
    }
    node_r = ptr;
    if (node_l == node_r){
        int len = r - l + 1;
        for (int i = 0; i < len / 2 ; i++){
            int tmp = node_l->array[i + (l - 1)];
            node_l->array[i + (l - 1)] = node_l->array[(r - 1) - i];
            node_l->array[(r - 1) - i] = tmp;
            tmp = node_l->rev->array[node_l->size - r + i];
            node_l->rev->array[node_l->size - r + i] = node_l->rev->array[node_l->size - l - i];
            node_l->rev->array[node_l->size - l - i] = tmp;
        }
    }else{
        l--;
        splitnode(&node_l);
        LinkNode *from = node_l;
        LinkNode *to = from->next;
        for (int i = l; i < from->size ; i++){
            to->array[i - l] = from->array[i];
            to->rev->array[i - l] = from->rev->array[i - l];
        }
        to->size = from->size - l;
        from->size = l;
        for (int i = 0; i < from->size; i++)
            from->rev->array[i] = from->array[from->size - i - 1];
        from->rev->size = from->size;
        to->rev->size = to->size;
        LinkNode *start = from;
        splitnode(&node_r);
        from = node_r;
        to = node_r->next;
        for (int i = r; i < from->size; i++){
            to->array[i - r] = from->array[i];
            to->rev->array[i - r] = from->rev->array[i - r];
        }
        to->size = from->size - r;
        from->size = r;
        for (int i = 0; i < from->size; i++)
            from->rev->array[i] = from->array[from->size - i - 1];
        from->rev->size = from->size;
        to->rev->size = to->size;
        LinkNode *end = to;
        node_r = start->next->rev;
        /* adjust the order */
        LinkNode **node_list = (LinkNode **)malloc(150 * sizeof(LinkNode *));
        int nodeLen = rightNode - leftNode + 1;
        ptr = node_l->next->rev;
        for (int i = 0; i < nodeLen; i++){
            node_list[i] = ptr;
            ptr = ptr->next;
        }
        ptr = start;
        for (int i = 0; i < nodeLen; i++){
            ptr->next = node_list[nodeLen - i - 1];
            ptr = ptr->next;
        }
        ptr->next = end;
        ptr = start;
        while (ptr != end){
            ptr->rev->next = ptr->next->rev;
            ptr = ptr->next;
        }
        free(node_list);
        /* merge the nodes if needed */
        ptr = end;
        if ((ptr->next != NULL) && (ptr->size + ptr->next->size <= Max)){
            merge(&ptr);
        }
        bool end_merged = false;
        ptr = node_r;
        if (ptr->size + ptr->next->size <= Max){
            end_merged = true;
            merge(&ptr);
        }
        bool noder_merged = false;
        ptr = start->next;
        if (ptr->size + ptr->next->size <= Max){
            if (ptr->next == node_r)
                noder_merged = true;
            merge(&ptr);
        }
        bool second_merged = false;
        ptr = start;
        if (ptr->size + ptr->next->size <= Max){
            merge(&ptr);
            second_merged = true;
        }
        /* sorting nodes */
        Sort(&start);
        SortRev(&start);
        if (!second_merged){
            Sort(&(start->next));
            SortRev(&(start->next));
        }
        if (!noder_merged){
            Sort(&node_r);
            SortRev(&node_r);
        }
        if (!end_merged){
            Sort(&end);
            SortRev(&end);
        }
    }
    return;
}
/* funciton of query */
int lessCount(int x, LinkNode *node){
    int left = 0;
    int right = node->size;
    while (left < right){
        int mid = (left + right) / 2;
        if (node->sorted[mid] >= x){
            right = mid;
        }else if (node->sorted[mid] < x) {
            left = mid + 1;
        }
    }
    return left;
}
int lessthanCount(int x, LinkNode *node){
    int left = 0;
    int right = node->size;
    while (left < right){
        int mid = (left + right) / 2;
        if (node->sorted[mid] <= x) {
            left = mid + 1;
        }else if (node->sorted[mid] > x) {
            right = mid;
        }
    }
    return left;
}
int k_least(LinkNode *start, LinkNode *end, int nodeLen, int k){
    LinkNode *ptr;
    int max = 100001;
    int min = -100000;
    int mid = (max + min) / 2;
    while (true){
        int lower = 0, upper = 0;
        ptr = start;
        for (int i = 0; i < nodeLen; i++){
            lower += lessCount(mid, ptr);
            ptr = ptr->next;
        }
        lower += lessCount(mid, end);
        if (k <= lower){
            max = mid - 1;
            mid = (max + min) / 2;
        }else {
            ptr = start;
            for (int i = 0; i < nodeLen; i++){
                upper += lessthanCount(mid, ptr);
                ptr = ptr->next;
            }
            upper += lessthanCount(mid, end);
            if (k > upper){
                min = mid + 1;
                mid = (max + min) / 2;
            }else
                break;
        }
    }
    return mid;
}
void query(int l, int r, int k, LinkNode **head){
    LinkNode *node_l, *node_r;
    LinkNode *ptr = *head;
    int leftNode = 0, rightNode = 0;
    while (l > ptr->size){
        l -= ptr->size;
        leftNode++;
        ptr = ptr->next;
    }
    node_l = ptr;
    ptr = *head;
    while (r > ptr->size){
        r -= ptr->size;
        rightNode++;
        ptr = ptr->next;
    }
    node_r = ptr;
    int nodeLen = rightNode - leftNode;
    if (node_l == node_r){
        LinkNode *empty =  (LinkNode *)malloc(sizeof(LinkNode));
        empty = NULL;
        LinkNode *end =  (LinkNode *)malloc(sizeof(LinkNode));
        end->next = NULL;
        end->size = r - l + 1;
        for (int i = l - 1; i < r; i++)
            end->array[i - (l - 1)] = node_l->array[i];
        Sort(&end);
        printf("%d\n", k_least(empty, end, nodeLen, k));
        free(empty);
        free(end);
    }else{
        LinkNode *start =  (LinkNode *)malloc(sizeof(LinkNode));
        LinkNode *end =  (LinkNode *)malloc(sizeof(LinkNode));
        start->next = node_l->next;
        start->size = node_l->size - l + 1;
        for (int i = 0; i < start->size; i++)
            start->array[i] = node_l->array[l + i - 1];
        Sort(&start);
        end->size = r;
        for (int i = 0; i < end->size; i++)
            end->array[i] = node_r->array[i];
        Sort(&end);
        printf("%d\n", k_least(start, end, nodeLen, k));
        free(start);
        free(end);
    }
    return;
}
int main(){
    int n, q;
    scanf("%d%d", &n, &q);
    LinkNode array;
    LinkNode *head = &array;
    initNode(&head);
    LinkNode *cur = head;
    int node_number = n / (6 * Max / 7 );
    int last_size = n % (6 * Max / 7 );
    /* input of array */
    for (int i = 0; i < node_number; i++){
        for (int j = 0; j < (6 * Max / 7 ); j++){
            int num;
            scanf("%d", &num);
            cur->array[j] = cur->sorted[j] = num;
            cur->rev->array[(6 * Max / 7 ) - j - 1] = cur->rev->sorted[j] = num;
        }
        cur->size = (6 * Max / 7 );
        cur->rev->size = (6 * Max / 7 );
        cur = addNode(&cur);
    }
    for (int i = 0; i < last_size; i++){
        int num;
        scanf("%d", &num);
        cur->array[i] = cur->sorted[i] = num;
        cur->rev->array[last_size - i - 1] = cur->rev->sorted[i] = num;
    }
    cur->size = last_size;
    cur->rev->size = last_size;
    /* Sort each node */
    LinkNode *ptr = head;
    while (ptr != NULL){
        Sort(&ptr);
        Sort(&(ptr->rev));
        ptr = ptr->next;
    }
    /* input of operation */
    for (int i = 0; i < q; i++){
        char operation[7];
        scanf("%s", operation);
        if (operation[0] == 'I'){
            int i, x;
            scanf("%d%d", &i, &x);
            insert(i, x, &head);
        }else if (operation[0] == 'D'){
            int i;
            scanf("%d", &i);
            Delete(i, &head);
        }else if (operation[0] == 'R'){
            int l, r;
            scanf("%d%d", &l, &r);
            reverse(l, r, &head);
        }else{
            int l, r, k;
            scanf("%d%d%d", &l, &r, &k);
            query(l, r, k, &head);
        }
    }
    return 0;
}

