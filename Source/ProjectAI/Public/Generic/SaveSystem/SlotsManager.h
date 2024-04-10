// Copyright Denis Faraci, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveManager.h"
#include "Data/Structs/SaveSlotData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlotsManager.generated.h"


UCLASS()
class PROJECTAI_API USlotsManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static FString SelectedSaveSlotName;
	static USaveManager* CurrentSaveManager;
	
public:
	static void Init(USaveManager* SaveManager);

	/**
	 * Selects a slot by name.
	 * @param SlotName The name of the slot to select.
	 * @return True if the slot was selected, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool SelectSlot(const FString& SlotName);

	/**
	 * Gets the name of the selected slot.
	 * @return The name of the selected slot.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static FString GetSelectedSlotName();

	/**
	 * Creates a slot file with the given name.
	 * @param SlotName The name of the slot to create.
	 * @param bSelectCreatedSlot If true, the created slot will be selected.
	 * @return True if the slot file was created, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool CreateSlotFileOfName(const FString& SlotName, bool bSelectCreatedSlot = true);

	/**
	 * Creates the next slot file and selects it.
	 * @return True if the slot file was created, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool CreateSlotFile();

	/**
	 * Deletes the slot file with the given name.
	 * @param SlotName The name of the slot to delete.
	 * @return True if the slot file was deleted, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static bool DeleteSlotFile(const FString& SlotName);

	/**
	 * Deletes all the slot files.
	 */
	UFUNCTION(BlueprintCallable, Category = "Save System")
	static void DeleteAllSlotFiles();

	/**
	 * Checks if a slot file with the given name exists.
	 * @param SlotName The name of the slot to check.
	 * @return True if the slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesSlotFileExist(const FString& SlotName);

	/**
	 * Checks if any slot file exists.
	 * @return True if any slot file exists, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool DoesAnySaveFileExist();

	/**
	 * Checks if the selected slot is valid.
	 * @return True if the selected slot is valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSelectedSlotValid();

	/**
	 * Checks if the given slot name is valid.
	 * @param SlotName The name of the slot to check.
	 * @return True if the slot name is valid, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static bool IsSaveSlotNameValid(const FString& SlotName);

	/**
	 * Gets the number of save slots.
	 * @return The number of save slots.
	 */
	UFUNCTION(BlueprintPure, Category = "Save System")
	static int32 GetNumberOfSaveSlots();

	static bool CreateAutoSaveSlotFile(FString& SlotName);
	static void SaveSlotData(FSaveSlotData& SlotData);
	static void SaveSlotsInfo();
	static void LoadSlotsInfo();
};
