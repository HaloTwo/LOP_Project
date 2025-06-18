// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "PlayerInputComponent.generated.h"


//UEnhancedInputComponent�� ��ӹ޾� �߰� ����� �����մϴ�.
UCLASS()
class LISEOFP_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputRelasedFunc);
};

template<class UserObject, typename CallbackFunc>
inline void UPlayerInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	// �Է� ���� �ּ��� ��ȿ���� Ȯ���ϰ�, �׷��� ������ ���� �޽����� �Բ� ���� �ߴ�
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

	// �����÷��� �±׸� ����Ͽ� �Է� �׼� ��ü�� ã��
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// ã�� �Է� �׼��� ������ Ʈ���� �̺�Ʈ, ���ؽ�Ʈ ��ü, �ݹ� �Լ��� �Բ� ���ε�
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template<class UserObject, typename CallbackFunc>
inline void UPlayerInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputRelasedFunc)
{
	// �Է� ���� �ּ��� ��ȿ���� Ȯ���ϰ�, �׷��� ������ ���� �޽����� �Բ� ���� �ߴ�
	checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

	for (const auto& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsVaild()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputRelasedFunc, AbilityInputActionConfig.InputTag);
	}
}
