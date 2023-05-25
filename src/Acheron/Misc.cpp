#include "Acheron/Misc.h"

namespace Acheron
{
	bool HasBeneficialPotion(RE::TESObjectREFR* a_container)
	{
		using Flag = RE::EffectSetting::EffectSettingData::Flag;

		const auto inventory = a_container->GetInventory();
		for (const auto& [form, data] : inventory) {
			if (data.first <= 0 || !form->Is(RE::FormType::AlchemyItem) || data.second->IsQuestObject())
				continue;

			const auto potion = form->As<RE::AlchemyItem>();
			if (potion->IsFood())
				continue;

			bool allow = false;
			for (auto&& eff : potion->effects) {
				auto base = eff ? eff->baseEffect : nullptr;
				if (!base)
					continue;

				const auto& eff_data = base->data;
				if (eff_data.flags.any(Flag::kDetrimental, Flag::kHostile)) {
					allow = false;
					break;
				}
				if (eff_data.flags.none(Flag::kRecover) && (eff_data.primaryAV == RE::ActorValue::kHealth || eff_data.secondaryAV == RE::ActorValue::kHealth)) {
					allow = true;
				}
			}
			if (allow) {
				return true;
			}
		}
		return false;
	}

	float GetAVPercent(RE::Actor* a_actor, RE::ActorValue a_av)
	{
		float tempAV = a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, a_av);
		float totalAV = a_actor->GetPermanentActorValue(a_av) + tempAV;
		float currentAV = a_actor->GetActorValue(a_av);
		return currentAV / totalAV;
	}

	std::vector<RE::TESObjectARMO*> GetWornArmor(RE::Actor* a_actor, uint32_t a_ignoredmasks)
	{
		std::vector<RE::TESObjectARMO*> sol;
		auto inventory = a_actor->GetInventory();
		for (const auto& [form, data] : inventory) {
			if (!data.second->IsWorn() || !form->IsArmor() || !form->GetPlayable() || form->GetName()[0] == '\0') {
				continue;
			}
			const auto item = data.second.get()->GetObject()->As<RE::TESObjectARMO>();
			if (a_ignoredmasks) {
				const auto slots = static_cast<uint32_t>(item->GetSlotMask());
				// sort out items which have no enabled slots (dont throw out if at least 1 slot matches)
				if ((slots & a_ignoredmasks) == 0)
					continue;
			}
			sol.push_back(item);
		}
		return sol;
	}

	std::vector<RE::Actor*> GetFollowers()
	{
		std::vector<RE::Actor*> ret;
		const auto processLists = RE::ProcessLists::GetSingleton();
		for (auto& actorHandle : processLists->highActorHandles) {
			if (auto actor = actorHandle.get(); actor && actor->IsPlayerTeammate()) {
				ret.push_back(actor.get());
			}
		}
		return ret;
	}

	RE::TESActorBase* GetLeveledActorBase(RE::Actor* a_actor)
	{
		const auto base = a_actor->GetTemplateActorBase();
		return base ? base : a_actor->GetActorBase();
	}

	bool IsHunter(RE::Actor* a_actor)
	{
		return a_actor->HasMagicEffect(GameForms::HunterPrideEffect);
	}

	bool UsesHunterPride(const RE::Actor* a_actor)
	{
		const auto target = a_actor->IsCommandedActor() ? a_actor->GetCommandingActor().get() : a_actor;
		return target->IsPlayerRef() || Settings::bHunterPrideFollower && target->IsPlayerTeammate();
	}

	bool IsNPC(const RE::Actor* a_actor)
	{
		static const auto ActorTypeNPC = RE::TESForm::LookupByID<RE::BGSKeyword>(0x13794);
		return a_actor->HasKeyword(ActorTypeNPC);
	}

	bool IsDaedric(const RE::TESForm* a_form)
	{
		const auto k = a_form->As<RE::BGSKeywordForm>();
		return k ? k->HasKeywordID(0xA8668) : false;	// DaedricArtifact
	}


	// c @ Fenix31415
	// Probably someone will find it useful.Didn't find it in clib. to call it :
	// 	auto idle = RE::TESForm::LookupByID<RE::TESIdleForm>(0x6440c);
	// PlayIdle(attacker->currentProcess, attacker, RE::DEFAULT_OBJECT::kActionIdle, idle, true, false, victim);
	bool PlayIdle(RE::AIProcess* proc, RE::Actor* attacker, RE::DEFAULT_OBJECT smth, RE::TESIdleForm* idle, bool a5, bool a6, RE::TESObjectREFR* target)
	{
		using func_t = decltype(&PlayIdle);
		REL::Relocation<func_t> func{ RELID(38290, 38290) };  // TODO: AE RELID
		return func(proc, attacker, smth, idle, a5, a6, target);
	}

}  // namespace Acheron
