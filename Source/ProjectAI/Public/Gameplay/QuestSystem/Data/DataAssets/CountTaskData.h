// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TaskData.h"
#include "CountTaskData.generated.h"

UCLASS(BlueprintType)
class PROJECTAI_API UCountTaskData : public UTaskData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Task Data")
	int32 CountToAchieve;

public:
	virtual ETaskType GetTaskType() const override;
};
