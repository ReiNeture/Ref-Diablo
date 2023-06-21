#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>
#include <orpheu>
#include <xs>

new PLUGIN_NAME[] = "藏海劍法";
new PLUGIN_AUTHOR[] = "reff";
new PLUGIN_VERSION[] = "1.0";

new const fireslashMdl[] = "models/ref/fireslash.mdl";

new Skill_Req_Level = 15;
new Skill_Max = 1;

new const weapon_sound[][] = 
{
	"weapons/samurai_slash1.wav",
	"weapons/samurai_slash2.wav"
	// "weapons/samurai_hit1.wav",
	// "weapons/samurai_hit2.wav",
	// "weapons/samurai_hit3.wav",
	// "weapons/samurai_hit4.wav"
	// "weapons/samurai_deploy1.wav",
	// "weapons/samurai_hitwall1.wav",
	// "weapons/samurai_stab.wav"
}

#define pDataKey_iOwner 41
#define m_flNextPrimaryAttack 46
#define m_flNextSecondaryAttack 47
#define m_flNextIdle 48
#define m_flNextAttack 83

new g_SkillId;
new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	g_SkillId = register_d2_skill(PLUGIN_NAME, "全面提升普通攻擊.", NECROMANCER, Skill_Req_Level, Skill_Max, NOT_DISPLAY);

	RegisterHam(Ham_Weapon_PrimaryAttack, "weapon_knife", "fw_Weapon_PrimaryAttack", 1);
	register_think("light_of_sword", "fw_Think_Light_of_sword");
	register_think("fade_out", "fw_Think_Fade_out");
}

public plugin_precache()
{
	engfunc(EngFunc_PrecacheModel, fireslashMdl);
	for(new i = 0; i < sizeof(weapon_sound); i++)
		engfunc(EngFunc_PrecacheSound, weapon_sound[i])
}

public fw_Weapon_PrimaryAttack(ent)
{
	static id; id = get_pdata_cbase(ent, pDataKey_iOwner, 4);
	if( !is_user_alive(id) )
		return HAM_IGNORED;

	if( get_p_skill( id, g_SkillId ) > 0 )
	{
		light_of_sword(id);
		radiusAttack(id);
		emit_sound(id, CHAN_WEAPON, weapon_sound[random_num(0, 1)], 1.0, ATTN_NORM, 0, PITCH_NORM);

		// 攻擊速度設定
		const Float:times = 0.2;
		set_pdata_float(ent, m_flNextPrimaryAttack, times, 4)
		set_pdata_float(ent, m_flNextSecondaryAttack, times, 4)
		set_pdata_float(id, m_flNextAttack, times, 5) 
		set_pdata_float(ent, m_flNextIdle, 1.2, 4);
		
	}

	return HAM_HANDLED;
}

light_of_sword(id)
{
	new ent = engfunc(EngFunc_CreateNamedEntity, engfunc(EngFunc_AllocString, "info_target"));
	set_pev(ent, pev_movetype, MOVETYPE_FLY);
	set_pev(ent, pev_nextthink, get_gametime() + 0.2);
	set_pev(ent, pev_classname, "light_of_sword");
	engfunc(EngFunc_SetModel, ent, fireslashMdl);
	set_pev(ent, pev_solid, SOLID_NOT);
	set_pev(ent, pev_owner, id);
	set_pev(ent, pev_rendermode, kRenderTransAdd);
	set_pev(ent, pev_renderamt, 130.0);

	static Float:origin[3], Float:angles[3], Float:velocity[3];

	pev(id, pev_origin, origin);
	set_pev(ent, pev_origin, origin);
	
	pev(id, pev_angles, angles);
	angles[0] = random_float(-7.0, 7.0); //上下翻轉
	angles[2] = random_float(-18.0, 18.0); //左右翻轉
	set_pev(ent, pev_angles, angles);

	velocity_by_aim(id, 10, velocity);
	set_pev(ent, pev_velocity, velocity);
}

