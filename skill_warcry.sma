#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "怒吼"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 30;

new const BarManaWarC[MAX_P_SKILLS] =  // 怒吼需要的能量.
{
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
};
new const Float:BarWarCDur[MAX_P_SKILLS] =  // 野蠻人怒吼後,敵人被固定的時間.
{
	0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 2.9, 3.0, 3.2, 3.4, 3.6, 3.7, 3.8, 4.0
};
new const Float:BarWarCDmg[MAX_P_SKILLS] =  // 野蠻人怒吼傷害.
{
	20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 80.0, 	90.0, 100.0, 105.0, 110.0, 115.0, 120.0, 125.0, 150.0
};

new const BarbarianWarCry[] = "d2lod/warcry.wav";
new const g_SpriteWarCry[] = "sprites/shockwave.spr";

#define TASKID_STUN 19444

new g_SkillId;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];
new g_spriteShockwave;
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "對敵人造成傷害並凍結敵人.", BARBARIAN, Skill_Level, DISPLAY)

	register_event("DeathMsg", "ev_DeathMsg", "a")

	g_iMaxPlayers = get_maxplayers();
}
public ev_DeathMsg()
{
	Stunned_Duration( read_data(2) + TASKID_STUN );
}

public plugin_precache()
{
	g_spriteShockwave = precache_model( g_SpriteWarCry );
	precache_sound( BarbarianWarCry );
}

public client_disconnect(id)
{
	Stunned_Duration( id + TASKID_STUN );
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
		cdown = 1.5;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= BarManaWarC[ get_p_skill( id, g_SkillId ) - 1 ] )
		{
			emit_sound(id, CHAN_ITEM, BarbarianWarCry, 1.0, ATTN_NORM, 0, PITCH_NORM);

			set_p_mana(id, get_p_mana(id) - BarManaWarC[ get_p_skill( id, g_SkillId ) - 1 ] );

			new Float: Porigin[3], Float: Torigin[3], Float: Distance, iOrigin[3];

			entity_get_vector( id, EV_VEC_origin, Porigin);
					
			iOrigin[0] = floatround(Porigin[0]);
			iOrigin[1] = floatround(Porigin[1]);
			iOrigin[2] = floatround(Porigin[2]);

			message_begin( MSG_PAS, SVC_TEMPENTITY, iOrigin );
			write_byte( TE_BEAMCYLINDER );
			engfunc( EngFunc_WriteCoord, Porigin[0]);
			engfunc( EngFunc_WriteCoord, Porigin[1]);
			engfunc( EngFunc_WriteCoord, Porigin[2] - 16.0);
			engfunc( EngFunc_WriteCoord, Porigin[0]);
			engfunc( EngFunc_WriteCoord, Porigin[1]);
			engfunc( EngFunc_WriteCoord, Porigin[2] - 16.0 + 250.0);
			write_short( g_spriteShockwave );
			write_byte( 0 );	// 幀幅開始.
			write_byte( 0 );	// 幀幅頻率.
			write_byte( 3 );	// 時間長度.
			write_byte( 12 );	// 寬度.
			write_byte( 0 );	// 響聲.
			write_byte( 255 );  // 顏色 R.
			write_byte( 90 );  // 顏色 G.
			write_byte( 0 );  // 顏色 B.
			write_byte( 255 );	// 顏色亮度.
			write_byte( 8 );	// 速度.
			message_end();

			for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
			{
				if ( is_user_alive(enemy) && id != enemy )
				{
					entity_get_vector( enemy, EV_VEC_origin, Torigin);

					Distance = get_distance_f(Porigin, Torigin);

					if ( Distance <= 250.0 && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) )
					{
						dmg_kill_player(enemy, id, BarWarCDmg[ get_p_skill( id, g_SkillId ) - 1 ], "warcry");

						set_pev( enemy , pev_flags , pev( enemy , pev_flags ) | FL_FROZEN );

						set_task(BarWarCDur[ get_p_skill( id, g_SkillId ) - 1 ], "Stunned_Duration", enemy + TASKID_STUN);
					}
				}
			}
		}
	}
	
	return PLUGIN_CONTINUE;
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		Stunned_Duration( id + TASKID_STUN );
	}
}


public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}
public Stunned_Duration(id)
{
	id -= TASKID_STUN;

	if ( !(1 <= id <= g_iMaxPlayers) )
		return;

	set_pev( id , pev_flags , pev( id , pev_flags ) & ~FL_FROZEN );

	remove_task( id + TASKID_STUN );
}