// Diablo II LOD的Forwards.

// 玩家重生.
public fwd_PlayerSpawn(id)
{
	if ( !is_user_alive(id) )
		return;
		
	Set_Custom_Model(id);
		
	if( g_iFakeplayer == id ) 
	{
		set_pev( id, pev_effects, pev( id, pev_effects ) | EF_NODRAW );
		set_pev( id, pev_solid, SOLID_NOT );
		entity_set_origin( id, Float:{ 999999.0, 999999.0, 999999.0 } );
		dllfunc( DLLFunc_Think, id );
	}
	else
	{
		set_task( 0.2, "Hide_Hud", id);

		if ( get_pcvar_num(d2_3rdperson) )
		{
			Create_PlayerCamera( id );
		}

		remove_task( id + TASKID_FIRSTCONNECT);
		remove_task( id + TASKID_MANAREGEN); 
		
		for (new item_id = 0; item_id <= g_charcounter; item_id++)
		{
			Set_Player_Armor_Item(id, item_id)
		}

		g_Armor[id] += (HEROES_DEFENCE_DEXTERITY[g_PlayerHero[id][g_CurrentChar[id]]] * g_Dexterity[id][g_CurrentChar[id]]);
		
		if( get_pcvar_num(d2_spawn_protection) )
		{
			g_PlayerProtected[id] = true;

			set_pev(id, pev_takedamage, 0.0);

			set_task(get_pcvar_float(d2_spawn_protection_time), "Remove_Spawn_Protection", id);
		}

		if ( g_Player_FirstTime_Connected[id] )
		{
			g_Player_FirstTime_Connected[id] = false;
		}
		
		strip_user_weapons(id);
		give_item(id, "weapon_knife");

		set_task(HEROES_REGAIN_MANA[g_PlayerHero[id][g_CurrentChar[id]]], "Mana_Regenerate", id + TASKID_MANAREGEN, _, _, "b");

		g_MaxHealth[id] = 100 + (HEROES_VITALITY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Vitality[id][g_CurrentChar[id]]);

		set_user_health( id, g_MaxHealth[id] );
	}
}

