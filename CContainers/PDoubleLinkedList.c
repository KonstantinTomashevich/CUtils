#include "PDoubleLinkedList.h"
#include "Utils.h"
#include "Errors.h"
#include <stdlib.h>

IOneDirectionIterator PDoubleLinkedListIterator_IOneDirectionIterator =
        {
                PDoubleLinkedListIterator_Next,
                PDoubleLinkedListIterator_Jump,
                PDoubleLinkedListIterator_ValueAt
        };

IBiDirectionalIterator PDoubleLinkedListIterator_IBiDirectionalIterator =
        {
                PDoubleLinkedListIterator_Next,
                PDoubleLinkedListIterator_Jump,
                PDoubleLinkedListIterator_Previous,
                PDoubleLinkedListIterator_JumpBack,
                PDoubleLinkedListIterator_ValueAt
        };

ISizedContainer PDoubleLinkedList_ISizedContainer =
        {
                PDoubleLinkedList_Size
        };

IIterableContainer PDoubleLinkedList_IIterableContainer =
        {
                PDoubleLinkedList_Begin,
                PDoubleLinkedList_End,
                PDoubleLinkedList_At
        };

IMutableContainer PDoubleLinkedList_IMutableContainer =
        {
                PDoubleLinkedList_Insert,
                PDoubleLinkedList_Erase
        };

typedef struct
{
    void *value;
    void *next;
    void *previous;
} PDoubleLinkedListNode;

typedef struct
{
    ulint size;
    PDoubleLinkedListNode *head;
    PDoubleLinkedListNode *tail;
} PDoubleLinkedList;

PDoubleLinkedListHandle PDoubleLinkedList_Create ()
{
    PDoubleLinkedList *list = malloc (sizeof (PDoubleLinkedList));
    list->size = 0;

    PDoubleLinkedListNode *endMarker = malloc (sizeof (PDoubleLinkedListNode));
    endMarker->value = NULL;
    endMarker->next = NULL;
    endMarker->previous = NULL;

    list->head = endMarker;
    list->tail = endMarker;
    return (PDoubleLinkedListHandle) list;
}

void PDoubleLinkedList_Destruct (PDoubleLinkedListHandle handle, void (*DestructCallback) (void **item))
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    PDoubleLinkedListNode *node = list->head;

    while (node != NULL)
    {
        PDoubleLinkedListNode *next = (PDoubleLinkedListNode *) node->next;
        DestructCallback (&(node->value));
        free (node);
        node = next;
    }

    free (list);
}

ulint PDoubleLinkedList_Size (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->size;
}

PDoubleLinkedListIterator PDoubleLinkedList_Begin (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->head;
}

PDoubleLinkedListIterator PDoubleLinkedList_End (PDoubleLinkedListHandle handle)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    return list->tail;
}

PDoubleLinkedListIterator PDoubleLinkedList_At (PDoubleLinkedListHandle handle, ulint index)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    if (index >= list->size)
    {
        return NULL;
    }

    if (index > list->size / 2)
    {
        PDoubleLinkedListIterator iterator = PDoubleLinkedList_End (handle);
        return PDoubleLinkedListIterator_JumpBack (iterator, list->size - index);
    }
    else
    {
        PDoubleLinkedListIterator iterator = PDoubleLinkedList_Begin (handle);
        return PDoubleLinkedListIterator_Jump (iterator, index);
    }
}

PDoubleLinkedListIterator PDoubleLinkedList_Insert (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator where, void *value)
{
    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    list->size += 1;

    PDoubleLinkedListNode *next = (PDoubleLinkedListNode *) where;
    PDoubleLinkedListNode *previous = next->previous;

    PDoubleLinkedListNode *new = malloc (sizeof (PDoubleLinkedListNode));
    new->value = value;
    new->next = next;
    new->previous = previous;

    next->previous = new;
    if (previous != NULL)
    {
        previous->next = new;
    }
    else
    {
        list->head = new;
    }

    return (PDoubleLinkedListIterator) new;
}

PDoubleLinkedListIterator PDoubleLinkedList_Erase (PDoubleLinkedListHandle handle,
        PDoubleLinkedListIterator iterator)
{
    if (iterator == PDoubleLinkedList_End (handle))
    {
        CContainers_SetLastError (PDOUBLELINKEDLIST_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    PDoubleLinkedList *list = (PDoubleLinkedList *) handle;
    list->size -= 1;

    PDoubleLinkedListNode *delete = (PDoubleLinkedListNode *) iterator;
    PDoubleLinkedListNode *next = delete->next;
    PDoubleLinkedListNode *previous = delete->previous;

    next->previous = previous;
    if (previous != NULL)
    {
        previous->next = next;
    }
    else
    {
        list->head = next;
    }

    free (delete);
    return (PDoubleLinkedListIterator) next;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_Next (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return (PDoubleLinkedListIterator) node->next;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_Jump (PDoubleLinkedListIterator iterator, ulint distance)
{
    while (iterator != NULL && distance--)
    {
        iterator = PDoubleLinkedListIterator_Next (iterator);
    }

    return iterator;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_Previous (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return (PDoubleLinkedListIterator) node->previous;
}

PDoubleLinkedListIterator PDoubleLinkedListIterator_JumpBack (PDoubleLinkedListIterator iterator, ulint distance)
{
    while (iterator != NULL && distance--)
    {
        iterator = PDoubleLinkedListIterator_Previous (iterator);
    }

    return iterator;
}

void **PDoubleLinkedListIterator_ValueAt (PDoubleLinkedListIterator iterator)
{
    PDoubleLinkedListNode *node = (PDoubleLinkedListNode *) iterator;
    return &(node->value);
}

IOneDirectionIterator *PDoubleLinkedListIterator_AsIOneDirectionIterator ()
{
    return &PDoubleLinkedListIterator_IOneDirectionIterator;
}

IBiDirectionalIterator *PDoubleLinkedListIterator_AsIBiDirectionalIterator ()
{
    return &PDoubleLinkedListIterator_IBiDirectionalIterator;
}

ISizedContainer *PDoubleLinkedList_AsISizedContainer ()
{
    return &PDoubleLinkedList_ISizedContainer;
}

IIterableContainer *PDoubleLinkedList_AsIIterableContainer ()
{
    return &PDoubleLinkedList_IIterableContainer;
}

IMutableContainer *PDoubleLinkedList_AsIMutableContainer ()
{
    return &PDoubleLinkedList_IMutableContainer;
}
