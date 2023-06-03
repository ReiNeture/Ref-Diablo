#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>

new PLUGIN_NAME[] = "流星攻擊"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 30;

new const SorcMeteorCast[] = "d2lod/meteorlaunch.wav";
new const SorcMeteorMdl[] = "models/d2lod/head.mdl";
new const g_SpriteExplode[] = "sprites/explosion1.spr";
new const FireSpr[] = "sprites/flame.spr";

#define RADIUS_DAMAGE 350.0
#define END_BURN_TIME 3.5
#define METEOR_WAIT_TIME 3.0

new const SorcaManaMeteor[MAX_P_SKILLS] =  // 流星攻擊需要的能量.
{
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200
};
new const Float:MeteorDamage[MAX_P_SKILLS] =  // 術士流星攻擊的傷害.
{
	50.0, 60.0, 70.0, 80.0, 90.0, 95.0, 100.0, 105.0, 110.0, 115.0, 120.0, 125.0, 	130.0, 140.0, 150.0, 160.0, 170.0, 180.0, 190.0, 200.0
};
new const Float:MeteorDamageBurn[MAX_P_SKILLS] =  // 術士火焰的傷害.
{
	1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 11.0
};

#define TASKID_METEOR 4343
#define TASKID_WARN 2343
#define TASKID_BURN 1343

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new g_spriteMeteor;
new g_spriteShockwave, firespr;
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "降下流星雨攻擊你的敵人.", AMAZON, Skill_Level, DISPLAY)

	register_forward(FM_Touch, "Entity_Touched");

	g_iMaxPlayers = get_maxplayers();
}

public plugin_precache()
{
	precache_sound( SorcMeteorCast );
	precache_model( SorcMeteorMdl );
	g_spriteMeteor = precache_model( g_SpriteExplode );
	g_spriteShockwave = precache_model( "sprites/shockwave.spr" );
	firespr = precache_model( FireSpr );
}

public client_disconnect(id)
{
	remove_task( id + TASKID_METEOR);
	remove_task( id + TASKID_WARN);
	remove_task( TASKID_BURN );
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
		cdown = 0.7;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= SorcaManaMeteor[ get_p_skill( id, g_SkillId ) - 1 ] )
		{
			emit_sound(id, CHAN_ITEM, SorcMeteorCast, 1.0, ATTN_NORM, 0, PITCH_NORM);

			set_p_mana( id, get_p_mana(id) - SorcaManaMeteor[ get_p_skill( id, g_SkillId ) - 1 ] );

			new AimOrigin[3];

			get_user_origin( id, AimOrigin, 3);


			set_task( 0.7, "Task_Warn", id + TASKID_WARN, AimOrigin, sizeof AimOrigin, "b");
			set_task( METEOR_WAIT_TIME, "Task_Meteor", id + TASKID_METEOR, AimOrigin, sizeof AimOrigin);
		}
	}
	
	return PLUGIN_CONTINUE;
}