// 玩家的傷害.
public fwd_PlayerDamaged(victim, inflictor, attacker, Float:damage, damagebits)
{
	if ( attacker == victim || !(1 <= attacker <= g_iMaxPlayers) )
		return HAM_IGNORED;
		
	new Float:counted_strength; 	
	new iArmor = g_Armor[victim];
	counted_strength = HEROES_STRENGTH_PERSTAT[g_PlayerHero[attacker][g_CurrentChar[attacker]]] * g_Strength[attacker][g_CurrentChar[attacker]];
	new Float:counted_at_strength = ((counted_strength + 10.0) * 4.0);
	new Float:counted_defence = (iArmor > counted_at_strength ? (iArmor / counted_at_strength) * 5.0 : 0.0);
	new Float: counted_damage;
	counted_damage = damage + counted_strength - counted_defence;

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItemWorn[attacker][g_CurrentChar[attacker]][item_id] && g_iPlayerItem[attacker][g_CurrentChar[attacker]][item_id] > 0 
		&& g_PlayerLevel[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_LEVEL] 
		&& g_Strength[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_STR] && g_Dexterity[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_DEX] && float(item_data[item_id][CHAR_DAMAGE]) > 0.0 )
		{
			counted_damage += float(item_data[item_id][CHAR_DAMAGE]);
		}
	}
	
	new Float:totally_counted;
	totally_counted = (counted_damage < 7.0 ? 7.0 : counted_damage );
	
	new Float:iArray[1]; iArray[0] = totally_counted;
	new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)
	
	ExecuteForward( g_TakeDamage, g_iReturn, victim, attacker, iArrayPass);
	
	if ( get_pcvar_num(d2_show_damage) && iArray[0] >= 1.0 )
	{
		set_hudmessage(255, 255, 255, 0.49, 0.35, 1, 1.5, 1.5, 0.0, 0.0, 3)
		ShowSyncHudMsg(attacker, g_SyncHudCreate_Dmg , "+%d", floatround(iArray[0]));
	}

	if ( get_pcvar_num(d2_show_damage_received) && iArray[0] >= 1.0 )
	{
		set_hudmessage(255, 0, 0, 0.49, 0.32, 1, 1.5, 1.5, 0.0, 0.0, 2)
		ShowSyncHudMsg(victim, g_SyncHudCreate_Dmg , "-%d", floatround(iArray[0]));
	}

	new iTeam = fm_get_user_team( victim )
	
	if( iTeam == fm_get_user_team(attacker) )
	{
		fm_set_user_team( victim, iTeam == 1 ? 2 : 1 );
		ExecuteHam(Ham_TakeDamage, victim, inflictor, attacker, iArray[0], damagebits);
		fm_set_user_team(victim, iTeam );
		return HAM_HANDLED;
	}
	else
	{
		SetHamParamFloat(4, iArray[0] );
		return HAM_HANDLED;
	}

	return HAM_IGNORED;
}
// 玩家給怪物的傷害.
public fwd_PlayerDamagedMonster(victim, inflictor, attacker, Float:damage, damagebits)
{
	if ( !(1 <= attacker <= g_iMaxPlayers) )
		return HAM_IGNORED;
		
	new Float:counted_strength; 	
	counted_strength = HEROES_STRENGTH_PERSTAT[g_PlayerHero[attacker][g_CurrentChar[attacker]]] * g_Strength[attacker][g_CurrentChar[attacker]];
	new Float: counted_damage;
	counted_damage = damage + counted_strength;

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItemWorn[attacker][g_CurrentChar[attacker]][item_id] && g_iPlayerItem[attacker][g_CurrentChar[attacker]][item_id] > 0 
		&& g_PlayerLevel[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_LEVEL] 
		&& g_Strength[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_STR] && g_Dexterity[attacker][g_CurrentChar[attacker]] >= item_data[item_id][CHAR_DEX] && float(item_data[item_id][CHAR_DAMAGE]) > 0.0 )
		{
			counted_damage += float(item_data[item_id][CHAR_DAMAGE]);
		}
	}
				
	new Float:totally_counted;
	totally_counted = (counted_damage < 7.0 ? 7.0 : counted_damage );
				
	new Float:iArray[1]; iArray[0] = totally_counted;
	new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)
	
	if ( (1 <= victim <= g_iMaxPlayers) )
	{
		ExecuteForward( g_TakeDamage, g_iReturn, victim, attacker, iArrayPass);
	}
			
	if ( get_pcvar_num(d2_show_damage) )
	{
		set_hudmessage(255, 255, 255, 0.49, 0.3, 1, 1.5, 1.5, 0.0, 0.0, 3)
		ShowSyncHudMsg(attacker, g_SyncHudCreate_Dmg , "+%d", floatround(iArray[0]));
	}

	SetHamParamFloat(4, iArray[0] );

	return HAM_HANDLED;
}

