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
	if (QuestsData.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("QS: QuestsData array is empty. Cannot initialize the QuestLog."));
		return;
	}

	for (UQuestData* QuestData : QuestsData)
	{
		UQuest* Quest = NewObject<UQuest>();
		Quest->Init(QuestData);

		// QuestData->GetUniqueID(); Could use a unique ID for the key
		AllQuests.Add(QuestData, Quest);

		if (bInitAllQuestsAsActive)
			ActiveQuests.Add(Quest);
		else
			InactiveQuests.Add(Quest);
	}

	UQSUtility::Init(this);
}

void UQuestManager::AchieveTaskInActiveQuests(const UTaskData* TaskDataKey)
{
	for (const UQuest* Quest : ActiveQuests)
		AchieveTaskInQuest(Quest->QuestData, TaskDataKey);
}

void UQuestManager::AchieveTaskInQuest(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey)
{
	const UQuest* Quest = GetQuest(QuestDataKey);
	if (!Quest) return;

	Quest->AchieveQuestTask(TaskDataKey);

	if (Quest->AreAllTasksAchieved())
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

bool UQuestManager::IsQuestCompleted(const UQuestData* QuestDataKey) const
{
	return CompletedQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsQuestActive(const UQuestData* QuestDataKey) const
{
	return ActiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsQuestInactive(const UQuestData* QuestDataKey) const
{
	return InactiveQuests.Contains(GetQuest(QuestDataKey));
}

bool UQuestManager::IsTaskAchieved(const UQuestData* QuestDataKey, const UTaskData* TaskDataKey) const
{
	const UQuest* Quest = GetQuest(QuestDataKey);
	
	if (!Quest) return false;
	if (!IsQuestActive(QuestDataKey))
	{
		if (IsQuestInactive(QuestDataKey))
			return false;

		if (IsQuestCompleted(QuestDataKey))
			return true;
	}

	return Quest->IsTaskAchieved(TaskDataKey);
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

UQuest* UQuestManager::GetQuest(const UQuestData* QuestDataKey) const
{
	return AllQuests.FindRef(QuestDataKey);
}