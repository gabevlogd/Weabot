// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Gameplay/QuestSystem/Utility/QSFactory.h"
#include "Gameplay/QuestSystem/UObjects/Quests/QuestParallel.h"
#include "Gameplay/QuestSystem/UObjects/Quests/QuestSequencial.h"


UQuestBase* UQSFactory::CreateQuestByType(UQuestData* QuestData, const EQuestType QuestType)
{
	UQuestParallel* QuestParallel = nullptr;
	UQuestSequencial* QuestSequencial = nullptr;
	
	switch (QuestType)
	{
		case EQuestType::None:
			return nullptr;
		
		case EQuestType::Parallel:
			QuestParallel = NewObject<UQuestParallel>();
			QuestParallel->Init(QuestData, QuestType);
			return QuestParallel;
		
		case EQuestType::Sequential:
			QuestSequencial = NewObject<UQuestSequencial>();
			QuestSequencial->Init(QuestData, QuestType);
			return QuestSequencial;
	}
	
	return nullptr;
}
