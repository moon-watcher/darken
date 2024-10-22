#include "manager.h"
#include "entity.h"
#include "../debug.h"
#include "../config.h"

void de_manager_init(de_manager *const this, unsigned bytes, unsigned datasize)
{
    uclist_initAlloc(&this->list, sizeof(de_entity) + bytes);

#if DARKEN_DEBUG
    if (datasize == 0)                                   
    {                                                    
        DARKEN_NOTICE("manager datasize is 0");   
    }                                                    
    else if ((this->data = malloc(datasize)) == 0)       
    {                                                    
        DARKEN_ERROR("manager malloc() is null"); 
    }
#else
    datasize && (this->data = malloc(datasize));
#endif
}

de_entity *de_manager_new(de_manager *const this, de_state desctructor)
{
    de_entity *entity = uclist_alloc(&this->list);

    if (entity != 0)
    {
        // _entity_init(entity, this, desctructor);
        entity->state = ({ void *f() { return 0; } f; });
        entity->destructor = desctructor;
        entity->manager = this;
    }
    else
    {
        DARKEN_ERROR("manager uclist_alloc() is null");
    }

    return entity;
}

void de_manager_update(de_manager *const this)
{
    uclist_iterator(&this->list, de_entity_update, 1);
}

void de_manager_reset(de_manager *const this)
{
    void _entity_destroy(de_entity *const this)
    {
        if (this->destructor != 0)
            this->destructor(this, this->data);
    }

    uclist_iterator(&this->list, _entity_destroy, 1);
    uclist_reset(&this->list);
}

void de_manager_end(de_manager *const this)
{
    de_manager_reset(this);
    uclist_end(&this->list);
    MEM_free(this->data);
    memset(this, 0, sizeof(de_manager));
}
