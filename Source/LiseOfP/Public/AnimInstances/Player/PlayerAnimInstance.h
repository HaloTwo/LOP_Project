// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharacterAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
/**
 *
 */
UCLASS()
class LISEOFP_API UPlayerAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	//�ִϸ��̼� �ν��Ͻ��� ������ �� ȣ��Ǹ�, �ʿ��� �ʱ� ������ �����մϴ�.
	virtual void NativeInitializeAnimation() override;

	//�� �����Ӹ��� ȣ��Ǿ� �ִϸ��̼� ���¸� ������Ʈ�մϴ�.
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
	APlayerCharacter* OwningPlayerCharacter;

	// ������ ���·� ��ȯ���� ���θ� ��Ÿ���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	// ������ ���·� ��ȯ�ϱ� ���� �Ӱ谪
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float  EnterRelaxtStateThreshold = 0.5f;

	// ���� �÷��̾ ������ ��������
	float IdleElpasedTime = 0.f;

	// ���� �÷��̾ �ȴ�	��������
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bWalkState;
};
