// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class ULOP_AbilitySystemComponent;
class ULOP_AttributeSet;

UCLASS()
class LISEOFP_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	// AbilitySystemComponent interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// AbilitySystemComponent interface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	ULOP_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	ULOP_AttributeSet* AttributeSet;



	virtual void PossessedBy(AController* NewController) override;

public:
	//�����Ƽ �ý��� ������Ʈ�� �������� �Լ�
	FORCEINLINE ULOP_AbilitySystemComponent* GetBaseAbilitySystemComponent() const { return AbilitySystemComponent; }

	//��Ʈ����Ʈ ���� �������� �Լ�
	FORCEINLINE ULOP_AttributeSet* GetBaseAttributeSet() const { return AttributeSet; }
};
