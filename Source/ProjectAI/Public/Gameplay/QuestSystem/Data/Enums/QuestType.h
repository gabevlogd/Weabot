// Copyright The Prototypers, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum EQuestType
{
	None UMETA(DisplayName = "None"),
	Active UMETA(DisplayName = "Active"),
	Inactive UMETA(DisplayName = "Inactive"),
	Completed UMETA(DisplayName = "Completed")
};
