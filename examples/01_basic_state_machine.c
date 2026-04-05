#include <stdio.h>

// Define the states
typedef enum {
    STATE_IDLE,
    STATE_MOVING,
    STATE_ATTACKING
} State;

// Player entity structure
typedef struct {
    State current_state;
} Player;

// Function to update player's state
void update_player_state(Player *player, State new_state) {
    player->current_state = new_state;
    switch (new_state) {
        case STATE_IDLE:
            printf("Player is idle.\n");
            break;
        case STATE_MOVING:
            printf("Player is moving.\n");
            break;
        case STATE_ATTACKING:
            printf("Player is attacking.\n");
            break;
    }
}

int main() {
    Player player;
    player.current_state = STATE_IDLE; // Initial state

    // Simulate state changes
    update_player_state(&player, STATE_MOVING);
    update_player_state(&player, STATE_ATTACKING);
    update_player_state(&player, STATE_IDLE);

    return 0;
}