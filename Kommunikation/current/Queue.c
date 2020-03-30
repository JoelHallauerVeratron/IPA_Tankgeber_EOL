/*******************************************************************
 *
 *    PROJECT:     Library Queue
 *
 *    EQUIPMENT:   ---
 *
 *    FILE:        Queue.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef min
  #define min(v1,v2) (v1)<(v2)?(v1):(v2)
#endif // !min

#include "Queue.h"

int iQueueItemCount_m;


/**
 * Alloziert eine neue Queue
 *
 * @author uidc9013 (22.09.2011)
 * @return Pointer auf neue Queue
 */
Queue_Root_T Queue_New(void)
{
  Queue_Node_T q=(Queue_Node_T)malloc(sizeof(struct node_s));
  q->pNextNode=q->pPrevNode=NULL;
  iQueueItemCount_m=0;
  return(q);
} // Queue_New()


/**
 * Löscht die Queue
 *
 * @author uidc9013 (23.12.2011)
 * @param q      Pointer auf Queue
 *
 * @return Anzahl gel&ouml;schte Elemente
 */
int Queue_Delete(Queue_Root_T q)
{
  Queue_Node_T tmp;
  int iDeletedCount=0;
  if(!q)
    return(0);

  do
  {
    tmp=QUEUE_HEAD(q);
    if(!tmp)
      return(iDeletedCount);

    QUEUE_HEAD(q) = tmp->pNextNode;
    if(QUEUE_TAIL(q)==tmp)
      QUEUE_TAIL(q)=NULL;
    free(tmp->pvData);
    free(tmp);

    --iQueueItemCount_m;
    ++iDeletedCount;
  } while(1);
  return(0);   // to satisfy compiler
} // Queue_Delete()


/**
 * Gibt zur&uuml;ck, ob die Queue leer ist
 *
 * @author uidc9013 (22.09.2011)
 * @param q      Pointer auf Queue
 *
 * @return 0=Queue enth&auml;lt Elemente<br>
 *         1=Queue ist leer
 */
int Queue_IsEmpty(Queue_Root_T q)
{
  if(!q)
    return(1);
  return(!QUEUE_HEAD(q));
} // Queue_IsEmpty()


/**
 * F&uuml;gt ein Element in die Queue ein
 *
 * @author uidc9013 (22.09.2011)
 * @param q      Pointer auf Queue
 * @param pvData Pointer auf einzuf&uuml;gende Daten
 * @param iSize  Anzahl bytes in pvData
 */
void Queue_Push(Queue_Root_T q,
                void *pvData,
                int   iSize)
{
  Queue_Node_T nd=(Queue_Node_T)malloc(sizeof(struct node_s));
  nd->pvData=malloc(iSize);

  memcpy(nd->pvData,pvData,iSize);
  nd->iSize=iSize;

  if(!QUEUE_HEAD(q))
    QUEUE_HEAD(q)=nd;
  nd->pPrevNode=QUEUE_TAIL(q);
  if(nd->pPrevNode)
    nd->pPrevNode->pNextNode=nd;
  QUEUE_TAIL(q)=nd;
  nd->pNextNode=NULL;
  ++iQueueItemCount_m;
} // Queue_Push()


/**
 * Gibt ein Element aus der Queue zurück
 *
 * @author uidc9013 (22.09.2011)
 * @param q      Pointer auf Queue
 * @param pvData Pointer auf Datenbereich. Wenn NULL angegeben wird,<br>
 *               dann wird nur ein Element gel&ouml;scht.
 * @param iSize  Gr&ouml;sse von Datenbereich in Bytes
 * @param piItemsRemaining
 *               pointer auf int Variable f&uuml;r R&uuml;ckgabe der Anzahl<br>
 *               verbleibende Elemente in Queue.
 *
 * @return &gt;=1=Es wurde ein Element zurückgegeben<br>
 *         0=Queue ist leer
 */
int Queue_Pop(Queue_Root_T q,
              void *pvData,
              int   iSize,
              int  *piItemsRemaining)
{
  Queue_Node_T tmp;

  if(!q)
    return(0);
  tmp=QUEUE_HEAD(q);
  if(!tmp)
    return(0);

  if(pvData)
    memcpy(pvData,tmp->pvData,min(iSize,tmp->iSize));

  QUEUE_HEAD(q) = tmp->pNextNode;
  if(QUEUE_TAIL(q)==tmp)
    QUEUE_TAIL(q)=NULL;
  free(tmp->pvData);
  free(tmp);

  --iQueueItemCount_m;
  if(piItemsRemaining)
  {
    *piItemsRemaining=iQueueItemCount_m;
  }
  return(1);
} // Queue_Pop()


