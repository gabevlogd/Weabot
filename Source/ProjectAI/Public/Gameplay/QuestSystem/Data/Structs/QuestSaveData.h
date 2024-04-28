// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskSaveData.h"
#include "Gameplay/QuestSystem/Data/Enums/QuestType.h"
#include "QuestSaveData.generated.h"

USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSaveData")
	EQuestStatus QuestStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSaveData")
	TMap<FName, FTaskSaveData> Tasks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSaveData")
	int32 CurrentTaskIndex;

	FQuestSaveData() = default;
};
