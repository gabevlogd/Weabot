// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Gameplay/QuestSystem/Data/Enums/TaskType.h"
#include "TaskData.generated.h"

UCLASS(BlueprintType)
class PROJECTAI_API UTaskData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task Data")
	FString TaskName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task Data")
	FString Description;

public:
	UFUNCTION(BlueprintPure, Category = "Quest System")
	virtual ETaskType GetTaskType() const;
};

