// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/Data/DataAssets/QuestData.h"
#include "Gameplay/QuestSystem/Data/Enums/QuestStatus.h"
#include "Gameplay/QuestSystem/Data/Structs/QuestSaveData.h"
#include "UObject/Object.h"
#include "QuestBase.generated.h"

class UTask;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);


UCLASS(Abstract, BlueprintType)
class PROJECTAI_API UQuestBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	UQuestData* QuestData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<UTaskData*, UTask*> AllTasks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsQuestCompleted;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnQuestCompleted OnQuestCompleted;
	
private:
	EQuestType QuestType;
	EQuestStatus QuestStatus;
	
public:
	virtual void Init(UQuestData* InitData, EQuestType QuestType);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey);
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveAllTasks() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsTaskAchieved(const UTaskData* TaskDataKey) const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UTask* GetTask(const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UTask* GetTaskByName(const FName TaskName) const;
	
	void SetQuestStatus(EQuestStatus Status);

	UFUNCTION(BlueprintPure, Category = "Quest System")
	EQuestStatus GetQuestStatus() const;

	UFUNCTION(BlueprintPure, Category = "Quest System")
	EQuestType GetQuestType() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	FQuestSaveData GetQuestSaveData() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	virtual void ResetQuest();

protected:
	bool AreAllTasksAchieved() const;
};
