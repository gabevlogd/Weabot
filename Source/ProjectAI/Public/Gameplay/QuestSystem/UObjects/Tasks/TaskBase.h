// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/Data/DataAssets/TaskData.h"
#include "Gameplay/QuestSystem/Data/Structs/TaskSaveData.h"
#include "UObject/Object.h"
#include "TaskBase.generated.h"

class UQuestBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskAchieved);


UCLASS(Abstract, NotBlueprintable, BlueprintType)
class PROJECTAI_API UTaskBase : public UObject
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
	virtual void Init(UTaskData* InitData, UQuestBase* Quest);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual void LoadSaveData(const FTaskSaveData& TaskSaveData);
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual void AchieveTask(bool bFullyAchieve = false);
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual FTaskSaveData CreateTaskSaveData() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual void ResetTask();

protected:
	void TriggerAchievement();
};
