#include <amxmodx>
#include <d2lod>

new PLUGIN_NAME[] = "致命一擊"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 1;

new const AmazonCrit[MAX_P_SKILLS] =  // 亞馬遜致命一擊的機率.
{
	5, 7, 8, 9, 10, 12, 15, 17, 20, 24, 25, 26, 30, 33, 35, 40, 45, 50, 55, 60
};

new g_SkillId;

new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "有機率造成重大傷害.", AMAZON, Skill_Level, NOT_DISPLAY)
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
	if ( !IsPlayerNearByMonster(victim) && get_p_hero(attacker) == AMAZON && get_p_skill( attacker, g_SkillId ) > 0 && random_num( 0, 100 ) < AmazonCrit[ get_p_skill( attacker, g_SkillId ) - 1 ] )
	{
		iDamage[0] *= 2.0;
	}
}
public d2_ranged_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( !IsPlayerNearByMonster(victim) && get_p_hero(attacker) == AMAZON && get_p_skill( attacker, g_SkillId ) > 0 && random_num( 0, 100 ) < AmazonCrit[ get_p_skill( attacker, g_SkillId ) - 1 ] )
	{
		iDamage[0] *= 2.0;
	}
}