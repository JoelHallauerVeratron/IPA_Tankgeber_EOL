/*******************************************************************
 *
 *    PROJECT:     Library Queue
 *
 *    EQUIPMENT:   ---
 *
 *    FILE:        Queue.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author$
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef QUEUE_H_INCLUDED
  #define QUEUE_H_INCLUDED
  #include <stdio.h>
  #include <stdlib.h>


typedef struct node_s* Queue_Node_T;
typedef struct node_s* Queue_Root_T;

struct node_s
{
  Queue_Node_T pPrevNode;
  Queue_Node_T pNextNode;
  void *pvData;
  int iSize;
};

  #define QUEUE_HEAD(q) q->pPrevNode
  #define QUEUE_TAIL(q) q->pNextNode


Queue_Root_T Queue_New(void);

int Queue_Delete(Queue_Root_T q);


int Queue_IsEmpty(Queue_Root_T q);

void Queue_Push(Queue_Root_T q,
                void *pvData,
                int iSize);

int Queue_Pop(Queue_Root_T q,
              void *pvData,
              int   iSize,
              int  *piItemsRemaining);

#endif // !QUEUE_H_INCLUDED

