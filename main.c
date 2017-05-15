/*
 *     Lecture : Algorithms
 *
 *     Topic : Max-heap with stacks & Binary search tree
 *
 *     Summary of this program :
 *        1. Read user input, service 2 option
 *        2. Max-heap with stacks
 *           options : heap insert, heap delete, stack pop, stack push, print heap
 *        3. BST
 *           options : insert, search, delete, preorder, inorder, postorder
 *
 *     Written by Bae, YoHan,
 *        School of Electrical and Electronics Engineering,
 *        Sungkyunkwan University.
 *
 *     Written on 01-04-2017
 *
 */
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define MAX_HEAP_SIZE 100
#define MAX_BUFFER_SIZE 1024

#define INT_MAX 0x7fffffff
#define INT_MIN 0x80000000

typedef struct linked_stack {   // for linked list stack
    int val;
    struct linked_stack *link;
} LINKED_STACK;

typedef struct heap_node {      // for heap nodes
    int key;
    LINKED_STACK *top;
} HEAP_NODE;

typedef struct bst_node {       // for bst nodes
    struct bst_node *right_link;
    struct bst_node *left_link;
    int val;
} BST_NODE;

// functions for heap with stacks
void pre_heap_insert(HEAP_NODE *heap_arr, int num_heap_node);
void heap_insert(HEAP_NODE *heap_arr, int num_heap_node);
void heap_delete(HEAP_NODE *heap_arr, int num_heap_node);
void node_free(HEAP_NODE *heap_arr, int index_heap_node);
void heap_pop(HEAP_NODE *heap_arr, int num_heap_node);
int stack_pop(LINKED_STACK **top);
void heap_push(HEAP_NODE *heap_arr, int num_heap_node);
void stack_push(LINKED_STACK **top, int val);
void max_heapify(HEAP_NODE *heap_arr, int index, int num_heap_node);
void heap_swap(HEAP_NODE *heap_arr, int i, int j);
void heap_print(HEAP_NODE *heap_arr, int num_heap_node);

// functions for bst
void pre_bst_insert(BST_NODE **bst_head);
int bst_insert(BST_NODE **bst_head, int val);
void pre_bst_delete(BST_NODE **bst_head);
int bst_delete(BST_NODE **bst_head, int val);
void pre_bst_search(BST_NODE *bst_head);
BST_NODE *bst_search(BST_NODE *bst_head, int val);
BST_NODE *bst_search_parent(BST_NODE *bst_head, int val);
void bst_preorder(BST_NODE *bst_head);
void bst_inorder(BST_NODE *bst_head);
void bst_postorder(BST_NODE *bst_head);

// functions for UI
int *getNumbers(int *num);
int getIndex(int minimum, int maximum);

// functions for memory handling
void dispose_heapMem(HEAP_NODE *heap_arr, int num_heap_node);
void dispose_bstMem(BST_NODE *node);



