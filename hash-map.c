#include <stddef.h>
#include <stdio.h>

#define ARRAY_SIZE 100       /* maximum valid index + 1 */
#define MAP_SIZE   256

// Arthur Rasmusson code problem

/* The global value to use when no per-index override exists */
static char defaultValue = 0;

/* Simple open-addressing hash table for overrides:
 *  - map_keys[i] == -1 means slot i is empty
 *  - otherwise map_keys[i] stores the overridden array index
 *  - map_vals[i] stores the override value for that index
 */
static int  map_keys[MAP_SIZE];
static char map_vals[MAP_SIZE];

/* Initialize or reset the hash map by marking every slot empty */
static void map_clear(void) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        map_keys[i] = -1;      /* -1 signals “unused” */
    }
}

/* Insert or update override[index] = value into the hash map */
static void map_put(int index, char value) {
    /* Compute initial bucket via simple bitmask hash */
    unsigned h = (unsigned)index & (MAP_SIZE - 1);
    /* Linear probing: advance until we find either:
     *  - an empty slot (map_keys[h] == -1), or
     *  - the same index already stored (map_keys[h] == index)
     */
    while (map_keys[h] != -1 && map_keys[h] != index) {
        h = (h + 1) & (MAP_SIZE - 1);
    }
    /* Store the key and its value */
    map_keys[h] = index;
    map_vals[h] = value;
}

/* Lookup override for `index`. 
 * If found, writes the override to *out_value and returns 1; else returns 0.
 */
static int map_get(int index, char *out_value) {
    /* Start at the same hash bucket */
    unsigned h = (unsigned)index & (MAP_SIZE - 1);
    unsigned start = h;
    /* Probe until we hit an empty slot or come full circle */
    while (map_keys[h] != -1) {
        if (map_keys[h] == index) {
            *out_value = map_vals[h];
            return 1;           /* found override */
        }
        h = (h + 1) & (MAP_SIZE - 1);
        if (h == start)       /* we've looped around—stop search */
            break;
    }
    return 0;                   /* no override present */
}

/* Set a single element override */
void set_value(int index, char value) {
    if (index < 0 || index >= ARRAY_SIZE)
        return;               /* ignore out-of-range */
    map_put(index, value);
}

/* Apply a new “global” default and clear all per-index overrides */
void set_all_values(char value) {
    defaultValue = value;
    map_clear();               /* clear any stored overrides */
}

/* Retrieve value: per-index override if present; otherwise defaultValue */
char get_value(int index) {
    char v;
    if (index < 0 || index >= ARRAY_SIZE)
        return 0;              /* out-of-range fallback */
    if (map_get(index, &v))
        return v;              /* return override */
    return defaultValue;       /* return global default */
}

/* main */
int main(void) {
    defaultValue = 0;
    map_clear();

    printf("1) Setting A[0] = 9 and A[2] = 10 via set_value()\n");
    set_value(0, 9);
    set_value(2, 10);

    printf("2) Reading A[0] and A[8]:\n");
    printf("   A[0] = %d  (explicitly set)\n", get_value(0));
    printf("   A[8] = %d  (never set → default 0)\n", get_value(8));

    printf("3) Calling set_all_values(10) to override every position to 10\n");
    set_all_values(10);

    printf("4) After override, reading A[0] and A[2]:\n");
    printf("   A[0] = %d  (global override)\n", get_value(0));
    printf("   A[2] = %d  (global override)\n", get_value(2));

    printf("5) Now setting A[3] = 99 with set_value() after override\n");
    set_value(3, 99);

    printf("6) Final values A[0] through A[100]:\n");
    for (int i = 0; i <= 100; i++) {
        printf("   A[%2d] = %d\n", i, get_value(i));
    }

    return 0;
}
