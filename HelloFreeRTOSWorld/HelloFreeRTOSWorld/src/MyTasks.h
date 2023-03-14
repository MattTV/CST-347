/*************************************************************
* Author:			Matt Volpe
* Filename:			MyTasks.h
* Date Created:		1/10/23
* Modifications:	3/8/23	Removed MyButtonTask, added TaskHeartbeat and TaskSystemControl
**************************************************************/

#ifndef MYTASKS_H_
#define MYTASKS_H_

void TaskHeartbeat(void* pvParameters);
void TaskSystemControl(void* pvParameters);
void TaskLED(void* pvParameters);

#endif /* MYTASKS_H_ */