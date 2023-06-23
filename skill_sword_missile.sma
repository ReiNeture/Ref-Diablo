#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <hamsandwich>
#include <xs>
#include <vector>

new PLUGIN_NAME[] = "追蹤";
new PLUGIN_AUTHOR[] = "reff";
new PLUGIN_VERSION[] = "1.0";

new const missile_model[] = "models/ref/gungnir_missile.mdl";
new const missile_explo_sprite[] = "sprites/ref/ef_gungnir_chargeexplo.spr";
new const missile_line_sprite[] = "sprites/ref/ef_gungnir_lightline2.spr";
new const missile_explo_sound[] = "ref/gungnir_charge_shoot_exp2.wav";

new explosionSpr, lineSpr;

const Skill_Req_Level = 33;
const Skill_Max = 1;
new g_SkillId;

new count[33];
const Float:basic_damge = 300.0;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);

	RegisterHam(Ham_Weapon_PrimaryAttack, "weapon_knife", "fw_Weapon_PrimaryAttack", 1);
	register_forward(FM_Touch, "fw_Touch")
	register_forward(FM_Think, "fw_Think");

	g_SkillId = register_d2_skill(PLUGIN_NAME, "輕砍十下在背後召喚劍陣追蹤敵人.", NECROMANCER, Skill_Req_Level, Skill_Max, NOT_DISPLAY);
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheModel, missile_model);
	engfunc(EngFunc_PrecacheSound, missile_explo_sound);
	explosionSpr = engfunc(EngFunc_PrecacheModel, missile_explo_sprite);
	lineSpr = engfunc(EngFunc_PrecacheModel, missile_line_sprite);
}

public fw_Weapon_PrimaryAttack(ent)
{
	static id; id = get_pdata_cbase(ent, 41, 4);
	if ( !is_user_connected(id) )
		return HAM_IGNORED;

	count[id]++;
	if( get_p_skill( id, g_SkillId ) > 0 && count[id] >= 10 )
	{
		count[id] = 0;
		create_sword_missile(id);
	}

	return HAM_IGNORED;
}

create_sword_missile(id)
{
	new Float:origin[3], Float:velocity[3], Float:offorigin[3], Float:angles[3];
	for( new i = 0; i < 5; i++ )
	{
		new sword_missile = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target") )
		set_pev(sword_missile, pev_classname, "sword_missile");
		set_pev(sword_missile, pev_movetype, MOVETYPE_FLY);
		set_pev(sword_missile, pev_solid, SOLID_TRIGGER);
		set_pev(sword_missile, pev_owner, id);
		set_pev(sword_missile, pev_animtime, get_gametime());
		set_pev(sword_missile, pev_framerate, 3.0);

		engfunc(EngFunc_SetModel, sword_missile, missile_model);
		engfunc(EngFunc_SetSize, sword_missile, Float:{-2.0, -2.0, -2.0}, Float:{2.0, 2.0, 2.0});

		pev(id, pev_v_angle, angles);
		set_pev(sword_missile, pev_angles, {90.0, 0.0, 0.0});

		pev(id, pev_origin, origin);
		switch(i)
		{
			case 0: getOffsetsVelocity(id, 100.0, velocity, -70.0);
			case 1: getOffsetsVelocity(id, 50.0, velocity, -80.0);
			case 2: getOffsetsVelocity(id, 0.0, velocity, 0.0);
			case 3: getOffsetsVelocity(id, 50.0, velocity, 80.0);
			case 4: getOffsetsVelocity(id, 100.0, velocity, 70.0);
		}
		
		xs_vec_add(origin, velocity, offorigin);
		offorigin[2] += 64.0;
		set_pev(sword_missile, pev_origin, offorigin);

		set_pev(sword_missile, pev_nextthink, get_gametime() + 2.0 + float(i) * 0.2);
	}
}

getOffsetsVelocity(id, Float:lengths, Float:vec[3], Float:rotaion)
{
	new Float:xOffsets[3];
	pev(id, pev_v_angle, xOffsets);
	xOffsets[0] = 0.0;
	xOffsets[1] += rotaion; // 90.0是向右
	xOffsets[2] = 0.0;
	angle_vector(xOffsets, ANGLEVECTOR_FORWARD, xOffsets);
	xs_vec_mul_scalar(xOffsets, lengths, xOffsets);
	vec = xOffsets;
}

