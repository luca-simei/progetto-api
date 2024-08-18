
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

typedef struct node {
    bool is_black;
    int distanza;
    int veicoli;
    int autonomie[512];
    int max_autonomia;
    struct node* prev;
    struct node* left;
    struct node* right;
} node_t;



typedef struct sstack {
    int distanza;
    int max_autonomia;
    struct sstack* prev;
    struct sstack* next;
    struct sstack* respective;
} sstack_t;



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

node_t* root = NULL;
node_t* NIL = NULL;
sstack_t* stack = NULL;
sstack_t* end_stack = NULL;



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------
//----- push ----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void push(int distanza) {

    sstack_t* top = NULL;

    if (stack == NULL) {
        stack = (sstack_t*)malloc(sizeof(sstack_t));
        stack->distanza = distanza;
        stack->prev = NULL;
        stack->next = NULL;
        stack->respective = NULL;
        return;

    } else {
        top = (sstack_t*)malloc(sizeof(sstack_t));
        top->distanza = distanza;
        top->prev = NULL;
        top->next = stack;
        stack->prev = top;
        stack = top;
        stack->respective = NULL;
        return;
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- free_stack ----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void free_stack() {

    sstack_t* temp = NULL;

    if (stack == NULL) {
        return;

    } else {
        while (stack != NULL) {
            temp = stack;
            stack = temp->next;
            temp->prev = NULL;
            temp->next = NULL;
            temp->respective = NULL;
            free(temp);
        }
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
//----- direct_solution -----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void direct_solution(sstack_t* start, sstack_t* goal) {

    int f = 0;
    sstack_t* curr = goal;
    sstack_t* checkpoint = goal;
    sstack_t* new_checkpoint = NULL;

    while (checkpoint->distanza != start->distanza) {
        f = 0;
        for (curr = checkpoint->prev; curr->distanza >= start->distanza; curr = curr->prev) {
            if (curr->distanza == start->distanza && stack == NULL) {
                return;
            }
            if (curr->distanza + curr->max_autonomia >= checkpoint->distanza) {
                if (f == 0) {
                    push(curr->distanza);
                    new_checkpoint = curr;
                    f = 1;
                } else if (f == 1) {
                    stack->distanza = curr->distanza;
                    new_checkpoint = curr;
                }
                if (curr->distanza == start->distanza) {
                    return;
                }
            }
            if (curr->prev == NULL) {
                break;
            }
        }
        if (f == 0) {
            return;
        }
        checkpoint = new_checkpoint;
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- first_solution ------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void first_solution(sstack_t* start, sstack_t* goal) {

    int f = 0;
    sstack_t* curr = goal;
    sstack_t* checkpoint = goal;
    sstack_t* new_checkpoint = NULL;

    push(goal->distanza);
    while (checkpoint->distanza != start->distanza) {
        f = 0;
        for (curr = checkpoint->next; curr->distanza <= start->distanza; curr = curr->next) {
            if (curr->distanza == start->distanza && stack == NULL) {
                return;
            }
            if (curr->distanza - curr->max_autonomia <= checkpoint->distanza) {
                if (f == 0) {
                    push(curr->distanza);
                    new_checkpoint = curr;
                    f = 1;
                } else if (f == 1) {
                    stack->distanza = curr->distanza;
                    new_checkpoint = curr;
                }
                if (curr->distanza == start->distanza) {
                    return;
                }
            }
            if (curr->next == NULL) {
                break;
            }
        }
        if (f == 0) {
            return;
        }
        checkpoint = new_checkpoint;
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- correzione_1 --------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void correzione_1(sstack_t* start, sstack_t* goal) {

    int new_value = -1;
    sstack_t* station = NULL;
    sstack_t* next_station = NULL;
    sstack_t* curr = end_stack;

    if (curr->prev == NULL) {
        return;
    }

    while (curr->prev->prev != NULL) {

        new_value = -1;

        station = curr->prev->respective;
        next_station = curr->prev->prev->respective;

        while (station->distanza != curr->distanza) {
            station = station->prev;
            if (station->distanza == curr->distanza) {
                break;
            } else if (station->distanza - station->max_autonomia <= curr->distanza) {
                if (next_station->distanza - next_station->max_autonomia <= station->distanza) {
                    new_value = station->distanza;
                    curr->prev->respective = station;
                }
            }
        }
        if (new_value != -1) {
            curr->prev->distanza = new_value;
        }

        curr = curr->prev;
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- correzione_2 --------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void correzione_2(sstack_t* start, sstack_t* goal) {

    int new_value = -1;
    int next_new_value = -1;
    sstack_t* station = goal;
    sstack_t* next_station = NULL;
    sstack_t* checkpoint = NULL;
    sstack_t* curr = end_stack;

    if (curr->prev == NULL) {
        return;
    }

    while (curr->prev->prev != NULL) {

        new_value = -1;
        next_new_value = -1;

        station = curr->prev->respective;
        next_station = curr->prev->prev->respective;
        checkpoint = next_station;

        while (station->distanza != curr->distanza) {
            station = station->prev;
            if (station->distanza == curr->distanza) {
                break;
            } else if (station->distanza - station->max_autonomia <= curr->distanza) {
                if (curr->prev->prev->prev == NULL) {
                    break;
                } else {
                    while (next_station->distanza != curr->prev->prev->prev->distanza) {
                        next_station = next_station->next;
                        if (next_station->distanza - next_station->max_autonomia <= station->distanza) {
                            new_value = station->distanza;
                            curr->prev->respective = station;
                            next_new_value = next_station->distanza;
                            curr->prev->prev->respective = next_station;
                        }
                    }
                    next_station = checkpoint;
                }
            }
        }
        if (new_value != -1) {
            curr->prev->distanza = new_value;
        }
        if (next_new_value != -1) {
            curr->prev->prev->distanza = next_new_value;
        }

        curr = curr->prev;
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- correzione_3 --------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void correzione_3(sstack_t* start, sstack_t* goal) {

    int new_value1 = -1;
    int new_value2 = -1;
    sstack_t* s1 = start;
    sstack_t* s2 = NULL;
    sstack_t* second_station = start;
    sstack_t* curr = stack;

    if (curr->next == NULL) {
        return;
    }

    if (curr->next->next == NULL) {
        return;
    }

    if (curr->next->next->next == NULL) {
        return;
    }

    while (curr->distanza != stack->next->next->distanza) {
        curr = curr->next;
    }
    second_station = curr->prev->respective;
    s2 = second_station;
    s1 = curr->respective;

    while (s1->distanza != curr->next->distanza) {
        new_value1 = -1;
        new_value2 = -1;
        s1 = s1->prev;
        if (s1->distanza == curr->next->distanza) {
            break;
        } else {
            while (s2->distanza != curr->distanza) {
                if (s2->distanza - s2->max_autonomia <= s1->distanza) {
                    if (start->distanza - start->max_autonomia <= s2->distanza) {
                        new_value1 = s1->distanza;
                        curr->respective = s1;
                        new_value2 = s2->distanza;
                        curr->prev->respective = s2;
                    }
                }
                s2 = s2->prev;
                if (s2->distanza == curr->distanza) {
                    break;
                }
            }
        }
        s2 = second_station;

        if (new_value1 != -1) {
            curr->distanza = new_value1;

        }
        if (new_value2 != -1) {
            curr->prev->distanza = new_value2;
        }
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- correzione_finale ---------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void correzione_finale(sstack_t* start, sstack_t* goal) {

    int f = 0;
    sstack_t* station = start;
    sstack_t* checkpoint = NULL;
    sstack_t* next_station = start;
    sstack_t* curr = stack;

    if (curr->next == NULL) {
        return;
    }

    if (curr->next->next == NULL) {
        return;
    }

    while (curr->distanza != goal->distanza) {

        f = 0;
        station = start;
        next_station = start;

        curr = curr->next;

        if (curr->distanza == goal->distanza) {
            return;
        }

        station = curr->respective;
        next_station = curr->next->respective;

        if (station->distanza - station->max_autonomia > next_station->distanza) {
            checkpoint = station;

            while (station->distanza - station->max_autonomia > next_station->distanza) {
                station = station->next;
                if (station->distanza == curr->prev->distanza) {
                    break;
                }
                if (station->distanza - station->max_autonomia <= next_station->distanza) {
                    if (f == 0) {
                        curr->distanza = station->distanza;
                        f = 1;
                    }
                }
            }
            
            if (f == 0) {
                station = checkpoint;
                while (station->distanza - station->max_autonomia > next_station->distanza) {
                    next_station = next_station->next;
                    if (station->distanza - station->max_autonomia <= next_station->distanza) {
                        if (next_station->distanza - next_station->max_autonomia <= curr->next->next->distanza) {
                            if (f == 0) {
                                curr->next->distanza = next_station->distanza;
                                f = 1;
                            }
                        }
                    }
                }
            }
        }
        if (curr->next->next == NULL) {
            return;
        }
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- reverse_solution ----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void reverse_solution(sstack_t* start, sstack_t* goal) {

    int new_value = -1;
    sstack_t* station = start;
    sstack_t* curr = stack;

    if (curr->next == NULL) {
        return;
    }

    if (curr->next->next == NULL) {
        return;
    }

    while (station->distanza != curr->next->distanza) {
        station = station->prev;
    }

    while (station->distanza != curr->next->next->distanza) {
        station = station->prev;
        if (station->distanza != curr->next->next->distanza) {
            if (start->distanza - start->max_autonomia <= station->distanza) {
                if (station->distanza - station->max_autonomia <= curr->next->next->distanza) {
                    new_value = station->distanza;
                }
            }
        }
    }
    if (new_value != -1) {
        curr->next->distanza = new_value;
    }

    return;

}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

node_t* tree_search(node_t* x, int distanza) {

    if (x == NIL || distanza == x->distanza) {
        return x;
    }
    if (distanza < x->distanza) {
        return tree_search(x->left, distanza);
    } else {
        return tree_search(x->right, distanza);
    }

}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

node_t* tree_minimum(node_t* x) {

    while (x->left != NIL) {
        x = x->left;
    }

    return x;

}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

node_t* tree_successor(node_t* x) {

    node_t* y = NULL;

    if (x->right != NIL) {
        return tree_minimum(x->right);
    }
    y = x->prev;
    while (y != NIL && x == y->right) {
        x = y;
        y = y->prev;
    }

    return y;

}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void left_rotate(node_t* x) {

    node_t* y = x->right;

    x->right = y->left;

    if (y->left != NIL) {
        y->left->prev = x;
    }
    y->prev = x->prev;
    if (x->prev == NIL) {
        root = y;
    } else if (x == x->prev->left) {
        x->prev->left = y;
    } else {
        x->prev->right = y;
    }
    y->left = x;
    x->prev = y;

    return;
}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void right_rotate(node_t* x) {

    node_t* y = x->left;

    x->left = y->right;

    if (y->right != NIL) {
        y->right->prev = x;
    }
    y->prev = x->prev;
    if (x->prev == NIL) {
        root = y;
    } else if (x == x->prev->right) {
        x->prev->right = y;
    } else {
        x->prev->left = y;
    }
    y->right = x;
    x->prev = y;

    return;
}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void rb_insert_fixup(node_t* z) {

    node_t* y = NULL;

    while (z != root && z->prev->is_black == false) {
        if (z->prev == z->prev->prev->left) {

            y = z->prev->prev->right;

            if (y->is_black == false) {
                z->prev->is_black = true;
                y->is_black = true;
                z->prev->prev->is_black = false;
                z = z->prev->prev;

            } else {
                if(z == z->prev->right) {
                    z = z->prev;
                    left_rotate(z);
                }
                z->prev->is_black = true;
                z->prev->prev->is_black = false;
                right_rotate(z->prev->prev);
            }

        } else {

            y = z->prev->prev->left;

            if (y->is_black == false) {
                z->prev->is_black = true;
                y->is_black = true;
                z->prev->prev->is_black = false;
                z = z->prev->prev;

            } else {
                if(z == z->prev->left) {
                    z = z->prev;
                    right_rotate(z);
                }
                z->prev->is_black = true;
                z->prev->prev->is_black = false;
                left_rotate(z->prev->prev);
            }

        }
    }
        
    root->is_black = true;

    return;
        
}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void rb_delete_fixup(node_t* x) {

    node_t* w = NULL;

    while (x != root && x->is_black == true) {

        if (x == x->prev->left) {

            w = x->prev->right;

            if (w->is_black == false) {
                w->is_black = true;
                x->prev->is_black = false;
                left_rotate(x->prev);
                w = x->prev->right;
            }
            if (w->left->is_black == true && w->right->is_black == true) {
                w->is_black = false;
                x = x->prev;
            } else {
                if (w->right->is_black == true) {
                    w->left->is_black = true;
                    w->is_black = false;
                    right_rotate(w);
                    w = x->prev->right;
                }
                w->is_black = x->prev->is_black;
                x->prev->is_black = true;
                w->right->is_black = true;
                left_rotate(x->prev);
                x = root;
            }

        } else {

            w = x->prev->left;

            if (w->is_black == false) {
                w->is_black = true;
                x->prev->is_black = false;
                right_rotate(x->prev);
                w = x->prev->left;
            }
            if (w->right->is_black == true && w->left->is_black == true) {
                w->is_black = false;
                x = x->prev;
            } else {
                if (w->left->is_black == true) {
                    w->right->is_black = true;
                    w->is_black = false;
                    left_rotate(w);
                    w = x->prev->left;
                }
                w->is_black = x->prev->is_black;
                x->prev->is_black = true;
                w->left->is_black = true;
                right_rotate(x->prev);
                x = root;
            }

        }

    }

    x->is_black = true;

    return;         
}



//---------------------------------------------------------------------------------------------------
//----- PIANIFICA_PERCORSO --------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void pianifica_percorso(int partenza, int arrivo) {

    int f = 0;
    sstack_t* head = NULL;
    sstack_t* prev = NULL;
    sstack_t* node = NULL;
    sstack_t* temp = NULL;
    sstack_t* start = NULL;
    sstack_t* goal = NULL;
    sstack_t* curr = NULL;
    node_t* x = NULL;
    node_t* y = NULL;
    node_t* z = NULL;

    // --- costruisci_autostrada --------------------------------------------------------------------

    if (partenza < arrivo) {
        x = tree_search(root, partenza);
        z = tree_search(root, arrivo);
    } else {
        x = tree_search(root, arrivo);
        z = tree_search(root, partenza);
    }

    head = (sstack_t*)malloc(sizeof(sstack_t));
    head->distanza = x->distanza;
    head->max_autonomia = x->max_autonomia;
    head->next = NULL;
    head->prev = NULL;
    head->respective = NULL;
    node = head;

    y = tree_successor(x);

    while (y != z) {
        node->next = (sstack_t*)malloc(sizeof(sstack_t));
        prev = node;
        node = node->next;
        node->distanza = y->distanza;
        node->max_autonomia = y->max_autonomia;
        node->next = NULL;
        node->prev = prev;
        node->respective = NULL;
        y = tree_successor(y);
    }

    node->next = (sstack_t*)malloc(sizeof(sstack_t));
    prev = node;
    node = node->next;
    node->distanza = z->distanza;
    node->max_autonomia = z->max_autonomia;
    node->next = NULL;
    node->prev = prev;
    node->respective = NULL;

    // --- pianifica_percorso -----------------------------------------------------------------------

    if (partenza < arrivo) {
        for (start = head; start != NULL; start = start->next) {
            if (start->distanza == partenza) {
                for (goal = start->next; goal != NULL; goal = goal->next) {
                    if (goal->distanza == arrivo) {
                        f = 1;
                        break;
                    }
                }   
            }
            if (f == 1) {
                break;
            }
        }
        direct_solution(start, goal); 
        
    } else {
        for (goal = head; goal != NULL; goal = goal->next) {
            if (goal->distanza == arrivo) {
                for (start = goal->next; start != NULL; start = start->next) {
                    if (start->distanza == partenza) {
                        f = 1;
                        break;
                    }
                }   
            }
            if (f == 1) {
                break;
            }
        }

        first_solution(start, goal);
        temp = start;
        curr = stack;
        while (temp->distanza >= goal->distanza) {
            if (temp->distanza == curr->distanza) {
                curr->respective = temp;
                if (curr->next != NULL) {
                    curr = curr->next;
                }
            }
            temp = temp->prev;
            if (temp == NULL) {
                break;
            }
        }
        end_stack = curr;

        for (int i = 0; i < 12; i++) {
            correzione_1(start, goal);
            correzione_2(start, goal);
            correzione_3(start, goal);
        }
        correzione_finale(start, goal);
        reverse_solution(start, goal);
    }

    if (stack == NULL || stack->distanza != partenza) {
        printf("nessun percorso\n");
    } else {
        for (curr = stack; curr != NULL; curr = curr->next) {
            printf("%d", curr->distanza);
            if (curr->next != NULL) {
                printf(" ");
            }
        }
        if (partenza < arrivo) {
            printf(" %d\n", arrivo);
        } else {
            printf("\n");
        }
    }

    free_stack();

    return;
}



//---------------------------------------------------------------------------------------------------
//----- AGGIUNGI_STAZIONE ---------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void aggiungi_stazione(int distanza, int numero_auto, int* autonomie) {

    int i = 0;
    node_t* y = NIL;
    node_t* x = root;
    node_t* z = NULL;

    // --- rb_create_node ---------------------------------------------------------------------------

    z = (node_t*)malloc(sizeof(node_t));

    z->max_autonomia = 0;
    z->distanza = distanza;
    z->veicoli = numero_auto;

    for (i = 0; i < numero_auto; i++) {
        z->autonomie[i] = autonomie[i];
        if (autonomie[i] > z->max_autonomia) {
            z->max_autonomia = autonomie[i];
        }
    }
    for (i = numero_auto; i < 512; i++) {
        z->autonomie[i] = -1;
    }

    z->prev = NULL;
    z->left = NIL;
    z->right = NIL;
    z->is_black = false;

    // --- rb_insert -------------------------------------------------------------------------------

    while(x != NIL) {
        y = x;
        if (z->distanza < x->distanza) {
            x = x->left;
        } else if (z->distanza == x->distanza) {
            free(z);
            printf("non aggiunta\n");
            return;
        } else {
            x = x->right;
        }
    }
    z->prev = y;

    if (y == NIL) { 
        root = z;
    } else if (z->distanza < y->distanza) {
        y->left = z;
    } else {
        y->right = z;
    }

    rb_insert_fixup(z);

    printf("aggiunta\n");

    return;

}



//---------------------------------------------------------------------------------------------------
//----- DEMOLISCI_STAZIONE --------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void demolisci_stazione(int distanza) {

    int i = 0;
    node_t* x = NULL;
    node_t* y = NULL;
    node_t* z = NULL;

    z = tree_search(root, distanza);
    if (z == NIL) {
        printf("non demolita\n");
        return;
    }

    // --- rb_delete --------------------------------------------------------------------------------

    if (z->left == NIL || z->right == NIL) {
        y = z;
    } else {
        y = tree_successor(z);
    }

    if (y->left != NIL) {
        x = y->left;
    } else {
        x = y->right;
    }

    x->prev = y->prev;
    if (y->prev == NIL) {
        root = x;
    } else if (y == y->prev->left) {
        y->prev->left = x;
    } else {
        y->prev->right = x;
    }

    if (y != z) {
        z->distanza = y->distanza;
        z->max_autonomia = y->max_autonomia;
        z->veicoli = y->veicoli;
        for (i = 0; i < 512; i++) {
            z->autonomie[i] = y->autonomie[i];
        }
    }
    if (y->is_black == true) {
        rb_delete_fixup(x);
    }

    printf("demolita\n");
    free(y);

    return;

}



//---------------------------------------------------------------------------------------------------
//----- AGGIUNGI_AUTO -------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void aggiungi_auto(int distanza, int autonomia) {

    int i = 0;
    node_t* node = NULL;

    node = tree_search(root, distanza);
    if (node == NIL) {
        printf("non aggiunta\n");
        return;
    } else {
        if (node->veicoli == 512) {
            printf("non aggiunta\n");
            return;
        } else {
            if (autonomia > node->max_autonomia) {
                node->max_autonomia = autonomia;
            }
            if (node->autonomie[node->veicoli] == -1) {
                node->autonomie[node->veicoli] = autonomia;
                node->veicoli = node->veicoli + 1;
                printf("aggiunta\n");
                return;
            }
            for (i = 0; i < 512; i++) {
                if (node->autonomie[i] == -1) {
                    node->autonomie[i] = autonomia;
                    node->veicoli = node->veicoli + 1;
                    printf("aggiunta\n");
                    return;
                }
            }
        }
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//----- ROTTAMA_AUTO --------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void rottama_auto(int distanza, int autonomia) {

    int i = 0;
    node_t* node = NULL;

    node = tree_search(root, distanza);

    if (node == NIL) {
        printf("non rottamata\n");
        return;
    } else {
        for (i = 0; i < 512; i++) {
            if (node->autonomie[i] == autonomia) {
                node->autonomie[i] = -1;
                node->veicoli = node->veicoli - 1;
                if (autonomia == node->max_autonomia) {
                    node->max_autonomia = -1;
                    for (i = 0; i < 512; i++) {
                        if (node->autonomie[i] > node->max_autonomia) {
                            node->max_autonomia = node->autonomie[i];
                        }
                    }
                }
                printf("rottamata\n");
                return;
            }
        }
        printf("non rottamata\n");
    }

    return;
}



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
//----- main ----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

int main() {

    int i = 0;
    int arg1 = 0;
    int arg2 = 0;
    int args[512];
    char line[8192];
    char* token;

    FILE* fp = stdin; //fopen("archivio_test_aperti/open_110.txt", "r");

    // --- rb_create_tree ---------------------------------------------------------------------------

    NIL = (node_t*)malloc(sizeof(node_t));
    NIL->distanza = -1;
    NIL->veicoli = 0;
    NIL->max_autonomia = -1;
    NIL->is_black = true;
    NIL->prev = NULL;
    NIL->left = NULL;
    NIL->right = NULL;
    for (int i = 0; i < 512; i++) {
        NIL->autonomie[i] = -1;
    }
    i = 0;

    root = NIL;

    // --- fgets ------------------------------------------------------------------------------------

    while (fgets(line, sizeof(line), fp)) {
        token = strtok(line, " ");

        //Call the function "aggiungi_stazione".
        if (strcmp(token, "aggiungi-stazione") == 0) {
            token = strtok(NULL, " ");
            while (token != NULL) {
                if (i == 0) {
                    arg1 = atoi(token);
                } else if (i == 1) {
                    arg2 = atoi(token);
                } else {
                    args[i - 2] = atoi(token);
                }
                i++;
                token = strtok(NULL, " ");
            }
            aggiungi_stazione(arg1, arg2, args);
            i = 0;
        }

        //Call the function "demolisci_stazione".
        else if (strcmp(line, "demolisci-stazione") == 0) {
            token = strtok(NULL, " ");
            arg1 = atoi(token);
            demolisci_stazione(arg1);
        }

        //Call the function "aggiungi_auto".
        else if (strcmp(line, "aggiungi-auto") == 0) {
            token = strtok(NULL, " ");
            arg1 = atoi(token);
            token = strtok(NULL, " ");
            arg2 = atoi(token);
            aggiungi_auto(arg1, arg2);
        }

        //Call the function "rottama_auto".
        else if (strcmp(line, "rottama-auto") == 0) {
            token = strtok(NULL, " ");
            arg1 = atoi(token);
            token = strtok(NULL, " ");
            arg2 = atoi(token);
            rottama_auto(arg1, arg2);
        }

        //Call the function "pianifica_percorso".
        else if (strcmp(line, "pianifica-percorso") == 0) {
            token = strtok(NULL, " ");
            arg1 = atoi(token);
            token = strtok(NULL, " ");
            arg2 = atoi(token);
            pianifica_percorso(arg1, arg2);
        }
    }

    return 0;
}
