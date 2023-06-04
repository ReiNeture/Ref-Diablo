#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "雷電斬"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 30;
new Mana_Fist = 25;

new const FistCast[] = "d2lod/vengeanceimpact3.wav";
new const FireCastSpr[] = "sprites/muzzleflash2.spr";

new const Float:FistDamage[MAX_P_SKILLS] =  // 雷電的傷害.
{
	40.0, 50.0, 60.0, 70.0, 75.0, 80.0, 90.0, 95.0, 100.0, 110.0, 120.0, 130.0, 	140.0, 150.0, 160.0, 170.0, 180.0, 190.0, 200.0, 205.0
};
new const Float:FistBoltDamage[MAX_P_SKILLS] =  // 雷電球的傷害.
{
	5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 25.0
};

#define TARGET_DISTANCE 500.0

new g_SkillId;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];
new g_lightning;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "對瞄準的敵人進行制裁.", PALADIN, Skill_Level, DISPLAY)

	register_forward(FM_Touch, "Entity_Touched");
}
public plugin_precache()
{
	precache_sound( FistCast );
	precache_model( FireCastSpr );
	g_lightning = precache_model( "sprites/lgtning.spr" );
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id
}
public Entity_Touched(ent, victim)
{
	if ( !pev_valid(ent) )
		return;
	
	new classname[32]
	pev( ent, pev_classname, classname, 31)

	new attacker = entity_get_edict(ent, EV_ENT_owner);
	
	if(equal(classname,"FistBolt")) 
	{
		if ( is_user_alive(victim) && victim != attacker && !IsPlayerNearByMonster(victim) && !is_p_protected(victim) && get_p_skill( attacker, g_SkillId ) > 0 )
		{
			dmg_kill_player(victim, attacker, FistBoltDamage[get_p_skill( attacker, g_SkillId ) - 1], "fistbolt");
		}	

		set_pev( ent, pev_flags, FL_KILLME);
	}
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		static Float:cdown;
		cdown = 2.5;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		
		static target, body;
		new Float:aim_distance = get_user_aiming(id, target, body), TOrigin[3], EndOrigin[3];
		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= Mana_Fist )
		{
			if (  aim_distance <= TARGET_DISTANCE )
			{
				if ( is_user_alive(target) && !IsPlayerNearByMonster(target) && !is_p_protected(target) )
				{
					if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
					{

						set_p_mana( id, get_p_mana(id) - Mana_Fist);

						get_user_origin( target, TOrigin);

						TOrigin[ 2 ] -= 26;
						EndOrigin[ 0 ] = TOrigin[ 0 ] + 150;
						EndOrigin[ 1 ] = TOrigin[ 1 ] + 150;
						EndOrigin[ 2 ] = TOrigin[ 2 ] + 800;

						Thunder( EndOrigin, TOrigin )

						dmg_kill_player(target, id, FistDamage[ get_p_skill( id, g_SkillId ) - 1], "fistofheaven");


						Set_Sprite_FireBolt(id, target, FireCastSpr, 50.0, 0.3, "FistBolt");


						g_LastPressedSkill[id] = get_gametime()
					}
				}
			}
		}
	}

	return PLUGIN_CONTINUE
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
Thunder( start[ 3 ], end[ 3 ] )
{
	message_begin( MSG_BROADCAST, SVC_TEMPENTITY ); 
	write_byte( TE_BEAMPOINTS ); 
	write_coord( start[ 0 ] ); 
	write_coord( start[ 1 ] ); 
	write_coord( start[ 2 ] ); 
	write_coord( end[ 0 ] ); 
	write_coord( end[ 1 ] ); 
	write_coord( end[ 2 ] ); 
	write_short( g_lightning ); 
	write_byte( 1 ); // 幀幅開始.
	write_byte( 5 ); // 幀幅頻率.
	write_byte( 6 ); // 時間長度.
	write_byte( 150 ); // 寬度.
	write_byte( 50 ); // 響聲.
	write_byte( 200 ); // 顏色 R.
	write_byte( 200 ); // 顏色 G.
	write_byte( 200 ); // 顏色 B.
	write_byte( 200 ); // 顏色亮度.
	write_byte( 200 ); // 速度.
	message_end();
	
	emit_sound( 0 , CHAN_ITEM, FistCast, 1.0, ATTN_NORM, 0, PITCH_NORM );
}
public Set_Sprite_FireBolt(id, target, const sprite[], Float:framerate, Float:scale, const classname[])
{
	for ( new i = 0; i < 7; i++)
	{
		new sprite_ent = create_entity("env_sprite")

		entity_set_string( sprite_ent, EV_SZ_classname, classname)
		entity_set_model( sprite_ent, sprite);

		entity_set_edict( sprite_ent, EV_ENT_owner, id)

		entity_set_size( sprite_ent, Float:{-2.1, -2.1, -2.1}, Float:{2.1, 2.1, 2.1})

		entity_set_int( sprite_ent, EV_INT_rendermode, kRenderTransAdd)
		entity_set_float( sprite_ent, EV_FL_renderamt, 200.0 )
    
		entity_set_float( sprite_ent, EV_FL_framerate, framerate )
		entity_set_float( sprite_ent, EV_FL_scale, scale )

		DispatchSpawn(sprite_ent);
		entity_set_int( sprite_ent, EV_INT_spawnflags, SF_SPRITE_STARTON)

		entity_set_int( sprite_ent, EV_INT_movetype, MOVETYPE_FLY)
		entity_set_int( sprite_ent, EV_INT_solid, SOLID_TRIGGER)

		new Origin[3], RandomOrigin[3], EndOrigin[3], RandomX, RandomY;

		get_user_origin( target, Origin);
	
		RandomX = random_num(-100,100)
		RandomY = random_num(-100,100)

		RandomOrigin[0] = Origin[0] + 1 * RandomX;
		RandomOrigin[1] = Origin[1] + 1 * RandomY;
		RandomOrigin[2] = Origin[2] + 30;

		new Float:LocVec[3]
		IVecFVec(RandomOrigin, LocVec)
                
		EndOrigin[0] = RandomOrigin[0]
		EndOrigin[1] = RandomOrigin[1]
		EndOrigin[2] = RandomOrigin[2] - 5

		new Float:EndVec[3]
		IVecFVec(EndOrigin, EndVec)
                
		new Float:VeloVec[3]
 		VeloVec[0] = (EndVec[0] - LocVec[0]) * 1;
		VeloVec[1] = (EndVec[1] - LocVec[1]) * 1 + 280;
		VeloVec[2] = (EndVec[2] - LocVec[2]) * 1 + 1;
		entity_set_origin( sprite_ent, LocVec) // 開始時的座標.
		entity_set_vector( sprite_ent, EV_VEC_velocity, VeloVec) // 跟隨.
	}
}