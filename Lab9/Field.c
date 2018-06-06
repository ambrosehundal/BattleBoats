#include "Field.h"
//#include <xc.h>
//#include <plib.h>
#include "BOARD.h"

#include <stdio.h>




//BoatType b = FIELD_BOAT_SMALL;
//printf("BoatType is : %d\n", b);

void FieldInit(Field *f, FieldPosition p) {
    //test again
    int i, j;

    //loop through the array to initialize all Field values with p
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 10; j++) {
            f->field[i][j] = p;
        }
    }
    //  f->field[4][6] = p;  
    f->smallBoatLives = 3;
    f->mediumBoatLives = 4;
    f->largeBoatLives = 5;
    f->hugeBoatLives = 6;
    printf("Boat lives from smallest to biggest: %d, %d, %d, %d\n",
            f->smallBoatLives, f->mediumBoatLives, f->largeBoatLives,
            f->hugeBoatLives);
    printf("Field Position: %d\n", p);
}

FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col) {
    FieldPosition p;
    p = f->field[row][col];
    printf("FieldPosition at the specified index is %d\n", p);
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
    printf("Old field value is: %d\n", d);

    //set new value at the field location
    f->field[row][col] = p;
    printf("New field value is: %d\n", p);

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
  //  BoatType d;
   // d = type;

    int r = row;
    int c = col;

    int boatLength = 0;
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

    //if BoatDirection = EAST
    //doesnt work for checking if field is empty in a loop

    if (dir == FIELD_BOAT_DIRECTION_EAST) {
        for (j = 0; j < boatLength; j++) {
            if (col >= FIELD_COLS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                printf("Boat out of range or another boat there");
                return FALSE;
            } else {
                spaces++;
                col++;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == boatLength) {
            for (i = 0; i < boatLength; i++) {
                f->field[r][c] = boatLength;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                c++;
            }
        }
        //reset space value
        spaces = 0;
    }//if BoatDirection = WEST
    else if (dir == FIELD_BOAT_DIRECTION_WEST) {
        for (j = 0; j < boatLength; j++) { //BOAT_TYPE_SIZE
            if (col < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                printf("Boat out of range or another boat there");
                return FALSE;

            } else {
                spaces++;
                col--;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == boatLength) {
            for (i = 0; i < boatLength; i++) {
                f->field[r][c] = FIELD_BOAT_MEDIUM;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                c--;
            }
        }
        //reset space value
        spaces = 0;
    }//if BoatDirection = NORTH
    else if (dir == FIELD_BOAT_DIRECTION_NORTH) {
        for (j = 0; j < boatLength; j++) {
            if (row < 0 || f->field[row][col] != FIELD_POSITION_EMPTY) {
                printf("Boat out of range or another boat there");
                return FALSE;

            } else {
                spaces++;
                row--;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }
        if (spaces == boatLength) {
            for (i = 0; i < boatLength; i++) {
                f->field[r][c] = FIELD_BOAT_LARGE;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                r--;
            }
        }
        //reset space value
        spaces = 0;
    }//if BoatDirection = SOUTH
    else if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
        for (j = 0; j < boatLength; j++) {
            if (row >= FIELD_ROWS || f->field[row][col] != FIELD_POSITION_EMPTY) {
                printf("Boat out of range or another boat there");
                return FALSE;

            } else {
                spaces++;
                row++;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == boatLength) {
            for (i = 0; i < boatLength; i++) {
                f->field[r][c] = FIELD_BOAT_HUGE;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                r++;
            }
        }
        //reset space value
        spaces = 0;
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
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_MEDIUM_BOAT) {
            f->mediumBoatLives--;
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_LARGE_BOAT) {
            f->largeBoatLives--;
        }
        if (f->field[gData->row][gData->col] == FIELD_POSITION_HUGE_BOAT) {
            f->hugeBoatLives--;
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



    printf("FieldPosition is %d", enemy);
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
        boatStatus = boatStatus || FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives > 0) {
        boatStatus = boatStatus || FIELD_BOAT_STATUS_MEDIUM;
    }

    if (f->largeBoatLives > 0) {
        boatStatus = boatStatus || FIELD_BOAT_STATUS_LARGE;
    }

    if (f->hugeBoatLives > 0) {
        boatStatus = boatStatus || FIELD_BOAT_STATUS_HUGE;
    }




    return boatStatus;
}