int main() {
    int input = 0;
    int num_heap_node = 0;

    HEAP_NODE heap_arr[MAX_HEAP_SIZE];   // initialize heap using heap
    BST_NODE *bst_head = NULL;           // head pointer of bst

    printf("Welcome to John`s HW1!\n");

    /*
     * Infinite loop until user want to quit
     */
    while (1) {
        printf("(1)Max heap with stack  (2)Binary search tree  (0)Quit\n");
        input = getIndex(0, 2);
        int heap_command = -1, bst_command = -1;

        if (input == 0) {
            printf("Dispose memory and Quit the program!");
            dispose_heapMem(heap_arr, num_heap_node);
            dispose_bstMem(bst_head);
            return 0;
        } else if (input == 1) {     // Heap service mode
            while (heap_command) {
                printf("(1)heap_insert  (2)heap_delete  (3)stack_pop"
                               "  (4)stack_push  (5)print_status  (0)main_menu\n");
                heap_command = getIndex(0, 5);
                switch (heap_command) {
                    case 0:
                        break;
                    case 1:
                        pre_heap_insert(heap_arr, num_heap_node);
                        num_heap_node++;
                        break;
                    case 2:
                        if (num_heap_node == 0) {
                            printf("There is no node in the Max-Heap\n");
                            break;
                        }
                        heap_delete(heap_arr, num_heap_node);
                        num_heap_node--;
                        break;
                    case 3:
                        if (num_heap_node == 0) {
                            printf("There is no node in the Max-Heap\n");
                            break;
                        }
                        heap_pop(heap_arr, num_heap_node);
                        break;
                    case 4:
                        if (num_heap_node == 0) {
                            printf("There is no node in the Max-Heap\n");
                            break;
                        }
                        heap_push(heap_arr, num_heap_node);
                        break;
                    case 5:
                        if (num_heap_node == 0) {
                            printf("There is no node in the Max-Heap\n");
                            break;
                        }
                        heap_print(heap_arr, num_heap_node);
                        break;
                    default:
                        break;
                }
            }
        } else if (input == 2) {    // BST service mode
            while (bst_command) {
                printf("(1)insert  (2)delete  (3)search  (4)preorder  "
                               "(5)inorder  (6)postorder  (0)main_menu\n");
                bst_command = getIndex(0, 6);
                switch (bst_command) {
                    case 0:
                        continue;
                    case 1:
                        pre_bst_insert(&bst_head);
                        break;
                    case 2:
                        pre_bst_delete(&bst_head);
                        break;
                    case 3:
                        pre_bst_search(bst_head);
                        break;
                    case 4:
                        printf("Preorder Traversal!\n");
                        bst_preorder(bst_head);
                        printf("\n");
                        break;
                    case 5:
                        printf("Inorder Traversal!\n");
                        bst_inorder(bst_head);
                        printf("\n");
                        break;
                    case 6:
                        printf("Postorder Traversal!\n");
                        bst_postorder(bst_head);
                        printf("\n");
                        break;
                    default:
                        break;
                }
            }
        }
    }
}



/*
 * HEAP INSERT
 */
void pre_heap_insert(HEAP_NODE *heap_arr, int num_heap_node) {
    int *numbers = NULL;
    int num_numbers;
    int key = 0;
    LINKED_STACK *top = NULL;

    // get array of numbers checking validity
    do {
        printf("Input your numbers to input to the stack of the created node:");
        numbers = getNumbers(&num_numbers);
        if(!num_numbers)
            printf("Please input decimal numbers\n");
    }while(!num_numbers);

    for (int i = 0; i < num_numbers; ++i) {   // calculate key
        stack_push(&top, numbers[i]);
        key += numbers[i];
    }
    heap_arr[num_heap_node].key = key;        // insert new node to last index
    heap_arr[num_heap_node].top = top;

    heap_insert(heap_arr, num_heap_node);     // make max heap

    printf("The key value of new node is %d and inserted to Max-Heap\n", key);
    free(numbers);
}

void heap_insert(HEAP_NODE *heap_arr, int num_heap_node) {
    int i = num_heap_node;
    while (i > -1) {   // make max heap down to up,
        if (heap_arr[i].key > heap_arr[(i - 1) / 2].key) {
            heap_swap(heap_arr, i, (i - 1) / 2);
            i = (i - 1) / 2;
        } else
            break;
    }
}


/*
 * HEAP DELETE
 */
void heap_delete(HEAP_NODE *heap_arr, int num_heap_node) {
    heap_swap(heap_arr, 0, num_heap_node - 1);    // swap first node with last one
    max_heapify(heap_arr, 0, num_heap_node - 1);  // remake max heap except last node
    node_free(heap_arr, num_heap_node - 1);           // dispose memory of deleted node
    printf("The root node of the Max-Heap is deleted\n");
}

void node_free(HEAP_NODE *heap_arr, int index_heap_node) {
    LINKED_STACK *tmp1 = heap_arr[index_heap_node].top;
    LINKED_STACK *tmp2;

    // free memory along the linked list
    while (tmp1) {
        tmp2 = tmp1->link;
        free(tmp1);
        tmp1 = tmp2;
    }
}


/*
 * HEAP POP
 */
void heap_pop(HEAP_NODE *heap_arr, int num_heap_node) {
    int val;

    if(!heap_arr[0].top){  // if empty stack
        printf("Empty stack!!\n");
        return;
    }

    val = stack_pop(&(heap_arr[0].top));      // pop top element of root node
    heap_arr[0].key -= val;                   // update key
    max_heapify(heap_arr, 0, num_heap_node);  // update max heap
    printf("Popped item is %d, and the Max-Heap is updated\n", val);
}

