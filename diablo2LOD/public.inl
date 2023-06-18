// 所有公用的Public.

Reset_Data(id)
{
	for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
	{
		g_PlayerHero[id][hero_id] = 0;
		g_PlayerCharActive[id][hero_id] = 0;
		g_PlayerLevel[id][hero_id] = 0
		g_PlayerXp[id][hero_id] = 0
		g_PlayerSkPoints[id][hero_id] = 0
		g_PlayerStPoints[id][hero_id] = 0
		g_Strength[id][hero_id] = 0
		g_Dexterity[id][hero_id] = 0
		g_Vitality[id][hero_id] = 0
		g_Energy[id][hero_id] = 0
		g_Coins[id][hero_id] = 0
		g_CoinsInv[id][hero_id] = 0
		g_Bosspoints[id][hero_id] = 0
	}	

	g_PlayerChars[id] = 0;
	g_CurrentChar[id] = 0
	g_CurrentMana[id] = 0

	for(new skill_id = 0; skill_id <= g_skillcounter ; skill_id++)
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			g_iSkills[id][hero_id][skill_id] = 0; 
		}
	}
	for(new item_p = 0; item_p < MAX_POTIONS ; item_p++)
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			g_iPlayerPotions[id][hero_id][item_p] = 0;
			g_iPlayerPotionsInv[id][hero_id][item_p] = 0;
		}
	}
	for(new item_id = 0; item_id <= g_charcounter ; item_id++)
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			g_iPlayerItem[id][hero_id][item_id] = 0;
			g_iPlayerItemInv[id][hero_id][item_id] = 0;
			g_iPlayerItemWorn[id][hero_id][item_id] = 0;
			g_iPlayerItemRepair[id][hero_id][item_id] = 0;
			g_iPlayerItemBolts[id][hero_id][item_id] = 0;
		}
	}
	for(new drops = 0; drops < MAX_DROPS ; drops++)
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			g_iPlayerDrops[id][hero_id][drops] = 0;
		}
	}
}
Reset_Data_Char(id, key)
{
	g_PlayerLevel[id][key] = 0
	g_PlayerXp[id][key] = 0
	g_PlayerSkPoints[id][key] = 0
	g_PlayerStPoints[id][key] = 0
	g_Strength[id][key] = 0
	g_Dexterity[id][key] = 0
	g_Vitality[id][key] = 0
	g_Energy[id][key] = 0
	g_Coins[id][key] = 0
	g_CoinsInv[id][key] = 0

	g_CurrentMana[id] = 0

	for(new skill_id = 0; skill_id <= g_skillcounter ; skill_id++)
	{
		g_iSkills[id][key][skill_id] = 0; 
	}
	for(new item_p = 0; item_p < MAX_POTIONS ; item_p++)
	{
		g_iPlayerPotions[id][key][item_p] = 0;
		g_iPlayerPotionsInv[id][key][item_p] = 0;
	}
	for(new item_id = 0; item_id <= g_charcounter ; item_id++)
	{
		g_iPlayerItem[id][key][item_id] = 0;
		g_iPlayerItemInv[id][key][item_id] = 0;
		g_iPlayerItemWorn[id][key][item_id] = 0;
		g_iPlayerItemRepair[id][key][item_id] = 0;
		g_iPlayerItemBolts[id][key][item_id] = 0;
	}
}

public Set_Player_Xp(id, value)
{
	if(g_PlayerLevel[id][g_CurrentChar[id]] < MAX_LEVELS-1) 
	{
		g_PlayerXp[id][g_CurrentChar[id]] = value

		if ( g_PlayerXp[id][g_CurrentChar[id]] < 0 )
		{
			g_PlayerXp[id][g_CurrentChar[id]] = 0;
		}

		if(g_PlayerXp[id][g_CurrentChar[id]] >= LEVELS[g_PlayerLevel[id][g_CurrentChar[id]]])
		{
			g_PlayerLevel[id][g_CurrentChar[id]]++;
			g_PlayerStPoints[id][g_CurrentChar[id]] += get_pcvar_num(d2_stat_points_levelup)
			g_PlayerSkPoints[id][g_CurrentChar[id]]++;				

			emit_sound(id, CHAN_ITEM, LevelUp, 1.0, ATTN_NORM, 0, PITCH_NORM);
		}
	}
}
public Set_Player_Mana(id, value)
{
	g_CurrentMana[id] = value

	if ( g_CurrentMana[id] < 0 )
	{
		g_CurrentMana[id] = 0;
	}

	if( g_CurrentMana[id] > HEROES_ENERGY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Energy[id][g_CurrentChar[id]])
	{
		g_CurrentMana[id] = HEROES_ENERGY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Energy[id][g_CurrentChar[id]];
	}
}

