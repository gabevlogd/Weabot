// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/QuestSystem/Components/QuestManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QSUtility.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class PROJECTAI_API UQSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static UQuestManager* CurrentQuestManager;
	
public:
	static void Init(UQuestManager* QuestManager);
	
	/**
	 * Gets the found QuestManager.
	 * @return QuestManager.
	 */
	UFUNCTION(BlueprintPure, Category = "Quest System")
	static UQuestManager* GetQuestManager();

	/**
	 * Achieves a task in the active quests list.
	 * @param TaskDataKey The task to achieve.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void AchieveTask(const UTaskData* TaskDataKey);

	/**
	 * Puts the quest in the active quests list.
	 * @param QuestDataKey The quest to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void ActivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Puts the quest in the inactive quests list.
	 * @param QuestDataKey The quest to deactivate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void DeactivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Puts the quest in the completed quests list and achieves all its tasks.
	 * @param QuestDataKey The quest to complete.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void CompleteQuestNow(const UQuestData* QuestDataKey);

	/**
	 * Tracks a quest.
	 * @param QuestDataKey The quest to track.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void TrackQuest(const UQuestData* QuestDataKey);
};
