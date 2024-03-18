// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/QuestLog.h"

void UQuestLog::Init()
{
	if (QuestsData.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("QuestsData array is empty!"));
		return;
	}

	for (UQuestData* QuestData : QuestsData)
	{
		UQuest* Quest = NewObject<UQuest>();
		Quest->Init(QuestData);
		AllQuests.Add(QuestData, Quest);

		if (bAreAllQuestsActive)
			ActiveQuests.Add(Quest);
		else
			InactiveQuests.Add(Quest);
	}
}

void UQuestLog::AchieveTaskInActiveQuests(const UTaskData* TaskData)
{
	for (const UQuest* Quest : ActiveQuests)
	{
		Quest->AchieveTask(TaskData);

		if (Quest->IsQuestCompleted())
			AddToCompletedQuests(Quest->QuestData);
	}
}

void UQuestLog::AddToActiveQuests(const UQuestData* QuestData)
{
	UQuest* Quest = nullptr;
	if (!TryGetQuest(QuestData, Quest)) return;

	ActiveQuests.Add(Quest);
	InactiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestLog::AddToInactiveQuests(const UQuestData* QuestData)
{
	UQuest* Quest = nullptr;
	if (!TryGetQuest(QuestData, Quest)) return;

	InactiveQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	CompletedQuests.Remove(Quest);
}

void UQuestLog::AddToCompletedQuests(const UQuestData* QuestData)
{
	UQuest* Quest = nullptr;
	if (!TryGetQuest(QuestData, Quest)) return;

	CompletedQuests.Add(Quest);
	ActiveQuests.Remove(Quest);
	InactiveQuests.Remove(Quest);
}

bool UQuestLog::TryGetQuest(const UQuestData* QuestData, const UQuest* Quest) const
{
	if (QuestsData.Contains(QuestData))
	{
		Quest = AllQuests[QuestData];
		return true;
	}

	Quest = nullptr;
	return false;
}
