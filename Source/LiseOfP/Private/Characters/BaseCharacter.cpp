// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

// 생성자
ABaseCharacter::ABaseCharacter()
{
	// 기본 설정
	PrimaryActorTick.bCanEverTick = false;

	//우선 멀티 없으니까 대기
	bReplicates = false;
	SetReplicateMovement(false);

	GetMesh()->bReceivesDecals = false;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	// Possessed된 후에 필요한 초기화 작업을 여기에 추가할 수 있습니다.
}


