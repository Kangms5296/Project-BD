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
	Equip	= 2		UMETA(Display = "Equip"),
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	NotUse	= 0		UMETA(Display = "NotUse"),
	Weapon	= 1		UMETA(Display = "Weapon"),
	Body	= 2		UMETA(Display = "Body"),
	Head	= 3		UMETA(Display = "Head"),
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
	EItemType ItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlotType EquipType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ItemDesc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EffectDesc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int iValue1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int iValue2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float fValue1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float fValue2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UTexture2D> ItemThumnail;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UStaticMesh> ItemStaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> ItemSkeletalMesh;
};
