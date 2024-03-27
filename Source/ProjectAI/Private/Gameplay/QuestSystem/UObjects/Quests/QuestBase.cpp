// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "Gameplay/QuestSystem/UObjects/Tasks/Task.h"


void UQuestBase::Init(UQuestData* InitData, const EQuestType Type)
{
	QuestData = InitData;
	QuestType = Type;
	
	for (UTaskData* TaskData : InitData->TasksData)
	{
		UTask* Task = NewObject<UTask>();
		Task->Init(TaskData, this);
		AllTasks.Add(TaskData, Task);
	}
}

void UQuestBase::AchieveQuestTask(const UTaskData* TaskDataKey)
{
}

void UQuestBase::AchieveAllTasks() const
{
	if (bIsQuestCompleted) return;
	
	for (auto& Task : AllTasks)
		Task.Value->AchieveTask();
}

bool UQuestBase::IsTaskAchieved(const UTaskData* TaskDataKey) const
{
	const UTask* Task = GetTask(TaskDataKey);
	if (!Task) return false;
	
	return Task->bIsAchieved;
}

UTask* UQuestBase::GetTask(const UTaskData* TaskDataKey) const
{
	return AllTasks.FindRef(TaskDataKey);
}

UTask* UQuestBase::GetTaskByName(const FName TaskName) const
{
	for (const auto& Task : AllTasks)
	{
		if (Task.Value->TaskData->GetFName() == TaskName)
			return Task.Value;
	}
	
	return nullptr;
}

void UQuestBase::SetQuestStatus(const EQuestStatus Status)
{
	QuestStatus = Status;
}

EQuestStatus UQuestBase::GetQuestStatus() const
{
	return QuestStatus;
}

EQuestType UQuestBase::GetQuestType() const
{
	return QuestType;
}

FQuestSaveData UQuestBase::GetQuestSaveData() const
{
	FQuestSaveData QuestSaveData = FQuestSaveData();
	
	for (const auto& Task : AllTasks)
	{
		QuestSaveData.QuestStatus = QuestStatus;
		QuestSaveData.Tasks.Add(Task.Key->GetFName(), Task.Value->GetTaskSaveData());
	}

	return QuestSaveData;
}

void UQuestBase::ResetQuest()
{
	for(const auto& Task : AllTasks)
		Task.Value->ResetTask();

	bIsQuestCompleted = false;
}

bool UQuestBase::AreAllTasksAchieved() const
{
	for (auto& Task : AllTasks)
	{
		if (!Task.Value->bIsAchieved)
			return false;
	}
	return true;
}
