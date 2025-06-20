// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CharacterAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * �ִϸ��̼� �ʱ�ȭ �Լ�
 *
 * �𸮾� ������ �ִϸ��̼� �ν��Ͻ��� ������ �� �ڵ����� ȣ���ϴ� �Լ��Դϴ�.
 * �� �Լ������� �ִϸ��̼� �ν��Ͻ��� ������ ��(Pawn)�� ã�� �����մϴ�.
 */
void UCharacterAnimInstance::NativeInitializeAnimation()
{
    // TryGetPawnOwner()�� ���� �� �ִϸ��̼� �ν��Ͻ��� ����ϴ� ���� ��������
    // WarriorBaseCharacter Ÿ������ ĳ�����մϴ�.
    OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

    // ���� ĳ���Ͱ� ��ȿ�� ��� �߰� �ʱ�ȭ�� �����մϴ�.
    if (OwningCharacter)
    {
        // CharacterMovement ������Ʈ�� �����ɴϴ�.
        // ����: �� ������� ������ �ʰ� �ֽ��ϴ�. �̴� �̿ϼ� �ڵ峪 �Ǽ��� �� �ֽ��ϴ�.
        // �Ϲ������� �� ���� ��� ������ �����ϰų� �ʱ�ȭ�� ����մϴ�.
        owningMovementComponent = OwningCharacter->GetCharacterMovement();
    }
}

/**
 * ������ ���� �ִϸ��̼� ������Ʈ �Լ�
 *
 * �� �����Ӹ��� �ִϸ��̼��� ������Ʈ�ϱ� ���� ȣ��˴ϴ�.
 * �� �Լ��� ������ �����ϰ� ����Ǿ�� �ϸ�, ���� �����尡 �ƴ�
 * �ִϸ��̼� �����忡�� ����� �� �ֽ��ϴ�.
 *
 * @param DeltaSeconds - ������ ������ ���� ����� �ð�(��)
 */
void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    // �Ϲ������� ���⼭�� ĳ������ �ӵ�, ����, ���� ���� Ȯ���Ͽ�
    // �ִϸ��̼� �������� ������Ʈ�մϴ�.

    //ĳ���Ͱ� ���ų�, ���� ������Ʈ�� ���ٸ� return
    if (!OwningCharacter || !owningMovementComponent)
    {
        return;
    }

    GroundSpeed = OwningCharacter->GetVelocity().Size2D();
    bHasAcceleration = owningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}