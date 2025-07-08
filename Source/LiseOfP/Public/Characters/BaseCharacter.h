// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class ULOP_AbilitySystemComponent;
class ULOP_AttributeSet;

UCLASS()
// 모든 캐릭터의 기본 클래스
class LISEOFP_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

// 기본 생성자
public:
	ABaseCharacter();

	// AbilitySystemComponent interface
// AbilitySystemComponent 반환
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// AbilitySystemComponent interface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
// GAS 컴포넌트
	ULOP_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
// 속성 세트
	ULOP_AttributeSet* AttributeSet;



// 컨트롤러가 빙의될 때 호출
	virtual void PossessedBy(AController* NewController) override;

public:
        // AbilitySystemComponent 접근 함수
	FORCEINLINE ULOP_AbilitySystemComponent* GetBaseAbilitySystemComponent() const { return AbilitySystemComponent; }

        // AttributeSet 접근 함수
	FORCEINLINE ULOP_AttributeSet* GetBaseAttributeSet() const { return AttributeSet; }
};
