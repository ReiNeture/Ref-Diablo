#include <amxmodx>
#include <fakemeta>
#include <engine>
#include <d2lod>

new PLUGIN_NAME[] = "魔力回復"
new PLUGIN_AUTHOR[] = "reff"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 5;
new Skill_Allocate = 10;

new const ManaRegenerate[MAX_P_SKILLS] = 
{
	1, 1, 2, 2, 3, 3, 4, 4, 5, 6
};

new g_SkillId;

new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	register_forward(FM_PlayerPreThink, "fwd_PreThink")
	g_SkillId = register_d2_skill(PLUGIN_NAME, "移動時快速回復魔力.", AMAZON, Skill_Level, Skill_Allocate, NOT_DISPLAY)
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

public fwd_PreThink(id)
{
	if ( !is_user_alive(id) )
		return FMRES_IGNORED;

	const Float:cdown = 0.1;

	static Float:g_LastPressedSkill[33];
	if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
	{
		return FMRES_IGNORED;
	}
	else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
	{
		g_LastPressedSkill[id] = get_gametime()
	}

	if ( is_user_alive(id) && IsPlayerMoving(id) && get_p_skill( id, g_SkillId ) > 0 )
	{
		set_p_mana(id, get_p_mana(id) + ManaRegenerate[ get_p_skill(id, g_SkillId) - 1 ] );
	}

	return FMRES_IGNORED;
}

public bool:IsPlayerMoving( id )
{
	static Float:Velo[3];
	get_user_velocity( id, Velo);

	if ( Velo[0] > 0.0 || Velo[1] > 0.0 || Velo[2] > 0.0 )
		return true;

	return false;
}