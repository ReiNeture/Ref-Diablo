#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <fun>

new PLUGIN_NAME[] = "疾風步"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 24;

new const Float:BarSpeed[MAX_P_SKILLS] =  // 野蠻人加速的百分比.
{
	5.0, 7.0, 9.0, 13.0, 15.0, 17.0, 19.0, 21.0, 23.0, 25.0, 27.0, 29.0, 31.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 42.0
};


new g_SkillId;

new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "增加移動速度.", BARBARIAN, Skill_Level, NOT_DISPLAY)

	register_forward(FM_PlayerPreThink, "fwd_PreThink")
}

public client_disconnect(id)
{
	
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id
}

public fwd_PreThink(id)
{
	if ( !is_user_alive(id) || is_freezetime()
	|| get_p_hero(id) != BARBARIAN )
		return;

	if ( get_p_skill( id, g_SkillId ) > 0 )
	{
		set_user_maxspeed(id, get_current_speed(id) + ( ( get_current_speed(id) / 100 ) * BarSpeed[ get_p_skill( id, g_SkillId ) - 1 ] ) );
	}
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