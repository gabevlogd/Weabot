// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/QuestSystem/Data/TaskData.h"
#include "TaskAchiever.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UTaskAchiever : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest System")
	UTaskData* TaskToAchieve;

public:
	UTaskAchiever();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTask();
};
