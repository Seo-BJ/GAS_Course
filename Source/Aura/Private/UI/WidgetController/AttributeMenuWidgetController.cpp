


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
//#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& AuraAttributeInfo : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeInfo.AttributeGetter).AddLambda(
			[this, AuraAttributeInfo](const FOnAttributeChangeData& Data)
			{
				BraodcastAttribueInfo(AuraAttributeInfo.AttributeTag);
			}
		);
	}
}

void UAttributeMenuWidgetController::BraodcastAttribueInfo(const FGameplayTag& Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& AuraAttributeInfo : AttributeInfo.Get()->AttributeInformation)
	{
		BraodcastAttribueInfo(AuraAttributeInfo.AttributeTag);
	}
}