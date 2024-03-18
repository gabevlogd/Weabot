// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Task.h"

void UTask::Init(UTaskData* Data)
{
	TaskData = Data;
}

bool UTask::IsTaskAchieved() const
{
	return bIsAchieved;
}

void UTask::AchieveTask()
{
	if(bIsAchieved) return; // If the task is already achieved, do not call it again to not trigger the event again
	
	bIsAchieved = true;
}