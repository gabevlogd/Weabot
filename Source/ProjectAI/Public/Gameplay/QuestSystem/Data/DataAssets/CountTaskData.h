// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskData.h"
#include "CountTaskData.generated.h"


UCLASS(BlueprintType)
class PROJECTAI_API UCountTaskData : public UTaskData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Task Data")
	int32 AchieveCount;
};
