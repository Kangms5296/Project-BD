// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "TitlePC.h"
#include "../BDGameInstance.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidgetBase::NativeConstruct()
{
	T_UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("T_UserID")));
	T_Password = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("T_Password")));
	T_ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("T_ServerIP")));
	B_Connect = Cast<UButton>(GetWidgetFromName(TEXT("B_Connect")));
	B_Start = Cast<UButton>(GetWidgetFromName(TEXT("B_Start")));
	
	if (B_Connect)
	{
		B_Connect->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnConnectButton);
	}

	if (B_Start)
	{
		B_Start->OnClicked.AddDynamic(this, &UTitleWidgetBase::OnStartButton);
	}
}

void UTitleWidgetBase::OnConnectButton()
{
	//서버에 접속
	ATitlePC* PC = GetOwningPlayer<ATitlePC>();
	if (PC)
	{
		if (T_ServerIP)
		{
			SaveUserID();

			FString ServerIPAddress = T_ServerIP->GetText().ToString();

			PC->ConnectServer(ServerIPAddress);
		}
	}
}

void UTitleWidgetBase::OnStartButton()
{
	//서버를 시작
	ATitlePC* PC = GetOwningPlayer<ATitlePC>();
	if (PC)
	{
		SaveUserID();

		PC->StartServer();
	}
}

void UTitleWidgetBase::SaveUserID()
{
	UBDGameInstance* GI = Cast<UBDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		if (T_UserID)
		{
			GI->SetUserID(T_UserID->GetText().ToString());
		}
	}
}


