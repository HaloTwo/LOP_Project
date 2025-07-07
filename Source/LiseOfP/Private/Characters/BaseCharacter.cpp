// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "AbilitySystem/LOP_AbilitySystemComponent.h"
#include "AbilitySystem/LOP_AttributeSet.h"

// ������
ABaseCharacter::ABaseCharacter()
{
	// �⺻ ����
	PrimaryActorTick.bCanEverTick = false;

	//�켱 ��Ƽ �����ϱ� ���
	bReplicates = false;
	SetReplicateMovement(false);

	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent = CreateDefaultSubobject<ULOP_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<ULOP_AttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
}


void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this); 
	}


	// Possessed�� �Ŀ� �ʿ��� �ʱ�ȭ �۾��� ���⿡ �߰��� �� �ֽ��ϴ�.
}


