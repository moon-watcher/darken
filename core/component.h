#pragma once

#include <stdint.h>
#include <string.h>

/**
 * @file component.h
 * @brief Component system for Darken - Adds ECS-style component composition
 * 
 * This module extends Darken with component support, allowing entities to have
 * multiple data components attached, similar to traditional ECS systems.
 * 
 * Components are identified by a unique ID and stored efficiently in a sparse
 * component store for fast iteration and good cache locality.
 */

/* Component ID type - up to 256 different component types */
typedef uint8_t de_component_id;

/* Invalid component ID */
#define DE_INVALID_COMPONENT ((de_component_id)255)

/**
 * @brief Component descriptor - metadata for a component type
 */
typedef struct {
    de_component_id id;           /* Unique component ID */
    uint16_t size;                /* Size of component data in bytes */
    void (*on_create)(void *);    /* Optional: called when component is created */
    void (*on_destroy)(void *);   /* Optional: called when component is destroyed */
} de_component_desc;

/**
 * @brief Initialize the component system
 * Call once before creating any entities with components
 */
void de_component_system_init(void);

/**
 * @brief Register a new component type
 * @param desc Component descriptor with size and callbacks
 * @return Component ID to use for de_entity_add_component()
 */
de_component_id de_component_register(const de_component_desc *desc);

/**
 * @brief Attach a component to an entity
 * @param entity The target entity
 * @param component_id The ID of the component type
 * @return Pointer to the component data, or NULL if failed
 */
void *de_entity_add_component(de_entity *entity, de_component_id component_id);

/**
 * @brief Get a component from an entity
 * @param entity The target entity
 * @param component_id The ID of the component type
 * @return Pointer to the component data, or NULL if not found
 */
void *de_entity_get_component(de_entity *entity, de_component_id component_id);

/**
 * @brief Check if entity has a component
 * @param entity The target entity
 * @param component_id The ID of the component type
 * @return 1 if has component, 0 otherwise
 */
int de_entity_has_component(de_entity *entity, de_component_id component_id);

/**
 * @brief Remove a component from an entity
 * @param entity The target entity
 * @param component_id The ID of the component type
 */
void de_entity_remove_component(de_entity *entity, de_component_id component_id);

/**
 * @brief Get component by name (helper for debugging)
 * @param name Component name registered in descriptor
 * @return Component ID, or DE_INVALID_COMPONENT if not found
 */
de_component_id de_component_find_by_name(const char *name);