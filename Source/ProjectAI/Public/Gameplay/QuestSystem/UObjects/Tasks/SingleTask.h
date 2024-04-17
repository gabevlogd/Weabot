// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/UObjects/Tasks/TaskBase.h"
#include "Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "SingleTask.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API USingleTask : public UTaskBase
{
	GENERATED_BODY()

public:
	virtual void AchieveTask(bool bFullyAchieve) override;
};
