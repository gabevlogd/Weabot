// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "Data/TaskData.h"
#include "Gameplay/QuestSystem/Task.h"
#include "UObject/Object.h"
#include "Quest.generated.h"


UCLASS()
class PROJECTAI_API UQuest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	UQuestData* QuestData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<UTaskData*, UTask*> TasksMap;
	
public:
	void Init(UQuestData* Data);
	void AchieveTask(const UTaskData* TaskData) const;
	bool IsQuestCompleted() const;
	bool TryGetTask(const UTaskData* TaskData, const UTask* Task) const;
};
