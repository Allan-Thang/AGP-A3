// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
#ifdef ADVGAMESPROGRAMMING_WeaponPickup_generated_h
#error "WeaponPickup.generated.h already included, missing '#pragma once' in WeaponPickup.h"
#endif
#define ADVGAMESPROGRAMMING_WeaponPickup_generated_h

#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_SPARSE_DATA
#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnGenerate);


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnGenerate);


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_EVENT_PARMS \
	struct WeaponPickup_eventOnPickup_Parms \
	{ \
		AActor* ActorThatPickedUp; \
	};


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_CALLBACK_WRAPPERS
#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAWeaponPickup(); \
	friend struct Z_Construct_UClass_AWeaponPickup_Statics; \
public: \
	DECLARE_CLASS(AWeaponPickup, APickup, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/AdvGamesProgramming"), NO_API) \
	DECLARE_SERIALIZER(AWeaponPickup)


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_INCLASS \
private: \
	static void StaticRegisterNativesAWeaponPickup(); \
	friend struct Z_Construct_UClass_AWeaponPickup_Statics; \
public: \
	DECLARE_CLASS(AWeaponPickup, APickup, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/AdvGamesProgramming"), NO_API) \
	DECLARE_SERIALIZER(AWeaponPickup)


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AWeaponPickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AWeaponPickup) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AWeaponPickup); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AWeaponPickup); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AWeaponPickup(AWeaponPickup&&); \
	NO_API AWeaponPickup(const AWeaponPickup&); \
public:


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AWeaponPickup() { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AWeaponPickup(AWeaponPickup&&); \
	NO_API AWeaponPickup(const AWeaponPickup&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AWeaponPickup); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AWeaponPickup); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AWeaponPickup)


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_PRIVATE_PROPERTY_OFFSET
#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_19_PROLOG \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_EVENT_PARMS


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_PRIVATE_PROPERTY_OFFSET \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_SPARSE_DATA \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_RPC_WRAPPERS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_CALLBACK_WRAPPERS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_INCLASS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_PRIVATE_PROPERTY_OFFSET \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_SPARSE_DATA \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_RPC_WRAPPERS_NO_PURE_DECLS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_CALLBACK_WRAPPERS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_INCLASS_NO_PURE_DECLS \
	AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h_22_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ADVGAMESPROGRAMMING_API UClass* StaticClass<class AWeaponPickup>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID AGP_A3_Source_AdvGamesProgramming_WeaponPickup_h


#define FOREACH_ENUM_EWEAPONPICKUPRARITY(op) \
	op(EWeaponPickupRarity::LEGENDARY) \
	op(EWeaponPickupRarity::MASTER) \
	op(EWeaponPickupRarity::RARE) \
	op(EWeaponPickupRarity::COMMON) 

enum class EWeaponPickupRarity : uint8;
template<> ADVGAMESPROGRAMMING_API UEnum* StaticEnum<EWeaponPickupRarity>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
