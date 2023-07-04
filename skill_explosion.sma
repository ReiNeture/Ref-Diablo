#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>
#include <xs>

new PLUGIN_NAME[] = "爆裂魔法"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 60;
new Skill_Allocate = 20;

new const g_SpriteExplode[] = "sprites/explosion1.spr";
new const FireStarExplodeSound[] = "weapons/c4_explode1.wav";
new const FireStarLaunchSound[] = "ref/airplane2.wav";
new const firestar_model[] = "models/ref/firemagic.mdl"

#define RADIUS_DAMAGE 3000.0
new const SorcaManaExplosion[MAX_P_SKILLS] =  // 流星攻擊需要的能量.
{
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500
};
new const Float:ExplosionDamage[MAX_P_SKILLS] =  // 流星攻擊的傷害.
{
	2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 
	3500.0, 3550.0, 3600.0, 3700.0, 4000.0, 4300.0, 4600.0, 4900.0, 5200.0, 5600.0
};

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new g_spriteMeteor;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	g_SkillId = register_d2_skill(PLUGIN_NAME, "爆裂!!", AMAZON, Skill_Level, Skill_Allocate, DISPLAY)
	register_think("skill_explosion", "fw_Think")
}

public plugin_precache()
{
	g_spriteMeteor = precache_model( g_SpriteExplode );
	precache_model( firestar_model );
	precache_sound( FireStarLaunchSound );
	precache_sound( FireStarExplodeSound );
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		const Float:cdown = 10.0;
		if ( get_gametime() - g_LastPressedSkill[id] <= cdown ) 
		{
			client_print(id, print_center, "技能冷卻中");
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if( get_p_mana(id) < SorcaManaExplosion[get_p_skill(id, g_SkillId) - 1] )
		{
			client_print(id, print_center, "需要 %d 點能量", SorcaManaExplosion[get_p_skill(id, g_SkillId) - 1]);
			return FMRES_IGNORED;
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= SorcaManaExplosion[get_p_skill(id, g_SkillId) - 1 ])
		{
			set_p_mana(id, get_p_mana(id) - SorcaManaExplosion[get_p_skill(id, g_SkillId) - 1]);
			emitsFireStar(id);
		}
	}
	return PLUGIN_CONTINUE;
}

public emitsFireStar(id)
{
	/* 火流星 */
	new firestarEnt = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"))

	set_pev(firestarEnt, pev_classname, "skill_explosion")
	set_pev(firestarEnt, pev_movetype, MOVETYPE_NONE)
	set_pev(firestarEnt, pev_solid, SOLID_NOT)
	set_pev(firestarEnt, pev_owner, id)
	set_pev(firestarEnt, pev_animtime, get_gametime())
	set_pev(firestarEnt, pev_framerate, 1.0)

	new Float:aimOrigin[3]
	fm_get_aim_origin(id, aimOrigin)
	set_pev(firestarEnt, pev_origin, aimOrigin)

	engfunc(EngFunc_SetModel, firestarEnt, firestar_model)
	set_pev(firestarEnt, pev_nextthink, get_gametime() + 9.7) // 幾秒後爆炸(需綁模組動畫)

	client_cmd(0, "spk ^"sound/%s^"", FireStarLaunchSound)
}

public fw_Think(ent)
{
	if( !pev_valid(ent) )
		return FMRES_IGNORED;

	new Float:fOrigin[3];
	pev(ent, pev_origin, fOrigin);

	new npcname[32];
	new id = pev(ent, pev_owner);
	new victim = FM_NULLENT;
	new Float:fDamage = ExplosionDamage[get_p_skill(id, g_SkillId) - 1] + float(get_p_magic(id)) * 10.0 + get_totaldmg_of_item(id);

	while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, RADIUS_DAMAGE) ) != 0 ) { // 火流星爆炸範圍
		
		pev(victim, pev_classname, npcname, sizeof(npcname));
		if( is_user_alive(victim) && id != victim && !IsPlayerNearByMonster(victim) && !is_p_protected(victim) && get_p_skill(id, g_SkillId) > 0 )
		{
			// dmg_kill_player(victim, id, fDamage, "skill_explosion");
		}
		else if( !is_user_alive(victim) && equal(npcname, "func_wall") )
		{
			cause_monster_damage(victim, ent, id, fDamage, DMG_BURN);
			// ExecuteHam(Ham_TakeDamage, victim, ent, id, fDamage, DMG_BURN)
		}

		if( is_user_alive(victim) ) {
			message_begin(MSG_ONE, get_user_msgid("ScreenShake"), {0,0,0}, victim)
			write_short(0xFFFF)
			write_short(1<<13)
			write_short(0xFFFF) 
			message_end()
		}
	}

	engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, fOrigin, 0)
	write_byte(TE_EXPLOSION)
	engfunc(EngFunc_WriteCoord, fOrigin[0])
	engfunc(EngFunc_WriteCoord, fOrigin[1])
	engfunc(EngFunc_WriteCoord, fOrigin[2])
	write_short(g_spriteMeteor)
	write_byte(210)
	write_byte(10)
	write_byte(TE_EXPLFLAG_NOPARTICLES | TE_EXPLFLAG_NOSOUND)
	message_end()

	for(new i = 1; i <= 5; ++i)
		client_cmd(0, "spk ^"sound/%s^"", FireStarExplodeSound)
	// client_cmd(0, "room_type 6")

	engfunc(EngFunc_RemoveEntity, ent)

	return FMRES_HANDLED;
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
	}
}

// ported from AMXX's core get_user_origin(..., 3) (suggested by Greenberet)
stock fm_get_aim_origin(index, Float:origin[3]) {
	new Float:start[3], Float:view_ofs[3];
	pev(index, pev_origin, start);
	pev(index, pev_view_ofs, view_ofs);
	xs_vec_add(start, view_ofs, start);

	new Float:dest[3];
	pev(index, pev_v_angle, dest);
	engfunc(EngFunc_MakeVectors, dest);
	global_get(glb_v_forward, dest);
	xs_vec_mul_scalar(dest, 9999.0, dest);
	xs_vec_add(start, dest, dest);

	engfunc(EngFunc_TraceLine, start, dest, 0, index, 0);
	get_tr2(0, TR_vecEndPos, origin);

	return 1;
}