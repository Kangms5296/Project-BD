// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "InventorySlotWidgetBase.h"
#include "WidgetHeaderBase.h"
#include "../../Item/MasterItem.h"
#include "../../JsonHelper.h"
#include "../../BDGameInstance.h"
#include "../../Battle/BattleGM.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "kismet/GameplayStatics.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Slot ����
	ItemSlots = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ItemSlots")));
	if (ItemSlots)
	{
		for (int Row = 0; Row < Rows; ++Row)
		{
			for (int Col = 0; Col < Cols; ++Col)
			{
				// Slot ����.
				UInventorySlotWidgetBase* CurSlot = CreateWidget<UInventorySlotWidgetBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0), InventorySlotWidgetClass);
				CurSlot->RowIndex = Row;
				CurSlot->ColIndex = Col;
				CurSlot->SetOwner(this);
				Slots.Add(CurSlot);

				// Slot ȭ�鿡 ǥ��.
				UUniformGridSlot* GridSlot = ItemSlots->AddChildToUniformGrid(CurSlot, Row, Col);
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}
	}

	Gold = Cast<UTextBlock>(GetWidgetFromName(TEXT("Gold")));
	if (Gold)
	{
		FString Temp = "0";
		Gold->SetText(FText::FromString(Temp));
	}

	InventoryHeader = Cast<UWidgetHeaderBase>(GetWidgetFromName(TEXT("InventoryHeader")));
	{
		InventoryHeader->SetOwnerWidget(this);
	}
}

bool UInventoryWidgetBase::AddItem(FItemDataTable ItemData, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->CurrentItem.ItemIndex == ItemData.ItemIndex)
		{
			bool Result = Slots[i]->AddCount(Count);
			if (Result)
			{
				// ���� ���Կ� ���������� ������ ����ŭ �߰�
				return true;
			}
		}
	}

	int EmptyRow;
	int EmptyCol;
	if (GetEmptySlotIndex(EmptyRow, EmptyCol))
	{
		bool Result = SetSlot(EmptyRow, EmptyCol, ItemData, Count);
		if (Result)
		{
			// ���ο� ���Կ� ���������� ������ ����ŭ �߰�
			return true;
		}
	}

	// ���� ����, �� ���Կ� �߰��� �� ����
	return false;
}

bool UInventoryWidgetBase::SubItem(FItemDataTable ItemData, int Count)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->IsUsing && Slots[i]->CurrentItem.ItemIndex == ItemData.ItemIndex)
		{
			bool Result = Slots[i]->SubCount(Count);
			if (Result)
			{
				// ���� ���Կ� ���������� ������ ����ŭ ����
				return true;
			}
		}
	}

	// ������ �������� ���� ���
	return false;
}

void UInventoryWidgetBase::ClearInventory()
{
	for (UInventorySlotWidgetBase* MySlot : Slots)
	{
		MySlot->ResetSlot();
	}
}

bool UInventoryWidgetBase::SetSlot(int Row, int Col, FItemDataTable ItemData, int Count)
{
	bool Result = Slots[GetSlotIndex(Row, Col)]->SetSlot(ItemData, Count);

	return Result;
}

void UInventoryWidgetBase::ResetSlot(int Row, int Col)
{
	Slots[GetSlotIndex(Row, Col)]->ResetSlot();
}

bool UInventoryWidgetBase::GetEmptySlotIndex(int& EmptyRow, int& EmptyCol)
{
	for (UInventorySlotWidgetBase* MySlot : Slots)
	{
		if (false == MySlot->IsUsing)
		{
			EmptyRow = MySlot->RowIndex;
			EmptyCol = MySlot->ColIndex;
			return true;
		}
	}

	return false;
}

int UInventoryWidgetBase::GetSlotIndex(int Row, int Col)
{
	return Row * Cols + Col;
}

void UInventoryWidgetBase::LoadDatasFromFile(FString SavedPath)
{
	UBDGameInstance* BDInstance = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (BDInstance)
	{
		FString OutputString = BDInstance->GetJsonHelper()->LoadFromFile(SavedPath);
		auto JsonArr = BDInstance->GetJsonHelper()->GetArrayField(OutputString, "Items");

		TSharedPtr<FJsonObject> TempObject;
		for (int i = 0; i < JsonArr.Num(); i++)
		{
			FString _ItemIndex = JsonArr[i]->AsObject()->GetStringField("ItemIndex");
			FString _ItemCount = JsonArr[i]->AsObject()->GetStringField("ItemCount");

			ABattleGM* GM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GM)
			{
				FItemDataTable NewItem = GM->GetItemData(FCString::Atoi(*_ItemIndex));
				AddItem(NewItem, FCString::Atoi(*_ItemCount));
			}
		}
	}
}

void UInventoryWidgetBase::SaveDatasToFile(FString SavePath)
{
	UBDGameInstance* BDInstance = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (BDInstance)
	{
		// Make JsonObject Array
		TArray<TSharedPtr<FJsonValue>> JsonArr;
		for (int i = 0; i < Slots.Num(); i++)
		{
			if (Slots[i]->IsUsing)
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetStringField("ItemIndex", FString::FromInt(Slots[i]->CurrentItem.ItemIndex));
				JsonObject->SetStringField("ItemCount", FString::FromInt(Slots[i]->ItemCount));
				TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObject));

				JsonArr.Add(JsonValue);
			}
		}

		// JsonArray To String
		TSharedPtr<class FJsonObject> JsonStr;
		BDInstance->GetJsonHelper()->StartMake(JsonStr);
		BDInstance->GetJsonHelper()->AddArrayField(JsonStr, "Items", JsonArr);
		FString OutputString = BDInstance->GetJsonHelper()->EndMake(JsonStr);

		// Save File
		BDInstance->GetJsonHelper()->SaveToFile(OutputString, SavePath);
	}
}

