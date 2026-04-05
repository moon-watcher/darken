# Darken Entity Manager with State Machines Example

This example demonstrates how to use Darken's entity manager with state machines, focusing on a player entity that cycles through different states: idle, running, and attacking.

```c
#include <stdio.h>
#include "darken.h" // Assuming the Darken library is included.

// Define the Player states
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ATTACKING
} PlayerState;

// Function to handle player's state transitions
void update_player_state(Entity* player_entity, PlayerState new_state) {
    de_entity_state(player_entity, new_state);
}

int main() {
    // Initialize the entity manager
    EntityManager* manager = de_manager_init();
    // Create a new player entity
    Entity* player = de_manager_new(manager);

    // Initialize player state
    update_player_state(player, STATE_IDLE);

    // Game loop (simplified)
    while (1) {
        // Simulate input handling and state changes
        char input;
        printf("Enter command (i: idle, r: run, a: attack, q: quit): ");
        scanf(" %c", &input);

        if (input == 'i') {
            update_player_state(player, STATE_IDLE);
        } else if (input == 'r') {
            update_player_state(player, STATE_RUNNING);
        } else if (input == 'a') {
            update_player_state(player, STATE_ATTACKING);
        } else if (input == 'q') {
            break; // Exit the loop
        }

        // Update the entity manager
        de_manager_update(manager);

        // Print current state
        printf("Current state: %d\n", player->state);
    }

    // Cleanup
    de_manager_destroy(manager);
    return 0;
}
```

## Explanation
- The `EntityManager` is initialized using `de_manager_init()`, and entities are created using `de_manager_new()`.
- The player's state can be set using `de_entity_state()`, allowing transitions between idle, running, and attacking states based on user input.
- The game loop continues to update the entity manager with `de_manager_update()` and responds to user inputs until 'q' is pressed.

Ensure to include appropriate error handling in a real application for robustness.