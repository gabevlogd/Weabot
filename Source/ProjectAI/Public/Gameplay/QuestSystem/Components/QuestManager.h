// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/QuestSystem/UObjects/Quests/QuestBase.h"
#include "Gameplay/QuestSystem/Data/DataAssets/QuestData.h"
#include "Gameplay/QuestSystem/Data/DataAssets/QuestLogData.h"
#include "Gameplay/QuestSystem/Data/Structs/QuestLogSaveData.h"
#include "QuestManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyQuestCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyTaskAchieved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnQuestTracked,
	const UQuestBase*, Quest);


UCLASS(NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTAI_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "QuestLog")
	UQuestLogData* QuestLogData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	UQuestBase* TrackedQuest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TMap<UQuestData*, UQuestBase*> AllQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuestBase*> ActiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuestBase*> InactiveQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestLog")
	TArray<UQuestBase*> CompletedQuests;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnAnyQuestCompleted OnAnyQuestCompleted;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnAnyTaskAchieved OnAnyTaskAchieved;
	UPROPERTY(BlueprintAssignable, Category = "Quest System")
	FOnQuestTracked OnQuestTracked;
	
public:
	UQuestManager();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void LoadSaveData(FQuestLogSaveData QuestLogSaveData);

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	FQuestLogSaveData CreateSaveData() const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void TrackQuest(const UQuestData* QuestDataKey);
	
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
	bool IsInCompletedQuestsList(const UQuestData* QuestDataKey) const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsInActiveQuestsList(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsInInactiveQuestsList(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	bool IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UQuestBase* GetQuest(const UQuestData* QuestDataKey) const;

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	UQuestBase* GetQuestByName(const FName QuestName) const;
	
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	TArray<UQuestBase*> GetQuestsByFilter(const UQuestFilterData* QuestFilterData) const;
	
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Quest System")
	void LogAllQuests() const;
#endif
	
protected:
	virtual void BeginPlay() override;
	void AddQuest(UQuestData* QuestData, const FQuestEntryData QuestEntryData);
	void RemoveQuest(const UQuestData* QuestDataKey);
};
