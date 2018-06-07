#include "Field.h"
//#include <xc.h>
//#include <plib.h>
#include "BOARD.h"

#include <stdio.h>

/*FieldInit()
 * @param - Field* f - pointer to the field to be initialized
 * @param = Fieldposition p - the field position to initialize the field with
 * This function initializes the given field with the given fieldposition, and 
 * initializes all boatlives to 3, 4, 5, 6 respectively from smallest to Huge 
 * boat
 */



void FieldInit(Field *f, FieldPosition p) {
    int i, j;

    //loop through the array to initialize all Field values with p
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            f->field[i][j] = p;
        }
    }

    f->smallBoatLives = 3;
    f->mediumBoatLives = 4;
    f->largeBoatLives = 5;
    f->hugeBoatLives = 6;
}

/*
 * FieldAt()
 * @param Field*f - pointer to enemy field
 * row - row coordinate of the field
 * col - column coordinate of the field
 * Function takes in row-col coordinate values and returns the FieldPosition
 * at those coordinates on the enemy field
 */


FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col) {
    FieldPosition p;
    p = f->field[row][col];
    return p;
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p) {
    FieldPosition d;
    //Get the old value of field location with FieldAt()
    d = FieldAt(f, row, col);

    //set new value at the field location
    f->field[row][col] = p;


    return d;

}

