#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>
#include <fun>

new PLUGIN_NAME[] = "神聖冰凍"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 18;

new const CastChill[] = "sprites/xspark3.spr";
new const ChilledSpr[] = "sprites/xspark1.spr";

new Float:DelayFreezeTime = 2.0;

new const Float:PalFreezeDmg[MAX_P_SKILLS] =  // 聖劍士的冰凍傷害.
{
	3.0, 5.0, 8.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 24.0, 	26.0, 28.0, 30.0, 32.0, 34.0, 35.0, 37.0, 45.0
};
new const Float:PalFreezeSpeed[MAX_P_SKILLS] =  // 聖劍士冰凍的速度.
{
	15.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 25.0, 26.0, 27.0, 28.0, 	29.0, 30.0, 32.0, 35.0, 38.0, 40.0, 45.0, 50.0
};
new const Float:PalFreezeRadius[MAX_P_SKILLS] =  // 聖劍士冰凍半徑.
{
	70.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0, 165.0, 170.0, 175.0, 180.0, 	185.0, 190.0, 195.0, 200.0, 210.0, 220.0, 240.0, 250.0
};

new g_SkillId;

#define TASKID_FRZ 15222

new g_iCurSkill[33];
new Float: g_EnemyFrozenSpeed[33];
new bool: g_EnemyFrozen[33];
new bool: g_IsHolyFreeze[33];
new Float: g_LastHolyFreeze[33];
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "釋放寒氣,冷凍敵人.", PALADIN, Skill_Level, DISPLAY)

	RegisterHam(Ham_Spawn, "player", "fwd_PlayerSpawn", 1);

	register_event("DeathMsg", "ev_DeathMsg", "a")

	register_forward(FM_PlayerPreThink, "fwd_PreThink")

	g_iMaxPlayers = get_maxplayers();

}

public plugin_precache()
{
	precache_model( CastChill );
	precache_model( ChilledSpr );
}

public client_disconnect(id)
{
	Reset_HolyFreeze( id );
	g_IsHolyFreeze[id] = false;

	g_EnemyFrozen[id] = false;
	remove_task( id + TASKID_FRZ );
}

public ev_DeathMsg()
{
	Reset_HolyFreeze( read_data(2) );
}

public fwd_PlayerSpawn(id)
{
	if ( !is_user_alive(id) )
		return;

	if ( g_IsHolyFreeze[id] )
	{
		Reset_HolyFreeze(id);

		Set_Sprite_Task(id, CastChill, 1.2, 0, 0.0, "HolyFreeze");
	}
}
public fwd_PreThink(id)
{
	if ( !is_user_alive(id) || is_freezetime() )
		return;

	static Float: Time;
	Time = get_gametime();

	static Float: cooldown;
	cooldown = 2.5;
	new Float: Porigin[3], Float: Torigin[3], Float: Distance;

	if (Time - cooldown > g_LastHolyFreeze[id])
	{
		if ( get_p_hero(id) == PALADIN )
		{
			entity_get_vector( id, EV_VEC_origin, Porigin)

			for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
			{
				if ( is_user_alive(enemy) && id != enemy && get_p_skill( id, g_SkillId) > 0 )
				{
					entity_get_vector( enemy, EV_VEC_origin, Torigin)

					Distance = get_distance_f(Porigin, Torigin);

					if ( g_IsHolyFreeze[id] )
					{
						if ( Distance <= PalFreezeRadius[get_p_skill( id, g_SkillId) - 1] && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) )
						{
							g_EnemyFrozen[enemy] = true;
							g_EnemyFrozenSpeed[enemy] = PalFreezeSpeed[get_p_skill( id, g_SkillId ) - 1];
							
							remove_task( enemy + TASKID_FRZ );
							set_task( DelayFreezeTime, "Reset_Freeze", enemy + TASKID_FRZ);

							dmg_kill_player(enemy, id, PalFreezeDmg[get_p_skill( id, g_SkillId) - 1], "holy_freeze");

							if ( !get_p_in_safezone(enemy) )
								Set_Sprite_Task(enemy, ChilledSpr, 1.5, 1, 1.0, "HolyChill");
						
						}
					}
				}
			}
		}

		g_LastHolyFreeze[id] = Time; 
	}

	if ( g_EnemyFrozen[id] )
	{
		if ( !get_p_in_safezone(id) )
			set_user_maxspeed( id, get_current_speed(id) - ( ( get_current_speed(id) / 100 ) * g_EnemyFrozenSpeed[id] ) );
	}
	else
		set_user_maxspeed(id, get_current_speed(id) );
}

public Reset_Freeze(id)
{
	id -= TASKID_FRZ;

	if ( !( 1 <= id <= g_iMaxPlayers ) )
		return;

	g_EnemyFrozen[id] = false;
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;

	if ( get_p_hero(id) == PALADIN )
	{
		if ( skill_id == g_SkillId )
		{
			Reset_HolyFreeze(id);

			g_IsHolyFreeze[id] = true;

			Set_Sprite_Task(id, CastChill, 1.2, 0, 0.0, "HolyFreeze");
		}
		else
		{
			Reset_HolyFreeze(id);

			g_IsHolyFreeze[id] = false;
		}
	}
}

public d2_skill_fired(id)
{

}

public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}
public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		Reset_HolyFreeze(id);

		g_IsHolyFreeze[id] = false;

		g_EnemyFrozen[id] = false;
		remove_task( id + TASKID_FRZ );
	}
}

public Reset_HolyFreeze(id)
{
	if ( get_p_hero(id) == PALADIN )
	{
		new sprite_ent = find_ent_by_class(-1, "HolyFreeze")
	
		if ( pev_valid(sprite_ent) ) 
		{
			remove_entity(sprite_ent)
			sprite_ent = find_ent_by_class(sprite_ent, "HolyFreeze")
		}
	}
}
public Set_Sprite_Task(id, const sprite[], Float:scale, istask, Float:task_time, const classname[])
{
	new sprite_ent = create_entity("env_sprite")

	entity_set_string( sprite_ent, EV_SZ_classname, classname)
	entity_set_int( sprite_ent, EV_INT_movetype, MOVETYPE_FOLLOW)
	entity_set_edict( sprite_ent, EV_ENT_aiment, id );
	entity_set_model( sprite_ent, sprite)

	entity_set_int( sprite_ent, EV_INT_rendermode, kRenderTransAdd)
	entity_set_float( sprite_ent, EV_FL_renderamt, 200.0 )
    
	entity_set_float( sprite_ent, EV_FL_framerate, 22.0 )
	entity_set_float( sprite_ent, EV_FL_scale, scale )
	entity_set_int( sprite_ent, EV_INT_spawnflags, SF_SPRITE_STARTON)
	DispatchSpawn( sprite_ent )

	if ( istask )
	{
		set_task(task_time, "End_Sprite_Task", sprite_ent);
	}
}
public End_Sprite_Task(sprite_ent)
{
	if ( is_valid_ent(sprite_ent) )
	{
		remove_entity(sprite_ent);
	}
}