public Entity_Touched(ent, victim)
{
	if ( !pev_valid(ent) )
		return;
	
	new classname[32], Param[4];
	pev( ent, pev_classname, classname, 31)

	new attacker = entity_get_edict(ent, EV_ENT_owner);
	
	if( equal(classname,"Meteor") ) 
	{
		new Float:Torigin[3], Float:Distance, Float:Damage;

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
		write_short(g_spriteMeteor);
		write_byte(30); // 幀幅開始.
		write_byte(25); // 幀幅頻率.
		write_byte(TE_EXPLFLAG_NOPARTICLES); // 標記.
		message_end();

		Param[0] = floatround(fOrigin[0]);
		Param[1] = floatround(fOrigin[1]);
		Param[2] = floatround(fOrigin[2]);
		Param[3] = attacker;

		set_task( 0.2, "Task_Burn", TASKID_BURN, Param, sizeof Param, "b");
		set_task( END_BURN_TIME, "Burn_End");

		Damage = MeteorDamage[get_p_skill(attacker, g_SkillId) - 1] + float(get_p_magic(attacker)) * 0.6;
		new npcname[32];
		new victim = FM_NULLENT;

		while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, RADIUS_DAMAGE) ) != 0 ) {

			pev(victim, pev_classname, npcname, sizeof(npcname));
			if( is_user_alive(victim) && attacker != victim && !IsPlayerNearByMonster(victim) && !is_p_protected(victim) && get_p_skill(attacker, g_SkillId) > 0 )
			{
				dmg_kill_player(victim, attacker, Damage, "fireball");
			}
			else if( !is_user_alive(victim) && equal(npcname, "func_wall") )
			{
				ExecuteHam(Ham_TakeDamage, victim, ent, attacker, Damage, DMG_BURN);
			}
		}

		/*
		for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
		{
			if ( is_user_alive(enemy) )
			{
				entity_get_vector( enemy, EV_VEC_origin, Torigin)
				Distance = get_distance_f(fOrigin, Torigin);
				if ( Distance <= RADIUS_DAMAGE && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) && get_p_skill( attacker, g_SkillId ) > 0 )
				{
					Damage = (((Distance / RADIUS_DAMAGE) * MeteorDamage[get_p_skill( attacker, g_SkillId ) - 1]) - MeteorDamage[get_p_skill( attacker, g_SkillId ) - 1]) * -1.0;
					if (Damage > 0.0 && attacker != enemy)
					{
						dmg_kill_player(enemy, attacker, Damage, "meteor");
					}
				}
			}
		}
		*/

		set_pev( ent, pev_flags, FL_KILLME);
	}
}
public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}
public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		remove_task( id + TASKID_METEOR);
		remove_task( id + TASKID_WARN);
		remove_task( TASKID_BURN );
	}
}
public Burn_End()
{
	remove_task( TASKID_BURN );
}
public Task_Burn(Param[])
{
	new Float:fOrigin[3], Float:Torigin[3], Float:Distance;
	fOrigin[0] = float(Param[0])
	fOrigin[1] = float(Param[1])
	fOrigin[2] = float(Param[2])
	new attacker = Param[3];

	message_begin( MSG_BROADCAST, SVC_TEMPENTITY);
	write_byte( TE_SPRITE );
	engfunc( EngFunc_WriteCoord, fOrigin[0] );
	engfunc( EngFunc_WriteCoord, fOrigin[1] );
	engfunc( EngFunc_WriteCoord, fOrigin[2] + 50.0 );
	write_short( firespr );
	write_byte( random_num(5, 8) ); // 幀幅開始.
	write_byte( 120 ); // 亮度.
	message_end();

	for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
	{
		if ( is_user_alive(enemy) )
		{
			entity_get_vector( enemy, EV_VEC_origin, Torigin)

			Distance = get_distance_f(fOrigin, Torigin);

			if ( Distance <= 70.0 && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) && get_p_skill( attacker, g_SkillId ) > 0 )
			{
				dmg_kill_player(enemy, attacker, MeteorDamageBurn[get_p_skill( attacker, g_SkillId ) - 1], "meteorfire");
			}
		}
	}
}
public Task_Warn(AimOrigin[3], id)
{
	id -= TASKID_WARN;

	if ( !is_user_connected(id) )
		return;

	AimOrigin[2] += 20;

	message_begin( MSG_PAS, SVC_TEMPENTITY, AimOrigin );
	write_byte( TE_BEAMCYLINDER );
	engfunc( EngFunc_WriteCoord, float(AimOrigin[0]));
	engfunc( EngFunc_WriteCoord, float(AimOrigin[1]));
	engfunc( EngFunc_WriteCoord, float(AimOrigin[2]) - 16.0);
	engfunc( EngFunc_WriteCoord, float(AimOrigin[0]));
	engfunc( EngFunc_WriteCoord, float(AimOrigin[1]));
	engfunc( EngFunc_WriteCoord, float(AimOrigin[2]) - 16.0 + RADIUS_DAMAGE + 200.0);
	write_short( g_spriteShockwave );
	write_byte( 0 );	// 幀幅開始.
	write_byte( 0 );	// 幀幅頻率.
	write_byte( 3 );	// 時間長度.
	write_byte( 50 );	// 寬度.
	write_byte( 0 );	// 響聲.
	write_byte( 255 );  // 顏色 R.
	write_byte( 90 );  // 顏色 G.
	write_byte( 0 );  // 顏色 B.
	write_byte( 255 );	// 顏色亮度.
	write_byte( 8 );	// 速度.
	message_end();

}
public Task_Meteor(AimOrigin[], id)
{
	id -= TASKID_METEOR;
	if ( !is_user_connected(id) )
		return;

	remove_task( id + TASKID_WARN);

	new RandomX, RandomY, RandomOrigin[3], EndOrigin[3];
	AimOrigin[2] += 230;

	for ( new i = 1; i <= 7; i++)
	{
		RandomX = random_num(-250, 250)
		RandomY = random_num(-250, 250)

		RandomOrigin[0] = AimOrigin[0] + 1 * RandomX;
		RandomOrigin[1] = AimOrigin[1] + 1 * RandomY;
		RandomOrigin[2] = AimOrigin[2];

		new Float:LocVec[3]
		IVecFVec(RandomOrigin, LocVec)
                
		EndOrigin[0] = RandomOrigin[0]
		EndOrigin[1] = RandomOrigin[1]
		EndOrigin[2] = RandomOrigin[2] - 5

		new Float:EndVec[3]
		IVecFVec(EndOrigin, EndVec)
                
		new Float:VeloVec[3]
 		VeloVec[0] = (EndVec[0] - LocVec[0]) * 1 + 1;
		VeloVec[1] = (EndVec[1] - LocVec[1]) * 1 + 1;
		VeloVec[2] = (EndVec[2] - LocVec[2]) * 1 - 540;

		new ent = create_entity("info_target") // 創造流星.

		entity_set_string( ent, EV_SZ_classname, "Meteor")
		entity_set_edict( ent, EV_ENT_owner, id)
		entity_set_int( ent, EV_INT_solid, SOLID_TRIGGER) 
		entity_set_int( ent, EV_INT_movetype, MOVETYPE_TOSS) 
		entity_set_model( ent, SorcMeteorMdl) 
		entity_set_origin( ent, LocVec) // 開始時的座標.
		entity_set_vector( ent, EV_VEC_velocity, VeloVec) // 跟隨. 
	}
}
