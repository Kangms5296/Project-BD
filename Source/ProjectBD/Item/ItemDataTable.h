// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Unknown = 0		UMETA(Display = "Unknown"),
	Consume = 1		UMETA(Display = "Consume"),
	Equip = 2		UMETA(Display = "Equip"),
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTBD_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ItemFloat01;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UTexture2D> ItemThumnail;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UStaticMesh>ItemMesh;
};
