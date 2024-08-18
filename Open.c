#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//---------------------------------------------------------------------------------------------------
//----- STRUCTURES ----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

typedef struct stazione {
    int distanza;
    int veicoli;
    int autonomie[512];
    struct stazione* prev;
    struct stazione* next;
} stazione_t;


typedef struct sstack {
    int dimensioni;
    int distanza;
    struct sstack* prev;
    struct sstack* next;
} sstack_t;



//---------------------------------------------------------------------------------------------------
//----- GLOBAL VARIABLES ----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

stazione_t* head = NULL;
sstack_t* stack = NULL;



//---------------------------------------------------------------------------------------------------
//----- FUNCTIONS -----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void push(int);
void direct_solution(stazione_t*, stazione_t*);
void first_solution(stazione_t*, stazione_t*);
void reverse_solution(stazione_t*, stazione_t*);
void correzione(stazione_t*, stazione_t*);
void patch(stazione_t*, stazione_t*);
void controllo_finale(stazione_t*, stazione_t*);
void pianifica_percorso(int, int);
void aggiungi_stazione(int, int, int*);
void aggiungi_auto(int, int);
void demolisci_stazione(int);
void rottama_auto(int, int);



//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
//----- push ----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void push(int distanza) {

    sstack_t* top = NULL;

    if (stack == NULL) {
        stack = (sstack_t*)malloc(sizeof(sstack_t));
        stack->dimensioni = 1;
        stack->distanza = distanza;
        stack->prev = NULL;
        stack->next = NULL;
        return;

    } else {
        top = (sstack_t*)malloc(sizeof(sstack_t));
        top->dimensioni = stack->dimensioni + 1;
        top->distanza = distanza;
        top->prev = NULL;
        top->next = stack;
        stack->prev = top;
        stack = top;
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

void direct_solution(stazione_t* start, stazione_t* goal) {

    int f = 0;
    stazione_t* curr = goal;
    stazione_t* checkpoint = goal;
    stazione_t* new_checkpoint = NULL;

    while (checkpoint->distanza != start->distanza) {
        f = 0;
        for (curr = checkpoint->prev; curr->distanza >= start->distanza; curr = curr->prev) {
            if (curr->distanza == start->distanza && stack == NULL) {
                return;
            }
            if (curr->distanza + curr->autonomie[0] >= checkpoint->distanza) {
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

void first_solution(stazione_t* start, stazione_t* goal) {

    int f = 0;
    stazione_t* curr = goal;
    stazione_t* checkpoint = goal;
    stazione_t* new_checkpoint = NULL;

    push(goal->distanza);
    while (checkpoint->distanza != start->distanza) {
        f = 0;
        for (curr = checkpoint->next; curr->distanza <= start->distanza; curr = curr->next) {
            if (curr->distanza == start->distanza && stack == NULL) {
                return;
            }
            if (curr->distanza - curr->autonomie[0] <= checkpoint->distanza) {
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
//----- reverse_solution ----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void reverse_solution(stazione_t* start, stazione_t* goal) {

    int new_value = -1;
    stazione_t* station = goal;
    stazione_t* next_station = NULL;
    sstack_t* curr = stack;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    if (curr->prev == NULL) {
        return;
    }

    while (curr->prev->prev != NULL) {

        new_value = -1;

        while (station->distanza != curr->prev->distanza) {
            station = station->next;
        }
        next_station = station;
        while (next_station->distanza != curr->prev->prev->distanza) {
            next_station = next_station->next;
        }

        while (station->distanza != curr->distanza) {
            station = station->prev;
            if (station->distanza == curr->distanza) {
                break;
            } else if (station->distanza - station->autonomie[0] <= curr->distanza) {
                if (next_station->distanza - next_station->autonomie[0] <= station->distanza) {
                    new_value = station->distanza;
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
//----- final_solution ------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void final_solution(stazione_t* start, stazione_t* goal) {

    int new_value = -1;
    int next_new_value = -1;
    stazione_t* station = goal;
    stazione_t* next_station = NULL;
    stazione_t* checkpoint = NULL;
    sstack_t* curr = stack;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    if (curr->prev == NULL) {
        return;
    }

    while (curr->prev->prev != NULL) {

        new_value = -1;
        next_new_value = -1;

        while (station->distanza != curr->prev->distanza) {
            station = station->next;
        }
        next_station = station;
        while (next_station->distanza != curr->prev->prev->distanza) {
            next_station = next_station->next;
        }
        checkpoint = next_station;

        while (station->distanza != curr->distanza) {
            station = station->prev;
            if (station->distanza == curr->distanza) {
                break;
            } else if (station->distanza - station->autonomie[0] <= curr->distanza) {
                if (curr->prev->prev->prev == NULL) {
                    break;
                } else {
                    while (next_station->distanza != curr->prev->prev->prev->distanza) {
                        next_station = next_station->next;
                        if (next_station->distanza - next_station->autonomie[0] <= station->distanza) {
                            new_value = station->distanza;
                            next_new_value = next_station->distanza;
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
//----- correzione ----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void correzione(stazione_t* start, stazione_t* goal) {

    int f = 0;
    stazione_t* station = start;
    stazione_t* checkpoint = NULL;
    stazione_t* next_station = start;
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

        while (station->distanza != curr->distanza) {
            station = station->prev;
        }
        while (next_station->distanza != curr->next->distanza) {
            next_station = next_station->prev;
        }

        if (station->distanza - station->autonomie[0] > next_station->distanza) {
            checkpoint = station;

            while (station->distanza - station->autonomie[0] > next_station->distanza) {
                station = station->next;
                if (station->distanza == curr->prev->distanza) {
                    break;
                }
                if (station->distanza - station->autonomie[0] <= next_station->distanza) {
                    if (f == 0) {
                        curr->distanza = station->distanza;
                        f = 1;
                    }
                }
            }
            
            if (f == 0) {
                station = checkpoint;
                while (station->distanza - station->autonomie[0] > next_station->distanza) {
                    next_station = next_station->next;
                    if (station->distanza - station->autonomie[0] <= next_station->distanza) {
                        if (next_station->distanza - next_station->autonomie[0] <= curr->next->next->distanza) {
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
//----- patch ---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void patch(stazione_t* start, stazione_t* goal) {

    int new_value1 = -1;
    int new_value2 = -1;
    stazione_t* s1 = start;
    stazione_t* s2 = NULL;
    stazione_t* second_station = start;
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
    while (second_station->distanza != curr->prev->distanza) {
        second_station = second_station->prev;
    }
    s2 = second_station;
    while (s1->distanza != curr->distanza) {
        s1 = s1->prev;
    }

    while (s1->distanza != curr->next->distanza) {
        new_value1 = -1;
        new_value2 = -1;
        s1 = s1->prev;
        if (s1->distanza == curr->next->distanza) {
            break;
        } else {
            while (s2->distanza != curr->distanza) {
                if (s2->distanza - s2->autonomie[0] <= s1->distanza) {
                    if (start->distanza - start->autonomie[0] <= s2->distanza) {
                        new_value1 = s1->distanza;
                        new_value2 = s2->distanza;
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
//----- finale --------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void controllo_finale(stazione_t* start, stazione_t* goal) {

    int new_value = -1;
    stazione_t* station = start;
    sstack_t* curr = stack;

    if (curr->next->next == NULL) {
        return;
    }

    while (station->distanza != curr->next->distanza) {
        station = station->prev;
    }

    while (station->distanza != curr->next->next->distanza) {
        station = station->prev;
        if (station->distanza != curr->next->next->distanza) {
            if (start->distanza - start->autonomie[0] <= station->distanza) {
                if (station->distanza - station->autonomie[0] <= curr->next->next->distanza) {
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



//---------------------------------------------------------------------------------------------------
//----- PIANIFICA_PERCORSO --------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void pianifica_percorso(int partenza, int arrivo) {

    int f = 0;
    stazione_t* start = NULL;
    stazione_t* goal = NULL;
    sstack_t* curr = NULL;

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
        for (int i = 0; i < 12; i++) {
            reverse_solution(start, goal);
            final_solution(start, goal);
            patch(start, goal);
        }
        correzione(start, goal);
        controllo_finale(start, goal);
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

    int i = 0, j = 0;
    int k = 0, l = 0;
    int t = 0;
    stazione_t* temp = NULL;
    stazione_t* prev = NULL;
    stazione_t* succ = NULL;

    temp = (stazione_t*)malloc(sizeof(stazione_t));

    //Create the parking of the stations,
    //and sort the cars in descending order.
    for (i = 0; i < numero_auto; i++) {
        temp->autonomie[i] = autonomie[i];
    }
    for (i = 0; i < numero_auto; i++) {
        for (j = i + 1; j < numero_auto; j++) {
            if (temp->autonomie[i] < temp->autonomie[j]) {
                t = temp->autonomie[i];
                temp->autonomie[i] = temp->autonomie[j];
                temp->autonomie[j] = t;
            }
        }
    }

    temp->distanza = distanza;
    temp->veicoli = numero_auto;

    //If the list of stations is empty,
    //add the new station as the head of the list.
    if (head == NULL) {
        head = (stazione_t *) malloc(sizeof(stazione_t));
        temp->next = NULL;
        head = temp;

        printf("aggiunta\n");
        return;
    }

    if(head->distanza > distanza) {
        temp->prev = NULL;
        temp->next = head;
        head->prev = temp;
        head = temp;
        printf("aggiunta\n");
        return;
    }

    //If the list of stations is not empty,
    //look for the place in which to put the station in the highway.
    for (prev = head; prev != NULL; prev = prev->next) {

        if (prev->next == NULL && prev->distanza <= distanza) {
            prev->next = temp;
            temp->prev = prev;
            temp->next = NULL;
            printf("aggiunta\n");
            return;
        }

        k = prev->distanza;
        succ = prev->next;
        l = succ->distanza;
    

        if (k < distanza && l > distanza) {

            prev->next = temp;
            temp->next = succ;

            succ->prev = temp;
            temp->prev = prev;

            printf("aggiunta\n");
            return;
        }

        //If the station is already present in the list,
        //it doesn't do anything.
        else if (k == distanza || l == distanza) {
            free(temp);
            printf("non aggiunta\n");
            return;
        }
    }
    return;
}



//Function that adds a car to the parking
//of a specific station of the list of stations.
void aggiungi_auto(int distanza, int autonomia) {

    int i = 0, j = 0;
    int f = 0;
    int t = 0;
    stazione_t* temp = NULL;

    //Look for the station,
    //and so the parking to which add the new car.
    for (temp = head; temp != NULL; temp = temp->next) {
        if (temp->distanza == distanza) {
            f = 1;
            break;
        }
    }

    //If the  specific station is not present in the list of stations,
    //the function doesn't do anything.
    if (f == 0) {
        printf("non aggiunta\n");
        return;
    }

    //If the parking of the specific station is full,
    //the function doesn't do anything.
    if (temp->veicoli == 512) {
        printf("non aggiunta\n");
        return;
    }

    //Otherwise, if the specific station has been found
    //in the list of stations, 
    //add the car as the last one of its parking.
    temp->veicoli = temp->veicoli + 1;
    temp->autonomie[temp->veicoli - 1] = autonomia;

    //Then, sort the parking with new car added
    //in descending order.
    for (i = 0; i < temp->veicoli; i++) {
        for (j = i + 1; j < temp->veicoli; j++) {
            if (temp->autonomie[i] < temp->autonomie[j]) {
                t = temp->autonomie[i];
                temp->autonomie[i] = temp->autonomie[j];
                temp->autonomie[j] = t;
            }
        }
    }

    printf("aggiunta\n");
    return;
}



//Function that removes a station from the list of stations.
void demolisci_stazione(int distanza) {

    stazione_t* curr = NULL;
    stazione_t* prev = NULL;
    stazione_t* succ = NULL;

    //If the list of stations is empty,
    //then there's no station to remove.
    if (head == NULL) {
        printf("non demolita\n");
        return;
    }

    //If the station we are looking for is
    //the head of the list of station,
    //update the head of the list.
    if(head->distanza == distanza) {
        if (head->next == NULL) {
            free(head);
            head = NULL;
            return;
        }
        head = head->next;
        curr = head->prev;
        head->prev = NULL;
        free(curr);
        printf("demolita\n");
        return;
    }

    //If the list of stations is not empty,
    //look for the first occurrence of the station
    //to remove in the list of stations.
    for (curr = head; curr != NULL; curr = curr->next) {

        //If the station to demolish is present in the list of stations,
        //remove it from the list of station.
        if (curr->distanza == distanza) {

            if (curr->next != NULL) {
                prev = curr->prev;
                succ = curr->next;
                prev->next = succ;
                succ->prev = prev;
                free(curr);
            } else {
                prev = curr->prev;
                prev->next = NULL;
                free(curr);
            }

            printf("demolita\n");
            return;
        }
    }

    //If the station to demolish is not present in the list of station,
    //don't remove any station from the list of stations.
    printf("non demolita\n");
    return;
}



//Function that removes a car from a parking
//of a station in the list of stations.
void rottama_auto(int distanza, int autonomia) {

    int i;
    int f = 0;
    stazione_t* temp = NULL;

    //Look for the station,
    //and so the parking from which to remove the car.
    for (temp = head; temp != NULL; temp = temp->next) {
        if (temp->distanza == distanza) {
            f = 1;
            break;
        }
    }

    //If the  specific station is not present in the list of stations,
    //the function doesn't do anything.
    if (f == 0) {
        printf("non rottamata\n");
        return;
    }

    //Otherwise, if the specific station has been found
    //in the list of stations, 
    //remove the car from its parking,
    //and order the rest of the parking in descending order.
    f = 0;
    for (i = 0; i < temp->veicoli; i++) {
        if ((temp->autonomie[i] == autonomia) && f == 0) {
            f = 1;
        }
        if (f == 1 && i != 511) {
            temp->autonomie[i] = temp->autonomie[i + 1];
        }
    }

    if (f == 1) {
        temp->veicoli = temp->veicoli - 1;
        printf("rottamata\n");
    } else {
        printf("non rottamata\n");
    }

    return;
}



int main() {

    int arg1 = 0;
    int arg2 = 0;
    int args[512];
    char line[50];

    FILE* fp = stdin; //fopen("archivio_test_aperti/open_9.txt", "r");

    while (fscanf(fp, "%s", line) != EOF) {

            //Call the function "aggiungi_stazione".
            if (strcmp(line, "aggiungi-stazione") == 0) {
                if (fscanf(fp, "%d", &arg1) != EOF);
                if (fscanf(fp, "%d", &arg2) != EOF);
                for (int i = 0; i < arg2; i++) {
                    if (fscanf(fp, "%d", &args[i]) != EOF);
                }
                for (int i = arg2; i < 512; i++) {
                    args[i] = 0;
                }
                aggiungi_stazione(arg1, arg2, args);
            }

            //Call the function "demolisci_stazione".
            else if (strcmp(line, "demolisci-stazione") == 0) {
                if (fscanf(fp, "%d", &arg1) != EOF);
                demolisci_stazione(arg1);
            }


            //Call the function "aggiungi_auto".
            else if (strcmp(line, "aggiungi-auto") == 0) {
                if (fscanf(fp, "%d", &arg1) != EOF);
                if (fscanf(fp, "%d", &arg2) != EOF);
                aggiungi_auto(arg1, arg2);
            }


            //Call the function "rottama_auto".
            else if (strcmp(line, "rottama-auto") == 0) {
                if (fscanf(fp, "%d", &arg1) != EOF);
                if (fscanf(fp, "%d", &arg2) != EOF);
                rottama_auto(arg1, arg2);
            }


            //Call the function "pianifica_percorso".
            else if (strcmp(line, "pianifica-percorso") == 0) {
                if (fscanf(fp, "%d", &arg1) != EOF);
                if (fscanf(fp, "%d", &arg2) != EOF);
                pianifica_percorso(arg1, arg2);
            }

    }

    return 0;
}