public fw_Think(ent)
{
	if( !pev_valid(ent) ) return FMRES_IGNORED;
	
	static classname[32];
	pev(ent, pev_classname, classname, 31);
	if( equal(classname, "sword_missile") )
	{
		new targetname[32], Float:origin1[3], Float:origin2[3], Float:velocity[3], Float:angles[3];
		new id, victim, Float:temp_distance, Float:min_distances, min_distance_id;

		id = pev(ent, pev_owner);
		min_distance_id = victim = FM_NULLENT;
		min_distances = 999999.9;
		pev(ent, pev_origin, origin1);

		while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, origin1, 600.0) ) != 0 )
		{
			pev(victim, pev_classname, targetname, 31);
			if( id == victim )
				continue;
			if( !is_user_connected(victim) && !equal("func_wall", targetname) )
				continue;
			if( is_user_connected(victim) && ( !is_user_alive(victim) || is_p_protected(victim) ) )
				continue;

			pev(victim, pev_origin, origin2);
			temp_distance = get_distance_f(origin1, origin2);

			if( temp_distance < min_distances )
			{
				min_distances = temp_distance;
				min_distance_id = victim;
			}
		}

		if( min_distance_id > FM_NULLENT )
		{
			pev(min_distance_id, pev_origin, origin2);
			get_speed_vector(origin1, origin2, 1500.0, velocity);
			set_pev(ent, pev_velocity, velocity);

			vector_to_angle(velocity, angles);
			set_pev(ent, pev_angles, angles);

			engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, 0, 0);
			write_byte(TE_BEAMFOLLOW);
			write_short(ent);
			write_short(lineSpr);
			write_byte(15); // life
			write_byte(5); // widtd
			write_byte(255); // r
			write_byte(231); // g
			write_byte(139); // b
			write_byte(225); // brightness
			message_end();
		}
		else
		{
			explode_sword_missile(ent);
			return FMRES_IGNORED;
		}

	}

	return FMRES_IGNORED;
}

public fw_Touch(ent, ptd)
{
	if( !pev_valid(ent) ) 
		return FMRES_IGNORED
	
	static classname[32], id;
	id = pev(ent, pev_owner);
	pev(ent, pev_classname, classname, sizeof(classname))

	if( equal(classname, "sword_missile") && id != ptd ) {
		explode_sword_missile(ent);
	}

	return FMRES_IGNORED;
}

explode_sword_missile(ent)
{
	new attacker = pev(ent, pev_owner);
	new targetname[32];
	new Float:iDamage = basic_damge + (float(get_p_strength(attacker)) * 0.5) + get_totaldmg_of_item(attacker);

	new Float:fOrigin[3];
	pev(ent, pev_origin, fOrigin);

	new victim = FM_NULLENT;

	while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, 125.0) ) != 0 ) {

		pev(victim, pev_classname, targetname, 31);
		if( attacker == victim ) 
			continue;

		if( is_user_alive(victim) && !is_p_protected(victim) && get_p_skill(attacker, g_SkillId) > 0 )
		{
			dmg_kill_player(victim, attacker, iDamage, "sword_missile");
		}
		else if( !is_user_connected(victim) && equal(targetname, "func_wall") )
		{
			ExecuteHam(Ham_TakeDamage, victim, ent, attacker, iDamage, DMG_BLAST);
		}
	}

	engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, fOrigin, 0);
	write_byte(TE_EXPLOSION);
	engfunc(EngFunc_WriteCoord, fOrigin[0]);
	engfunc(EngFunc_WriteCoord, fOrigin[1]);
	engfunc(EngFunc_WriteCoord, fOrigin[2]);
	write_short(explosionSpr);
	write_byte(10);
	write_byte(20);
	write_byte(TE_EXPLFLAG_NOPARTICLES | TE_EXPLFLAG_NOSOUND);
	message_end();

	emit_sound(ent, CHAN_WEAPON, missile_explo_sound, 1.0, ATTN_NORM, 0, PITCH_NORM);
	engfunc(EngFunc_RemoveEntity, ent);
}

stock get_speed_vector(const Float:origin1[3], const Float:origin2[3], Float:speed, Float:new_velocity[3])
{
	new_velocity[0] = origin2[0] - origin1[0]
	new_velocity[1] = origin2[1] - origin1[1]
	new_velocity[2] = origin2[2] - origin1[2]
	new Float:num = floatsqroot(speed*speed / (new_velocity[0]*new_velocity[0] + new_velocity[1]*new_velocity[1] + new_velocity[2]*new_velocity[2]))
	new_velocity[0] *= num
	new_velocity[1] *= num
	new_velocity[2] *= num
	return 1
}