public Set_Player_Coins(id, value)
{
	if ( g_Coins[id][g_CurrentChar[id]] <= MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] )
	{
		g_Coins[id][g_CurrentChar[id]] = value

		if ( g_Coins[id][g_CurrentChar[id]] < 0 )
		{
			g_Coins[id][g_CurrentChar[id]] = 0;
		}

		if(g_Coins[id][g_CurrentChar[id]] > MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]])
		{
			g_Coins[id][g_CurrentChar[id]] = MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]]
		}
	}
}

public Set_Player_CoinsInv(id, value)
{
	if ( g_CoinsInv[id][g_CurrentChar[id]] <= MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]] )
	{
		g_CoinsInv[id][g_CurrentChar[id]] = value

		if ( g_CoinsInv[id][g_CurrentChar[id]] < 0 )
		{
			g_CoinsInv[id][g_CurrentChar[id]] = 0;
		}

		if(g_CoinsInv[id][g_CurrentChar[id]] > MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]])
		{
			g_CoinsInv[id][g_CurrentChar[id]] = MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]]
		}
	}
}

public Set_Player_Bosspoint(id, value)
{
	g_Bosspoints[id][g_CurrentChar[id]] = value

	if ( g_Bosspoints[id][g_CurrentChar[id]] < 0 )
	{
		g_Bosspoints[id][g_CurrentChar[id]] = 0;
	}

	if( g_Bosspoints[id][g_CurrentChar[id]] > 1000000000 )
	{
		g_Coins[id][g_CurrentChar[id]] = 1000000000
	}
}

public Set_Custom_Model(id)
{
	fm_set_user_model( id, Custom_Models[g_PlayerHero[id][g_CurrentChar[id]]] );
}

Set_Plugin_State(id, bool:bActive)
{
	if( bActive )
	{
		if( !g_iFwd )
		{
			g_iFwd = register_forward(FM_SetClientKeyValue, "SetClientKeyValue")
		}
		if( !g_iMsg  )
		{
			g_iMsg = register_message(gmsgClCorpse, "Message_ClCorpse")
		}
	}
	else
	{
		for(new i=1; i<=g_iMaxPlayers; i++)
		{
			if( i != id && g_szModel[i][0] )
			{
				return
			}
		}

		if( g_iFwd )
		{
			unregister_forward(FM_SetClientKeyValue, g_iFwd)
			g_iFwd = 0
		}

		if( g_iMsg )
		{
			g_iMsg = 0
		}
	}
}

public Adv_Message()
{
	if ( !get_pcvar_num(d2_advert) )
		return
	
	for(new id = 1; id <= g_iMaxPlayers; id++) 
	{
		if ( !is_user_connected(id) )
			continue;
		client_printcolor(id, "/y主要指令: /ctr/d2menu/y, /ctr/worn/y, /ctr/level/y, /ctr/trade/y, /ctr/potion/y, /ctr/spec/y.");
		client_printcolor(id, "/y倉庫用指令: /ctr/deposit 5/y, /ctr/withdraw 5/y.");
		client_printcolor(id, "/y控制台指令: /ctrbind t +skills/y, /ctrbind q potion/y.");
		client_printcolor(id, "/y特殊按鈕: /ctrE/y, /ctrMOUSE2/y - 遠距離攻擊/y, /ctrM/y - 打開主選單.");
	}
}
public Mana_Regenerate(id)
{
	id -= TASKID_MANAREGEN;

	if ( !is_user_alive(id) )
		return;

	Set_Player_Mana(id, g_CurrentMana[id] + 20);
}