// 給玩家造成傷害的事情.
public fwd_TraceAttack(victim, attacker, Float:damage, Float:direction[3], traceresult, damagebits)
{
	if ( attacker == victim || !(1 <= attacker <= g_iMaxPlayers) )
		return HAM_IGNORED;
		
	new counted_dexterity, block_value; 
	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		block_value = (g_iPlayerItemWorn[victim][g_CurrentChar[victim]][item_id] && g_iPlayerItem[victim][g_CurrentChar[victim]][item_id] > 0 && item_data[item_id][CHAR_BLOCK] > 0 ? item_data[item_id][CHAR_BLOCK] : 0)
	}
	
	counted_dexterity = HEROES_DEXTERITY_PERSTAT[g_PlayerHero[victim][g_CurrentChar[victim]]] * g_Dexterity[victim][g_CurrentChar[victim]]; 
	new Param[2];
	Param[0] = attacker;

	if ( native_get_p_in_safezone(victim) )
	{
		return HAM_SUPERCEDE;
	}
	else if ( native_get_p_near_monster(victim) )
	{
		return HAM_SUPERCEDE;
	}
	else
	{	
		if ( random_num(0, get_pcvar_num(d2_max_random_value) ) < (counted_dexterity + block_value) )
		{
			set_hudmessage(0, 150, 255, 0.05, 0.35, 1, 1.5, 1.5, 0.0, 0.0, 3)
			ShowSyncHudMsg(victim, g_SyncHudCreate_Avade , "迴避了攻擊!" )
			ShowSyncHudMsg(attacker, g_SyncHudCreate_Avade , "攻擊未命中!" )

			return HAM_SUPERCEDE;
		}
		else
		{	
			for(new item_id = 0; item_id <= g_charcounter ; item_id++) 
			{
				if ( g_iPlayerItemWorn[attacker][g_CurrentChar[attacker]][item_id] && g_iPlayerItem[attacker][g_CurrentChar[attacker]][item_id] > 0 )
				{
					if ( item_data[item_id][CHAR_HASP] > 0 )
					{
						Param[1] = item_id;
						
						set_user_rendering(victim, kRenderFxGlowShell, 0, 255, 0, kRenderTransAlpha, 100)
						
						set_task( 1.0, "Start_poison_damage", victim + TASKID_POISON, Param, sizeof Param, "b");
						set_task( float(item_data[item_id][CHAR_PDURATION]), "End_poison_damage", victim);
					}
					
					if ( item_data[item_id][CHAR_DURABILITY] > 0 )
					{
						Set_Player_Item_Repair(attacker, item_id, g_iPlayerItemRepair[attacker][g_CurrentChar[attacker]][item_id] - 1);
					}
				}
			}
				
			new iTeam = fm_get_user_team( victim )

			if( iTeam == fm_get_user_team(attacker) )
			{
				fm_set_user_team( victim, iTeam == 1 ? 2 : 1 );
				ExecuteHam(Ham_TraceAttack, victim, attacker, damage, direction, traceresult, damagebits)
				fm_set_user_team(victim, iTeam )
				return HAM_SUPERCEDE;
			}
		}
	}

        return HAM_IGNORED;
}
// 殺死玩家後
public fwd_KilledPost(victim, attacker, shouldgib)
{
	if ( attacker > g_iMaxPlayers && get_pdata_int( victim, m_bitsDamageType ) & DMG_CRUSH && pev( attacker, pev_dmg ) ) 
	{ 
		if( get_pcvar_num(d2_enable_respawn) ) 
		{ 
			set_task(get_pcvar_float(d2_spawn_cooldown), "Spawn_Player", victim); 
		} 

		End_poison_damage(victim);
	}
}
// 殺死玩家後
public fwd_Killed(victim, attacker, shouldgib)
{
	if ( attacker )
	{
		if ( attacker != victim && (1 <= attacker <= g_iMaxPlayers) )
		{
			Set_Data_Kill(victim, attacker)

			new iTeam = fm_get_user_team( victim )
			
			if( iTeam == fm_get_user_team(attacker) )
			{
				fm_set_user_team( victim, iTeam == 1 ? 2 : 1 );
				ExecuteHamB(Ham_Killed, victim, attacker, shouldgib)
				fm_set_user_team(victim,iTeam)
			}
		}
	}

	if( get_pcvar_num(d2_enable_respawn) )
	{
		set_task(get_pcvar_float(d2_spawn_cooldown), "Spawn_Player", victim);
	}

	End_poison_damage(victim);

	return HAM_IGNORED;
}
public fwd_AttackSpeed ( const Entity )
{
	if ( !pev_valid(Entity) || !get_pcvar_num(d2_block_attack2) ) return HAM_IGNORED;

	set_pdata_float( Entity , m_flNextSecondaryAttack , 99999.0, 4 );
	
	return HAM_IGNORED;
}

public SetClientKeyValue(id, szInfoBuffer[], szKey[], szValue[])
{
	if(g_szModel[id][0] && equal(szKey, "model") && !equal(szValue, g_szModel[id]))
	{
		set_user_info(id, "model", g_szModel[id])
		return FMRES_SUPERCEDE
	}
	return FMRES_IGNORED
}

