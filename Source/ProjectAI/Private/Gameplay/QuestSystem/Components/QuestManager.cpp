// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Components/QuestManager.h"
#include "Gameplay/QuestSystem/Utility/QSUtility.h"


UQuestManager::UQuestManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManager::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UQuestManager::Init()
{
	if (DefaultQuestsData.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("QS: QuestsData array is empty. Cannot initialize the QuestLog."));
		return;
	}

	for (UQuestData* QuestData : DefaultQuestsData)
		AddQuest(QuestData, bSetDefaultQuestsAsActive);

	UQSUtility::Init(this);
}

void UQuestManager::AddQuest(UQuestData* QuestData, const bool bIsActiveQuest)
{
	if (!QuestData)
	{
		UE_LOG(LogTemp, Error, TEXT("QS: QuestData is null. Cannot add the quest."));
		return;
	}
	
	UQuest* Quest = NewObject<UQuest>();
	Quest->Init(QuestData);

	// QuestData->GetUniqueID(); Could use a unique ID for the key
	AllQuests.Add(QuestData, Quest);

	if (bIsActiveQuest)
		ActiveQuests.Add(Quest);
	else
		InactiveQuests.Add(Quest);
}

void UQuestManager::RemoveQuest(const UQuestData* QuestDataKey)
{
	UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	AllQuests.Remove(QuestDataKey);
	ActiveQuests.Remove(Quest);
	InactiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestManager::AchieveTaskInActiveQuests(const UTaskData* TaskDataKey)
{
	TArray<UQuest*> TempActiveQuests = ActiveQuests; // Shallow copy, avoid modifying the original array while iterating

	for (const UQuest* Quest : TempActiveQuests)
		AchieveTaskInQuest(Quest->QuestData, TaskDataKey);
}

void UQuestManager::AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey)
{
	UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;
	
	Quest->AchieveQuestTask(TaskDataKey);

	if (Quest->bIsQuestCompleted)
		AddToCompletedQuests(Quest->QuestData);
}

void UQuestManager::AddToActiveQuests(const UQuestData* QuestDataKey)
{
	UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;
	
	ActiveQuests.Add(Quest);
	InactiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestManager::AddToInactiveQuests(const UQuestData* QuestDataKey)
{
	UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	InactiveQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestManager::AddToCompletedQuests(const UQuestData* QuestDataKey, const bool bAchieveAllTasks)
{
	UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	if (bAchieveAllTasks)
		Quest->AchieveAllTasks();
	
	CompletedQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	InactiveQuests.Remove(Quest);
}

bool UQuestManager::IsInCompletedQuestsList(const UQuestData* QuestDataKey) const
{
	return CompletedQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsInActiveQuestsList(const UQuestData* QuestDataKey) const
{
	return ActiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsInInactiveQuestsList(const UQuestData* QuestDataKey) const
{
	return InactiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const
{
	const UQuest* Quest = GetQuest(QuestDataKey);
	
	if (!Quest) return false;
	if (!IsInActiveQuestsList(QuestDataKey))
	{
		if (IsInInactiveQuestsList(QuestDataKey))
			return false;

		if (IsInCompletedQuestsList(QuestDataKey))
			return true;
	}

	return Quest->IsTaskAchieved(TaskDataKey);
}

UQuest* UQuestManager::GetQuest(const UQuestData* QuestDataKey) const
{
	return AllQuests.FindRef(QuestDataKey);
}

#if WITH_EDITOR
void UQuestManager::LogAllQuests() const
{
	for( const auto& QuestTuple : AllQuests)
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest: %s"), *QuestTuple.Key->QuestName);

		for( const auto& TaskTuple : QuestTuple.Value->AllTasks)
		{
			const UTaskData* TaskData = TaskTuple.Key;

			UE_LOG(LogTemp, Warning, TEXT("Task: %s"), *TaskData->Description);
			UE_LOG(LogTemp, Warning, TEXT("Task is achieved: %s"), TaskTuple.Value->bIsAchieved ? TEXT("true") : TEXT("false") );
		}
	}
}
#endif