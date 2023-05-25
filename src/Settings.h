#pragma once

struct Settings
{
	enum FollowerRecovery : uint8_t
	{
		Never = 0,
		Health = 1,
		Unload = 2
	};

	static void Initialize();
	static void Save();

	// Processing
	static inline bool ProcessingEnabled{ true };
	static inline bool ConsequenceEnabled{ true };
	static inline bool bCreatureDefeat{ false };

	// Hunter Pride
	static inline int iHunterPrideKey{ -1 };					// hotkey to enable/disable hunter pride
	static inline int iHunterPrideKeyMod{ -1 };				// modifier key for hunter pride activaition
	static inline bool bHunterPrideFollower{ true };	// follower may onle defeat while hunter pride is active

	// Notifications
	static inline bool bNotifyDefeat{ false };						// notify when a NPC gets knocked down
	static inline bool bNotifyDestroy{ false };						// notify when item gets destroyed
	static inline bool bNotifyColored{ false };						// colored notify
	static inline std::string rNotifyColor{ "#FF0000" };	// color code

	// Knockdown - General
	static inline float fMidCombatBlackout{ 10.0f };	// Chance to blackout upon defeat (player only)
	static inline bool bPlayerDefeat{ true };					// if the player may be defeated
	static inline bool bNPCDefeat{ true };						// if NPC may be defeated

	// Passive Recovery
	static inline float fKdHealthThresh{ 0.20f };	 // Player: Minimum health required to recover from defeat status
	static inline int iKdFallbackTimer{ 90 };			 // Player: maximum time the player can stay defeated mid combat, independent of health
	static inline bool bKdFollowerUnload{ true };	 // Follower: Recover on unload

	// Lethal
	static inline bool bLethalEssential{ true };	// defeat essential NPC when they enter bleedout?
	static inline float fLethalPlayer{ 100.0f };	// prevent player death  chance
	static inline float fLethalNPC{ 100.0f };			// prevent NPC death chance

	// Trauma
	static inline float bTraumeEnabled{ false };	// Defeated when staggered
	static inline bool bTraumaHealth{ true };			// Consider current health
	static inline float fTraumaMult{ 32.0f };

	// Exposed
	static inline int iStrips{ 1'066'390'941 };	 // Slots to recognize
	static inline int iExposed{ 2 };						 // Min amount of slots equipped to NOT be exposed
	static inline float fExposedChance{ 0.0f };	 // Chance for an exposed strike to defeat

	// Stripping
	static inline float fStripChance{ 0.0f };		// chance to get stripped
	static inline float fStripDestroy{ 5.0f };	// Chance for stripped item to be destroyed
	static inline bool bStripDrop{ false };			// should non destroyed items be dropped

	// Table for dynamic access
	struct StringCmp
	{
		bool operator()(const std::string& a_lhs, const std::string& a_rhs) const
		{
			return _strcmpi(a_lhs.c_str(), a_rhs.c_str()) < 0;
		}
	};

	enum class VariantType
	{
		FLOAT = 0,
		STRING = 1,
		BOOL = 2,
		INT = 3
	};

#define ENTRY(var) { #var##s, &var }
	static inline std::map<std::string, std::variant<float*, std::string*, bool*, int*>, StringCmp> table{
		ENTRY(ProcessingEnabled),
		ENTRY(ConsequenceEnabled),
		ENTRY(bCreatureDefeat),
		ENTRY(iHunterPrideKey),
		ENTRY(iHunterPrideKeyMod),
		ENTRY(bHunterPrideFollower),
		ENTRY(bNotifyDefeat),
		ENTRY(bNotifyDestroy),
		ENTRY(bNotifyColored),
		ENTRY(rNotifyColor),
		ENTRY(bPlayerDefeat),
		ENTRY(bNPCDefeat),
		ENTRY(fMidCombatBlackout),
		ENTRY(bLethalEssential),
		ENTRY(fLethalPlayer),
		ENTRY(fLethalNPC),
		ENTRY(iStrips),
		ENTRY(iExposed),
		ENTRY(fExposedChance),
		ENTRY(fStripChance),
		ENTRY(fStripDestroy),
		ENTRY(bStripDrop),
		ENTRY(fKdHealthThresh),
		ENTRY(iKdFallbackTimer),
		ENTRY(bKdFollowerUnload)
	};
};
