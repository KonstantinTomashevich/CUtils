#ifndef __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__
#define __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__

#include <ShortTypes.h>

typedef void *PSingleLinkedListHandle;
typedef void *PSingleLinkedListIterator;

PSingleLinkedListHandle PSingleLinkedList_Create ();
void PSingleLinkedList_Destruct (PSingleLinkedListHandle handle, void (*DestructCallback) (void **item));
uint PSingleLinkedList_Size (PSingleLinkedListHandle handle);

PSingleLinkedListIterator PSingleLinkedList_Begin (PSingleLinkedListHandle handle);
PSingleLinkedListIterator PSingleLinkedList_End (PSingleLinkedListHandle handle);
PSingleLinkedListIterator PSingleLinkedList_At (PSingleLinkedListHandle handle, uint index);

PSingleLinkedListIterator PSingleLinkedList_Insert (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator where, void *value);
PSingleLinkedListIterator PSingleLinkedList_Erase (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator);
PSingleLinkedListIterator PSingleLinkedList_EraseNext (PSingleLinkedListHandle handle,
        PSingleLinkedListIterator iterator);

PSingleLinkedListIterator PSingleLinkedListIterator_Next (PSingleLinkedListIterator iterator);
void **PSingleLinkedListIterator_ValueAt (PSingleLinkedListIterator iterator);
void PSingleLinkedListIterator_ForEach (PSingleLinkedListIterator begin, PSingleLinkedListIterator end,
        void (*Callback) (void **item));

#endif // __CUTILS_CCONTAINERS_PSINGLELINKEDLIST_H__