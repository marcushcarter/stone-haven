
typedef struct InventoryItem {
    Block* block;  // Pointer to the Block object
    int quantity;  // How many of this block type in the inventory
} InventoryItem;

// The inventory will store up to MAX_INVENTORY_SIZE items.
InventoryItem inventory[MAX_INVENTORY_SIZE] = {0};

void add_to_inventory(Block* block) {

    bool found = false;

    // Check if the block already exists in the inventory
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        if (inventory[i].block == block) {
            inventory[i].quantity++;
            found = true;
            break;
        }
    }

    // If the block does not exist, find the first available spot
    if (!found) {
        for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
            if (inventory[i].block == NULL) {
                inventory[i].block = block;
                inventory[i].quantity = 1;
                break;
            }
        }
    }
}

void remove_from_inventory(Block* block) {
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        if (inventory[i].block == block) {
            inventory[i].quantity--;

            if (inventory[i].quantity <= 0) {
                inventory[i].block = NULL;
            }
            return;
        }
    }
}
