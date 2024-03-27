// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestBase.h"
#include "QuestSequencial.generated.h"


UCLASS(NotBlueprintable, BlueprintType)
class PROJECTAI_API UQuestSequencial : public UQuestBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Quest System")
	TArray<UTaskData*> TasksByIndex;

private:
	int32 CurrentTaskIndex = 0;
	
public:
	virtual void Init(UQuestData* InitData, const EQuestType Type) override;
	virtual void AchieveQuestTask(const UTaskData* TaskDataKey) override;
	virtual void ResetQuest() override;
	
	UFUNCTION(BlueprintPure, Category = "Quest System")
	int32 GetCurrentTaskIndex() const;

	UFUNCTION(BlueprintPure, Category = "Quest System")
	int32 GetTaskIndex(UTaskData* TaskDataKey) const;
};
