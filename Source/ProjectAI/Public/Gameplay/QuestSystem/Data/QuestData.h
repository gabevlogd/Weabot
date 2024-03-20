// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskData.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"


UCLASS()
class PROJECTAI_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	TSet<UTaskData*> TasksData;
};
