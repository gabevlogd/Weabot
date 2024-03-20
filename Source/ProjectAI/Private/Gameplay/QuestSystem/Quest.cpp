// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Quest.h"


void UQuest::Init(UQuestData* InitData)
{
	QuestData = InitData;
	
	for (UTaskData* TaskData : InitData->TasksData)
	{
		UTask* Task = NewObject<UTask>();
		Task->Init(TaskData);
		AllTasks.Add(TaskData, Task);
	}
}

void UQuest::AchieveQuestTask(const UTaskData* TaskDataKey) const
{
	UTask* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	Task->AchieveTask();
}

void UQuest::AchieveAllTasks() const
{
	for (auto& Task : AllTasks)
		Task.Value->AchieveTask();
}

bool UQuest::AreAllTasksAchieved() const
{
	for (auto& Task : AllTasks)
	{
		if (!Task.Value->bIsAchieved)
			return false;
	}
	return true;
}

bool UQuest::IsTaskAchieved(const UTaskData* TaskDataKey) const
{
	const UTask* Task = GetTask(TaskDataKey);
	if (!Task) return false;
	
	return Task->bIsAchieved;
}

UTask* UQuest::GetTask(const UTaskData* TaskDataKey) const
{
	return AllTasks.FindRef(TaskDataKey);
}
