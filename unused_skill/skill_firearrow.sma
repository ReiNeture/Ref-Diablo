#include <amxmodx>
#include <d2lod>

new PLUGIN_NAME[] = "火焰箭"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 1;

new Mana_FireArrow = 3;

new const Float:FireArrowDmg[MAX_P_SKILLS] =  // 火焰箭傷害百分比.
{
	23.0, 29.0, 34.0, 39.0, 42.0, 45.0, 50.0, 53.0, 57.0, 60.0, 63.0, 66.0, 69.0, 75.0, 80.0, 90.0, 100.0, 105.0, 115.0, 125.0
};

new g_SkillId;

new bool: g_ShootWithFireArrow[33];
new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "將火焰付在弓箭上.", AMAZON, Skill_Level, DISPLAY)
}

public client_disconnect(id)
{
	g_ShootWithFireArrow[id] = false;
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
		g_ShootWithFireArrow[id] = false;
	}
}

public d2_ranged_actshoot( id, entity )
{
	if ( get_p_hero(id) == AMAZON && get_p_skill( id, g_SkillId ) > 0 && g_iCurSkill[id] == g_SkillId
	&& get_p_mana(id) >= Mana_FireArrow )
	{
		g_ShootWithFireArrow[id] = true;

		set_p_mana(id, get_p_mana(id) - Mana_FireArrow );
	}
}

public d2_ranged_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( get_p_hero(attacker) == AMAZON && get_p_skill( attacker, g_SkillId ) > 0 && g_ShootWithFireArrow[attacker] )
	{
		iDamage[0] = iDamage[0] + ( ( iDamage[0] / 100.0 ) * FireArrowDmg[ get_p_skill( attacker, g_SkillId ) - 1 ] );

		g_ShootWithFireArrow[attacker] = false;
	}
}