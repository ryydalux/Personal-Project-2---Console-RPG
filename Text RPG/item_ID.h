#pragma once

enum class itemID //Stores integer ID for every item in the game.
{
	None = 0,

	//Physical Weapons ----------------------------------------------------- 1 - 99

	RustyDagger = 1,
	RustySword,

	IronDagger,
	IronSword,
	IronGreatSword ,

	SteelDagger,
	SteelSword,
	SteelGreatSword,




	//Magical Weapons ----------------------------------------------------- 100 - 299

	WoodenStaff = 100,
	IronRod,
	SteelRod,


	//Unique Weapons ----------------------------------------------------- 200 - 299

	VladimirScepter = 200, //spellVamp (epic)
	SwordBloodthirst, //lifesteal (rare)
	JuggernautGreatsword,
	RogueDagger,
	ScholarRod,
	UnbrokenGreatSword,
	UnbrokenSword,
	ElderRod,
	WandererGreatSword,

	//Shields ----------------------------------------------------- 300 - 399

	LeatherShield = 300,
	IronShield = 301,
	SteelShield,
	ThornrootShield,

	//Unique Shields ----------------------------------------------------- 400 - 499

	UnbrokenShield,

	//Medium and Heavy Armor ----------------------------------------------------- 500 - 799

	LeatherHelmet = 500,
	LeatherShoulder = 501,
	LeatherChest = 502,
	LeatherArms = 503,
	LeatherLegs = 504,
	LeatherFeet = 505,

	RogueHelmet,
	RogueShoulder,
	RogueChest,
	RogueArms,
	RogueLegs,
	RogueFeet,

	IronHelmet,
	IronShoulder,
	IronChest,
	IronArms,
	IronLegs,
	IronFeet,

	ScaleHelmet,
	ScaleShoulder,
	ScaleChest,
	ScaleArms,
	ScaleLegs,
	ScaleFeet,

	SteelHelmet,
	SteelShoulder,
	SteelChest,
	SteelArms,
	SteelLegs,
	SteelFeet,

	JuggernautHelmet,
	JuggernautShoulder,
	JuggernautChest,
	JuggernautArms,
	JuggernautLegs,
	JuggernautFeet,

	UnbrokenHelmet,
	UnbrokenShoulder,
	UnbrokenChest,
	UnbrokenArms,
	UnbrokenLegs,
	UnbrokenFeet,

	UnseenHelmet,
	UnseenShoulder,
	UnseenChest,
	UnseenArms,
	UnseenLegs,
	UnseenFeet,

	ThornrootHelmet,
	ThornrootShoulder,
	ThornrootChest,
	ThornrootArms,
	ThornrootLegs,
	ThornrootFeet,

	//Light Armor ----------------------------------------------------- 800 - 899

	ClothHelmet = 800,
	ClothShoulder,
	ClothChest,
	ClothArms,
	ClothLegs,
	ClothFeet,

	SilkHelmet, 
	SilkShoulder,
	SilkChest,
	SilkArms,
	SilkLegs,
	SilkFeet,

	LinenHelmet,
	LinenShoulder,
	LinenChest,
	LinenArms,
	LinenLegs,
	LinenFeet,

	ScholarHelmet,
	ScholarShoulder,
	ScholarChest, 
	ScholarArms,
	ScholarLegs,
	ScholarFeet,

	ElderHelmet,
	ElderShoulder,
	ElderChest,
	ElderArms,
	ElderLegs,
	ElderFeet,


	//Unique Armor ----------------------------------------------------- 900 - 999

	//Capes ----------------------------------------------------- 1000 - 1199

	LeatherCape = 1000,
	ReinforcedLeatherCape,
	AdventurerCape,
	ShadowCloak,

	SpellWeaverCape,
	OverlordCape,
	UnseenCape,

	//Rings ----------------------------------------------------- 1200 - 1299

	//Commons
	RingHealth = 1200,
	RingDefense,
	RingMana,
	RingWillpower,
	RingStrength,
	RingMagicResistance,

	//Uncommons
	RingWarden,
	RingRessource,
	RingMight, //strength +3
	RingFortitude, //hp +15
	RingEnergy, //mana +9
	RingProtection, //+2 defense
	RingAccuracy,
	RingSorcery,
	RingImmunity,

	//Rare
	ScholarRing,
	JuggernautRing,
	RogueRing,
	ThornsRing,

	//Legendary
	UnbrokenRing,
	ElderRing,
	UnseenRing,

	//Necklaces ----------------------------------------------------- 1300 - 1399

	NecklaceVersatility = 1300,
	NecklaceRuby,
	NecklaceSapphire,
	NecklaceThorns,
	NecklaceUnbroken,
	NecklaceElder,

	//Potions ----------------------------------------------------- 1400 - 1599

	//Foods ----------------------------------------------------- 1600 - 1699

	//hp only
	Bread = 1600,
	Cheese,
	BoiledEgg,
	CookedMeat, 
	HotBroth,
	HealthyStew,
	BakedPotato,

	//hp and mana
	Apple,
	Pear, 
	Mango, 
	Kiwi, 
	DragonFruit, 
	Pomegranate, 

	
	//mana only
	Water, 
	Milk,
	Juice,

	//SpellBooks Phsyical ----------------------------------------------------- 1700 - 1899

	SB_HeavyStrike = 1700,	
	SB_PreciseStrike,
	SB_UltimateStrike,
	SB_MomentumStrike,
	SB_DizzyingBlow,
	

	//SpellBooks Magical ----------------------------------------------------- 1900 - 1999

	SB_BlindingLights = 1900,
	SB_Fire,
	SB_Heal,
	SB_Renew,
	SB_GreaterFire,
	SB_GreaterHeal,
	SB_Transfusion,

	//SpellBooks Buffs/Debuffs 2000 - 2099

	SB_Guarded = 2000,
	SB_SpiritShield,
	SB_AmbitiousBlade,
	SB_Perseverence,
	SB_Opportunity,
	SB_CurePoison,
	SB_InnerFocus,
	SB_EverlastingWill,
	SB_Vampirism,
	SB_Invisibility, 
	SB_Resilience,
	SB_EmpoweredMind,
	SB_Berserk,

	


	

	//Mob Loot ----------------------------------------------------- 2100-2499

	RatTail = 2100,
	BoarSnout = 2101,
	WolfPelt,
	BrokenDagger,
	BoneAshes,
	RottenFlesh,
	GhostEssence,
	BundleSilk,
	MossyGoo,
	Thornroot,
	GiantFeather,

	//Potions ----------------------------------------------------- 2500-2999

	//Health
	HPPhial = 2500,
	HPPot,
	HPTonic,

	//Mana
	MPPhial,
	MPPot,
	MPTonic,

	//Strength 
	StrPhial,
	StrPot,
	StrTonic,

	//SpellPower
	SpellPowerPhial,
	SpellPowerPot,
	SpellPowerTonic,

	//Thorns
	ThornsPhial,
	ThornsPot,
	ThornsTonic,

	//HP Regen
	HPRegenPhial,
	HPRegenPot,
	HPRegenTonic,

	//MP Regen
	MPRegenPhial,
	MPRegenPot,
	MPRegenTonic,


	//SpellBooks Non phys/magic 3000+
	SB_FirstAid = 3000,
	SB_Meditate,


};