public fw_Think_Light_of_sword(ent)
{
	if( !pev_valid(ent) )
		return FMRES_IGNORED;

	const Float:fadeSpeed = 5.0;
	set_pev(ent, pev_classname, "fade_out");
	set_pev(ent, pev_fuser1, fadeSpeed);
	set_pev(ent, pev_nextthink, get_gametime() + 0.01);

	return FMRES_IGNORED;
}

public fw_Think_Fade_out(ent)
{
	if( !pev_valid(ent) )
		return FMRES_IGNORED;

	static Float:fadeSpeed, Float:renderamt
	pev(ent, pev_fuser1, fadeSpeed) // 此欄位紀錄淡出的速度
	pev(ent, pev_renderamt, renderamt)

	if( renderamt > 0.0 ) {
		if( fadeSpeed > renderamt )  
			set_pev(ent, pev_renderamt, 0.0)
		else
			set_pev(ent, pev_renderamt, renderamt - fadeSpeed) // 漸層淡出的速度
		set_pev(ent, pev_nextthink, get_gametime() + 0.01)

	} else 
		engfunc(EngFunc_RemoveEntity, ent)

	return FMRES_IGNORED;
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
	}
}

public d2_takedamage(victim, attacker, Float:iDamage[1])
{

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
	}
}


#define	RESULT_HIT_NONE 			0
#define	RESULT_HIT_PLAYER			1
#define	RESULT_HIT_METAL			2
#define	RESULT_HIT_GENERIC			3

radiusAttack(id)
{
	#define ATTACK_RANGE 175.0
	#define ATTACK_ANGLE 80.0
	#define ATTACK_DAMAGE 15.0
	#define ATTACK_KNOCK 2.0

	new iHitResult = KnifeAttack_Main(id, 1, ATTACK_RANGE, ATTACK_ANGLE, ATTACK_DAMAGE, ATTACK_KNOCK)
	switch (iHitResult)
	{
		// case RESULT_HIT_PLAYER : emit_sound(id, CHAN_WEAPON, weapon_sound[random_num(0, 3)], 1.0, ATTN_NORM, 0, PITCH_NORM);
		// case RESULT_HIT_METAL : emit_sound(id, CHAN_WEAPON, weapon_sound[random_num(0, 3)], 1.0, ATTN_NORM, 0, PITCH_NORM);
		// case RESULT_HIT_GENERIC : emit_sound(id, CHAN_WEAPON, weapon_sound[random_num(0, 3)], 1.0, ATTN_NORM, 0, PITCH_NORM);
		// case RESULT_HIT_NONE : emit_sound(id, CHAN_WEAPON, weapon_sound[random_num(4, 5)], 1.0, ATTN_NORM, 0, PITCH_NORM);
	}
}

stock KnifeAttack_Main(id, bStab, Float:flRange, Float:fAngle, Float:flDamage, Float:flKnockBack)
{
	new iHitResult
	if(fAngle > 0.0) iHitResult = KnifeAttack2(id, bStab, Float:flRange, Float:fAngle, Float:flDamage, Float:flKnockBack)
	else iHitResult = KnifeAttack(id, bStab, Float:flRange, Float:flDamage, Float:flKnockBack)

	return iHitResult
}

