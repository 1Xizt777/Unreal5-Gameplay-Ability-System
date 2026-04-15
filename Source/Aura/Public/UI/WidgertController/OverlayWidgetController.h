
#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"


class UAuraUserWidget;
struct FOnAttributeChangeData;

//自定义结构体
USTRUCT()
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag =  FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText  Message = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};


//只有一个参数，可以有三个（ NewValue (新数值), OldValue (老数值), GEModData (伤害来源等详细上下文)。）

		//DYNAMIC动态广播，蓝图可以看到
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSingature, float, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetDataSignature, FUIWidgetRow , Row);


UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	//重写
	virtual void BroadcastInitialValues() override;
	
	//重写
	virtual void BindCallbacksToDependencies() override;
	
	
	//实例化这个委托同时允许蓝图Assign
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Attributes")
	FOnAttributeChangedSingature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable , Category="GAS | Messages")
	FMessageWidgetDataSignature MessageWidgetDataDelegate;
	
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{	//Return   DataTable
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
