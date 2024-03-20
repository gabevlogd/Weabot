// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "Data/TaskData.h"
#include "Gameplay/QuestSystem/Task.h"
#include "UObject/Object.h"
#include "Quest.generated.h"


UCLASS(NotBlueprintable, NotBlueprintType)
class PROJECTAI_API UQuest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	UQuestData* QuestData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<UTaskData*, UTask*> AllTasks;
	
public:
	void Init(UQuestData* InitData);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveQuestTask(const UTaskData* TaskDataKey) const;
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveAllTasks() const;
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool AreAllTasksAchieved() const;
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsTaskAchieved(const UTaskData* TaskDataKey) const;
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UTask* GetTask(const UTaskData* TaskDataKey) const;
};