int stack_pop(LINKED_STACK **top) {   // dispose top of the stack
    LINKED_STACK *tmp = *top;
    int val = tmp->val;
    *top = (*top)->link;
    free(tmp);
    return val;
}


/*
 * HEAP PUSH
 */
void heap_push(HEAP_NODE *heap_arr, int num_heap_node) {
    printf("Input your number to push to the root node of the Max-Heap\n");
    int val = getIndex(INT_MIN, INT_MAX);

    stack_push(&(heap_arr[0].top), val);      // push to top of root node
    heap_arr[0].key += val;                   // update key
    max_heapify(heap_arr, 0, num_heap_node);  // update max heap
    printf("Pushed item is %d, and the Max-Heap is updated\n", val);
}

void stack_push(LINKED_STACK **top, int val) {  // push to top of the stack
    LINKED_STACK *tmp = (LINKED_STACK *) malloc(sizeof(LINKED_STACK));
    assert(tmp);

    tmp->val = val;
    tmp->link = *top;
    *top = tmp;
}


/*
 * MAX HEAPIFY
 */
void max_heapify(HEAP_NODE *heap_arr, int index, int num_heap_node) {
    int i = index;
    int key, left_child_key, right_child_key;
    int bigger_child_index;
    int bigger_key;

    // start in index node, max max heap
    while (2 * i + 1 < num_heap_node) {   // if node has left child
        key = heap_arr[i].key;
        left_child_key = heap_arr[2 * i + 1].key;
        if (2 * i + 1 == num_heap_node - 1)   // if node only has left child
            bigger_child_index = 2 * i + 1;
        else {
            right_child_key = heap_arr[2 * (i + 1)].key;
            // choose bigger child
            bigger_child_index = (left_child_key > right_child_key) ? (2 * i + 1) : (2 * (i + 1));
        }
        bigger_key = heap_arr[bigger_child_index].key;

        // if child is bigger than parent, swap and move down
        if (key < bigger_key) {
            heap_swap(heap_arr, i, bigger_child_index);
            i = bigger_child_index;
        } else
            break;
    }
}


/*
 * OTHER HEAP FUNCTION
 */
void heap_print(HEAP_NODE *heap_arr, int num_heap_node) {
    LINKED_STACK *tmp;

    // print every stack elements of every heap nodes
    for (int i = 0; i < num_heap_node; ++i) {
        printf("%dth key = %d\n", i + 1, heap_arr[i].key);
        tmp = heap_arr[i].top;
        printf("stack values : ");
        while (tmp) {
            printf("%d ", tmp->val);
            tmp = tmp->link;
        }
        printf("\n");
    }
}

void heap_swap(HEAP_NODE *heap_arr, int i, int j) {
    // swap top pointer of each stack
    LINKED_STACK *tmp = heap_arr[i].top;
    heap_arr[i].top = heap_arr[j].top;
    heap_arr[j].top = tmp;

    // swap key of each stack
    heap_arr[i].key ^= heap_arr[j].key;
    heap_arr[j].key ^= heap_arr[i].key;
    heap_arr[i].key ^= heap_arr[j].key;
}
//--------------------------End of heap functions---------------------------------//



/*
 * BST INSERT
 */
void pre_bst_insert(BST_NODE **bst_head) {
    int *numbers = NULL;
    int num_numbers;
    int *inserted_numbers = (int *) malloc(sizeof(int) * MAX_BUFFER_SIZE);
    assert(inserted_numbers);

    int num_inserted_numbers = 0;
    int insert_result;

    do {
        printf("Input your numbers to input to BST: ");
        numbers = getNumbers(&num_numbers);   // array of user input
        if(!num_numbers)
            printf("Please input decimal numbers\n");
    }while(!num_numbers);

    // insert numbers to bst except duplication
    for (int i = 0; i < num_numbers; ++i) {
        insert_result = bst_insert(bst_head, numbers[i]);
        if (insert_result) {              // array of inserted number
            inserted_numbers[num_inserted_numbers++] = numbers[i];
        }
    }

    // print inserted number
    for (int j = 0; j < num_inserted_numbers; ++j) {
        printf("%d ", inserted_numbers[j]);
    }

    if (!num_inserted_numbers)
        printf("Fail to insert because of duplication\n");
    else
        printf("inserted into the BST\n");

    free(inserted_numbers);
    free(numbers);
}

