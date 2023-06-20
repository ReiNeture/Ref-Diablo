#include <amxmodx>
#include <d2lod>
#include <fakemeta>

new PLUGIN_NAME[] = "衝擊結界";
new PLUGIN_AUTHOR[] = "reff";
new PLUGIN_VERSION[] = "1.0";

new const g_SpriteShockWave[] = "sprites/shockwave.spr";

new g_SkillId;

new g_iCurSkill[33];
new Float:g_LastPressedSkill[33];

new gShockwaveSpr;

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR);
	g_SkillId = register_d2_skill(PLUGIN_NAME, "發出衝擊波擊退周邊的人.", SHOCKWAVE_RING, 0, 0, DISPLAY);
}

public plugin_precache()
{
	gShockwaveSpr = precache_model(g_SpriteShockWave);
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
	if ( g_iCurSkill[id] == g_SkillId )
	{
		const Float:cdown = 1.0;
		if (get_gametime() - g_LastPressedSkill[id] <= cdown) 
		{
			return PLUGIN_HANDLED;
		}
		else if ( get_gametime() - g_LastPressedSkill[id] >= cdown )
		{
			g_LastPressedSkill[id] = get_gametime()
		}

		shock_wave(id);
	}
	
	return PLUGIN_CONTINUE;
}

public shock_wave(id)
{
	new Float:fOrigin[3], Float:vOrigin[3], Float:velocity[3], classname[32];
	pev(id, pev_origin, fOrigin);
	create_blast(fOrigin);

	const Float:Range = 600.0;
	const Float:HighRange = 64.0;

	new victim = FM_NULLENT;
	while( (victim = engfunc(EngFunc_FindEntityInSphere, victim, fOrigin, Range) ) != 0 )
	{
		pev(victim, pev_classname, classname, 31);
		if( victim != id && ( is_user_alive(victim) || ( !is_user_alive(victim) && equal("func_wall", classname) ) ) )
		{
			pev(victim, pev_origin, vOrigin);
			if( floatabs(vOrigin[2] - fOrigin[2]) <= HighRange )
			{
				get_speed_vector_to_entity(id, victim, 1200.0, velocity);
				velocity[2] += 300.0;
				set_pev(victim, pev_velocity, velocity);
			}
		}
	}

	return PLUGIN_HANDLED;
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

stock create_blast(const Float:fOrigin[3])
{
	// Largest ring (大的光環)
	engfunc(EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, fOrigin, 0)
	write_byte(TE_BEAMCYLINDER) // TE id (TE 的代碼)
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x (X 座標)
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y (Y 座標)
	engfunc(EngFunc_WriteCoord, fOrigin[2]) // z (Z 座標)
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x axis (X 軸)
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y axis (Y 軸)
	engfunc(EngFunc_WriteCoord, fOrigin[2]+550.0) // z axis (Z 軸)
	write_short(gShockwaveSpr) // sprite (Sprite 物件代碼)
	write_byte(0) // startframe (幀幅開始)
	write_byte(0) // framerate (幀幅頻率)
	write_byte(3) // life (時間長度)
	write_byte(30) // width (寬度)
	write_byte(0) // noise (響聲)
	write_byte(255) // red (顏色 R)
	write_byte(255) // green (顏色 G)
	write_byte(255) // blue (顏色 B)
	write_byte(50) // brightness (顏色亮度)
	write_byte(0) // speed (速度)
	message_end()
	
	// Medium ring (中的光環)
	engfunc(EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, fOrigin, 0)
	write_byte(TE_BEAMCYLINDER) // TE id
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y
	engfunc(EngFunc_WriteCoord, fOrigin[2]) // z
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x axis
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y axis
	engfunc(EngFunc_WriteCoord, fOrigin[2]+400.0)
	write_short(gShockwaveSpr) // sprite
	write_byte(0) // startframe
	write_byte(0) // framerate
	write_byte(3) // life (時間長度)
	write_byte(30) // width
	write_byte(0) // noise
	write_byte(255) // red (顏色 R)
	write_byte(255) // green (顏色 G)
	write_byte(255) // blue (顏色 B)
	write_byte(100) // brightness
	write_byte(0) // speed
	message_end()
	
	// Smallest ring (小的光環)
	engfunc(EngFunc_MessageBegin, MSG_PVS, SVC_TEMPENTITY, fOrigin, 0)
	write_byte(TE_BEAMCYLINDER) // TE id
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y
	engfunc(EngFunc_WriteCoord, fOrigin[2]) // z
	engfunc(EngFunc_WriteCoord, fOrigin[0]) // x axis
	engfunc(EngFunc_WriteCoord, fOrigin[1]) // y axis
	engfunc(EngFunc_WriteCoord, fOrigin[2]+250.0)
	write_short(gShockwaveSpr) // sprite
	write_byte(0) // startframe
	write_byte(0) // framerate
	write_byte(3) // life (時間長度)
	write_byte(30) // width
	write_byte(0) // noise
	write_byte(255) // red (顏色 R)
	write_byte(255) // green (顏色 G)
	write_byte(255) // blue (顏色 B)
	write_byte(50) // brightness
	write_byte(0) // speed
	message_end()
}

stock get_speed_vector_to_entity(ent1, ent2, Float:speed, Float:new_velocity[3])
{
	if(!pev_valid(ent1) || !pev_valid(ent2))
		return 0;
	
	static Float:origin1[3]
	pev(ent1,pev_origin,origin1)
	static Float:origin2[3]
	pev(ent2,pev_origin,origin2)
	
	new_velocity[0] = origin2[0] - origin1[0];
	new_velocity[1] = origin2[1] - origin1[1];
	new_velocity[2] = origin2[2] - origin1[2];
	
	static Float:num
	num = speed / vector_length(new_velocity);
				
	new_velocity[0] *= num;
	new_velocity[1] *= num;
	new_velocity[2] *= num;
	
	return 1;
}