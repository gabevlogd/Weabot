// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFilterData.h"
#include "TaskData.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"


UCLASS(BlueprintType)
class PROJECTAI_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	FString QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	UQuestFilterData* QuestFilter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	TArray<UTaskData*> TasksData;
};
