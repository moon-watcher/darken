// Example of basic entity and state management in C

#include <stdio.h>
#include <stdlib.h>

// Define an Entity structure
typedef struct {
    int id;
    const char *name;
    int state;
} Entity;

// Function to create a new Entity
Entity* create_entity(int id, const char *name, int state) {
    Entity *new_entity = malloc(sizeof(Entity));
    new_entity->id = id;
    new_entity->name = name;
    new_entity->state = state;
    return new_entity;
}

// Function to change the state of an Entity
void change_entity_state(Entity *entity, int new_state) {
    entity->state = new_state;
}

// Function to display an Entity's information
void display_entity(Entity *entity) {
    printf("Entity ID: %d\n", entity->id);
    printf("Entity Name: %s\n", entity->name);
    printf("Entity State: %d\n", entity->state);
}

int main() {
    // Create a new entity
    Entity *my_entity = create_entity(1, "MyEntity", 0);
    display_entity(my_entity);

    // Change state
    change_entity_state(my_entity, 1);
    display_entity(my_entity);

    // Clean up
    free(my_entity);
    return 0;
}