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

void UQuest::AchieveQuestTask(const UTaskData* TaskDataKey)
{
	if (bIsQuestCompleted) return;
	
	UTask* Task = GetTask(TaskDataKey);
	if (!Task) return;
	
	Task->AchieveTask();
	bIsQuestCompleted = AreAllTasksAchieved();

	if (bIsQuestCompleted)
		OnQuestCompleted.Broadcast();
}

void UQuest::AchieveAllTasks() const
{
	if (bIsQuestCompleted) return;
	
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

UTask* UQuest::GetTaskByName(const FName TaskName) const
{
	for (const auto& Task : AllTasks)
	{
		if (Task.Value->TaskData->GetFName() == TaskName)
			return Task.Value;
	}
	
	return nullptr;
}

void UQuest::SetQuestType(const EQuestType QuestType)
{
	CurrentQuestType = QuestType;
}

EQuestType UQuest::GetQuestType() const
{
	return CurrentQuestType;
}

FQuestSaveData UQuest::GetQuestSaveData() const
{
	FQuestSaveData QuestSaveData = FQuestSaveData();
	
	for (const auto& Task : AllTasks)
	{
		QuestSaveData.QuestType = CurrentQuestType;
		QuestSaveData.Tasks.Add(Task.Key->GetFName(), Task.Value->GetTaskSaveData());
	}

	return QuestSaveData;
}

void UQuest::ResetQuest()
{
	for(const auto& Task : AllTasks)
		Task.Value->ResetTask();

	bIsQuestCompleted = false;
}
