// Copyright The Prototypers, Inc. All Rights Reserved.


#include "Generic/SaveSystem/Components/Savers/Saver.h"
#include "Generic/SaveSystem/Utility/SlotsUtility.h"
#include "Generic/SaveSystem/Utility/SSUtility.h"
#include "Kismet/GameplayStatics.h"


USaver::USaver()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FName USaver::GetUniqueSaveID() const
{
	const FName SlotName = USSUtility::GetSaveManager()->GetSaveGameInstance()->SlotInfoName;
	const FName OwnerName = GetOwner()->GetFName();
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	const FString UniqueID = SlotName.ToString() + "_" + OwnerName.ToString() + "::" + LevelName;
	const int32 Hash = GetTypeHash(UniqueID);
	const FString Hex = FString::Printf(TEXT("%08X"), Hash);
	
	return FName(*Hex);
}

void USaver::PrepareSave(UDefaultSaveGame* SaveGame, USlotInfoItem* SlotInfoItem)
{
	UE_LOG(LogTemp, Warning, TEXT("USaver::PrepareSave"));
	OnPrepSave.Broadcast(SaveGame, SlotInfoItem);
	OnPrepareSave(SaveGame, SlotInfoItem);
}

void USaver::PrepareLoad(UDefaultSaveGame* SaveGame)
{
	OnPrepLoad.Broadcast(SaveGame);
	OnPrepareLoad(SaveGame);
}

void USaver::OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameInstance, USlotInfoItem* SlotInfoItem)
{
}

void USaver::OnPrepareLoad_Implementation(UDefaultSaveGame* SaveGame)
{
}

void USaver::OnSaveCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, bool bSuccess, UDefaultSaveGame* SaveGame)
{
	OnSaveGameCompleted.Broadcast(SlotName, UserIndex, bSuccess, SaveGame);
}

void USaver::OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData)
{
	OnLoadGameCompleted.Broadcast(SlotName, UserIndex, LoadedData);
}

void USaver::BeginPlay()
{
	Super::BeginPlay();
	if (!USSUtility::GetSaveManager()) return;
	USSUtility::GetSaveManager()->OnPrepareSave.AddDynamic(this, &USaver::PrepareSave);
	USSUtility::GetSaveManager()->OnPrepareLoad.AddDynamic(this, &USaver::PrepareLoad);
	USSUtility::GetSaveManager()->OnSaveGame.AddDynamic(this, &USaver::OnSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnLoadGame.AddDynamic(this, &USaver::OnLoadCompletedEvent);
}

void USaver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!USSUtility::GetSaveManager()) return;
	USSUtility::GetSaveManager()->OnPrepareSave.RemoveDynamic(this, &USaver::PrepareSave);
	USSUtility::GetSaveManager()->OnPrepareLoad.RemoveDynamic(this, &USaver::PrepareLoad);
	USSUtility::GetSaveManager()->OnSaveGame.RemoveDynamic(this, &USaver::OnSaveCompletedEvent);
	USSUtility::GetSaveManager()->OnLoadGame.RemoveDynamic(this, &USaver::OnLoadCompletedEvent);
}
