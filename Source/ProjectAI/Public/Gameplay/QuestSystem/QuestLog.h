// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "Data/QuestData.h"
#include "UObject/Object.h"
#include "QuestLog.generated.h"


UCLASS(EditInlineNew)
class PROJECTAI_API UQuestLog : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "QuestLog")
	bool bAreAllQuestsActive = true;
	UPROPERTY(EditAnywhere, Category = "QuestLog")
	TArray<UQuestData*> QuestsData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TMap<UQuestData*, UQuest*> AllQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> ActiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> InactiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> CompletedQuests;
	
public:
	void Init();
	void AchieveTaskInActiveQuests(const UTaskData* TaskData);
	void AddToActiveQuests(const UQuestData* Quest);
	void AddToInactiveQuests(const UQuestData* Quest);
	void AddToCompletedQuests(const UQuestData* Quest);

private:
	bool TryGetQuest(const UQuestData* QuestData, const UQuest* Quest) const;
};
