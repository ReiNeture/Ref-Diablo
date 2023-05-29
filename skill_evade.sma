#include <amxmodx>
#include <engine>
#include <d2lod>

new PLUGIN_NAME[] = "迴避"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 24;

new const AmazonEvade[MAX_P_SKILLS] =  // 亞馬遜迴避技能的機率.
{
	5, 7, 8, 9, 10, 12, 15, 17, 20, 22, 24, 26, 28, 30, 31, 32, 33, 34, 35, 38
};

new g_SkillId;

new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "移動中被攻擊時,有一定機率可以迴避掉.", AMAZON, Skill_Level, NOT_DISPLAY)
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
	if ( is_user_alive(victim) && get_p_hero(victim) == AMAZON && get_p_skill( victim, g_SkillId ) > 0 && random_num( 0, 100 ) < AmazonEvade[ get_p_skill( victim, g_SkillId ) - 1 ] && IsPlayerMoving(victim) )
	{
		iDamage[0] = 0.0;
	}
}
public d2_ranged_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( is_user_alive(victim) && get_p_hero(victim) == AMAZON && get_p_skill( victim, g_SkillId ) > 0 && random_num( 0, 100 ) < AmazonEvade[ get_p_skill( victim, g_SkillId ) - 1 ] && IsPlayerMoving(victim) )
	{
		iDamage[0] = 0.0;
	}
}
public bool:IsPlayerMoving( id )
{
	new Float:Velo[3];
	get_user_velocity( id, Velo);

	if ( Velo[0] > 0.0 || Velo[1] > 0.0 || Velo[2] > 0.0 )
		return true;

	return false;
}