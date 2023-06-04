#include <amxmodx>
#include <engine>
#include <fakemeta>
#include <d2lod>

new PLUGIN_NAME[] = "爆炸箭"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 12;

new const g_SpriteExplode[] = "sprites/explosion1.spr";

new const ManaExplodingArrow[MAX_P_SKILLS] =  // 爆炸箭需要的能量.
{
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 14, 15, 15, 15, 15, 16, 17, 18, 19
};
new const Float:ExplodingArrowDamage[MAX_P_SKILLS] =  // 爆炸箭傷害.
{
	10.0, 20.0, 25.0, 30.0, 35.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 	110.0, 120.0, 125.0, 130.0, 140.0, 150.0, 155.0, 160.0
};

new g_SkillId;

new bool: g_ShootWithFireArrow[33];
new g_iCurSkill[33];

new g_spriteBall;

new g_iMaxPlayers;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "將爆炸屬性付在弓箭上.", AMAZON, Skill_Level, DISPLAY)

	g_iMaxPlayers = get_maxplayers();
}

public plugin_precache()
{
	g_spriteBall = precache_model( g_SpriteExplode );
}

public client_disconnect(id)
{
	g_ShootWithFireArrow[id] = false;
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id
}

public d2_skill_fired(id)
{

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		g_ShootWithFireArrow[id] = false;
	}
}

public d2_ranged_actshoot( id, entity )
{
	if ( get_p_hero(id) == AMAZON && get_p_skill( id, g_SkillId ) > 0 && g_iCurSkill[id] == g_SkillId
	&& get_p_mana(id) >= ManaExplodingArrow[ get_p_skill( id, g_SkillId ) - 1 ] )
	{
		g_ShootWithFireArrow[id] = true;

		set_p_mana( id, get_p_mana(id) - ManaExplodingArrow[ get_p_skill( id, g_SkillId ) - 1 ]);
	}
}

public d2_ranged_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( get_p_hero(attacker) == AMAZON && get_p_skill( attacker, g_SkillId ) > 0 && g_ShootWithFireArrow[attacker] )
	{
		new Float: Torigin[3], Float: Distance, Float: Damage;

		new Float:fOrigin[3], iOrigin[3];
		entity_get_vector( victim, EV_VEC_origin, fOrigin)	
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
					Damage = (((Distance / 175.0) * ExplodingArrowDamage[get_p_skill( attacker, g_SkillId ) - 1]) - ExplodingArrowDamage[get_p_skill( attacker, g_SkillId ) - 1]) * -1.0;

					if (Damage > 0.0 && attacker != enemy)
					{
						dmg_kill_player(enemy, attacker, Damage, "exploding_arrow");
					}
				}
			}
		}

		g_ShootWithFireArrow[attacker] = false;
	}
}