public HUD_Say_Info_Task(id)
{
	if ( !g_iLogged[id] ) return PLUGIN_HANDLED;

	if ( !g_Player_Info[id] )
	{
		set_task(0.2, "HUD_Info_Task", id + TASKID_HUDINFO) // 顯示玩家資訊.
		g_Player_Info[id] = true;
	}
	else
	{
		remove_task(id + TASKID_HUDINFO);

		g_Player_Info[id] = false;
	}

	return PLUGIN_HANDLED;
}
public HUD_Info_Task(id)
{
	id -= TASKID_HUDINFO;

	if ( !is_user_connected(id) )
		return;
	
	if ( is_user_alive(id) )
	{
		set_hudmessage(255, 255, 150, 0.02, 0.12, 0, 0.0, 0.3, 0.0, 0.0)
		ShowSyncHudMsg(id, g_SyncHudCreate , "[職業: %s] [等級: %d] [經驗值: %d / %d]^n[能量: %d / %d]^n[血量: %d / %d]^n[錢: %d / %d]^n[BP: %d]^n[經驗倍率: %d]", HEROES[g_PlayerHero[id][g_CurrentChar[id]]], g_PlayerLevel[id][g_CurrentChar[id]],
		g_PlayerXp[id][g_CurrentChar[id]], LEVELS[g_PlayerLevel[id][g_CurrentChar[id]]],  g_CurrentMana[id], HEROES_ENERGY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]] * g_Energy[id][g_CurrentChar[id]], get_user_health(id), 
		g_MaxHealth[id], g_Coins[id][g_CurrentChar[id]], MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]], g_Bosspoints[id][g_CurrentChar[id]], get_pcvar_num(d2_exp_scale) )
	}
	else
	{
		set_hudmessage(255, 255, 150, 0.02, 0.12, 0, 0.0, 0.3, 0.0, 0.0)
		ShowSyncHudMsg(id, g_SyncHudCreate , "[職業: %s] [等級: %d] [經驗值: %d / %d]^n[錢: %d / %d]^n[BP: %d]^n[經驗倍率: %d]", HEROES[g_PlayerHero[id][g_CurrentChar[id]]], g_PlayerLevel[id][g_CurrentChar[id]],
		g_PlayerXp[id][g_CurrentChar[id]], LEVELS[g_PlayerLevel[id][g_CurrentChar[id]]], 
		g_Coins[id][g_CurrentChar[id]], MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]], g_Bosspoints[id][g_CurrentChar[id]], get_pcvar_num(d2_exp_scale) )

	}

	set_task(0.2, "HUD_Info_Task", id + TASKID_HUDINFO)
}

public Hide_Hud(id) 
{
	if ( !is_user_alive(id) ) return;
	
	message_begin(MSG_ONE_UNRELIABLE, MsgHideWeapon, _ , id )
	write_byte( HIDE_TIMER | HIDE_RADAR | HIDE_MONEY | HIDE_AMMO | HIDE_FLASH )
	message_end()

	message_begin( MSG_ONE_UNRELIABLE, MsgHideCrosshair, _, id )
	write_byte( 0 )
	message_end()
}
public Remove_Spawn_Protection(id)
{
	if ( !is_user_alive(id) )
		return;	

	g_PlayerProtected[id] = false;
	set_pev(id, pev_takedamage, 1.0);
}
public Spawn_Player(id)
{
	if ( !is_user_connected(id) || is_user_alive(id) || cs_get_user_team(id) == CS_TEAM_SPECTATOR )
		return;

	ExecuteHamB(Ham_CS_RoundRespawn, id);
}
public Spawn_Player_First_Connected(id)
{
	id -= TASKID_FIRSTCONNECT;

	if ( !is_user_connected(id) || is_user_alive(id) || cs_get_user_team(id) == CS_TEAM_SPECTATOR )
		return;
	
	ExecuteHamB(Ham_CS_RoundRespawn, id);
}
public Start_poison_damage(Param[], victim)
{
	victim -= TASKID_POISON;

	new attacker = Param[0];
	new item_id = Param[1];

	if ( !is_user_alive(victim) || !is_user_connected(attacker) )
		return;

	new Float:iArray[1]; iArray[0] = float(item_data[item_id][CHAR_PDAMAGE]);
	new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)
	
	ExecuteForward( g_DaggerPoisonDamage, g_iReturn, victim, attacker, iArrayPass);

	if ( iArray[0] > 0.0 )
		Dmg_Kill_Player(victim, attacker, iArray[0], "poison");
}
public End_poison_damage(victim)
{
	if ( task_exists( victim + TASKID_POISON ) )
	{
		if ( is_user_connected(victim) )
		{	
			set_user_rendering(victim);
		}

		remove_task( victim + TASKID_POISON );
	}
}
public Native_Dmg_Kill_Player( iPlugin ) 
{
	new id = get_param( 1 )
	new attacker = get_param ( 2 )
	new Float:damage; 
	damage = get_param_f ( 3 );
	new weaponDescription[32];
	get_string( 4, weaponDescription, sizeof ( weaponDescription ) - 1 );
	
	if( is_user_alive(id) && is_user_alive(attacker) && damage > 0.0 ) 
	{
		entity_set_edict( g_FakeEnt, EV_ENT_owner, attacker);
		entity_set_string( g_FakeEnt, EV_SZ_classname, weaponDescription);
		
		static Float:userHealth;
		pev(id, pev_health, userHealth);

		new Float:iArray[1]; iArray[0] = damage;
		new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)

		ExecuteForward( g_DamagedbySkill, g_iReturn, id, attacker, iArrayPass);

		if ( iArray[0] > 0.0 )
		{
			if(userHealth - iArray[0] < 1.0)
			{
				make_deathmsg( attacker, id, 0 , weaponDescription);

				user_silentkill(id);

				if ( id != attacker )
				{
					Set_Data_Kill(id, attacker);
				}
			}
			else
			{
				fakedamage( id, weaponDescription, iArray[0], DMG_GENERIC );
			}
		}
	}
}
public Dmg_Kill_Player(id, attacker, Float:damage, weaponDescription[])
{
	if( is_user_alive(id) ) 
	{
		static Float:userHealth;
		pev(id, pev_health, userHealth);

		new Float:iArray[1]; iArray[0] = damage;
		new iArrayPass = PrepareArray( _:iArray, sizeof(iArray), 1)

		ExecuteForward( g_DamagedbySkill, g_iReturn, id, attacker, iArrayPass);

		if ( iArray[0] > 0.0 )
		{
			if(userHealth - iArray[0] < 1.0)
			{
				make_deathmsg( attacker, id, 0 , weaponDescription);

				user_silentkill(id);

				Set_Data_Kill(id, attacker);
			}
			else
			{
				fakedamage( id, weaponDescription, iArray[0], DMG_GENERIC );
			}
		}
	}
}

