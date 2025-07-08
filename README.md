# 0(1) Time Solution

This code demonstrates a simple C implementation of array-like storage that supports **constant-time** global and per-index updates without iterating through the entire array.

## 0(1)-time C code

0(1)-time.c implements per-index timestamps and a single global timestamp to decide in **O(1)** time whether to use an individual value or a global override value.

We maintain:

1. **`values[ARRAY_SIZE]`** – stores the last value written at each index.
2. **`updatedTimestamps[ARRAY_SIZE]`** – records when each index was last updated via `set_value()`.
3. **`overrideActive`** – flag indicating whether a global override is in effect.
4. **`overrideValue`** – the value to return for all indices not individually updated since the override.
5. **`overrideTime`** – the timestamp when `set_all_values()` was last called.

`set_value()`, `set_all_values()`, and `get_value()`—run in constant time.

---

## API

### `void set_value(int index, char value)`

* Stores **`value`** at **`index`** in **`values[]`**.
* Records the current time in **`updatedTimestamps[index]`**.

### `void set_all_values(char value)`

* Enables global override by setting **`overrideActive = true`**.
* Stores the override value in **`overrideValue`**.
* Records the current time in **`overrideTime`**.

### `char get_value(int index)`

1. If **`overrideActive`** is **`true`**:

   1. If **`updatedTimestamps[index] > overrideTime`**, the index was updated after the override: return **`values[index]`**.
   2. Otherwise: return **`overrideValue`**.
2. If **`overrideActive`** is **`false`**: return **`values[index]`**.

---

## How It Works

1. **Global reset in O(1):**

   * Calling **`set_all_values(v)`** does **not** iterate over every element. It sets **`overrideActive`**, sets **`overrideValue = v`**, and records **`overrideTime`**.

2. **Per-index updates in O(1):**

   * **`set_value(i, v)`** writes **`values[i] = v`** and stamps **`updatedTimestamps[i]`** with the current time.

3. **Lookups in O(1):**

   * **`get_value(i)`** compares two timestamps:

     * **`updatedTimestamps[i]`** (individual update time)
     * **`overrideTime`** (last global reset time)
   * A simple comparison and branch determine which value to return.

0(1)-time.c only uses time comparisons which avoids any per-element work on a global reset, yet still allows subsequent per-index writes.

| Operation        | Time | Space |
| ---------------- | ---- | ----- |
| `set_all_values` | O(1) | O(1)  |
| `set_value`      | O(1) | O(1)  |
| `get_value`      | O(1) | O(1)  |

---


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