// [RETURN] success : 1 , fail : 0
int bst_insert(BST_NODE **bst_head, int val) {
    BST_NODE *new_node = (BST_NODE *) malloc(sizeof(BST_NODE));
    assert(new_node);

    // make new node
    new_node->val = val;
    new_node->left_link = NULL;
    new_node->right_link = NULL;

    BST_NODE *tmp = bst_search(*bst_head, val);

    // if inserted first
    if (!tmp) {
        *bst_head = new_node;
        return 1;
    }

    if (tmp->val == val) // if already exist -> fail insert
        return 0;
    else {
        if (tmp->val < val)
            tmp->right_link = new_node;
        else
            tmp->left_link = new_node;
    }
    return 1;
}


/*
 * BST DELETE
 */
void pre_bst_delete(BST_NODE **bst_head) {
    printf("Input your number to delete from the BST\n");
    int delete_number = getIndex(INT_MIN, INT_MAX);

    int result = bst_delete(bst_head, delete_number);
    if (!result)
        printf("%d is does not exist in the BST\n", delete_number);
    else
        printf("%d is deleted from the BST\n", delete_number);
}

// [RETURN] success : 1, fail : 0
int bst_delete(BST_NODE **bst_head, int val) {
    BST_NODE *tmp = bst_search(*bst_head, val);  // for delete node
    BST_NODE *parent = NULL;                   // for parent node of delete node

    if (!tmp || tmp->val != val)               // if not exists
        return 0;

    if (!(tmp->left_link) && !(tmp->right_link)) { // has 0 child
        if (tmp == *bst_head)   // head node
            *bst_head = NULL;
        else {
            parent = bst_search_parent(*bst_head, val);
            // the node is right or left child of parent
            if (parent->right_link && parent->right_link->val == val)
                parent->right_link = NULL;
            else
                parent->left_link = NULL;
        }
    } else if (tmp->left_link && !(tmp->right_link)) { // only has left child
        if (tmp == *bst_head)   // head node
            *bst_head = tmp->left_link;
        else {
            parent = bst_search_parent(*bst_head, val);
            if (parent->right_link && parent->right_link->val == val)
                parent->right_link = tmp->left_link;
            else
                parent->left_link = tmp->left_link;
        }
    } else if (!(tmp->left_link) && tmp->right_link) { // only has right child
        if (tmp == *bst_head)   // head node
            *bst_head = tmp->right_link;
        else {
            parent = bst_search_parent(*bst_head, val);
            if (parent->right_link && parent->right_link->val == val)
                parent->right_link = tmp->right_link;
            else
                parent->left_link = tmp->right_link;
        }
    } else {                                           // has 2 child
        BST_NODE *left_biggest = tmp;
        left_biggest = tmp->left_link;                 // find left_biggest node
        while (left_biggest->right_link) {
            left_biggest = left_biggest->right_link;
        }

        BST_NODE *parent_left_biggest =
                bst_search_parent(*bst_head, left_biggest->val);

        if (tmp == *bst_head)   // head node
            *bst_head = left_biggest;
        else
            parent = bst_search_parent(*bst_head, val);

        // take up left_biggest
        if (!(left_biggest->left_link)) {    // left_biggest has 0 child
            left_biggest->right_link = tmp->right_link;
            // left_biggest is not left node of the delete node
            if (left_biggest != tmp->left_link) {
                left_biggest->left_link = tmp->left_link;
                parent_left_biggest->right_link = NULL;
            }
            if (parent->right_link && parent->right_link->val == val)
                parent->right_link = left_biggest;
            else
                parent->left_link = left_biggest;

        } else {                            // left_biggest has left child
            left_biggest->right_link = tmp->right_link;
            if (left_biggest != tmp->left_link) {
                parent_left_biggest->right_link = left_biggest->left_link;
                left_biggest->left_link = tmp->left_link;
            }
            if (parent->right_link && parent->right_link->val == val)
                parent->right_link = left_biggest;
            else
                parent->left_link = left_biggest;
        }
    }

    free(tmp);
    return 1;
}


/*
 * BST SEARCH
 */