public Set_Data_Kill(victim, attacker)
{
	new random_DROP = random_num(0, 100);

	if ( g_PlayerLevel[victim][g_CurrentChar[victim]] >= get_pcvar_num(d2_minlevel_items) && g_PlayerLevel[attacker][g_CurrentChar[attacker]] >= get_pcvar_num(d2_minlevel_items) && random_DROP <= get_pcvar_num(d2_chance_to_drop) && get_pcvar_num(d2_chance_to_drop) > 0 )
	{
		Drop_Items(victim);
	}

	if ( g_PlayerLevel[victim][g_CurrentChar[victim]] >= get_pcvar_num(d2_minlevel_potions) && g_PlayerLevel[attacker][g_CurrentChar[attacker]] >= get_pcvar_num(d2_minlevel_potions) && random_DROP <= get_pcvar_num(d2_chance_to_dropp) && get_pcvar_num(d2_chance_to_dropp) > 0 )
	{
		Drop_Potions(victim);
	}

	if ( g_PlayerLevel[victim][g_CurrentChar[victim]] >= get_pcvar_num(d2_minlevel_coins) && g_PlayerLevel[attacker][g_CurrentChar[attacker]] >= get_pcvar_num(d2_minlevel_coins) )
	{
		g_CoinsDropDeath[victim] = g_Coins[victim][g_CurrentChar[victim]] / 4 + g_PlayerLevel[victim][g_CurrentChar[victim]];

		Create_Coins(victim);

		if ( g_iLogged[victim] )
			Set_Player_Coins(victim, g_Coins[victim][g_CurrentChar[victim]] - g_CoinsDropDeath[victim])
	}


	if ( g_PlayerLevel[attacker][g_CurrentChar[attacker]] < get_pcvar_num(d2_high_level) && g_PlayerLevel[victim][g_CurrentChar[victim]] >= get_pcvar_num(d2_high_level) )
	{
		Set_Player_Xp(attacker, g_PlayerXp[attacker][g_CurrentChar[attacker]] + get_pcvar_num(d2_high_level_xp) )
	}

	if ( g_iLogged[attacker] )
		Set_Player_Xp(attacker, g_PlayerXp[attacker][g_CurrentChar[attacker]] + get_pcvar_num(d2_xp_kill) )

}

public give_player_item(id, item_id)
{
	g_iPlayerItem[id][g_CurrentChar[id]][item_id]++;

	if ( item_data[item_id][CHAR_TYPE] == TYPE_BOLTS )
	{
		g_iPlayerItemBolts[id][g_CurrentChar[id]][item_id] = get_pcvar_num(d2_bolts_value);
	}

	if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] <= 1 )
	{
		g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;
	}

	g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] = item_data[item_id][CHAR_DURABILITY];
}