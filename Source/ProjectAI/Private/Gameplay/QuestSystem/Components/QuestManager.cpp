// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Components/QuestManager.h"


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
	if(QuestLog == nullptr) return;

	QuestLog->Init();
	SetActiveQuest(QuestLog->QuestsData[0]);
}

void UQuestManager::SetActiveQuest(const UQuestData* QuestData)
{
	CurrentQuest = QuestLog->AllQuests[QuestData];
}
