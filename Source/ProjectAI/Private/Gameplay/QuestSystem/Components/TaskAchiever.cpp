// Copyright The Prototypers, Inc. All Rights Reserved.

#include "Gameplay/QuestSystem/Components/TaskAchiever.h"
#include "Gameplay/QuestSystem/Utility/QSUtility.h"

UTaskAchiever::UTaskAchiever(): TaskToAchieve(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTaskAchiever::AchieveTask()
{
	if (!TaskToAchieve)
	{
		UE_LOG(LogQuestSystem, Error, TEXT("TaskToAchieve is null. Cannot achieve the task."));
		return;
	}
	
	UQSUtility::AchieveTask(TaskToAchieve);
}
