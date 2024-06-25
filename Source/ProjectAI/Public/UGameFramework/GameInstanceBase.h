// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceBase.generated.h"

UCLASS()
class PROJECTAI_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
