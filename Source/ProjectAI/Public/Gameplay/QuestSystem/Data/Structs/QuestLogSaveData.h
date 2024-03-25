// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "QuestLogSaveData.generated.h"


USTRUCT(BlueprintType)
struct FQuestLogSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLogSaveData" )
	TMap<FName, FQuestSaveData> Quests;
};
