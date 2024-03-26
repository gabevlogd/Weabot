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
	 * Get the found QuestLog.
	 * @return QuestLog.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static UQuestManager* GetQuestManager();

	/**
	 * Achieve a task in the active quests.
	 * @param TaskDataKey The task to achieve.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void AchieveTask(const UTaskData* TaskDataKey);

	/**
	 * Put the quest in the Active Quests list.
	 * @param QuestDataKey The quest to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void ActivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Put the quest in the Inactive Quests list.
	 * @param QuestDataKey The quest to deactivate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void DeactivateQuest(const UQuestData* QuestDataKey);

	/**
	 * Put the quest in the Completed Quests list achieving all its tasks.
	 * @param QuestDataKey The quest to complete.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	static void CompleteQuestNow(const UQuestData* QuestDataKey);
};
