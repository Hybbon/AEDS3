#include "parser.h"
#include "attack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DIGIT_OFFSET 48

////////////////////////////////////////////////////////////////////////////////
// FSM State reference                                                        //
//                                                                            //
// PHASE 1                                                                    //
// 0: Initial state                                                           //
// 1: read 'p'                                                                //
// 2: read 'po+', can accept more 'o's for extra panzers                      //
// 3: read 'po+i'                                                             //
// 4: read 'po+in'                                                            //
// 5: read 'po+int'                                                           //
//                                                                            //
// PHASE 2: reading '(', more than a single ',' or a value that exceeds 99999 //
//          will reset to state zero.                                         //
// 6: read 'po+int(', reading ',' will reset to zero                          //
// 7: read 'po+int([0-9]+'                                                    //
// 8: read 'po+int([0-9]+,'                                                   //
// 9: read 'po+int([0-9]+,[0-9]+'                                             //
// '10': read 'po+int([0-9]+,[0-9]+)' (resets to zero and saves coordinates)  //
////////////////////////////////////////////////////////////////////////////////

/**
 * Resets the finite state machine to state zero.
 * @param a     Temporary Attack in which the coordinates and the number of pan-
 *              zers is stored, to be reset.
 * @param state Variable which stores the current state.
 */
void resetFSM(Attack *a, char *state){
    a->panzers = 1;
    a->x = 0;
    a->y = 0;
    *state = 0;
}

/**
 * Parses stdin for a ciphered message containing the attacks to be performed by
 * the enemy.
 * @param  addr Address of the file where the Attacks will be written.
 * @return      Size of the file.
 */
size_t parseData(const char *addr){
    FILE *out = fopen(addr, "wb");
    char c, state;
    Attack a;
    resetFSM(&a, &state); // Sets to state 0
    while ((c = getchar()) != EOF){
        if (state < 6){ // PHASE 1
            if ((c == 'p' && state == 0) ||
                (c == 'o' && state == 1) ||
                (c == 'i' && state == 2) ||
                (c == 'n' && state == 3) ||
                (c == 't' && state == 4) ||
                (c == '(' && state == 5))
                state++;
            else if (c == 'o' && (state == 2)){
                a.panzers++;
            }
        } else { // PHASE 2
            if (c >= '0' && c <= '9'){
                if (state < 8){
                    state = 7;
                    a.x = (a.x << 3) + (a.x << 1) + c - DIGIT_OFFSET; // * 10 + char
                    if (a.x >= 100000)
                        resetFSM(&a, &state);
                } else {
                    state = 9;
                    a.y = (a.y << 3) + (a.y << 1) + c - DIGIT_OFFSET; // * 10 + char
                    if (a.y >= 100000)
                        resetFSM(&a, &state);
                }
            }
            else if (c == ','){
                if (state == 7){
                    state++;
                }
                else {
                    resetFSM(&a, &state);
                }
            }
            else if (c == ')' && state == 9){ // Done and valid, output to file
                fwrite(&a, sizeof(Attack), 1, out);
                resetFSM(&a, &state);
            }
            else if (c == '('){
                resetFSM(&a, &state);
            }
        }
    }
    size_t size = ftell(out);
    fclose(out);
    return size;
}

/**
 * Reads a file containing a list of attacks and prints it to stdout.
 * @param addr Address of the file which contains a list of attacks.
 */
void unpackData(const char *addr){
    FILE *in = fopen(addr, "rb");
    Attack a;
    while (fread(&a, sizeof(Attack), 1, in))
        printf("%d;(%d,%d)\n", a.panzers, a.x, a.y);
    fclose(in);
}