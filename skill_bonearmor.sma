#include <amxmodx>
#include <d2lod>
#include <engine>

new PLUGIN_NAME[] = "骨之盔甲"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 1;
new Mana_BoneArmor = 20;

new const NecroAbDmg[MAX_P_SKILLS] =  // 法師吸收的傷害.
{
	15, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210
};

new const BoneArmorSnd[] = "d2lod/bonearmor.wav";
new const BoneArmorSpr[] = "sprites/cnt1.spr";

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];
new g_AbsorbDamage[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "吸收敵人傷害.", NECROMANCER, Skill_Level, DISPLAY)

	register_event("DeathMsg", "ev_DeathMsg", "a")
}
public ev_DeathMsg()
{
	Reset_BoneArmor( read_data(2) );

	g_AbsorbDamage[ read_data(2) ] = 0;
}

public plugin_precache()
{
	precache_sound( BoneArmorSnd );
	precache_model( BoneArmorSpr );
}

public client_disconnect(id)
{
	Reset_BoneArmor(id);

	g_AbsorbDamage[id] = 0;
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

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= Mana_BoneArmor )
		{
			emit_sound(id, CHAN_ITEM, BoneArmorSnd, 1.0, ATTN_NORM, 0, PITCH_NORM);

			set_p_mana( id, get_p_mana(id) - Mana_BoneArmor);

			g_AbsorbDamage[id] = NecroAbDmg[get_p_skill( id, g_SkillId ) - 1];

			Reset_BoneArmor(id);
			Set_Sprite_Task(id, BoneArmorSpr, 0.6, 0, 0.0, "BoneArmor");
		}
	}
	
	return PLUGIN_CONTINUE;
}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		Reset_BoneArmor(id);

		g_AbsorbDamage[id] = 0;
	}
}


public d2_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( !IsPlayerNearByMonster(victim) && get_p_hero(victim) == NECROMANCER && get_p_skill( victim, g_SkillId ) > 0 && g_AbsorbDamage[victim] > 0 )
	{
		Set_Player_BoneArmorDmg(victim, g_AbsorbDamage[victim] - floatround(iDamage[0]));
		
		iDamage[0] = 0.0;
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
public Reset_BoneArmor(id)
{
	if ( get_p_hero(id) == NECROMANCER )
	{
		new sprite_ent = find_ent_by_class(-1, "BoneArmor")
	
		if ( is_valid_ent(sprite_ent) ) 
		{
			remove_entity(sprite_ent)
			sprite_ent = find_ent_by_class(sprite_ent, "BoneArmor")
		}
	}
}
public Set_Player_BoneArmorDmg(id, value)
{
	g_AbsorbDamage[id] = value;

	if ( g_AbsorbDamage[id] <= 0 )
	{
		Reset_BoneArmor(id);
	}
}