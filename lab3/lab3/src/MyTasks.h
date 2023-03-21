/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.h
* Date Created:		1/10/23
* Modifications:	3/8/23	Removed MyButtonTask, added TaskHeartbeat, TaskLED, and TaskSystemControl
**************************************************************/

#ifndef MYTASKS_H_
#define MYTASKS_H_

typedef enum
{
	increase,
	decrease
} LEDAction;

TaskHandle_t mainTasks[3];
QueueHandle_t qLeds[3];

void TaskHeartbeat(void* pvParameters);
void TaskLED(void* pvParameters);
void TaskModifiedLED(void* pvParameters);
void TaskMainControl(void* pvParameters);
void TaskUART(void* pvParameters);

#endif /* MYTASKS_H_ */