// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "Data/TaskData.h"
#include "Data/Enums/QuestType.h"
#include "Data/Structs/QuestSaveData.h"
#include "Gameplay/QuestSystem/Task.h"
#include "UObject/Object.h"
#include "Quest.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UQuest : public UObject
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
	EQuestType CurrentQuestType;
	
public:
	void Init(UQuestData* InitData);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveQuestTask(const UTaskData* TaskDataKey);
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveAllTasks() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsTaskAchieved(const UTaskData* TaskDataKey) const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UTask* GetTask(const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UTask* GetTaskByName(const FName TaskName) const;
	
	void SetQuestType(EQuestType QuestType);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	EQuestType GetQuestType() const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	FQuestSaveData GetQuestSaveData() const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void ResetQuest();

private:
	bool AreAllTasksAchieved() const;
};