stock KnifeAttack(id, bStab, Float:flRange, Float:flDamage, Float:flKnockBack, iHitgroup = -1, bitsDamageType = DMG_NEVERGIB | DMG_CLUB)
{
	new Float:vecSrc[3], Float:vecEnd[3], Float:v_angle[3], Float:vecForward[3];
	GetGunPosition(id, vecSrc);

	pev(id, pev_v_angle, v_angle);
	engfunc(EngFunc_MakeVectors, v_angle);

	global_get(glb_v_forward, vecForward);
	xs_vec_mul_scalar(vecForward, flRange, vecForward);
	xs_vec_add(vecSrc, vecForward, vecEnd);

	new tr = create_tr2();
	engfunc(EngFunc_TraceLine, vecSrc, vecEnd, 0, id, tr);

	new Float:flFraction; get_tr2(tr, TR_flFraction, flFraction);
	if (flFraction >= 1.0) engfunc(EngFunc_TraceHull, vecSrc, vecEnd, 0, 3, id, tr);
	
	get_tr2(tr, TR_flFraction, flFraction);

	new Float:EndPos2[3]
	get_tr2(tr, TR_vecEndPos, EndPos2)
	
	new iHitResult = RESULT_HIT_NONE;
	
	if (flFraction < 1.0)
	{
		new pEntity = get_tr2(tr, TR_pHit);
		
		new iTtextureType, pTextureName[64];
		engfunc(EngFunc_TraceTexture, 0, vecSrc, vecEnd, pTextureName, charsmax(pTextureName));
		iTtextureType = dllfunc(DLLFunc_PM_FindTextureType, pTextureName);
		
		if (iTtextureType == 'M') iHitResult = RESULT_HIT_METAL
		else iHitResult = RESULT_HIT_GENERIC;
		
		if (pev_valid(pEntity) && (IsPlayer(pEntity) || IsHostage(pEntity)))
		{
			if (CheckBack(id, pEntity) && bStab && iHitgroup == -1)
				flDamage *= 3.0;

			iHitResult = RESULT_HIT_PLAYER;
		}

		if (pev_valid(pEntity))
		{
			engfunc(EngFunc_MakeVectors, v_angle);
			global_get(glb_v_forward, vecForward);

			if (iHitgroup != -1)
				set_tr2(tr, TR_iHitgroup, iHitgroup);

			Stock_Fake_KnockBack(id, pEntity, flKnockBack)

			ClearMultiDamage();
			ExecuteHamB(Ham_TraceAttack, pEntity, id, flDamage, vecForward, tr, bitsDamageType);
			ApplyMultiDamage(id, id);
			
			if (IsAlive(pEntity))
			{
				free_tr2(tr);
				return iHitResult;
			}
		}
	}
	free_tr2(tr);
	return iHitResult;
}

