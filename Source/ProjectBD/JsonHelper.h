// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonHelper.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UJsonHelper : public UObject
{
	GENERATED_BODY()
	
public:
	void SaveToFile(FString Json, FString Path); // File에 새로 JSON 저장.
	void SaveToFile_Append(FString Json, FString Path); // File 작성 내용 뒤로 Json을 추가. 
	FString LoadFromFile(FString Path); // File에서 JSON 캐싱

	void StartMake(TSharedPtr<class FJsonObject>& JsonStr); // 새로운 JSON 시작.
	void AddStringField(TSharedPtr<class FJsonObject>& JsonStr, FString Key, FString Value); // String Field 추가.
	void AddArrayField(TSharedPtr<class FJsonObject>& JsonStr, FString Key, TArray<class TSharedPtr<class FJsonValue>> Value); // Array Field 추가.
	FString EndMake(TSharedPtr<class FJsonObject>& JsonStr); // 생성한 JSON 반환.

	FString GetStringField(FString Json, FString Key); // 해당 Json에서, Key에 해당하는 String 값 반환
	TArray<class TSharedPtr<class FJsonValue>> GetArrayField(FString Json, FString Key); // 해당 Json에서, Key에 해당하는 Array 값 반환
};
