#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>

new PLUGIN_NAME[] = "骨之靈魂"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 30;
new Mana_BoneSpirit = 15;

new const SkullSpr[] = "sprites/skull.spr";
new const BoneSpirit[] = "d2lod/bonespirit.wav";

new const Float:NecroSpiritDmg[MAX_P_SKILLS] =  // 法師的靈魂攻擊傷害值.
{
	30.0, 50.0, 65.0, 75.0, 85.0, 95.0, 105.0, 120.0, 130.0, 140.0, 150.0, 160.0, 	165.0, 170.0, 175.0, 180.0, 185.0, 190.0, 195.0, 200.0
};

#define TASKID_BONESPIRIT 29444

new g_SkillId, g_spriteBlueFlare;

new Float:g_LastPressedSkill[33];
new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "創造一個靈魂,追蹤並且攻擊敵人.", NECROMANCER, Skill_Level, DISPLAY)

	register_forward(FM_Touch, "Entity_Touched");
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheSound, BoneSpirit);
	engfunc(EngFunc_PrecacheModel, SkullSpr);
	g_spriteBlueFlare = engfunc(EngFunc_PrecacheModel, "sprites/blueflare2.spr");
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

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= Mana_BoneSpirit )
		{
				emit_sound(id, CHAN_ITEM, BoneSpirit, 1.0, ATTN_NORM, 0, PITCH_NORM);

				set_p_mana( id, get_p_mana(id) - Mana_BoneSpirit);

				Set_Sprite_FireBolt(id, SkullSpr, 20.0, 0.2, "BoneSpirit", 1);
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
	
	if ( equal(classname,"BoneSpirit") ) 
	{
		if ( is_user_alive(victim) && victim != attacker && !is_p_protected(victim) && !IsPlayerNearByMonster(victim) && get_p_skill( attacker, g_SkillId ) > 0 )
		{
			dmg_kill_player(victim, attacker, NecroSpiritDmg[get_p_skill( attacker, g_SkillId ) - 1], "bone_spirit");

			new Torigin[3];
			get_user_origin( victim, Torigin );

			new vStartOrigin[3];

			vStartOrigin[0] = Torigin[0];
			vStartOrigin[1] = Torigin[1];
			vStartOrigin[2] = Torigin[2] + 40;

			Create_SpriteTrail( vStartOrigin, Torigin, g_spriteBlueFlare, 30, 10, 1, 50, 10 );

		}	

		set_pev( ent, pev_flags, FL_KILLME);

		remove_task(ent + TASKID_BONESPIRIT);
	}
}
public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}
public Set_Sprite_FireBolt(id, const sprite[], Float:framerate, Float:scale, const classname[], follow)
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

	if ( follow )
	{
		set_task(0.1, "sprite_follow", sprite_ent + TASKID_BONESPIRIT, _, _, "b");
	}

}
public sprite_follow(ent) 
{
	ent -= TASKID_BONESPIRIT;
	
	new Float: shortest_distance = 450.0;
	new NearestPlayer = 0;

	if (pev_valid(ent)) 
	{
		new id_owner = entity_get_edict(ent, EV_ENT_owner);

		new iClient[32], livePlayers, iNum;
		get_players(iClient, livePlayers, "a"); 

		for(iNum = 0; iNum < livePlayers; iNum++) 
		{ 
			if ( is_user_alive(iClient[iNum]) ) 
			{
				if ( id_owner != iClient[iNum] )
				{
					new Float:PlayerOrigin[3], Float:RocketOrigin[3]
					entity_get_vector( ent, EV_VEC_origin, RocketOrigin)
					entity_get_vector( iClient[iNum], EV_VEC_origin, PlayerOrigin)
					
					new Float: distance = get_distance_f(PlayerOrigin, RocketOrigin)
					
					if ( distance <= shortest_distance )
					{
						shortest_distance = distance;
						NearestPlayer = iClient[iNum];
					}
				}
			}
		}
		if (NearestPlayer > 0) 
		{
			entity_set_follow(ent, NearestPlayer, 500.0)
		}

	}
}
stock Create_SpriteTrail(start[3], end[3], iSprite, count, life, scale, velocity, random ){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte( TE_SPRITETRAIL )
	write_coord( start[0] )				// 開始時的座標 (X).
	write_coord( start[1] )				// 開始時的座標 (Y).
	write_coord( start[2] )				// 開始時的座標 (Z).
	write_coord( end[0] )				// 結束的座標 (X).
	write_coord( end[1] )				// 結束的座標 (Y).
	write_coord( end[2] )				// 結束的座標 (Z).
	write_short( iSprite )				// Sprite的目錄.
	write_byte( count )					// 數量.
	write_byte( life)					// 時間長度.
	write_byte( scale)					// 範圍時間.
	write_byte( velocity )				// 動量.
	write_byte( random )				// 隨機動量.
	message_end()
}
stock entity_set_follow(entity, target, Float:speed) 
{
	if(!pev_valid(entity) || !is_user_alive(target)) 
		return 0

	new Float:entity_origin[3], Float:target_origin[3]
	pev(entity, pev_origin, entity_origin)
	pev(target, pev_origin, target_origin)

	new Float:diff[3]
	diff[0] = target_origin[0] - entity_origin[0]
	diff[1] = target_origin[1] - entity_origin[1]
	diff[2] = target_origin[2] - entity_origin[2]

	new Float:length = floatsqroot(floatpower(diff[0], 2.0) + floatpower(diff[1], 2.0) + floatpower(diff[2], 2.0))

	new Float:velocity[3]
	velocity[0] = diff[0] * (speed / length)
	velocity[1] = diff[1] * (speed / length)
	velocity[2] = diff[2] * (speed / length)

	set_pev(entity, pev_velocity, velocity)

	return 1
}