/* @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type) {
    int j = 0;
    int i;
    int spaces = 0;
    FieldPosition p;
    int r = row;
    int c = col;

    int boatLength = 0;
    //determine length of boat based on BoatType
    if (type == FIELD_BOAT_SMALL) {
        boatLength = 3;
    } else if (type == FIELD_BOAT_MEDIUM) {
        boatLength = 4;
    } else if (type == FIELD_BOAT_LARGE) {
        boatLength = 5;
    } else if (type == FIELD_BOAT_HUGE) {
        boatLength = 6;
    }


    //base case
    if (f->field[row][col] != FIELD_POSITION_EMPTY) {
        printf("Cannot place boat at these coordinates\n");
        return FALSE;
    }


    /* IF BOAT TYPE = FIELD_POSITION_SMALL_BOAT
     ************************************************************************
     */
    if (type == FIELD_BOAT_SMALL) {
        //if BoatDirection = EAST
        if (dir == FIELD_BOAT_DIRECTION_EAST) {
            for (j = 0; j < boatLength; j++) {
                if (col >= FIELD_COLS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_SMALL_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c++;
                }
                printf("Boat successfully placed!\n");

            }
            //reset space value
            spaces = 0;
        }/*
         *if BoatDirection = WEST
         */
        else if (dir == FIELD_BOAT_DIRECTION_WEST) {
            for (j = 0; j < boatLength; j++) {
                if (col < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_SMALL_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }/*
     *
     *if BoatDirection = NORTH
     */
        else if (dir == FIELD_BOAT_DIRECTION_NORTH) {
            for (j = 0; j < boatLength; j++) {
                if (row < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_SMALL_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }//if BoatDirection = SOUTH
        else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
            for (j = 0; j < boatLength; j++) {
                if (row >= FIELD_ROWS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {

                    f->field[r][c] = FIELD_POSITION_SMALL_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r++;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }
    }/* IF BOAT TYPE = FIELD_POSITION_MEDIUM_BOAT
         ************************************************************************
         */
    else if (type == FIELD_BOAT_MEDIUM) {
        //if BoatDirection = EAST
        if (dir == FIELD_BOAT_DIRECTION_EAST) {
            for (j = 0; j < boatLength; j++) {
                if (col >= FIELD_COLS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_MEDIUM_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c++;
                }
                printf("Boat successfully placed!\n");

            }
            //reset space value
            spaces = 0;
        }/*
         *if BoatDirection = WEST
         */
        else if (dir == FIELD_BOAT_DIRECTION_WEST) {
            for (j = 0; j < boatLength; j++) {
                if (col < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_MEDIUM_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }/*
     *
     *if BoatDirection = NORTH
     */
        else if (dir == FIELD_BOAT_DIRECTION_NORTH) {
            for (j = 0; j < boatLength; j++) {
                if (row < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_MEDIUM_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }//if BoatDirection = SOUTH
        else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
            for (j = 0; j < boatLength; j++) {
                if (row >= FIELD_ROWS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {

                    f->field[r][c] = FIELD_POSITION_MEDIUM_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r++;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }
    }/* IF BOAT TYPE = FIELD_POSITION_LARGE_BOAT
         ************************************************************************
         */
    else if (type == FIELD_BOAT_LARGE) {
        //if BoatDirection = EAST
        if (dir == FIELD_BOAT_DIRECTION_EAST) {
            for (j = 0; j < boatLength; j++) {
                if (col >= FIELD_COLS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_LARGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c++;
                }
                printf("Boat successfully placed!\n");

            }
            //reset space value
            spaces = 0;
        }/*
         *if BoatDirection = WEST
         */
        else if (dir == FIELD_BOAT_DIRECTION_WEST) {
            for (j = 0; j < boatLength; j++) {
                if (col < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_LARGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }/*
     *
     *if BoatDirection = NORTH
     */
        else if (dir == FIELD_BOAT_DIRECTION_NORTH) {
            for (j = 0; j < boatLength; j++) {
                if (row < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_LARGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }//if BoatDirection = SOUTH
        else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
            for (j = 0; j < boatLength; j++) {
                if (row >= FIELD_ROWS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {

                    f->field[r][c] = FIELD_POSITION_LARGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r++;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }
    }/* IF BOAT TYPE = FIELD_POSITION_HUGE_BOAT
         ************************************************************************
         */
    else if (type == FIELD_BOAT_HUGE) {
        //if BoatDirection = EAST
        if (dir == FIELD_BOAT_DIRECTION_EAST) {
            for (j = 0; j < boatLength; j++) {
                if (col >= FIELD_COLS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_HUGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c++;
                }
                printf("Boat successfully placed!\n");

            }
            //reset space value
            spaces = 0;
        }/*
         *if BoatDirection = WEST
         */
        else if (dir == FIELD_BOAT_DIRECTION_WEST) {
            for (j = 0; j < boatLength; j++) {
                if (col < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    col--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_HUGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    c--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }/*
     *
     *if BoatDirection = NORTH
     */
        else if (dir == FIELD_BOAT_DIRECTION_NORTH) {
            for (j = 0; j < boatLength; j++) {
                if (row < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row--;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);
            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {
                    f->field[r][c] = FIELD_POSITION_HUGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r--;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }//if BoatDirection = SOUTH
        else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
            for (j = 0; j < boatLength; j++) {
                if (row >= FIELD_ROWS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                    printf("Boat out of range or another boat already placed\n");
                    return FALSE;
                } else {
                    spaces++;
                    row++;
                }
            }
            printf("Total spaces available to place boat: %d\n", spaces);

            if (spaces == boatLength) {
                for (i = 0; i < boatLength; i++) {

                    f->field[r][c] = FIELD_POSITION_HUGE_BOAT;
                    p = f->field[r][c];
                    printf("Fieldposition at Field[%d][%d] is: %d\n", r, c, p);
                    r++;
                }
                printf("Boat successfully placed!\n");
            }
            //reset space value
            spaces = 0;
        }
    }








    return TRUE;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData) {
    FieldPosition old; // to return old FieldPosition value 
    old = f->field[gData->row][gData->col];

    if (f->field[gData->row][gData->col] != FIELD_POSITION_EMPTY) {
        if (f->field[gData->row][gData->col] == FIELD_POSITION_SMALL_BOAT) {
            f->smallBoatLives--;
            //  printf("Smallboatlives left = %d\n", f->smallBoatLives);
            //if the hit sunk the small boat
            if (f->smallBoatLives == 0) {
                gData->hit = HIT_SUNK_SMALL_BOAT;
                //  printf("Small boat sunk");
            }
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_MEDIUM_BOAT) {
            f->mediumBoatLives--;
            //if the hit sunk the medium boat
            if (f->mediumBoatLives == 0) {
                gData->hit = HIT_SUNK_MEDIUM_BOAT;
            }
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_LARGE_BOAT) {
            f->largeBoatLives--;
            //if the hit sunk the large boat
            if (f->largeBoatLives == 0) {
                gData->hit = HIT_SUNK_LARGE_BOAT;
            }
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_HUGE_BOAT) {
            f->hugeBoatLives--;
            //if the hit sunk the huge boat
            if (f->hugeBoatLives == 0) {
                gData->hit = HIT_SUNK_HUGE_BOAT;
            }
        }
        gData->hit = HIT_HIT;
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
    } else {

        gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }

    return old;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData) {
    FieldPosition enemy; // to return old FieldPosition value 
    enemy = f->field[gData->row][gData->col];

    if (gData->hit == HIT_HIT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
    } else if (gData->hit == HIT_SUNK_SMALL_BOAT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->smallBoatLives = 0;
    } else if (gData->hit == HIT_SUNK_MEDIUM_BOAT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->mediumBoatLives = 0;
    } else if (gData->hit == HIT_SUNK_LARGE_BOAT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->largeBoatLives = 0;
    } else if (gData->hit == HIT_SUNK_HUGE_BOAT) {
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->hugeBoatLives = 0;
    } else {
        //  gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
    }
    return enemy;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f) {
    uint8_t boatStatus = 0;
    if (f->smallBoatLives > 0) {
        boatStatus = boatStatus | FIELD_BOAT_STATUS_SMALL;
    } else if (f->smallBoatLives == 0) {
        boatStatus = boatStatus & 0b1110;
    }
    if (f->mediumBoatLives > 0) {
        boatStatus = boatStatus | FIELD_BOAT_STATUS_MEDIUM;
    } else if (f->smallBoatLives == 0) {
        boatStatus = boatStatus & 0b1101;
    }

    if (f->largeBoatLives > 0) {
        boatStatus = boatStatus | FIELD_BOAT_STATUS_LARGE;
    } else if (f->smallBoatLives == 0) {
        boatStatus = boatStatus & 0b1011;
    }

    if (f->hugeBoatLives > 0) {
        boatStatus = boatStatus | FIELD_BOAT_STATUS_HUGE;
    } else if (f->smallBoatLives == 0) {
        boatStatus = boatStatus & 0b0111;
    }

    printf("BoatStatus is : %x", boatStatus);



    return boatStatus;
}


//helper function to print boatstates

uint8_t PrintBoat(uint8_t boatStatus) {
    uint8_t boat = boatStatus;
    uint8_t temp = 0;
    int z = 128;

    while (z > 0) {
        //temp = boatStatus << 1;
        printf("%u ", boat & 0b1111 ? 1: 0);
        boat = boat<<1;
        z>>=1;
    }
    return temp;

}