public fwd_CmdStart(client, uc_handle, seed)
{
	if ( !is_user_alive(client) ) 
		return FMRES_IGNORED;

	new button = get_uc(uc_handle, UC_Buttons);
	new oldbuttons = pev(client, pev_oldbuttons);

	static ent, body;
	new Float:aim_distance = get_user_aiming(client, ent, body);
	new Param[1];
	new model[33];
	pev(client, pev_viewmodel2, model, 32);
	
	if ( PRESSED( IN_ATTACK2 ) && !g_FreezeTime && !g_Shooting[client])
	{
		for (new item_id = 0; item_id <= g_charcounter; item_id++)
		{
			if ( Player_WItemDataActive(client, CHAR_CROSSBOWDAMAGE) && g_iPlayerItemBolts[client][g_CurrentChar[client]][item_id] > 0 && Player_WItemType(client, TYPE_BOLTS) )
			{
				Param[0] = item_id;
			
				g_Shooting[client] = true;
				
				entity_set_string(client, EV_SZ_viewmodel, g_v_crossbow);
				entity_set_string(client, EV_SZ_weaponmodel, g_p_crossbow);
		
				fm_set_animation(client, 6)
				set_task(float(SHOOT_TIME), "act_shoot", client + TASKID_CROSSBOW, Param, sizeof Param);
			}
		}
	}
	else if ( RELEASED( IN_ATTACK2 ) )
	{
		g_Shooting[client] = false;

		if (equali(model, g_v_crossbow))
		{
			entity_set_string(client, EV_SZ_viewmodel, g_brassknuckles);
			entity_set_string(client, EV_SZ_weaponmodel, "");
			
			for (new item_id = 0; item_id <= g_charcounter; item_id++)
			{
				if ( g_iPlayerItemWorn[client][g_CurrentChar[client]][item_id] && item_data[item_id][CHAR_TYPE] == TYPE_WEAPON && g_iPlayerItem[client][g_CurrentChar[client]][item_id] > 0 )
				{
					if ( equal(item_vmodel[item_id], "models/", 7) )
					{
						entity_set_string(client, EV_SZ_viewmodel, item_vmodel[item_id]);
					}
					else
					{
						entity_set_string(client, EV_SZ_viewmodel, g_brassknuckles);
					}
		
					if ( equal(item_pmodel[item_id], "models/", 7) )
					{
						entity_set_string(client, EV_SZ_weaponmodel, item_pmodel[item_id]);
					}
					else
					{
						entity_set_string(client, EV_SZ_weaponmodel, "");
					}
				}
			}
		}
		
		remove_task(client + TASKID_CROSSBOW);
	}

	if ( PRESSED( IN_USE ) && aim_distance <= DISTANCE_E_BUTTON_USE_ENT && !g_Shooting[client] )
	{
		if( pev_valid(ent) )
    	{
			static classname[32];
			pev( ent, pev_classname, classname, charsmax(classname) );

			if( equal(classname, "Inventory") )
			{
				main_inventory_menu(client)
				return FMRES_IGNORED;
			}
			else if ( equal(classname, "Charsi") )
			{
				main_charsi_menu(client);
				return FMRES_IGNORED;
			}
			else if ( equal(classname, "Akara") )
			{
				main_akara_menu(client)
				return FMRES_IGNORED;
			}
			else if ( equal(classname, "Miyu") )
			{
				main_miyu_menu(client)
				return FMRES_IGNORED;
			}
		}
	}

	if ( PRESSED( IN_USE ) && !g_Shooting[client] )
	{
		main_hero_skill_menu(client);
	}

	return FMRES_IGNORED;
}

// 玩家Kill指令.
public fwd_ClientKill()
{
	if ( get_pcvar_num(d2_block_kill_command) )
		return FMRES_SUPERCEDE;
	
	return FMRES_IGNORED;
}

// 禁止撿起武器.
public Fwd_Touch(ent, id)
{
	if ( is_user_alive(id) )
		return HAM_SUPERCEDE;
	
	return HAM_IGNORED;
}

// 清除掉落物
public SetModel_Post(entity, const model[])
{
	if (!pev_valid(entity) ) return FMRES_IGNORED

	new classname[32];
	pev(entity, pev_classname, classname, charsmax(classname))

	if (equal(classname, "weaponbox"))
		set_pev(entity, pev_nextthink, get_gametime())

	return FMRES_HANDLED
} 

