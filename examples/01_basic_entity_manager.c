#include <stdio.h>
#include <stdlib.h>

// Define states
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
} State;

// Entity structure with a state and an ID
typedef struct {
    int id;
    State state;
} Entity;

// Function to change the entity state
void change_state(Entity *entity, State new_state) {
    printf("Entity %d changing state from %d to %d\n", entity->id, entity->state, new_state);
    entity->state = new_state;
}

int main() {
    Entity entity;
    entity.id = 1;
    entity.state = STATE_IDLE;

    // Change state of the entity
    change_state(&entity, STATE_RUNNING);
    change_state(&entity, STATE_PAUSED);
    change_state(&entity, STATE_IDLE);

    return 0;
}