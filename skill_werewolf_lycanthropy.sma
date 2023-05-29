#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>
#include <cstrike>
#include <fun>

new PLUGIN_NAME[] = "德魯伊的技能"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level_Werewolf = 1;
new Skill_Level_Lycanthropy = 1;
new Skill_Level_Hunger = 24;
new Skill_Level_FireClaws = 18;

new Mana_WereWolf = 15;
new Mana_Hunger = 3;
new Mana_FireClaws = 4;

new Hunger_Damage_Minus = 25; // 這是百分比.

new const Float:Werewolf_Attack_Speed[MAX_P_SKILLS] =  // 狼人化後的攻擊速度.
{
	0.50, 
	0.49, 
	0.48, 
	0.47, 
	0.46, 
	0.45, 
	0.40, 
	0.39, 
	0.37, 
	0.35, 
	0.34, 
	0.33, 
	0.32, 	
	0.31, 
	0.30, 
	0.29, 
	0.28, 
	0.27, 
	0.26, 
	0.25
}
new const Float:Werewolf_Lycanthropy_Dur[MAX_P_SKILLS] =  // 狼人化的持續時間.
{
	20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 	140.0, 150.0, 160.0, 200.0, 210.0, 220.0, 230.0, 250.0
};
new const Werewolf_Lycanthropy_Hp[MAX_P_SKILLS] =  // 狼人化後的血量最大值百分比.
{
	15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110
};
new const Werewolf_LifeSteal[MAX_P_SKILLS] =  // 狼人吸血/能量的數值.
{
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22
};
new const Float:Werewolf_DmgFireClaws[MAX_P_SKILLS] =  // 德魯伊火爪的傷害百分比.
{
	5.0, 10.0, 15.0, 20.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0, 80.0, 90.0, 100.0, 110.0, 115.0, 120.0
};

new const WerewolfMetamorphosys[] = "d2lod/morph.wav";
new const WerewolfSpr[] = "sprites/xfire2.spr";
new const WerewolfSprFireClaws[] = "sprites/rjet1.spr";

new const WerewolfModel[] = "werewolf";

#define TASKID_WEREWOLF 9444

const m_pPlayer	= 41;
const m_flPrimaryAttack = 46;

new g_SkillId, g_SkillId2, g_SkillId3, g_SkillId4;

new Float:g_LastPressedSkill[33];
new bool:g_IsWereWolf[33];
new g_MaxHp[33];
new g_iCurSkill[33];
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill("狼人化", "變成狼人.", DRUID, Skill_Level_Werewolf, DISPLAY)
	g_SkillId2 = register_d2_skill("狼人化增強", "增強狼人化的能力數值.", DRUID, Skill_Level_Lycanthropy, NOT_DISPLAY)
	g_SkillId3 = register_d2_skill("吸收", "可以吸取敵人的血/能量.", DRUID, Skill_Level_Hunger, DISPLAY)
	g_SkillId4 = register_d2_skill("火爪", "攻擊時有額外傷害.", DRUID, Skill_Level_FireClaws, DISPLAY)


	RegisterHam( Ham_Weapon_PrimaryAttack, "weapon_knife", "fwd_AttackSpeed" , 1 );
	RegisterHam( Ham_Item_Deploy , "weapon_knife", "fwd_AttackSpeed", 1);

	register_event("DeathMsg", "ev_DeathMsg", "a")

	g_iMaxPlayers = get_maxplayers();
}
public ev_DeathMsg()
{
	Reset_WereWolf( read_data(2) + TASKID_WEREWOLF );
}

public plugin_precache()
{
	precache_model( WerewolfSpr );
	precache_sound( WerewolfMetamorphosys );
	precache_sound( WerewolfSprFireClaws );

	new Mdl_Path[100];

	formatex( Mdl_Path, sizeof Mdl_Path - 1, "models/player/%s/%s.mdl", WerewolfModel, WerewolfModel)

	precache_model( Mdl_Path );

	formatex( Mdl_Path, sizeof Mdl_Path - 1, "models/player/%s/%sT.mdl", WerewolfModel, WerewolfModel);

	if ( file_exists( Mdl_Path ) )
	{
		precache_model( Mdl_Path );
	}
}

