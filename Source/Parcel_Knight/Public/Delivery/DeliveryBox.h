#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DeliveryTypes.h"
#include "Delivery/Carryable.h"
#include "DeliveryBox.generated.h"

class UStaticMeshComponent;

UCLASS()
class PARCEL_KNIGHT_API ADeliveryBox : public AActor, public ICarryable
{
	GENERATED_BODY()
	
public:	
	ADeliveryBox();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> BoxMesh;

    /** 매니저 서브시스템에서 발급받을 고유 인덱스 ID (Replicated) */
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Delivery Box")
    int32 BoxID;

    /** 현재 박스가 가진 모든 상태 태그 */
    UPROPERTY(ReplicatedUsing = OnRep_BoxStateTags, VisibleAnywhere, BlueprintReadOnly, Category = "Delivery Box")
    FGameplayTagContainer BoxStateTags;

    /** 초기화 시 데이터 테이블에서 받아온 박스의 고유 데이터 */
    UPROPERTY(ReplicatedUsing = OnRep_BoxData, EditAnywhere, BlueprintReadOnly, Category = "Delivery Box")
    FBoxData BoxData;

    /** 현재 이 상자를 들고 있는 플레이어 컨트롤러*/
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Delivery Box")
    TObjectPtr<APlayerController> HolderPlayer;

    /** 태그 변경 시 클라이언트 연출용 RepNotify */
    UFUNCTION()
    void OnRep_BoxStateTags();

    /** 데이터 설정 시 클라이언트 Mesh 동기화용 RepNotify */
    UFUNCTION()
    void OnRep_BoxData();
	
	//Todo : 물리적 충돌

public:
    /** 서버에서 안전하게 상태 태그를 제어하는 권한 전용 함수들 */
    UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Delivery Box")
    void AddStateTag(FGameplayTag NewStateTag);

    UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Delivery Box")
    void RemoveStateTag(FGameplayTag StateTag);

    UFUNCTION(BlueprintPure, Category = "Delivery Box")
    bool HasStateTag(FGameplayTag StateTag) const;

    /** [핵심] 서브시스템이 스폰 직후 서버에서 박스를 초기화할 때 호출 */
    void InitializeBox(int32 InBoxID, const FBoxData& InBoxData);

    /** ICarryable 인터페이스 구현 상속 */
    virtual bool CanCarry(AActor* Carrier) override;
    virtual void OnPickedUp(AActor* Carrier) override;
    virtual void OnDropped() override;

    /** 게터(Getter) 모음 */
    FORCEINLINE int32 GetBoxID() const { return BoxID; }
    FORCEINLINE FGameplayTagContainer GetBoxStateTags() const { return BoxStateTags; }
    FORCEINLINE FBoxData GetBoxData() const { return BoxData; }
    FORCEINLINE float GetDamageThreshold() const { return BoxData.DamageThreshold; }

};
