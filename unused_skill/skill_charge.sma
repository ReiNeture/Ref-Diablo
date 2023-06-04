#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <hamsandwich>
#include <engine>

new PLUGIN_NAME[] = "衝撞"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 12;
new Mana_Charge = 5;

new const ChargeCast[] = "d2lod/charge.wav";
new const ChargeCastSpr[] = "sprites/effects/ripple.spr";

new const PalChDmg[MAX_P_SKILLS] =  // 衝撞的傷害百分比.
{
	15, 20, 25, 30, 40, 50, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125
};

#define CHARGE_DELAY 3.0

#define TASKID_CHARGE 140

new g_SkillId;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];
new bool:IsChargeDelay[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "向前衝撞並在瞬間增加攻擊力.", PALADIN, Skill_Level, DISPLAY)

	RegisterHam(Ham_Spawn, "player", "fwd_PlayerSpawn", 1);
}

public plugin_precache()
{
	precache_sound( ChargeCast );
	precache_model( ChargeCastSpr );
}
public fwd_PlayerSpawn(id)
{
	if ( !is_user_alive(id) )
		return;

	remove_task( id + TASKID_CHARGE );

	IsChargeDelay[id] = false;
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
		cdown = 1.0;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= Mana_Charge )
		{
			if ( !get_p_item_wear_type(id, TYPE_SHIELD) )
			{
				client_print(id, print_chat, "You must wear shield, to use this skill!");
				return PLUGIN_HANDLED;
			}

			emit_sound(id, CHAN_ITEM, ChargeCast, 1.0, ATTN_NORM, 0, PITCH_NORM);
		
			set_p_mana(id, get_p_mana(id) - Mana_Charge );

			static Float: velocity[3];

			velocity_by_aim(id, 1600, velocity);

			set_pev(id, pev_velocity, velocity);

			Set_Sprite_Task(id, ChargeCastSpr, 4.0, 1, 1.2, "Morph");

			IsChargeDelay[id] = true;

			set_task( CHARGE_DELAY, "Task_Charge", id + TASKID_CHARGE);
		}
	}
	
	return PLUGIN_CONTINUE;
}
public Task_Charge(id)
{
	id -= TASKID_CHARGE;

	IsChargeDelay[id] = false;
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		IsChargeDelay[id] = false;
	}
}
public d2_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( !IsPlayerNearByMonster(victim) && IsChargeDelay[attacker] && get_p_skill( attacker, g_SkillId ) > 0 && get_p_hero(attacker) == PALADIN && g_iCurSkill[attacker] == g_SkillId )
	{
		iDamage[0] += (iDamage[0] * PalChDmg[ get_p_skill( attacker, g_SkillId ) - 1 ] / 100.0);
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