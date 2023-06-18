// Diablo II LOD的Stock.
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
stock fm_velocity_by_aim(iIndex, Float:fDistance, Float:fVelocity[3], Float:fViewAngle[3])
{
	pev(iIndex, pev_v_angle, fViewAngle)
	fVelocity[0] = floatcos(fViewAngle[1], degrees) * fDistance
	fVelocity[1] = floatsin(fViewAngle[1], degrees) * fDistance
	fVelocity[2] = floatcos(fViewAngle[0]+90.0, degrees) * fDistance
	return 1
}
get_cam_owner(iEnt)
{
    static id
    for(id = 1; id<=g_iMaxPlayers; id++)
    {
        if( g_iCam[id] == iEnt )
        {
            return id
        }
    }
    return 0
}
stock create_velocity_vector(victim,attacker,Float:velocity[3])
{
	if(!is_user_alive(attacker))
		return 0;

	new Float:vicorigin[3];
	new Float:attorigin[3];
	entity_get_vector(victim   , EV_VEC_origin , vicorigin);
	entity_get_vector(attacker , EV_VEC_origin , attorigin);

	new Float:origin2[3]
	origin2[0] = vicorigin[0] - attorigin[0];
	origin2[1] = vicorigin[1] - attorigin[1];

	new Float:largestnum = 0.0;

	if(floatabs(origin2[0])>largestnum) largestnum = floatabs(origin2[0]);
	if(floatabs(origin2[1])>largestnum) largestnum = floatabs(origin2[1]);

	origin2[0] /= largestnum;
	origin2[1] /= largestnum;

	velocity[0] = ( origin2[0] * (get_pcvar_float(d2_stun_knockback_power) * 3000) ) / get_entity_distance(victim , attacker);
	velocity[1] = ( origin2[1] * (get_pcvar_float(d2_stun_knockback_power) * 3000) ) / get_entity_distance(victim , attacker);
	if(velocity[0] <= 20.0 || velocity[1] <= 20.0)
		velocity[2] = random_float(200.0 , 275.0);

	return 1;
}
stock fm_set_animation(id, anim) 
{
	if ( !is_user_alive(id) )
		return;

	set_pev(id, pev_weaponanim, anim)
    
	message_begin(MSG_ONE, SVC_WEAPONANIM, {0, 0, 0}, id)
	write_byte(anim)
	write_byte(pev(id, pev_body))
	message_end()
}

public fm_set_user_model( id, Model[])
{
	if( ! ( 1 <= id <= g_iMaxPlayers ) )
	{
		return 0
	}

	if( !is_user_connected(id) )
	{
		return 0
	}

	Set_Plugin_State(id, true)
	
	new szModel[MAX_MODEL_LENGTH];
	formatex( szModel, charsmax(szModel), Model)

	formatex(g_szModel[id], charsmax(szModel), szModel)
	set_user_info(id, "model", g_szModel[id])

	return 1
}

public register_skill( skill_index, skill_name[], skill_desc[], skill_hero, skill_level, skill_max, skill_display )
{
	if( g_skillcounter == MAX_PLUGIN_SKILLS )
	{
		return -2
	}
	
	g_skillcounter++;

	g_skillindex[skill_hero][g_skillcounter] = skill_index;

	formatex( g_skillname[skill_hero][g_skillcounter], MAX_SKILL_NAME_SIZE, skill_name)
	formatex( g_skilldesc[skill_hero][g_skillcounter], MAX_SKILL_DESC_SIZE, skill_desc)

	g_skillhero[g_skillcounter] = skill_hero;
	g_skilllevel[skill_hero][g_skillcounter] = skill_level;
	g_skilldisplay[g_skillcounter] = skill_display;
	g_skillmax[g_skillcounter] = skill_max;

	return g_skillcounter;
}

// ported from AMXX's core get_user_origin(..., 3) (suggested by Greenberet)
stock fm_get_aim_origin(index, Float:origin[3]) {
    new Float:start[3], Float:view_ofs[3];
    pev(index, pev_origin, start);
    pev(index, pev_view_ofs, view_ofs);
    xs_vec_add(start, view_ofs, start);

    new Float:dest[3];
    pev(index, pev_v_angle, dest);
    engfunc(EngFunc_MakeVectors, dest);
    global_get(glb_v_forward, dest);
    xs_vec_mul_scalar(dest, 9999.0, dest);
    xs_vec_add(start, dest, dest);

    engfunc(EngFunc_TraceLine, start, dest, 0, index, 0);
    get_tr2(0, TR_vecEndPos, origin);

    return 1;
} 