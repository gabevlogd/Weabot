// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Quest.h"

void UQuest::Init(UQuestData* Data)
{
	QuestData = Data;
	
	for (UTaskData* TaskData : Data->TasksData)
	{
		UTask* Task = NewObject<UTask>();
		Task->Init(TaskData);
		TasksMap.Add(TaskData, Task);
	}
}

void UQuest::AchieveTask(const UTaskData* TaskData) const
{
	UTask* Task = nullptr;
	if(!TryGetTask(TaskData, Task)) return;
	Task->AchieveTask();
}

bool UQuest::IsQuestCompleted() const
{
	for (auto& Task : TasksMap)
	{
		if (!Task.Value->IsTaskAchieved())
			return false;
	}
	return true;
}

bool UQuest::TryGetTask(const UTaskData* TaskData, const UTask* Task) const
{
	if (!TasksMap.Contains(TaskData)) return false;
	Task = TasksMap[TaskData];
	return true;
}
