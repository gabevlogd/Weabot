// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestBase.h"
#include "Gameplay/QuestSystem/Data/DataAssets/TaskData.h"
#include "UObject/Object.h"
#include "QuestParallel.generated.h"


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UQuestParallel : public UQuestBase
{
	GENERATED_BODY()

public:
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey, const bool bFullyAchieve) override;
};
