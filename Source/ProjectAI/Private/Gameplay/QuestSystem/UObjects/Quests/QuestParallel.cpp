// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/UObjects/Quests/QuestParallel.h"
#include "Gameplay/QuestSystem/UObjects/Tasks/Task.h"


void UQuestParallel::AchieveQuestTask(const UTaskData* TaskDataKey)
{
	if (bIsQuestCompleted) return;
	
	UTask* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	Task->AchieveTask();
	bIsQuestCompleted = AreAllTasksAchieved();

	if (bIsQuestCompleted)
		OnQuestCompleted.Broadcast();
}
