#include <amxmodx>
#include <d2lod>
#include <fakemeta>
#include <engine>
#include <hamsandwich>

new PLUGIN_NAME[] = "長跳"
new PLUGIN_AUTHOR[] = "Reff"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 5;
new Skill_Allocate = 20;

new const SorcaManaLongJump[MAX_P_SKILLS] = 
{
	7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 3, 3, 2, 2, 1, 0
};
new const LongJumpDistance[MAX_P_SKILLS] = 
{
	370, 410, 440, 470, 500, 530, 560, 590, 620, 650, 680, 710, 740, 770, 800, 830, 860, 890, 920, 950
};

new g_SkillId;
new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)
	g_SkillId = register_d2_skill(PLUGIN_NAME, "移動跳躍.", ASSASSIN, Skill_Level, Skill_Allocate, NOT_DISPLAY)
	register_forward(FM_CmdStart, "fw_CmdStart")
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;
}

public d2_skill_fired(id)
{
}

public d2_takedamage(victim, attacker, Float:iDamage[1])
{
}

public fw_CmdStart(id, uc_handle, seed)
{
	if ( !is_user_alive(id) ) 
		return FMRES_IGNORED
	
	new Float:game_time = get_gametime()
	static CurButton
	CurButton = get_uc(uc_handle, UC_Buttons)

	static Float:nextJumpTime[33]
	if( (CurButton & IN_JUMP) && (CurButton & IN_DUCK) ) {
		if( game_time < nextJumpTime[id] )
			return FMRES_IGNORED

		if ( get_p_skill( id, g_SkillId ) > 0 && get_p_mana(id) >= SorcaManaLongJump[ get_p_skill( id, g_SkillId ) - 1 ] ) {
			new Float:velocity[3];
			velocity_by_aim(id, LongJumpDistance[ get_p_skill( id, g_SkillId ) - 1 ], velocity); // 大跳距離
			velocity[2] = 300.0;
			set_pev(id, pev_velocity, velocity);
			set_p_mana( id, get_p_mana(id) - SorcaManaLongJump[ get_p_skill( id, g_SkillId ) - 1 ]);
			nextJumpTime[id] = game_time + 0.2;
		}
	}
	return FMRES_IGNORED
}
