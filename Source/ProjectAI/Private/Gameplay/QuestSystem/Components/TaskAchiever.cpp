// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Components/TaskAchiever.h"


UTaskAchiever::UTaskAchiever()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTaskAchiever::AchieveTask()
{
	if (!TaskToAchieve)
	{
		UE_LOG(LogTemp, Error, TEXT("TaskToAchieve is null. Cannot achieve the task."));
		return;
	}
	
	UQSUtility::AchieveTask(TaskToAchieve);
}
