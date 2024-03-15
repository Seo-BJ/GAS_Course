


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    for (auto InputAction : AbilityInputActions)
    {
        if (InputAction.InputAction && InputAction.InputTag == InputTag)
        {
            return InputAction.InputAction;
        }
    }
    if (bLogNotFound)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
    }
    return nullptr;
}
