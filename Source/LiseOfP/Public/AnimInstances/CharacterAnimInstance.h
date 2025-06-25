// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class LISEOFP_API UCharacterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	

public:

	//�ִϸ��̼� �ν��Ͻ��� ������ �� ȣ��Ǹ�, �ʿ��� �ʱ� ������ �����մϴ�.
	//UAnimInstance Ŭ������ ������ �Լ��� �������̵��մϴ�.
	virtual void NativeInitializeAnimation() override;

	//�� �����Ӹ��� ȣ��Ǿ� �ִϸ��̼� ���¸� ������Ʈ�մϴ�.
	//�� �Լ��� ������ �����忡�� �����ϰ� ����� �� �ֵ��� ����Ǿ�� �մϴ�.
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

protected:

	UPROPERTY()
	ABaseCharacter* OwningCharacter;

	//ĳ������ �������� ����
	UPROPERTY()
	UCharacterMovementComponent* owningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;
};
