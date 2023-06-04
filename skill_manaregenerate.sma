#include <amxmodx>
#include <engine>
#include <d2lod>
#include <hamsandwich>

new PLUGIN_NAME[] = "魔力吸收"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 24;
new Skill_Allocate = 10;

new const ManaRegenerate[MAX_P_SKILLS] = 
{
	14, 15, 16, 17, 18, 19, 20, 25, 26, 30
};

new g_SkillId;

new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	RegisterHam(Ham_TakeDamage, "func_wall", "fwd_PlayerDamagedMonster");
	g_SkillId = register_d2_skill(PLUGIN_NAME, "攻擊時回復魔力.", AMAZON, Skill_Level, Skill_Allocate, NOT_DISPLAY)
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id
}

public d2_skill_fired(id)
{

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		
	}
}
public d2_takedamage(victim, attacker, Float:iDamage[1])
{
}
public d2_ranged_takedamage(victim, attacker, Float:iDamage[1])
{
}

public fwd_PlayerDamagedMonster(victim, inflictor, attacker, Float:damage, damagebits)
{
	if ( is_user_alive(attacker) && get_p_hero(attacker) == AMAZON && get_p_skill( attacker, g_SkillId ) > 0 )
	{
		set_p_mana(attacker, get_p_mana(attacker) + ManaRegenerate[ get_p_skill( attacker, g_SkillId ) - 1 ] );
	}
}