#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "飛焰風暴"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new const WhirlCast[] = "d2lod/whirlwind1.wav";
new const WhirlCastSpr[] = "sprites/xflare2.spr";

new Skill_Level = 30;

#define COOLDOWN_WHIRLWIND 5.0
#define WHIRL_DAMAGE_DIST 80.0

new const Float:WhirlWindDmg[MAX_P_SKILLS] =  // 飛焰風暴的傷害.
{
	3.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 25.0
};
new const WhirlWindMana[MAX_P_SKILLS] =  // 飛焰風暴需要的能量.
{
	5, 5, 7, 8, 9, 10, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 20, 20, 20, 22
};

#define TASKID_WHIRL 340
#define TASKID_WHIRLEND 440

new g_SkillId;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "飛起來並且可以撞人.", BARBARIAN, Skill_Level, DISPLAY)

	register_event("DeathMsg", "ev_DeathMsg", "a")
	
	g_iMaxPlayers = get_maxplayers();
}

public plugin_precache()
{
	precache_sound( WhirlCast );
	precache_model( WhirlCastSpr );
}
public client_disconnect(id)
{
	remove_task( id + TASKID_WHIRL );
	remove_task( id + TASKID_WHIRLEND );
}
public ev_DeathMsg()
{
	remove_task( read_data(2) + TASKID_WHIRL );
	remove_task( read_data(2) + TASKID_WHIRLEND );
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
		cdown = COOLDOWN_WHIRLWIND;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= WhirlWindMana[ get_p_skill( id, g_SkillId ) - 1 ] )
		{
			emit_sound(id, CHAN_ITEM, WhirlCast, 1.0, ATTN_NORM, 0, PITCH_NORM);
		
			set_p_mana(id, get_p_mana(id) - WhirlWindMana[ get_p_skill( id, g_SkillId ) - 1 ] );

			set_task( 0.2, "Task_Whirl", id + TASKID_WHIRL, _, _, "b")
			set_task( COOLDOWN_WHIRLWIND, "Task_WhirlEnd", id + TASKID_WHIRLEND);

			Set_Sprite_Task(id, WhirlCastSpr, 3.5, 1, COOLDOWN_WHIRLWIND, "Morph");
		}
	}
	
	return PLUGIN_CONTINUE;
}
public Task_Whirl(id)
{
	id -= TASKID_WHIRL;

	if ( !is_user_alive(id) )
		return;

	new Float: Porigin[3], Float: Torigin[3], Float: Distance, Float:velocity[3];

	entity_get_vector( id, EV_VEC_origin, Porigin)

	velocity_by_aim(id, 500, velocity);

	set_pev(id, pev_velocity, velocity);

	for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
	{
		if ( is_user_alive(enemy) && id != enemy && get_p_skill( id, g_SkillId) > 0 )
		{
			entity_get_vector( enemy, EV_VEC_origin, Torigin)

			Distance = get_distance_f(Porigin, Torigin);

			if ( Distance <= WHIRL_DAMAGE_DIST && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) )
			{
				dmg_kill_player(enemy, id, WhirlWindDmg[get_p_skill( id, g_SkillId) - 1], "whirlwind");
			}
		}
	}
}
public Task_WhirlEnd(id)
{
	id -= TASKID_WHIRLEND;

	if ( !is_user_connected(id) )
		return;

	remove_task( id + TASKID_WHIRL );
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		
	}
}
public Set_Sprite_Task(id, const sprite[], Float:scale, istask, Float:task_time, const classname[])
{
	new sprite_ent = create_entity("env_sprite")

	entity_set_string(sprite_ent, EV_SZ_classname, classname)
	entity_set_int(sprite_ent, EV_INT_movetype, MOVETYPE_FOLLOW)
	entity_set_edict(sprite_ent, EV_ENT_aiment, id );
	entity_set_model(sprite_ent, sprite)

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