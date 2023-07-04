#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <hamsandwich>
#include <vector>
#include <xs>


new PLUGIN_NAME[] = "導彈風暴"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 65;
new Skill_Allocate = 1;

new const MissileMdl[] = "models/ref/stinger_rocket_frk14.mdl";
new const MissileExpSpr[] = "sprites/ref/zerogxplode.spr";
new const MissileSmoke[] = "sprites/ref/smoke_ia.spr";

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new explosionSpr, beamSpr;

#define TASK_CODE 1912

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	g_SkillId = register_d2_skill(PLUGIN_NAME, "周圍降下大量導彈.", ASSASSIN, Skill_Level, Skill_Allocate, DISPLAY);

	RegisterHam(Ham_Killed, "player", "fw_PlayerKilled")
	register_forward(FM_Touch, "fw_Touch");
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheModel, MissileMdl);
	beamSpr = engfunc(EngFunc_PrecacheModel, MissileSmoke);
	explosionSpr = engfunc(EngFunc_PrecacheModel, MissileExpSpr);
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		static Float:cdown;
		cdown = 10.0;

		if ( get_gametime() - g_LastPressedSkill[id] <= cdown ) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 )
		{
			set_task(0.1, "skill_missile_storm", id + TASK_CODE, _, _, "b");
		}
	}
	
	return PLUGIN_CONTINUE;
}

public skill_missile_storm(id)
{
	id = id - TASK_CODE;
	if( get_gametime() > g_LastPressedSkill[id] + 7.0 )
	{
		shutdown_missile_storm(id);
	}

	new Float:origin[3], Float:angles[3], Float:missile_velocity[3];

	// 一般設定
	new missile_storm = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target") )
	set_pev(missile_storm, pev_classname, "missile_storm");
	set_pev(missile_storm, pev_movetype, MOVETYPE_TOSS);
	set_pev(missile_storm, pev_solid, SOLID_TRIGGER);
	set_pev(missile_storm, pev_owner, id);
	set_pev(missile_storm, pev_gravity, 0.8);
	engfunc(EngFunc_SetModel, missile_storm, MissileMdl);
	engfunc(EngFunc_SetSize, missile_storm, Float:{-2.0, -2.0, -2.0}, Float:{2.0, 2.0, 2.0});

	// 起點設定
	pev(id, pev_origin, origin);
	set_pev(missile_storm, pev_origin, origin);

	// 方向設定
	pev(id, pev_v_angle, angles);
	angles[0] += random_float(-3.0, 3.0);
	angles[1] += random_float(-15.0, 15.0);
	// angles[2] *= -1.0;
	set_pev(missile_storm, pev_angles, angles);

	// 向量設定
	angle_vector(angles, ANGLEVECTOR_FORWARD, missile_velocity);
	xs_vec_mul_scalar(missile_velocity, 2300.0, missile_velocity);
	set_pev(missile_storm, pev_velocity, missile_velocity);

	// 車尾燈特效
	engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, 0, 0);
	write_byte(TE_BEAMFOLLOW);
	write_short(missile_storm);
	write_short(beamSpr);
	write_byte(10); // life
	write_byte(5); // widtd
	write_byte(255); // r
	write_byte(255); // g
	write_byte(255); // b
	write_byte(215); // brightness
	message_end();
}

shutdown_missile_storm(id)
{
	if( task_exists(id + TASK_CODE) )
	{
		remove_task(id + TASK_CODE);
	}
}

public fw_Touch(ent, ptd)
{
	if( !pev_valid(ent) ) 
		return FMRES_IGNORED
	
	static classname[32], id;
	id = pev(ent, pev_owner);
	pev(ent, pev_classname, classname, sizeof(classname))

	if( equal(classname, "missile_storm") && id != ptd ) {
		explode_missile_storm(ent);
	}

	return FMRES_IGNORED;
}

explode_missile_storm(ent)
{
	const Float:basic_damge = 150.0;

	new attacker = pev(ent, pev_owner);
	new targetname[32];

	new Float:iDamage = basic_damge + (float(get_p_dexterity(attacker)) * 0.3) + get_totaldmg_of_item(attacker);
	new Float:fOrigin[3];
	pev(ent, pev_origin, fOrigin);

	new victim = FM_NULLENT;
	while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, 135.0) ) != 0 ) {

		pev(victim, pev_classname, targetname, 31);
		if( attacker == victim ) 
			continue;

		if( is_user_alive(victim) && !is_p_protected(victim) && get_p_skill(attacker, g_SkillId) > 0 )
		{
			// dmg_kill_player(victim, attacker, iDamage, "missile_storm");
		}
		else if( !is_user_connected(victim) && equal(targetname, "func_wall") )
		{
			cause_monster_damage(victim, ent, attacker, iDamage, DMG_BLAST);
			// ExecuteHam(Ham_TakeDamage, victim, ent, attacker, iDamage, DMG_BLAST);
		}
	}

	engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, fOrigin, 0);
	write_byte(TE_EXPLOSION);
	engfunc(EngFunc_WriteCoord, fOrigin[0]);
	engfunc(EngFunc_WriteCoord, fOrigin[1]);
	engfunc(EngFunc_WriteCoord, fOrigin[2]);
	write_short(explosionSpr);
	write_byte(10);
	write_byte(20);
	write_byte(TE_EXPLFLAG_NOPARTICLES);
	message_end();

	engfunc(EngFunc_RemoveEntity, ent);
}

public client_connect(id)
{
	shutdown_missile_storm(id);
}

public client_disconnect(id)
{
	shutdown_missile_storm(id);
}

public fw_PlayerKilled(victim, attacker, shouldgib)
{
	if ( !is_user_connected(victim) ) 
		return HAM_IGNORED;

	shutdown_missile_storm(victim);

	return HAM_HANDLED;
}