stock KnifeAttack2(id, bStab, Float:flRange, Float:fAngle, Float:flDamage, Float:flKnockBack, iHitgroup = -1, bNoTraceCheck = 0, bitsDamageType = DMG_NEVERGIB | DMG_CLUB)
{
	new Float:vecOrigin[3], Float:vecSrc[3], Float:vecEnd[3], Float:v_angle[3], Float:vecForward[3];
	pev(id, pev_origin, vecOrigin);

	new iHitResult = RESULT_HIT_NONE;
	GetGunPosition(id, vecSrc);

	pev(id, pev_v_angle, v_angle);
	engfunc(EngFunc_MakeVectors, v_angle);

	global_get(glb_v_forward, vecForward);
	xs_vec_mul_scalar(vecForward, flRange, vecForward);
	xs_vec_add(vecSrc, vecForward, vecEnd);

	new tr = create_tr2();
	engfunc(EngFunc_TraceLine, vecSrc, vecEnd, 0, id, tr);
	
	new Float:EndPos2[3]
	get_tr2(tr, TR_vecEndPos, EndPos2)
	
	new Float:flFraction; get_tr2(tr, TR_flFraction, flFraction);
	if (flFraction < 1.0) 
	{
		new iTtextureType, pTextureName[64];
		engfunc(EngFunc_TraceTexture, 0, vecSrc, vecEnd, pTextureName, charsmax(pTextureName));
		iTtextureType = dllfunc(DLLFunc_PM_FindTextureType, pTextureName);
		
		if (iTtextureType == 'M') iHitResult = RESULT_HIT_METAL
		else iHitResult = RESULT_HIT_GENERIC
	}
	
	new Float:vecEndZ = vecEnd[2];
	
	new pEntity = -1;
	while ((pEntity = engfunc(EngFunc_FindEntityInSphere, pEntity, vecOrigin, flRange)) != 0)
	{
		if (!pev_valid(pEntity))
			continue;
		if (id == pEntity)
			continue;
		if (!IsAlive(pEntity))
			continue;
		if (!CheckAngle(id, pEntity, fAngle))
			continue;

		GetGunPosition(id, vecSrc);
		Stock_Get_Origin(pEntity, vecEnd);
		
		vecEnd[2] = vecSrc[2] + (vecEndZ - vecSrc[2]) * (get_distance_f(vecSrc, vecEnd) / flRange);

		xs_vec_sub(vecEnd, vecSrc, vecForward);
		xs_vec_normalize(vecForward, vecForward);
		xs_vec_mul_scalar(vecForward, flRange, vecForward);
		xs_vec_add(vecSrc, vecForward, vecEnd);

		engfunc(EngFunc_TraceLine, vecSrc, vecEnd, 0, id, tr);
		get_tr2(tr, TR_flFraction, flFraction);

		if (flFraction >= 1.0) engfunc(EngFunc_TraceHull, vecSrc, vecEnd, 0, 3, id, tr);

		get_tr2(tr, TR_flFraction, flFraction);
		
		if (flFraction < 1.0)
		{
			if (IsPlayer(pEntity) || IsHostage(pEntity))
			{
				iHitResult = RESULT_HIT_PLAYER;
				
				if (CheckBack(id, pEntity) && bStab && iHitgroup == -1)
					flDamage *= 3.0;
			}

			if( IsMonster(pEntity) )
			{
				iHitResult = RESULT_HIT_PLAYER;
			}

			if (get_tr2(tr, TR_pHit) == pEntity || bNoTraceCheck)
			{
				engfunc(EngFunc_MakeVectors, v_angle);
				global_get(glb_v_forward, vecForward);

				if (iHitgroup != -1) set_tr2(tr, TR_iHitgroup, iHitgroup);

				Stock_Fake_KnockBack(id, pEntity, flKnockBack)

				ClearMultiDamage();
				ExecuteHamB(Ham_TraceAttack, pEntity, id, flDamage, vecForward, tr, bitsDamageType);
				ApplyMultiDamage(id, id);
			}
		}
		free_tr2(tr);
	}
	return iHitResult;
}

stock fake_traceattack(ent, victim, Float:damages, dmgBit)
{
	new Float:vecSrc[3], Float:vecEnd[3], Float:v_angle[3], Float:vecForward[3]
	new id = pev(ent, pev_owner)
	pev(ent, pev_origin, vecSrc)
	pev(victim, pev_origin, vecEnd)
	xs_vec_sub(vecEnd, vecSrc, v_angle)
	angle_vector(v_angle, ANGLEVECTOR_FORWARD, v_angle)
	new tr = create_tr2()
	engfunc(EngFunc_TraceLine, vecSrc, vecEnd, 0, id, tr);
	ClearMultiDamage()
	ExecuteHamB(Ham_TraceAttack, victim, id, damages, vecForward, tr, dmgBit)
	ApplyMultiDamage(id, id)
	free_tr2(tr)
}

stock CheckAngle(iAttacker, iVictim, Float:fAngle)  return(Stock_CheckAngle(iAttacker, iVictim) > floatcos(fAngle,degrees))
stock IsPlayer(pEntity) return is_user_connected(pEntity)
stock IsMonster(pEntity)
{
	new healths = pev(pEntity, pev_health);
	new classname[32];
	pev(pEntity, pev_classname, classname, 31);
	return !IsPlayer(pEntity) && equal("func_wall", classname) && healths > 0;
}