// 刪除實體.
public fwd_Remove_Ents(ent)
{
	if( pev_valid(ent) )
	{
		static classname[32];
		pev(ent, pev_classname, classname, sizeof(classname) - 1);
        
		for (new i = 0; i < sizeof g_Objective_Ents; ++i) 
		{
			if( equal(classname, g_Objective_Ents[i]) )
			{
				engfunc(EngFunc_RemoveEntity, ent);
				return FMRES_SUPERCEDE;
			}
		}
	}
    
	return FMRES_IGNORED;
}

// 遊戲模式.
public fwd_GameDescription() 
{ 
	forward_return(FMV_STRING, mod_name);
	
	return FMRES_SUPERCEDE;
} 

// 獲得玩家的目前速度.
public fwd_ItemGetMaxSpeed ( const WeapIndex )
{
	if ( !pev_valid(WeapIndex) )
		return HAM_IGNORED;

	new PlayerId = get_pdata_cbase( WeapIndex, 41, 4 );
	GetOrigHamReturnFloat( Current_Speed[ PlayerId ] );

	return HAM_IGNORED;
}

public D2_Item_Gone(id, item_id)
{
	Take_Player_Armor_Item(id, item_id);

	if ( item_data[item_id][CHAR_GIVEVIT] > 0 )
	{
		g_Vitality[id][g_CurrentChar[id]] -= item_data[item_id][CHAR_GIVEVIT];
	}

	if ( item_data[item_id][CHAR_GIVEENE] > 0 )
	{
		g_Energy[id][g_CurrentChar[id]] -= item_data[item_id][CHAR_GIVEENE];
	}

	if( item_data[item_id][CHAR_SKILLSID] > 0  )
	{
		for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
		{
			if ( g_skillhero[skill_id] == item_data[item_id][CHAR_SKILLSID] )
			{
				g_iSkills[id][g_CurrentChar[id]][skill_id]--;
			}
		}
	}

}
public d2_skill_takedamage(victim, attacker, Float:iDamage[1])
{
	if ( native_get_p_in_safezone(victim) || native_get_p_in_safezone(attacker) )
		iDamage[0] = 0.0;

	if ( iDamage[0] > 0.0 )
	{
		if ( get_pcvar_num(d2_show_damage) )
		{
			set_hudmessage(255, 255, 100, 0.49, 0.40, 1, 1.5, 1.5, 0.0, 0.0, 3)
			ShowSyncHudMsg(attacker, g_SyncHudCreate_Dmg , "+%d", floatround(iDamage[0]));
		}

		if ( get_pcvar_num(d2_show_damage_received) )
		{
			set_hudmessage(255, 50, 0, 0.49, 0.43, 1, 1.5, 1.5, 0.0, 0.0, 2)
			ShowSyncHudMsg(victim, g_SyncHudCreate_Dmg , "-%d", floatround(iDamage[0]));
		}
	}
}
public Entity_Touched(ent, victim)
{
	if ( !pev_valid(ent) )
		return;
	
	new classname[32]
	pev( ent, pev_classname, classname, 31)

	new attacker = entity_get_edict(ent, EV_ENT_owner);
	
	if ( equal(classname,"Crossbow") )
	{
		if ( is_user_alive(victim) && victim != attacker && !native_get_p_in_safezone(victim) && !g_PlayerProtected[victim] && is_user_alive(attacker) && !native_get_p_in_safezone(attacker)
		&& !native_get_p_near_monster(victim) && !native_get_p_near_monster(attacker) )
		{
			for (new item_id = 0; item_id <= g_charcounter; item_id++)
			{
				if ( g_iPlayerItemWorn[attacker][g_CurrentChar[attacker]][item_id] 
				&& g_iPlayerItem[attacker][g_CurrentChar[attacker]][item_id] > 0 && item_data[item_id][CHAR_CROSSBOWDAMAGE] > 0 )
				{
					new Float:iArray[1]; iArray[0] = float(item_data[item_id][CHAR_CROSSBOWDAMAGE]);
					new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)
	
					ExecuteForward( g_RangedDamage, g_iReturn, victim, attacker, iArrayPass);

					Dmg_Kill_Player(victim, attacker, iArray[0], "ranged");
				}
			}
		}	

		set_pev( ent, pev_flags, FL_KILLME);
	}
		
}