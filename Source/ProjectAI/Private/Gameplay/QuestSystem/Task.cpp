// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Task.h"

void UTask::Init(UTaskData* InitData)
{
	TaskData = InitData;
}

void UTask::AchieveTask()
{
	if(bIsAchieved) return; // If the task is already achieved, do not call it again to not trigger the event again
	bIsAchieved = true;
	OnTaskAchieved.Broadcast();
}

void UTask::ResetTask()
{
	bIsAchieved = false;
}
