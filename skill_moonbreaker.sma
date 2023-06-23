#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <xs>
#include <orpheu>
#include <hamsandwich>

new PLUGIN_NAME[] = "破月斬";
new PLUGIN_AUTHOR[] = "reff";
new PLUGIN_VERSION[] = "1.0";

new const moonbreak_model[] = "models/ref/moon_break.mdl";
new const moonbreak_kill_spr[] = "sprites/ref/moonkiller.spr"

new const skills_sound[][] = 
{
	"ref/moonbreak_summon.wav",
	"ref/moonbreak_hit.wav"
}

const Float:basic_damge = 1000.0;
new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];

new g_moonBreakLightFlag;
new kill_spr;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	register_forward(FM_Think, "fw_Think");

	g_SkillId = register_d2_skill(PLUGIN_NAME, "瞬間召喚大劍攻擊前面敵人.", NECROMANCER, 1, 1, DISPLAY);
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheModel, moonbreak_model);
	kill_spr = engfunc(EngFunc_PrecacheModel, moonbreak_kill_spr);

	for(new i = 0; i < sizeof(skills_sound); i++)
		engfunc(EngFunc_PrecacheSound, skills_sound[i]);
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		const Float:cdown = 0.5;
		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime();
		}

		summon_moonbreaker(id);
	}
	
	return PLUGIN_CONTINUE;
}

public fw_Think(ent)
{
	if( !pev_valid(ent) ) return FMRES_IGNORED

	static classname[32], npcname[32];
	pev(ent, pev_classname, classname, sizeof(classname))

	if( equal(classname, "d2_moon_breker") ) {

		new id = pev(ent, pev_owner);
		new Float:iDamage = basic_damge + float(get_p_strength(id)) + get_totaldmg_of_item(id);

		new Float:fOrigin[3]
		pev(ent, pev_origin, fOrigin)

		new victim = FM_NULLENT;
		while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, 500.0) ) != 0 ) {

			pev(victim, pev_classname, npcname, sizeof(npcname));
			if( id != victim && is_user_alive(victim) && !is_p_protected(victim) && get_p_skill(id, g_SkillId) > 0 )
			{
				// dmg_kill_player(victim, id, iDamage, "d2_moon_breker");
			}
			else if( !is_user_connected(victim) && equal(npcname, "func_wall") )
			{
				ExecuteHam(Ham_TakeDamage, victim, ent, id, iDamage, DMG_BURN);
			}
			else
			{
				continue;
			}

			new Float:vOrigin[3];
			pev(victim, pev_origin, vOrigin);

			engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, vOrigin, 0)
			write_byte(TE_EXPLOSION)
			engfunc(EngFunc_WriteCoord, vOrigin[0])
			engfunc(EngFunc_WriteCoord, vOrigin[1])
			engfunc(EngFunc_WriteCoord, vOrigin[2])
			write_short(kill_spr)
			write_byte(10)
			write_byte(15)
			write_byte(TE_EXPLFLAG_NOPARTICLES | TE_EXPLFLAG_NOSOUND)
			message_end()
		}

		client_cmd(0, "spk ^"sound/%s^"", skills_sound[1]);
		set_task(0.1, "moonbreakSetDefaultLighTask");
		toFadeOutEntity(ent, 3.0)
	}

	return FMRES_IGNORED
}

public moonbreakSetDefaultLighTask() {
	// 代表目前總發動次數
	if( --g_moonBreakLightFlag <= 0 ) {
		engfunc(EngFunc_LightStyle, 0, "") // Default value
	}
}

summon_moonbreaker(id)
{
	new moon = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target") )
	set_pev(moon, pev_movetype, MOVETYPE_NONE)
	set_pev(moon, pev_owner, id)
	set_pev(moon, pev_classname, "d2_moon_breker")
	set_pev(moon, pev_solid, SOLID_NOT)
	set_pev(moon, pev_animtime, get_gametime() )
	set_pev(moon, pev_framerate, 1.0)
	set_pev(moon, pev_rendermode, kRenderTransAlpha);
	set_pev(moon, pev_renderamt, 255.0)

	new Float:origin[3], Float:angles[3]
	pev(id, pev_origin, origin)
	set_pev(moon, pev_origin, origin)

	pev(id, pev_v_angle, angles)
	angles[2] = 0.0
	set_pev(moon, pev_angles, angles)

	engfunc(EngFunc_SetModel, moon, moonbreak_model)
	set_pev(moon, pev_nextthink, get_gametime() + 1.9)
	
	engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, origin, 0)
	write_byte(TE_DLIGHT)
	engfunc(EngFunc_WriteCoord, origin[0])
	engfunc(EngFunc_WriteCoord, origin[1])
	engfunc(EngFunc_WriteCoord, origin[2])
	write_byte(110) // radius in 10's
	write_byte(100) // rgb
	write_byte(100)
	write_byte(100)
	write_byte(50) // life in 10's
	write_byte(20) // decay rate in 10's
	message_end()
	
	client_cmd(0, "spk ^"sound/%s^"", skills_sound[0])
	engfunc(EngFunc_LightStyle, 0, "a");
	g_moonBreakLightFlag++;

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
	}
}

// in skill_light_of_sword.sma think
stock toFadeOutEntity(ent, Float:fadeSpeed=5.0)
{
	set_pev(ent, pev_classname, "fade_out")
	set_pev(ent, pev_fuser1, fadeSpeed)
	set_pev(ent, pev_nextthink, get_gametime() + 0.01)
}
