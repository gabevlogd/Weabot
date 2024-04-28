// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/QuestSystem/UObjects/Tasks/TaskBase.h"

void UTaskBase::Init(UTaskData* InitData, UQuestBase* Quest)
{
	TaskData = InitData;
	RelatedQuest = Quest;
}

void UTaskBase::LoadSaveData(const FTaskSaveData& TaskSaveData)
{
	bIsAchieved = TaskSaveData.bIsAchieved;
	if (bIsAchieved)
		AchieveTask(true);
}

void UTaskBase::AchieveTask(bool bFullyAchieve)
{
}

FTaskSaveData UTaskBase::CreateTaskSaveData() const
{
	FTaskSaveData TaskSaveData = FTaskSaveData();
	TaskSaveData.bIsAchieved = bIsAchieved;
	return TaskSaveData;
}

void UTaskBase::ResetTask()
{
	bIsAchieved = false;
}

void UTaskBase::TriggerAchievement()
{
	bIsAchieved = true;
	OnTaskAchieved.Broadcast();
}
