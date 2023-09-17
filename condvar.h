#ifndef CONDVAR_H
#define CONDVAR_H


#include "FreeRTOS.h"
#include "semphr.h"

#ifndef INC_FREERTOS_H
    #error "include FreeRTOS.h" must appear in source files before "include condvar.h"
#endif

#include "queue.h"

#include "condvar.h"


typedef QueueHandle_t SemaphoreHandle_t;
//typedef SemaphoreHandle_t CondvarHandle_t;

 struct CondVarHandle_t{
    SemaphoreHandle_t mutex;
    SemaphoreHandle_t coda; 
};

#define semCONDVAR_SEMAPHORE_QUEUE_LENGTH    ( ( uint8_t ) 1U )
#define semSEMAPHORE_QUEUE_ITEM_LENGTH      ( ( uint8_t ) 0U )
#define semGIVE_BLOCK_TIME                  ( ( TickType_t ) 0U )



#if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
#define xCondVarCreate()                                                                                    \
    {                    \
        struct CondVarHandle_t xCondVar;                    \
        ( xCondVar->mutex ) = xSemaphoreCreateMutex(); \
        (xCondVar->coda) = xSemaphoreCreateCounting(1,0); \
           return xCondVar \
    }
#endif


#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
    #define xCondVarCreateStatic( pxStaticCondVar ) \
{                                                           \
        CondVarHandle_t cv; \
       cv.mutex =  xSemaphoreCreateMutexStatic( pxStaticCondVar.mutex ); \
       cv.coda = xSemaphoreCreateCountingStatic(1,0,pxStaticCondVar.coda); \
        return cv; \
    }  // xQueueGenericCreateStatic( ( UBaseType_t ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH, NULL, ( pxStaticSemaphore ), queueQUEUE_TYPE_BINARY_SEMAPHORE )
#endif /* configSUPPORT_STATIC_ALLOCATION */

#if (1==1)//assuming you have already the possession of the mutex->
#define xCondVarWaitCondition( xCondVar, xBlockTime )       \
{                                        \
    xSemaphoreGive(xCondVar.mutex); \
    xSemaphoreTake(xCondVar->coda, xBlockTime); \
    xSemaphoreTake(xCondVar->mutex, xBlockTime);\
} 
#endif
 //  xQueueSemaphoreTake( ( xSemaphore ), ( xBlockTime ) )
#if (1==1)
   //assuming you have already the possession of the mutex 
#define xCondVarNotify( xCondVar )    \
{                                                               \
    xSemaphoreGive(xCondVar->coda); \
    xSemaphoreGive(xCondVar->mutex); \
}//xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK )
#endif

#if (1==1)
#define xCondVarNotifyFromISR( xCondVar, pxHigherPriorityTaskWoken )  \
 {                                                                          \
    xSemaphoreGiveFromISR(xCondVar->coda);\
    xSemaphoreGiveFromISR(xCondVar->mutex);\
}// xQueueGiveFromISR( ( QueueHandle_t ) ( xSemaphore ), ( pxHigherPriorityTaskWoken ) )
#endif
/*
#if (1==1)
#define xCondVarWaitConditionFromISR( xCondVar, pxHigherPriorityTaskWoken ) \ 
    {                                                                               \
    xSemaphoreGiveFromISR(xCondVar->mutex); \
    xSemaphoreTakeFromISR(xCondVar->coda, pxHigherPriorityTaskWoken); \
    xSemaphoreTakeFromISR(xCondVar->mutex, pxHigherPriorityTaskWoken); \
}//   xQueueReceiveFromISR( ( QueueHandle_t ) ( xSemaphore ), NULL, ( pxHigherPriorityTaskWoken ) )
#endif*/

#if (1==1)
#define vCondVarDelete( xCondVar )    \
{                                           \
    vSemaphoreDelete(xCondVar->mutex); \
    vSemaphoreDelete(xCondVar->coda); \
}//vQueueDelete( ( QueueHandle_t ) ( xSemaphore ) )
#endif

#if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )
    #define xCondVarGetMutexHolder( xCondVar )    xQueueGetMutexHolder( ( xCondVar->mutex ) )
#endif

#if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )
    #define xCondVarGetMutexHolderFromISR( xCondVar )    xQueueGetMutexHolderFromISR( ( xCondVar->mutex ) )
#endif
#define uxCondVarGetCountFromISR( xCondVar )    uxQueueMessagesWaitingFromISR( ( QueueHandle_t ) ( xCondVar ) )

#endif


