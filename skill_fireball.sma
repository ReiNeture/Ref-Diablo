#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "爆炸火球"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 12;

new const SorcFireCast[] = "d2lod/firecast.wav";
new const OnPFireSpr[] = "sprites/xfire2.spr";
new const FireCast[] = "sprites/rjet1.spr";
new const g_SpriteExplode[] = "sprites/explosion1.spr";

new const SorcaManaFireBall[MAX_P_SKILLS] =  // 發射火球需要的能量.
{
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 14, 15, 15, 15, 15, 16, 17, 18, 19
};
new const Float:FireBallDamage[MAX_P_SKILLS] =  // 術士的火球傷害.
{
	10.0, 20.0, 25.0, 30.0, 35.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 	110.0, 120.0, 125.0, 130.0, 140.0, 150.0, 155.0, 160.0
};

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new g_spriteBall;
new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "發射一顆會爆炸的火球.", SORCERESS, Skill_Level, DISPLAY)

	register_forward(FM_Touch, "Entity_Touched");

	g_iMaxPlayers = get_maxplayers();
}

public plugin_precache()
{
	precache_sound( SorcFireCast );
	precache_model( OnPFireSpr );
	precache_model( FireCast );
	g_spriteBall = precache_model( g_SpriteExplode );
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
		cdown = 1.0;

		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= SorcaManaFireBall[ get_p_skill( id, g_SkillId ) - 1 ] )
		{
			emit_sound(id, CHAN_ITEM, SorcFireCast, 1.0, ATTN_NORM, 0, PITCH_NORM);

			set_p_mana( id, get_p_mana(id) - SorcaManaFireBall[ get_p_skill( id, g_SkillId ) - 1 ]);

			Set_Sprite_FireBolt(id, FireCast, 50.0, 0.7, "FireBall");

			Set_Sprite_Task(id, OnPFireSpr, 2.7, 1, 0.8, "Morph");
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
	
	if(equal(classname,"FireBall")) 
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
		write_short(g_spriteBall);
		write_byte(30); // 幀幅開始.
		write_byte(30); // 幀幅頻率.
		write_byte(0); // 標記.
		message_end();

		for(new enemy = 1; enemy <= g_iMaxPlayers; enemy++) 
		{
			if ( is_user_alive(enemy) )
			{
				entity_get_vector( enemy, EV_VEC_origin, Torigin)

				Distance = get_distance_f(fOrigin, Torigin);

				if ( Distance <= 175.0 && !IsPlayerNearByMonster(enemy) && !is_p_protected(enemy) && get_p_skill( attacker, g_SkillId ) > 0 )
				{
					Damage = (((Distance / 175.0) * FireBallDamage[get_p_skill( attacker, g_SkillId ) - 1]) - FireBallDamage[get_p_skill( attacker, g_SkillId ) - 1]) * -1.0;

					if (Damage > 0.0 && attacker != enemy)
					{
						dmg_kill_player(enemy, attacker, Damage, "fireball");
					}
				}
			}
		}

		set_pev( ent, pev_flags, FL_KILLME);
	}
}
public d2_takedamage(victim, attacker, Float:iDamage[1])
{

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
public Set_Sprite_FireBolt(id, const sprite[], Float:framerate, Float:scale, const classname[])
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
	entity_set_int( sprite_ent, EV_INT_solid, SOLID_BBOX)

	new Float:fAim[3],Float:fAngles[3],Float:fOrigin[3];

	velocity_by_aim(id,64,fAim)
	vector_to_angle(fAim,fAngles)
	entity_get_vector( id, EV_VEC_origin, fOrigin)
	
	fOrigin[0] += fAim[0]
	fOrigin[1] += fAim[1]
	fOrigin[2] += fAim[2] + 25.0
	
	entity_set_vector( sprite_ent, EV_VEC_origin, fOrigin)
	entity_set_vector( sprite_ent, EV_VEC_angles, fAngles)
	
	new Float:fVel[3]
	velocity_by_aim(id, 500, fVel)	

	entity_set_vector( sprite_ent, EV_VEC_velocity, fVel)
}