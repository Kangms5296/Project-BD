// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "LobbyPC.h"
#include "LobbyGS.h"
#include "LobbyGM.h"
#include "../BDGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"



void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SB_ChatBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("SB_ChatBox")));
	ETB_ChatInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ETB_ChatInput")));
	if (ETB_ChatInput)
	{
		ETB_ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommited);
	}

	T_ConnectCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("T_ConnectCount")));
	T_LeftTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("T_LeftTime")));

	B_StartGameButton = Cast<UButton>(GetWidgetFromName(TEXT("B_StartGameButton")));
	if (B_StartGameButton)
	{
		B_StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnClickStartButton);
		B_StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULobbyWidgetBase::OnClickStartButton()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidgetBase::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		if (ETB_ChatInput->GetText().ToString() != "")
		{
			ALobbyPC* PC = GetOwningPlayer<ALobbyPC>();
			if (PC)
			{
				UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI)
				{
					FString Temp = FString::Printf(TEXT("%s : %s"), *GI->GetUserID(), *Text.ToString());

					PC->C2S_SendMessage(FText::FromString(Temp));
					ETB_ChatInput->SetText(FText::FromString(TEXT("")));
				}
			}
		}
	}
	break;
	case ETextCommit::OnCleared:
	{
		ETB_ChatInput->SetUserFocus(GetOwningPlayer());
	}
	break;
	}
}

void ULobbyWidgetBase::AddMessage(FText Message)
{
	if (SB_ChatBox)
	{
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(SB_ChatBox);
		if (NewTextBlock)
		{
			NewTextBlock->SetText(Message);
			NewTextBlock->Font.Size = 20;
			SB_ChatBox->AddChild(NewTextBlock);
			SB_ChatBox->ScrollToEnd();
		}
	}
}

void ULobbyWidgetBase::SetConnectCount(int NewConnectCount)
{
	FString Temp = FString::FromInt(NewConnectCount);
	if (T_ConnectCount)
	{
		T_ConnectCount->SetText(FText::FromString(Temp));
	}
}

void ULobbyWidgetBase::SetPublicMessage(int LeftTime)
{
	FString Temp = FString::FromInt(LeftTime);;
	if (T_LeftTime)
	{
		T_LeftTime->SetText(FText::FromString(Temp));
	}
}

void ULobbyWidgetBase::ShowStartGameButton()
{
	if (B_StartGameButton)
	{
		B_StartGameButton->SetVisibility(ESlateVisibility::Visible);
	}
}


