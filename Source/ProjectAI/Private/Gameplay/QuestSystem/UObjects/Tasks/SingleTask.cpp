// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/QuestSystem/UObjects/Tasks/SingleTask.h"

void USingleTask::AchieveTask(bool bFullyAchieve)
{
	if(bIsAchieved) return; // If the task is already achieved, do not call it again to not trigger the event again

	TriggerAchievement();
}
