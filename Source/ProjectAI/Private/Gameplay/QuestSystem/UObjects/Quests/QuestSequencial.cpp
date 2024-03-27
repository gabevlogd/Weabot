// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/UObjects/Quests/QuestSequencial.h"
#include "Gameplay/QuestSystem/UObjects/Tasks/Task.h"


void UQuestSequencial::Init(UQuestData* InitData, const EQuestType Type)
{
	Super::Init(InitData, Type);
	TasksByIndex = InitData->TasksData;
}

void UQuestSequencial::AchieveQuestTask(const UTaskData* TaskDataKey)
{
	if (bIsQuestCompleted) return;
	UTask* Task = GetTask(TaskDataKey);
	if (!Task) return;

	if (CurrentTaskIndex > TasksByIndex.Num() - 1 || CurrentTaskIndex < 0) return;

	if (TasksByIndex[CurrentTaskIndex] == TaskDataKey) // Checks if the task is at the current index
	{
		Task->AchieveTask();
		CurrentTaskIndex++;
	}
}

void UQuestSequencial::ResetQuest()
{
	Super::ResetQuest();
	CurrentTaskIndex = 0;
}

int32 UQuestSequencial::GetCurrentTaskIndex() const
{
	return CurrentTaskIndex;
}

int32 UQuestSequencial::GetTaskIndex(UTaskData* TaskDataKey) const
{
	const UTask* Task = GetTask(TaskDataKey);
	if (!Task) return -1;

	return TasksByIndex.Find(TaskDataKey);
}
