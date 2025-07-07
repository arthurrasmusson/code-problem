# Hash-Map-Based Solution

This project provides a simple C implementation of an array-like storage where you can:

* **Set a single element** to a specific value (`set_value`).
* **Set all elements** to the same value in O(1) time (`set_all_values`), without iterating through the entire array.
* **Get an element’s value** (`get_value`), returning either an individual value.

Under the hood, we keep:

1. **A global default value** (`defaultValue`) that applies to every index whenever `set_all_values` is called.
2. **A fixed-size open-addressing hash map** to store only the indices you’ve overridden individually after the last global reset.

This design lets us avoid touching every element on a global reset while still providing fast per-index updates and lookups.

---

## Files

* **`hash-map.c`**: Contains the full implementation, including:

  * Hash map helper functions (`map_clear`, `map_put`, `map_get`).
  * Functions (`set_value`, `set_all_values`, `get_value`).
  * A simple `main()` demonstrating basic usage.

---

## How It Works

1. **`set_all_values(char value)`**

   * Sets `defaultValue = value`.
   * Calls `map_clear()` to drop all per-index overrides in O(MAP\_SIZE).

2. **`set_value(int index, char value)`**

   * Inserts or updates the override in the hash map using linear probing.

3. **`get_value(int index)`**

   * Looks up the index in the hash map:

     * If found, returns the override value.
     * Otherwise, returns `defaultValue`.

---

## Hash Map Details

* We use **open addressing with linear probing**:

  1. Hash = `index & (MAP_SIZE - 1)` (requires `MAP_SIZE` to be a power of two).
  2. If the slot is empty (`map_keys[h] == -1`) or matches the key, we stop.
  3. Otherwise, advance `h = (h + 1) & (MAP_SIZE - 1)` and repeat.

* **Storage**:

  * `map_keys[]` stores the overridden indexes or `-1` for empty.
  * `map_vals[]` stores the corresponding override char values.

This simple code gives **O(1)** average-time inserts and lookups.

