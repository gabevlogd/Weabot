// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/Data/DataAssets/TaskData.h"
#include "Gameplay/QuestSystem/Data/Structs/TaskSaveData.h"
#include "UObject/Object.h"
#include "Task.generated.h"

class UQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskAchieved);


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UTask : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	UTaskData* TaskData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	UQuestBase* RelatedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task")
	bool bIsAchieved;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnTaskAchieved OnTaskAchieved;
	
public:
	void Init(UTaskData* InitData, UQuestBase* Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTask();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	FTaskSaveData GetTaskSaveData() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void ResetTask();
};