public client_disconnect(id)
{
	g_IsWereWolf[id] = false;

	g_MaxHp[id] = 0;

	remove_task(id + TASKID_WEREWOLF);
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

		if ( get_p_skill( id, g_SkillId ) > 0 )
		{
			if ( !g_IsWereWolf[id] )
			{
				if ( get_p_mana(id) >= Mana_WereWolf )
				{
					Set_Sprite_Task(id, WerewolfSpr, 3.0, 1, 1.1, "Morph");

					emit_sound(id, CHAN_ITEM, WerewolfMetamorphosys, 1.0, ATTN_NORM, 0, PITCH_NORM);

					g_IsWereWolf[id] = true;

					set_p_mana(id, get_p_mana(id) - Mana_WereWolf )

					cs_set_user_model(id, WerewolfModel);

					set_task(20.0 + (get_p_skill( id, g_SkillId2 ) > 0 ? Werewolf_Lycanthropy_Dur[ get_p_skill( id, g_SkillId2 ) - 1] : 0.0), "Reset_WereWolf", id + TASKID_WEREWOLF);
						
					if ( get_p_skill( id, g_SkillId2 ) > 0 )
					{
						g_MaxHp[id] = (get_p_maxhealth(id) * Werewolf_Lycanthropy_Hp[ get_p_skill( id, g_SkillId2 ) - 1 ] / 100);

						set_p_maxhealth(id, get_p_maxhealth(id) + g_MaxHp[id] );
					}
				}
			}
			else
			{
				Set_Sprite_Task(id, WerewolfSpr, 3.0, 1, 1.1, "Morph");

				emit_sound(id, CHAN_ITEM, WerewolfMetamorphosys, 1.0, ATTN_NORM, 0, PITCH_NORM);

				Reset_WereWolf(id + TASKID_WEREWOLF);
			}
		}
	}
	
	return PLUGIN_CONTINUE;
}
public fwd_AttackSpeed ( const Entity )
{
	if ( !pev_valid(Entity) ) return HAM_IGNORED;

	static id ; id = get_pdata_cbase(Entity, m_pPlayer, 4)
	
	if ( ( 1 <= id <= g_iMaxPlayers ) ) 
	{
		if ( get_p_skill( id, g_SkillId ) > 0 && g_IsWereWolf[id] && get_p_hero(id) == DRUID ) 
		{ 
			set_pdata_float( Entity, m_flPrimaryAttack, Werewolf_Attack_Speed[ get_p_skill( id, g_SkillId ) - 1 ], 4 ); 
		} 
	}
	
	return HAM_IGNORED;
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		g_IsWereWolf[id] = false;

		remove_task(id + TASKID_WEREWOLF);
	}
}


public d2_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( g_iCurSkill[attacker] == g_SkillId3 )
	{
		if ( !IsPlayerNearByMonster(victim) && g_IsWereWolf[attacker] && get_p_skill( attacker, g_SkillId3 ) > 0 && get_p_hero(attacker) == DRUID && get_p_mana(attacker) >= Mana_Hunger )
		{
			new Counted_Data = get_user_health(attacker) + (get_user_health(victim) * Werewolf_LifeSteal[ get_p_skill( attacker, g_SkillId3 ) - 1 ] / 100);

			if ( Counted_Data > get_p_maxhealth(attacker) )
				Counted_Data = get_p_maxhealth(attacker);

			set_user_health( attacker, Counted_Data );
	
			Counted_Data = get_p_mana(attacker) + (get_p_mana(victim) * Werewolf_LifeSteal[ get_p_skill( attacker, g_SkillId3 ) - 1 ] / 100);

			set_p_mana(attacker, get_p_mana(attacker) - Mana_WereWolf + Counted_Data );

			new Float:counted_damage; counted_damage = (iDamage[0] - (iDamage[0] * Hunger_Damage_Minus / 100));

			iDamage[0] = (counted_damage < 7.0 ? 7.0 : counted_damage);
		}
	}
	else if ( g_iCurSkill[attacker] == g_SkillId4 )
	{
		if ( !IsPlayerNearByMonster(victim) && g_IsWereWolf[attacker] && get_p_skill( attacker, g_SkillId4 ) > 0 && get_p_hero(attacker) == DRUID && get_p_mana(attacker) >= Mana_FireClaws )
		{
			Set_Sprite_Task(victim, WerewolfSprFireClaws, 1.4, 1, 0.7, "FireClaws");

			iDamage[0] = (iDamage[0] + Werewolf_DmgFireClaws[ get_p_skill( attacker, g_SkillId4 ) - 1 ]);
		}
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
public Reset_WereWolf(id)
{
	id -= TASKID_WEREWOLF;

	if ( g_IsWereWolf[id] && get_p_hero(id) == DRUID )
	{
		g_IsWereWolf[id] = false;

		if ( is_user_alive(id) )
		{
			set_p_maxhealth(id, get_p_maxhealth(id) - g_MaxHp[id] );

			reset_p_model(id);
		}
	}

	remove_task(id + TASKID_WEREWOLF);
}