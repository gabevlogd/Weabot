// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "QuestLogSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FQuestLogSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestSaveData")
	FName TrackedQuestFName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLogSaveData" )
	TMap<FName, FQuestSaveData> Quests;

	FQuestLogSaveData() = default;
};