void pre_bst_search(BST_NODE *bst_head) {
    printf("Input your number to search: ");
    int val = getIndex(INT_MIN, INT_MAX);

    BST_NODE *result = bst_search(bst_head, val);
    if (!result || result->val != val)
        printf("%d does not exist in the BST\n", val);
    else
        printf("%d exists in the BST\n", val);
}

//           exist : that node
// [RETURN]  not exist : leaf node
//           initial state : null
BST_NODE *bst_search(BST_NODE *bst_head, int val) {
    BST_NODE *tmp = bst_head;
    BST_NODE *parent = NULL;

    while (tmp) {
        if (tmp->val == val)
            return tmp;  // val exist
        parent = tmp;
        if (val < tmp->val)
            tmp = tmp->left_link;
        else
            tmp = tmp->right_link;
    }
    return parent;      // val does not exist
}

// [WARNING] val must exist
BST_NODE *bst_search_parent(BST_NODE *bst_head, int val) {
    BST_NODE *tmp = bst_head;

    // find parent node of val node
    while (tmp) {
        if (tmp->left_link && tmp->left_link->val == val)
            return tmp;
        if (tmp->right_link && tmp->right_link->val == val)
            return tmp;

        if (tmp->val < val)
            tmp = tmp->right_link;
        else
            tmp = tmp->left_link;
    }
    return tmp;
}


/*
 * BST TRAVERSAL
 */
void bst_preorder(BST_NODE *bst_head) {
    if (!bst_head)
        return;

    // root -> left -> right
    printf("%d ", bst_head->val);
    bst_preorder(bst_head->left_link);
    bst_preorder(bst_head->right_link);

}

void bst_inorder(BST_NODE *bst_head) {
    if (!bst_head)
        return;

    // left -> root -> right
    bst_inorder(bst_head->left_link);
    printf("%d ", bst_head->val);
    bst_inorder(bst_head->right_link);
}

void bst_postorder(BST_NODE *bst_head) {
    if (!bst_head)
        return;

    // left -> right -> root
    bst_postorder(bst_head->left_link);
    bst_postorder(bst_head->right_link);
    printf("%d ", bst_head->val);
}
//---------------------------End of BST functions---------------------------------//



/*
 * UI FUNCTION
 */
int *getNumbers(int *num) {
    char *buff = (char *) malloc(sizeof(char) * MAX_BUFFER_SIZE);
    int *arr = (int *) malloc(sizeof(int) * MAX_BUFFER_SIZE);
    assert(buff && arr);

    int count_letters = 0;
    int index = 0;

    // get string and make array of numbers
    fgets(buff, MAX_BUFFER_SIZE, stdin);
    while (sscanf(buff, "%d%n", &arr[index], &count_letters) == 1) {
        buff = buff + count_letters;
        index++;
    }
    *num = index;

    return arr;

}

int getIndex(int minimum, int maximum) {
    const int BUFSIZE = 256;  // line buffer size
    char *buf, *tmpstr;       // line buffer and garbage storage for user input
    int num = 0;             // integer returned by sscanf()
    int index;               // user input returned by this function

    // Prepare line buffer
    buf = (char *) malloc(BUFSIZE * sizeof(char));
    tmpstr = (char *) malloc(BUFSIZE * sizeof(char));
    assert(buf && tmpstr);

    // Get index
    while (!num) {
        printf("Input your number: ");
        fgets(buf, BUFSIZE, stdin);
        num = sscanf(buf, "%d%s", &index, tmpstr);
        if (num != 1) {
            num = 0;
            printf("[ERROR] index should be a non-negative integer.\n");
        } else if (index > maximum || index < minimum) {
            num = 0;
            printf("[ERROR] index should be integer from %d to %d.\n", minimum, maximum);
        }
    }
    free(buf);
    free(tmpstr);
    return index;
}


/*
 * DISPOSE DYNAMIC MEMORY
 */
void dispose_heapMem(HEAP_NODE *heap_arr, int num_heap_node) {
    for (int i = 0; i < num_heap_node; ++i) {
        node_free(heap_arr, i);
    }
}

void dispose_bstMem(BST_NODE *node) {
    if (!node)
        return;

    // use postOrder traversal
    dispose_bstMem(node->left_link);
    dispose_bstMem(node->right_link);
    free(node);
}