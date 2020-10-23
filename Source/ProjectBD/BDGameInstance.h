// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/ItemDataTable.h"
#include "BDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UBDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FString GetUserID();
	void SetUserID(FString NewUserID);

	class UJsonHelper* GetJsonHelper();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable)
	FItemDataTable GetItemData(int Index) const;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserID;

	UPROPERTY()
	class UJsonHelper* JsonHelper;
};
