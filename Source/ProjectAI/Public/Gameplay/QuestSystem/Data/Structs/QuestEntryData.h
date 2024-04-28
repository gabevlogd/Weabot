// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveData.h"
#include "Gameplay/QuestSystem/Data/Enums/QuestStatus.h"
#include "QuestEntryData.generated.h"

USTRUCT(BlueprintType)
struct FQuestEntryData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestEntryData")
	EQuestType QuestType = EQuestType::Parallel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestEntryData")
	EQuestStatus InitialQuestStatus = EQuestStatus::Active;

	FQuestEntryData() = default;
};
