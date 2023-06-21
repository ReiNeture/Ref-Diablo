#include <amxmodx>
#include <d2lod>
#include <fakemeta>

new PLUGIN_NAME[] = "翅膀";
new PLUGIN_AUTHOR[] = "reff";
new PLUGIN_VERSION[] = "1.0";

new const wings_model[] = "models/ref/krilo2.mdl";

const Skill_Req_Level = 3;
const Skill_Max = 1;

new g_SkillId;
new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];

new wing_status[33], had_wing[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);

	register_forward(FM_PlayerPreThink, "fw_playerPreThink");
	register_forward(FM_CmdStart, "fw_CmdStart");

	g_SkillId = register_d2_skill(PLUGIN_NAME, "翅膀移動.", NECROMANCER, Skill_Req_Level, Skill_Max, DISPLAY);
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheModel, wings_model);
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId && get_p_skill( id, g_SkillId ) > 0 )
	{
		const Float:cdown = 0.4;
		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
			return PLUGIN_HANDLED;
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
			g_LastPressedSkill[id] = get_gametime();

		if( !had_wing[id] )
		{
			had_wing[id] = 1
			new wing = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target") )
			set_pev(wing, pev_classname, "ref_wing")
			set_pev(wing, pev_movetype, MOVETYPE_FOLLOW)
			set_pev(wing, pev_solid, SOLID_NOT)
			set_pev(wing, pev_owner, id)
			set_pev(wing, pev_aiment, id)
			engfunc(EngFunc_SetModel, wing, wings_model)

			client_print(id, print_center, "[開關技能] 已啟動翅膀")
		} 
		else
		{
			remove_wing(id)
			client_print(id, print_center, "[開關技能] 已關閉翅膀")
		} 
	}

	return PLUGIN_CONTINUE;
}

remove_wing(id)
{
	had_wing[id] = 0
	wing_status[id] = 0

	new wing = fm_find_ent_by_owner(0, "ref_wing", id)
	if( pev_valid(wing) ) {
		engfunc(EngFunc_RemoveEntity, wing)
	}
}

set_wing_float(id)
{
	if( !wing_status[id] ) {
		wing_status[id] = 1 // 啟動懸空
		client_print(id, print_center, "[翅膀] 啟動懸空")
	} else {
		wing_status[id] = 0 // 關閉懸空
		client_print(id, print_center, "[翅膀] 關閉懸空")
	}
}

public fw_CmdStart(id, uc_handle, seed)
{
	if (!is_user_alive(id) ) 
		return FMRES_IGNORED
	if( !had_wing[id] )
		return FMRES_IGNORED

	static CurButton; CurButton = get_uc(uc_handle, UC_Buttons)
	static Float:nextJumpCheckTime[33], Float:game_time
	game_time = get_gametime()

	if( CurButton & IN_JUMP && !(CurButton & IN_DUCK) ) {

		if( game_time < nextJumpCheckTime[id] )
			return FMRES_IGNORED

		nextJumpCheckTime[id] = game_time + 0.2

		if( !(pev(id, pev_flags) & FL_ONGROUND) )
			set_wing_float(id)
    }

	if( CurButton & IN_USE ) {
		static Float:velocity[3]
		velocity_by_aim(id, 1100, velocity)
		velocity[2] = 200.0
		set_pev(id, pev_velocity, velocity)
	}

	return FMRES_IGNORED
}

public fw_playerPreThink(id)
{
	if( !is_user_alive(id) ) 
		return FMRES_IGNORED

	if( wing_status[id] ) {

		static Float:velocity[3]
		pev(id, pev_velocity, velocity)

		if( velocity[2] < 0.0 ) // 下降時
		{
			velocity[2] = (velocity[2] + 15.0 < -50.0) ? velocity[2] + 15.0 : -50.0
			set_pev(id, pev_velocity, velocity)
		}

	}

	return FMRES_IGNORED
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		remove_wing(id)
	}
}

public client_connect(id) 
{
	remove_wing(id)
}
public client_disconnected(id) 
{
	remove_wing(id)
}

stock fm_find_ent_by_owner(index, const classname[], owner, jghgtype = 0) {

	new strtype[11] = "classname", ent = index;
	switch (jghgtype) {
		case 1: strtype = "target";
		case 2: strtype = "targetname";
	}

	while ((ent = engfunc(EngFunc_FindEntityByString, ent, strtype, classname)) && pev(ent, pev_owner) != owner) {}

	return ent;
}