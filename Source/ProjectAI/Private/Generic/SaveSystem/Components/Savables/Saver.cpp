// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savables/Saver.h"
#include "Generic/SaveSystem/Utility/SVUtility.h"
#include "Kismet/GameplayStatics.h"


USaver::USaver()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName USaver::GetUniqueNameID() const
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString OwnerName = GetOwner()->GetName();
	return FName(*FString::Printf(TEXT("%s_%s"), *LevelName, *OwnerName));
}

void USaver::PrepareSave(UGenericSaveGame* SaveGameData)
{
	UE_LOG(LogTemp, Warning, TEXT("USaver::PrepareSave"));
	OnPrepSave.Broadcast(SaveGameData);
	OnPrepareSave(SaveGameData);
}

void USaver::PrepareLoad(UGenericSaveGame* SaveGameData)
{
	OnPrepLoad.Broadcast(SaveGameData);
	OnPrepareLoad(SaveGameData);
}

void USaver::OnPrepareSave_Implementation(UGenericSaveGame* SaveGameData)
{
}

void USaver::OnPrepareLoad_Implementation(UGenericSaveGame* SaveGameData)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UGenericSaveGame* SaveGameData)
{
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGameData);
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UGenericSaveGame* LoadedData)
{
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData);
}

void USaver::BeginPlay()
{
	Super::BeginPlay();
	if (!USVUtility::GetSaveManager()) return;

	USVUtility::GetSaveManager()->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	USVUtility::GetSaveManager()->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	USVUtility::GetSaveManager()->OnSaveGame.AddDynamic(this, &USaver::OnSaveCompletedEvent);
	USVUtility::GetSaveManager()->OnLoadGame.AddDynamic(this, &USaver::OnLoadCompletedEvent);
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	USVUtility::GetSaveManager()->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	USVUtility::GetSaveManager()->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	USVUtility::GetSaveManager()->OnSaveGame.RemoveDynamic(this, &USaver::OnSaveCompletedEvent);
	USVUtility::GetSaveManager()->OnLoadGame.RemoveDynamic(this, &USaver::OnLoadCompletedEvent);
}