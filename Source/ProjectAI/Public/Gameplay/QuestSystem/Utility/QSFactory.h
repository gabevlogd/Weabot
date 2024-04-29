// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QSFactory.generated.h"

UCLASS()
class PROJECTAI_API UQSFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UQuestBase* CreateQuestByType(UQuestData* QuestData, const FQuestEntryData& QuestEntryData);
	static UTaskBase* CreateTaskByType(UQuestBase* RelatedQuest, UTaskData* TaskData, const ETaskType TaskType);
};
