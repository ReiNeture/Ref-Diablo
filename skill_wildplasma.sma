#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>

new PLUGIN_NAME[] = "武器強化"
new PLUGIN_AUTHOR[] = "Reff"
new PLUGIN_VERSION[] = "1.0"
				
#define TASK_CODE 7777

new Skill_Level = 55;
new Skill_Allocate = 20;

new const Float:WildUtilTime[MAX_P_SKILLS] = 
{
	5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 25.0
};

new g_SkillId;
new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new bool:g_enable[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	RegisterHam(Ham_Killed, "player", "fw_PlayerKilled");
	g_SkillId = register_d2_skill(PLUGIN_NAME, "大幅提升綠汁雷射槍射速跟傷害.", ASSASSIN, Skill_Level, Skill_Allocate, DISPLAY)
}

public plugin_natives()
{
	register_native("get_wild_status", "native_get_wild_status", 1);
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		const Float:cdown = 0.1;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
			return PLUGIN_HANDLED;
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
			g_LastPressedSkill[id] = get_gametime()

		if( g_enable[id] == true ) {
			client_print(id, print_center, "武器強化啟動中");
			return PLUGIN_HANDLED;
		}
		if( get_p_mana(id) < 10 ) {
			client_print(id, print_center, "能量不足");
			return PLUGIN_HANDLED;
		}
		if ( get_p_skill(id, g_SkillId) < 1 )
		{
			client_print(id, print_center, "沒有學習此技能");
			return PLUGIN_HANDLED;
		}

		g_enable[id] = true;
		set_p_mana(id, get_p_mana(id) - 10);
		set_task(WildUtilTime[get_p_skill(id, g_SkillId) - 1], "disable_wild", id + TASK_CODE)
		client_print(id, print_center, "啟動:武器強化");
	}	

	return PLUGIN_CONTINUE
}

public disable_wild(id) {
	id = id - TASK_CODE
	if( g_enable[id] )
		g_enable[id] = false;
}

removeWildAndTask(id) {
	if( task_exists(id + TASK_CODE) )
		remove_task(id + TASK_CODE)
	if( g_enable[id] )
		g_enable[id] = false;
}

public client_disconnect(id)
{
	removeWildAndTask(id)
}
public client_connect(id)
{
	removeWildAndTask(id)
}

public fw_PlayerKilled(this, attacker, shouldgib)
{
	if ( !is_user_connected(this) ) 
		return HAM_IGNORED;

	removeWildAndTask(this)
	return HAM_IGNORED;
}

public bool:native_get_wild_status(id) {
	return g_enable[id];
}

public d2_takedamage(victim, attacker, Float:iDamage[1])
{
}