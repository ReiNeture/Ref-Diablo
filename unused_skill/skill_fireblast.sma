#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "炸彈投擲"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 1;
new Mana_FireBlast = 4;

new const Float:AssBlastDmg[MAX_P_SKILLS] =  // 刺客炸彈傷害.
{
	4.0, 6.0, 9.0, 11.0, 14.0, 16.0, 19.0, 22.0, 27.0, 33.0, 38.0, 43.0, 	49.0, 55.0, 60.0, 77.0, 89.0, 100.0, 105.0, 110.0
};

new const g_w_ass_blast[] = "models/w_hegrenade.mdl";
new const g_SpriteFireBlast[] = "sprites/dexplo.spr";

new g_SkillId;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];
new g_spriteBlast;
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "丟出一顆炸彈.", ASSASSIN, Skill_Level, DISPLAY)

	register_forward(FM_Touch, "Entity_Touched");

	g_iMaxPlayers = get_maxplayers();
}

public plugin_precache()
{
	g_spriteBlast = precache_model( g_SpriteFireBlast );
	precache_model( g_w_ass_blast );
}

public client_disconnect(id)
{

}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		static Float:cdown;
		cdown = 2.0;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= Mana_FireBlast )
				{
					set_p_mana(id, get_p_mana(id) - Mana_FireBlast );

					Throw_FireBlast(id);
				}
	}
	
	return PLUGIN_CONTINUE;
}
public Entity_Touched(ent, victim)
{
	if ( !pev_valid(ent) )
		return;
	
	new classname[32]
	pev( ent, pev_classname, classname, 31)

	new attacker = entity_get_edict(ent, EV_ENT_owner);
	
	if ( equal(classname,"FireBlast") )
	{
		new Float: Torigin[3], Float: Distance, Float: Damage;

		new Float:fOrigin[3], iOrigin[3];
		entity_get_vector( ent, EV_VEC_origin, fOrigin)	
		iOrigin[0] = floatround(fOrigin[0])
		iOrigin[1] = floatround(fOrigin[1])
		iOrigin[2] = floatround(fOrigin[2])	

		message_begin(MSG_BROADCAST,SVC_TEMPENTITY, iOrigin);
		write_byte(TE_EXPLOSION);
		engfunc( EngFunc_WriteCoord,fOrigin[0]);
		engfunc( EngFunc_WriteCoord,fOrigin[1]);
		engfunc( EngFunc_WriteCoord,fOrigin[2]);
		write_short(g_spriteBlast);
		write_byte(32); // 幀幅開始.
		write_byte(20); // 幀幅頻率.
		write_byte(0); // 標記.
		message_end();

		for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
		{
			if ( is_user_alive(enemy) )
			{
				entity_get_vector( enemy, EV_VEC_origin, Torigin)

				Distance = get_distance_f(fOrigin, Torigin);

				if ( Distance <= 250.0 && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) && get_p_skill( attacker, g_SkillId ) > 0 )
				{
					Damage = (((Distance / 250.0) * AssBlastDmg[get_p_skill( attacker, g_SkillId ) - 1]) - AssBlastDmg[get_p_skill( attacker, g_SkillId ) - 1]) * -1.0;

					if (Damage > 0.0)
					{
						dmg_kill_player(enemy, attacker, Damage, "fireblast");
					}
				}
			}
		}

		set_pev( ent, pev_flags, FL_KILLME);
	}
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
// 刺客炸彈投擲技能
public Throw_FireBlast(id)
{
	new Float: fOrigin[3], Float:fAngle[3],Float: fVelocity[3];

	entity_get_vector( id, EV_VEC_origin, fOrigin);
	entity_get_vector( id, EV_VEC_view_ofs, fAngle);

	fOrigin[0] += fAngle[0];
	fOrigin[1] += fAngle[1];
	fOrigin[2] += fAngle[2];
	
	fm_velocity_by_aim(id, 0.6, fVelocity, fAngle);
	fAngle[0] *= -1.0;

	new sprite_ent = create_entity("info_target");

	entity_set_string( sprite_ent, EV_SZ_classname, "FireBlast");
	entity_set_model( sprite_ent, g_w_ass_blast);

	entity_set_edict( sprite_ent, EV_ENT_owner, id);

	entity_set_size( sprite_ent, Float:{-2.1, -2.1, -2.1}, Float:{2.1, 2.1, 2.1});

	entity_set_vector( sprite_ent, EV_VEC_origin, fOrigin);

	fOrigin[0] += fVelocity[0];
	fOrigin[1] += fVelocity[1];
	fOrigin[2] += fVelocity[2];

	entity_set_int( sprite_ent, EV_INT_movetype, MOVETYPE_BOUNCE);
	entity_set_int( sprite_ent, EV_INT_solid, SOLID_BBOX);
	
	entity_set_float( sprite_ent, EV_FL_gravity, 0.55);
	
	fVelocity[0] *= 1000.0;
	fVelocity[1] *= 1000.0;
	fVelocity[2] *= 1000.0;

	entity_set_vector( sprite_ent, EV_VEC_velocity, fVelocity);
	entity_set_vector( sprite_ent, EV_VEC_angles, fAngle);

}
stock fm_velocity_by_aim(iIndex, Float:fDistance, Float:fVelocity[3], Float:fViewAngle[3])
{
	pev(iIndex, pev_v_angle, fViewAngle)
	fVelocity[0] = floatcos(fViewAngle[1], degrees) * fDistance
	fVelocity[1] = floatsin(fViewAngle[1], degrees) * fDistance
	fVelocity[2] = floatcos(fViewAngle[0]+90.0, degrees) * fDistance
	return 1
}