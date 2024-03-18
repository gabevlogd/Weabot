// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/QuestSystem/Quest.h"
#include "Gameplay/QuestSystem/QuestLog.h"
#include "Gameplay/QuestSystem/Data/QuestData.h"
#include "QuestManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Quest Manager")
	UQuestLog* QuestLog;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest Manager")
	UQuest* CurrentQuest;
	
public:
	UQuestManager();
	UFUNCTION(BlueprintCallable, Category = "Quest Manager")
	void SetActiveQuest(const UQuestData* QuestData);

protected:
	virtual void BeginPlay() override;

private:
	void Init();
};
