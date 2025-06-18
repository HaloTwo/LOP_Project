// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

// ������
ABaseCharacter::ABaseCharacter()
{
	// �⺻ ����
	PrimaryActorTick.bCanEverTick = false;

	//�켱 ��Ƽ �����ϱ� ���
	bReplicates = false;
	SetReplicateMovement(false);

	GetMesh()->bReceivesDecals = false;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	// Possessed�� �Ŀ� �ʿ��� �ʱ�ȭ �۾��� ���⿡ �߰��� �� �ֽ��ϴ�.
}


