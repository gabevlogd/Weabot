// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskSaveData.generated.h"


USTRUCT(BlueprintType)
struct FTaskSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskSaveData")
	bool bIsAchieved;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskSaveData")
	int32 CurrentAchieveCount;

	FTaskSaveData() = default;
};
