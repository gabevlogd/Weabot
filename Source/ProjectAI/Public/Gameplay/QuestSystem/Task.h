// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/TaskData.h"
#include "UObject/Object.h"
#include "Task.generated.h"


UCLASS(NotBlueprintable, NotBlueprintType)
class PROJECTAI_API UTask : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	UTaskData* TaskData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	bool bIsAchieved;

public:
	void Init(UTaskData* InitData);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTask();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void ResetTask();
};
