#include <amxmodx>
#include <d2lod>

new PLUGIN_NAME[] = "毒性傷害"
new PLUGIN_AUTHOR[] = "xbatista"
new PLUGIN_VERSION[] = "1.0"

new Skill_Level = 6;

new Mana_PoisonDagger = 2; // 攻擊時需要的魔力.

new const Float:DmgDagPoison[MAX_P_SKILLS] =  // Poison dagger damage %.
{
	8.0, 14.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0, 80.0, 85.0, 90.0, 95.0, 100.0, 105.0
};

new g_SkillId;

new bool: g_IsPoisonDagger[33];
new g_iCurSkill[33];

public plugin_init() 
{
	register_plugin(PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR)

	g_SkillId = register_d2_skill(PLUGIN_NAME, "讓你的刀有毒性.", NECROMANCER, Skill_Level, DISPLAY)
}

public client_disconnect(id)
{
	g_IsPoisonDagger[id] = false;
}

public d2_skill_selected(id, skill_id)
{
	g_iCurSkill[id] = skill_id;

	if ( get_p_hero(id) == NECROMANCER )
	{
		if ( skill_id == g_SkillId )
		{
			g_IsPoisonDagger[id] = true;
		}
		else
		{
			g_IsPoisonDagger[id] = false;
		}
	}
}

public d2_skill_fired(id)
{

}

public d2_logged(id, log_type)
{
	if ( log_type == UNLOGGED )
	{
		
	}
}
public d2_dagger_poisondamage(victim, attacker, Float:iDamage[1])
{
	if ( get_p_hero(attacker) == NECROMANCER && get_p_skill( attacker, g_SkillId ) > 0 && g_IsPoisonDagger[attacker] && get_p_mana(attacker) >= Mana_PoisonDagger )
	{
		iDamage[0] = iDamage[0] + ( ( iDamage[0] / 100.0 ) * DmgDagPoison[ get_p_skill( attacker, g_SkillId ) - 1 ] );

		set_p_mana(attacker, get_p_mana(attacker) - Mana_PoisonDagger );
	}
}