stock ClearMultiDamage() OrpheuCall(OrpheuGetFunction("ClearMultiDamage"));
stock ApplyMultiDamage(inflictor, iAttacker) OrpheuCall(OrpheuGetFunction("ApplyMultiDamage"), inflictor, iAttacker);
stock GetGunPosition(id, Float:vecScr[3])
{
	new Float:vecViewOfs[3]
	pev(id, pev_origin, vecScr)
	pev(id, pev_view_ofs, vecViewOfs)
	xs_vec_add(vecScr, vecViewOfs, vecScr)
}
stock CheckBack(iEnemy,id)
{
	new Float:anglea[3], Float:anglev[3]
	pev(iEnemy, pev_v_angle, anglea)
	pev(id, pev_v_angle, anglev)
	new Float:angle = anglea[1] - anglev[1] 
	if (angle < -180.0) angle += 360.0
	if (angle <= 45.0 && angle >= -45.0) return 1
	return 0
}
public Stock_Fake_KnockBack(id, iVic, Float:iKb)
{
	if(iVic > 32) return
	
	new Float:vAttacker[3], Float:vVictim[3], Float:vVelocity[3], flags
	pev(id, pev_origin, vAttacker)
	pev(iVic, pev_origin, vVictim)
	vAttacker[2] = vVictim[2] = 0.0
	flags = pev(id, pev_flags)
	
	xs_vec_sub(vVictim, vAttacker, vVictim)
	new Float:fDistance
	fDistance = xs_vec_len(vVictim)
	xs_vec_mul_scalar(vVictim, 1 / fDistance, vVictim)
	
	pev(iVic, pev_velocity, vVelocity)
	xs_vec_mul_scalar(vVictim, iKb, vVictim)
	xs_vec_mul_scalar(vVictim, 50.0, vVictim)
	vVictim[2] = xs_vec_len(vVictim) * 0.15
	
	if(flags &~ FL_ONGROUND)
	{
		xs_vec_mul_scalar(vVictim, 1.2, vVictim)
		vVictim[2] *= 0.4
	}
	if(xs_vec_len(vVictim) > xs_vec_len(vVelocity)) set_pev(iVic, pev_velocity, vVictim)
}
stock IsAlive(pEntity)
{
	if (pEntity < 1) return 0
	return (pev(pEntity, pev_deadflag) == DEAD_NO && pev(pEntity, pev_health) > 0)
}
stock Stock_Get_Origin(id, Float:origin[3])
{
	new Float:maxs[3],Float:mins[3]
	if (pev(id, pev_solid) == SOLID_BSP)
	{
		pev(id,pev_maxs,maxs)
		pev(id,pev_mins,mins)
		origin[0] = (maxs[0] - mins[0]) / 2 + mins[0]
		origin[1] = (maxs[1] - mins[1]) / 2 + mins[1]
		origin[2] = (maxs[2] - mins[2]) / 2 + mins[2]
	} else pev(id, pev_origin, origin)
}
stock IsHostage(pEntity)
{
	new classname[32]; pev(pEntity, pev_classname, classname, charsmax(classname))
	return equal(classname, "hostage_entity")
}
stock Float:Stock_CheckAngle(id,iTarget)
{
	new Float:vOricross[2],Float:fRad,Float:vId_ori[3],Float:vTar_ori[3],Float:vId_ang[3],Float:fLength,Float:vForward[3]
	Stock_Get_Origin(id, vId_ori)
	Stock_Get_Origin(iTarget, vTar_ori)
	
	pev(id,pev_angles,vId_ang)
	for(new i=0;i<2;i++) vOricross[i] = vTar_ori[i] - vId_ori[i]
	
	fLength = floatsqroot(vOricross[0]*vOricross[0] + vOricross[1]*vOricross[1])
	
	if (fLength<=0.0)
	{
		vOricross[0]=0.0
		vOricross[1]=0.0
	} else {
		vOricross[0]=vOricross[0]*(1.0/fLength)
		vOricross[1]=vOricross[1]*(1.0/fLength)
	}
	
	engfunc(EngFunc_MakeVectors,vId_ang)
	global_get(glb_v_forward,vForward)
	
	fRad = vOricross[0]*vForward[0]+vOricross[1]*vForward[1]
	
	return fRad   //->   RAD 90' = 0.5rad
}