// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/TaskData.h"
#include "UObject/Object.h"
#include "Task.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskAchieved);


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UTask : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	UTaskData* TaskData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	bool bIsAchieved;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnTaskAchieved OnTaskAchieved;
	
public:
	void Init(UTaskData* InitData);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTask();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void ResetTask();
};
