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
    BoatType d;
    d = type;

    int r = row;
    int c = col;


    //base case
    //  if (f->field[row][col] != FIELD_POSITION_EMPTY) {
    //     printf("Cannot place boat at these coordinates");
    //     return FALSE;
    //  }

    //if BoatDirection = EAST
    if (dir == FIELD_BOAT_DIRECTION_EAST) {
        for (j = 0; j < 3; j++) {
            if (f->field[row][col] != FIELD_POSITION_EMPTY) {
                return FALSE;

            } else {
                spaces++;
                col++;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == 3) {
            for (i = 0; i < 3; i++) {
                f->field[r][c] = FIELD_BOAT_HUGE;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                c++;
            }
        }
        //reset space value
        spaces = 0;
    }


    //if BoatDirection = WEST
    else if (dir == FIELD_BOAT_DIRECTION_WEST) {
        for (j = 0; j < 3; j++) { //BOAT_TYPE_SIZE
            if (f->field[row][col] != FIELD_POSITION_EMPTY) {
                return FALSE;

            } else {
                spaces++;
                col--;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == 3) {
            for (i = 0; i < 3; i++) {
                f->field[r][c] = FIELD_BOAT_HUGE;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                c--;
            }
        }
        //reset space value
        spaces = 0;
    }

    //if BoatDirection = NORTH
    if (dir == FIELD_BOAT_DIRECTION_NORTH) {
        for (j = 0; j < 3; j++) {
            if (f->field[row][col] != FIELD_POSITION_EMPTY) {
                return FALSE;

            } else {
                spaces++;
                row--;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == 3) {
            for (i = 0; i < 3; i++) {
                f->field[r][c] = FIELD_BOAT_HUGE;
                p = f->field[r][c];
                printf("Fieldposition is: %d\n", p);
                r--;
            }
        }
        //reset space value
        spaces = 0;
    }

    //if BoatDirection = SOUTH
    if (dir == FIELD_BOAT_DIRECTION_SOUTH) {
        for (j = 0; j < 3; j++) {
            if (f->field[row][col] != FIELD_POSITION_EMPTY) {
                return FALSE;

            } else {
                spaces++;
                row++;
                printf("Total spaces available for boat: %d\n", spaces);

            }



        }

        if (spaces == 3) {
            for (i = 0; i < 3; i++) {
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
