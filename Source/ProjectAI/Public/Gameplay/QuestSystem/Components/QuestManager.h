// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/QuestSystem/Quest.h"
#include "QuestManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "QuestLog")
	bool bInitAllQuestsAsActive = true;
	UPROPERTY(EditAnywhere, Category = "QuestLog")
	TSet<UQuestData*> QuestsData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TMap<UQuestData*, UQuest*> AllQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> ActiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> InactiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuest*> CompletedQuests;
	
public:
	UQuestManager();
	
	void Init();
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTaskInActiveQuests(const UTaskData* TaskDataKey);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey);
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Quest System")
	void AddToActiveQuests(const UQuestData* QuestDataKey);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AddToInactiveQuests(const UQuestData* QuestDataKey);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void AddToCompletedQuests(const UQuestData* QuestDataKey, bool bAchieveAllTasks = false);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsQuestCompleted(const UQuestData* QuestDataKey) const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsQuestActive(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsQuestInactive(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const;
	
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void LogAllQuests() const;
#endif
	
protected:
	virtual void BeginPlay() override;
	
private:
	UQuest* GetQuest(const UQuestData* QuestDataKey) const;
};
