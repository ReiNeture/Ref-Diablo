// Diablo II LOD 選單.

// 主選單.
public main_hero_menu_connect(id)
{
	new szMsg[60];
	formatex(szMsg, 59, "主選單 : ")
	
	new menu = menu_create(szMsg , "hero_menu_connect");
	menu_additem(menu, "創造初始角色", "0", 0);
	if ( g_PlayerChars[id] > 0 )
		menu_additem(menu, "選擇角色", "1", 0);
	menu_additem(menu, "刪除角色", "2", 0);
	
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public hero_menu_connect(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 0:
		{
			main_hero_menu(id);
		}
		case 1:
		{
			if ( g_PlayerChars[id] < 1 )
			{
				client_printcolor( id, "/y你有 (/ctr0/y) 個角色, 創造一個新的!" )
				main_hero_menu_connect(id)
				return PLUGIN_HANDLED;
			}
			
			main_hero_menu_choose(id);
		}
		case 2:
		{
			if ( g_PlayerChars[id] < 1 )
			{
				client_printcolor( id, "/y你有 (/ctr0/y) 個角色, 創造一個新的!" )
				main_hero_menu_connect(id)
				return PLUGIN_HANDLED;
			}
			
			main_hero_menu_delete(id);
		}
	}
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 
public main_hero_menu(id)
{
	new szMsg[60], szTempid[32], szItems[512];
	formatex(szMsg, 59, "創造初始角色 : ")
	
	new menu = menu_create(szMsg , "hero_menu");
	for (new hero_id = 0; hero_id < 1; hero_id++)
	{
		formatex(szItems, charsmax(szItems), "%s", HEROES[hero_id])
		num_to_str(hero_id, szTempid, 31);
		menu_additem(menu, szItems, szTempid, 0);
	}
	
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public hero_menu(id , menu , item) 
{ 
	if ( !is_user_connected(id) || g_PlayerChars[id] >= 1 )
	{
		// client_printcolor( id, "/y你的角色太多了 (/ctr%d/y) !", MAX_CHARS )
		client_printcolor( id, "/y你已經有初始角色了!")
		main_hero_menu_connect(id)
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data), Val;

	g_PlayerChars[id]++;
	
	for (new hero_id = 0; hero_id < g_PlayerChars[id]; hero_id++)
	{
		if ( !g_PlayerCharActive[id][hero_id] )
		{
			Val = hero_id;
		}
	}
	
	g_PlayerCharActive[id][Val] = CHAR_ACTIVE;
	
	g_PlayerHero[id][Val] = key;
	
	g_Vitality[id][Val] = 10;
	g_Energy[id][Val] = 10;

	client_printcolor( id, "/y角色 /g%s /y以創造, 角色數量 : /ctr%d", HEROES[key], g_PlayerChars[id]);
	
	main_hero_menu_connect(id)
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 
public main_hero_menu_choose(id)
{
	new szMsg[60], szTempid[32], szItems[512];
	formatex(szMsg, 59, "選擇角色 : ")
	
	new menu = menu_create(szMsg , "hero_menu_choose");
	for (new hero_id = 0; hero_id < MAX_CHARS + 1; hero_id++)
	{
		if ( g_PlayerCharActive[id][hero_id] )
		{
			formatex(szItems, charsmax(szItems), "%s \r編號: %d - 等級: %d", HEROES[g_PlayerHero[id][hero_id]], hero_id, g_PlayerLevel[id][hero_id])

			num_to_str(hero_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}	
	}
	
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public hero_menu_choose(id , menu , item) 
{ 
	if ( g_PlayerChars[id] < 1 || !is_user_connected(id) )
	{
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	g_CurrentChar[id] = key;
	
	g_iLogged[id] = true;
	ExecuteForward( g_iLog, g_iReturn, id, LOGGED);

	if ( !g_Player_Info[id] )
	{
		set_task(0.2, "HUD_Info_Task", id + TASKID_HUDINFO) // 顯示玩家資訊
		g_Player_Info[id] = true;
	}

	cs_set_user_team(id, CS_TEAM_T);
	
	set_task( get_pcvar_float(d2_spawn_cooldown), "Spawn_Player_First_Connected", id + TASKID_FIRSTCONNECT);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 
public main_hero_menu_delete(id)
{
	new szMsg[60], szTempid[32], szItems[512];
	formatex(szMsg, 59, "刪除角色 : ")
	
	new menu = menu_create(szMsg , "hero_menu_delete");
	for (new hero_id = 0; hero_id < MAX_CHARS + 1; hero_id++)
	{
		if ( g_PlayerCharActive[id][hero_id] )
		{
			formatex(szItems, charsmax(szItems), "%s \r編號: %d - 等級: %d", HEROES[g_PlayerHero[id][hero_id]], hero_id, g_PlayerLevel[id][hero_id])

			num_to_str(hero_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}
	}
	
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public hero_menu_delete(id , menu , item) 
{ 
	if ( g_PlayerChars[id] < 1 )
	{
		main_hero_menu_connect(id)
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	g_PlayerChars[id]--;
	g_PlayerCharActive[id][key] = CHAR_DELETED;
	
	client_printcolor( id, "/y角色 /g%s /y已刪除, 角色數量 : /ctr%d", HEROES[g_PlayerHero[id][key]], g_PlayerChars[id]);
	
	Reset_Data_Char(id, key)
	
	main_hero_menu_connect(id);
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 

// 模式主選單.
public main_mod_menu(id)
{
	if ( !g_iLogged[id] )
		return PLUGIN_HANDLED;

	new menu = menu_create("Diablo II LOD 選單" , "mod_menu");
	menu_additem(menu ,"\w能力選單", "1" , 0); 
	menu_additem(menu ,"\w技能選單", "2" , 0); 
	menu_additem(menu ,"\w個人狀態", "3" , 0);
	menu_additem(menu ,"\w回到觀戰者", "4" , 0);
	menu_additem(menu ,"\w職業 \r技能 \w選擇", "5" , 0);
	menu_additem(menu ,"\w玩家選單", "6" , 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
	
	return PLUGIN_HANDLED;
}
public mod_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			main_stats_menu(id);
		}
		case 2:
		{
			main_skill_menu(id);
		}
		case 3:
		{
			main_info_menu(id);
		}
		case 4:
		{
			force_player_not_joined(id);
		}
		case 5:
		{
			main_hero_skill_menu(id)
		}
		case 6:
		{
			main_misc_client_menu(id)
		}
	}
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 
// 玩家選單.
public main_misc_client_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new menu = menu_create("玩家選單" , "misc_client_menu");
	menu_additem(menu ,"裝備道具", "1" , 0); 
	menu_additem(menu ,"使用藥水", "2" , 0); 
	menu_additem(menu ,"交易道具", "3" , 0);
	menu_additem(menu ,"給予道具", "4" , 0);
	menu_additem(menu ,"其他物品", "5" , 0);
	if ( get_user_flags(id) & ADMIN_RCON )
	{
		menu_additem(menu ,"管理員 : 創造NPC", "6" , 0);
	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public misc_client_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			main_worn_menu(id);
		}
		case 2:
		{
			main_potion_menu(id);
		}
		case 3:
		{
			main_trade_menu(id);
		}
		case 4:
		{
			main_give_menu(id);
		}
		case 5:
		{
			main_drops_menu(id);
		}
		case 6:
		{
			Open_MapItemConfig(id);
		}
	}
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 
// 選擇技能選單.
public main_hero_skill_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new szMsg[60];

	new szTempid[32], szTempidNone[32];

	formatex(szMsg, 59, "選擇你要使用的技能:");
	new menu = menu_create(szMsg , "hero_skill_menu");

	for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
	{
		if ( g_iSkills[id][g_CurrentChar[id]][skill_id] > 0 && g_skilldisplay[skill_id] )
		{
			new szItems[60];
			formatex(szItems, 59, "%s", g_skillname[skill_id])

			num_to_str(skill_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}
	}
	/*
	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][CHAR_SKILLSID] > 0 )
		{
			for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
			{
				if( item_data[item_id][CHAR_SKILLSID] == g_skillhero[skill_id] )
				{
					new szItems[60];
					formatex(szItems, 59, "%s", g_skillname[skill_id])

					num_to_str(skill_id, szTempid, 31);

					menu_additem(menu, szItems, szTempid, 0);
				}
			}
		}
	}
	*/

	num_to_str(g_skillcounter + 3, szTempidNone, 31);
	menu_additem(menu, "取消選擇", szTempidNone, 0);
	
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public hero_skill_menu(id , menu , item) 
{ 
	if ( !is_user_alive(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new skill_id = str_to_num(data);
	
	ExecuteForward( g_SkillSelected, g_iReturn, id, skill_id);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
} 

// 能力選單.
public main_stats_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new szStats[60],szDexterity[60],szStrength[60],szVitality[60],szEnergy[60];
	formatex(szStats, 59, "剩餘能力值 \d[ \y%d \d]", g_PlayerStPoints[id][g_CurrentChar[id]])
	formatex(szDexterity, 59, "敏捷 ( \y%d \w)", g_Dexterity[id][g_CurrentChar[id]])
	formatex(szStrength, 59, "力量 ( \y%d \w)", g_Strength[id][g_CurrentChar[id]])
	formatex(szVitality, 59, "體力 ( \y%d \w)", g_Vitality[id][g_CurrentChar[id]])
	formatex(szEnergy, 59, "魔力 ( \y%d \w)", g_Energy[id][g_CurrentChar[id]])

	new menu = menu_create(szStats , "stats_menu");

	menu_additem(menu , szDexterity, "1" , 0); 
	menu_additem(menu , szStrength, "2" , 0); 
	menu_additem(menu , szVitality, "3" , 0);
	menu_additem(menu , szEnergy, "4" , 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public stats_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			if(g_PlayerStPoints[id][g_CurrentChar[id]] > 0)
			{
				g_Dexterity[id][g_CurrentChar[id]]++
				g_PlayerStPoints[id][g_CurrentChar[id]]--
				
				main_stats_menu(id)
			}
		}
		case 2:
		{
			if(g_PlayerStPoints[id][g_CurrentChar[id]] > 0)
			{
				g_Strength[id][g_CurrentChar[id]]++
				g_PlayerStPoints[id][g_CurrentChar[id]]--
				
				main_stats_menu(id)
			}
		}
		case 3:
		{
			if(g_PlayerStPoints[id][g_CurrentChar[id]] > 0)
			{
				g_Vitality[id][g_CurrentChar[id]]++
				g_PlayerStPoints[id][g_CurrentChar[id]]--

				g_MaxHealth[id] += HEROES_VITALITY_PERSTAT[g_PlayerHero[id][g_CurrentChar[id]]];
				
				main_stats_menu(id)
			}
		}
		case 4:
		{
			if(g_PlayerStPoints[id][g_CurrentChar[id]] > 0)
			{
				
				g_Energy[id][g_CurrentChar[id]]++
				g_PlayerStPoints[id][g_CurrentChar[id]]--
				
				main_stats_menu(id)
			}
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
// Stat Points Menu
public main_skill_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new szInfo[60];
	formatex(szInfo, 59, "職業: \r%s \d- \ySP\d[ \r%d \d]", HEROES[g_PlayerHero[id][g_CurrentChar[id]]], g_PlayerSkPoints[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "skill_menu");

	new szTempid[32];

	for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
	{
		if ( g_PlayerHero[id][g_CurrentChar[id]] == g_skillhero[skill_id] || g_iSkills[id][g_CurrentChar[id]][skill_id] > 0 )
		{
			new szItems[90];
			formatex(szItems, 89, "%s \d( \y%d/%d \d) %s - 需要等級 %s%d", 
				g_skillname[skill_id], 
				g_iSkills[id][g_CurrentChar[id]][skill_id], 
				g_skillmax[skill_id], 
				(g_skilldisplay[skill_id] ? "" : "\w[ \y被動 \w]\d" ), 
				(g_PlayerLevel[id][g_CurrentChar[id]] < g_skilllevel[skill_id] ? "\r" : "\y"), 
				g_skilllevel[skill_id] )

			num_to_str(skill_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}
	}

	menu_setprop(menu, MPROP_PERPAGE , 6);
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public skill_menu(id , menu , item) 
{ 
	if ( !is_user_connected(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new skill_id = str_to_num(data);
	if ( g_PlayerLevel[id][g_CurrentChar[id]] < g_skilllevel[skill_id] )
	{
		client_printcolor(id, "/y需要 /g%d 等", g_skilllevel[skill_id]);

		return PLUGIN_HANDLED;
	}

	if ( g_iSkills[id][g_CurrentChar[id]][skill_id] >= g_skillmax[skill_id] )
	{
		client_printcolor(id, "/ctr%s 的等級已達到 /g%d!", g_skillname[g_PlayerHero[id][g_CurrentChar[id]]][skill_id], g_skillmax[skill_id]);

		return PLUGIN_HANDLED;
	}

	if(g_PlayerSkPoints[id][g_CurrentChar[id]] > 0)
	{
		g_iSkills[id][g_CurrentChar[id]][skill_id]++;

		g_PlayerSkPoints[id][g_CurrentChar[id]]--
				
		main_skill_menu(id)
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// NPC創造選單.
public Open_MapItemConfig(id)
{
	if ( get_user_flags(id) & ADMIN_RCON )
	{
		new menu = menu_create("NPC選單" , "map_item_menu");
		menu_additem(menu ,"創造倉庫 ", "1" , 0); 
		menu_additem(menu ,"創造武器商人", "2" , 0); 
		menu_additem(menu ,"創造藥水商人", "3" , 0); 
		menu_additem(menu ,"創造角色商人", "4" , 0); 
		menu_additem(menu ,"刪除所有倉庫", "5" , 0); 
		menu_additem(menu ,"刪除所有武器商人", "6" , 0); 
		menu_additem(menu ,"刪除所有藥水商人", "7" , 0); 
		menu_additem(menu ,"刪除所有角色商人", "8" , 0); 
		
		menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
		menu_display(id , menu , 0); 
	}
	else
	{
		client_print(id, print_chat, "你沒有權限使用!")
	}
	
	return PLUGIN_HANDLED;
}
public map_item_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new MapName[33]
	get_mapname(MapName, 32)
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);
	
	switch(key) 
	{
		case 1: // 倉庫.
		{
			if(g_MapItemNum >= MAX_MAPITEMS)
			{
				client_print(id, print_chat, "達到地圖最大限制!")
				return PLUGIN_HANDLED
			}
			
			new Origin[3]
			get_user_origin(id, Origin, 3)

			Create_Inventory(Origin)
			Save_Origin(MapName, Origin)
			Load_Origins(MapName)
			
			Open_MapItemConfig(id)
			client_print(id, print_chat, "倉庫已創造!")
		}
		case 2: // 武器商人.
		{
			if(g_MapItemNum >= MAX_MAPITEMS)
			{
				client_print(id, print_chat, "達到地圖最大限制!")
				return PLUGIN_HANDLED
			}
			
			new Origin[3]
			get_user_origin(id, Origin, 3)

			Create_Charsi(Origin)
			Save_Origin_Charsi(MapName, Origin)
			Load_Origins_Charsi(MapName)
			
			Open_MapItemConfig(id)
			client_print(id, print_chat, "武器商人已創造!")
		}
		case 3: // 藥水商人.
		{
			if(g_MapItemNum >= MAX_MAPITEMS)
			{
				client_print(id, print_chat, "達到地圖最大限制!")
				return PLUGIN_HANDLED
			}
			
			new Origin[3]
			get_user_origin(id, Origin, 3)

			Create_Akara(Origin)
			Save_Origin_Akara(MapName, Origin)
			Load_Origins_Akara(MapName)
			
			Open_MapItemConfig(id)
			client_print(id, print_chat, "藥水商人以創造!")
		}
		case 4: // 角色商人.
		{
			if(g_MapItemNum >= MAX_MAPITEMS)
			{
				client_print(id, print_chat, "達到地圖最大限制!")
				return PLUGIN_HANDLED
			}
			
			new Origin[3]
			get_user_origin(id, Origin, 3)
			Origin[2] += 36
			Create_Miyu(Origin)
			Save_Origin_Miyu(MapName, Origin)
			Load_Origins_Miyu(MapName)
			
			Open_MapItemConfig(id)
			client_print(id, print_chat, "角色商人以創造!")
		}
		case 5: 
		{
			RemoveMapItems()
			Remove_All_Inventory_Ents();
			client_print(id, print_chat, "所有倉庫已刪除!")
		}
		case 6: 
		{
			RemoveMapItems_Charsi()
			Remove_All_Charsi_Ents();
			client_print(id, print_chat, "所有武器商人已刪除!")
		}
		case 7: 
		{
			RemoveMapItems_Akara()
			Remove_All_Akara_Ents();
			client_print(id, print_chat, "所有藥水商人已刪除!")
		}
		case 8: 
		{
			RemoveMapItems_Miyu()
			Remove_All_Miyu_Ents();
			client_print(id, print_chat, "所有角色商人已刪除!")
		}
	}
	
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 武器商人選單.
public main_charsi_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d] - \r武器商人", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "charsi_menu");

	menu_additem(menu , "購買道具", "1" , 0); 
	menu_additem(menu , "販賣道具", "2" , 0); 
	menu_additem(menu , "修復道具", "3" , 0); 
	menu_additem(menu , "道具資訊", "4" , 0); 

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public charsi_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_charsi(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			main_charsi_buy_menu(id);
		}
		case 2:
		{
			main_charsi_sell_menu(id);
		}
		case 3:
		{
			main_charsi_repair_menu(id);
		}
		case 4:
		{
			main_charsi_info_menu(id);
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_charsi_buy_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d]", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "charsi_buy_menu");

	new szTempid[32];

	const last_item = 12;
	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		num_to_str(item_id, szTempid, 31);

		new szItems[60];
		formatex(szItems, 59, "%s \d( \y%d \d)", item_name[item_id], item_data[item_id][CHAR_COST])

		menu_additem(menu, szItems, szTempid, 0);

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public charsi_buy_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_charsi(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(id, "無法購買,數量達到最大限制!")

		return PLUGIN_HANDLED;
	}

	if ( g_Coins[id][g_CurrentChar[id]] >= item_data[item_id][CHAR_COST] )
	{
		client_printcolor(id, "/y道具 /g%s /y已購買!", item_name[item_id] );

		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - item_data[item_id][CHAR_COST]);

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
	else
	{
		client_printcolor(id, "/y金錢不足! 你的錢:/g%d/y, 道具價格:/g%d", g_Coins[id][g_CurrentChar[id]], item_data[item_id][CHAR_COST])
	}
	
	main_charsi_menu(id);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
public main_charsi_sell_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d]", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "charsi_sell_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s \d( \y%d \d) - \r%d", item_name[item_id], item_data[item_id][CHAR_COST] * (g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] + 1) / (item_data[item_id][CHAR_DURABILITY] + 1) / 2  ,
			g_iPlayerItem[id][g_CurrentChar[id]][item_id])

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public charsi_sell_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_charsi(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "無法販賣,你沒有道具!")
		return PLUGIN_HANDLED;
	}

	new item_s_cost = item_data[item_id][CHAR_COST] * (g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] + 1) / (item_data[item_id][CHAR_DURABILITY] + 1) / 2;

	if ( g_Coins[id][g_CurrentChar[id]] < MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] && g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/y道具 /g%s /y已販賣!", item_name[item_id])

		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] + item_s_cost );

		g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] < 1 && g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] )
		{
			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;

			ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
		}
	} 
	
	main_charsi_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
public main_charsi_repair_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d]", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "charsi_repair_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 && item_data[item_id][CHAR_DURABILITY] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s \d( \y%d \d) - \r 耐久度 %d", item_name[item_id], (item_data[item_id][CHAR_DURABILITY] - g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] + 1) * (item_data[item_id][CHAR_COST] + 1) / (item_data[item_id][CHAR_DURABILITY] + 1) / 2 ,
			g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id])

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public charsi_repair_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_charsi(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] == item_data[item_id][CHAR_DURABILITY] || Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "無法修復,你沒有道具或是耐久度已滿!")

		return PLUGIN_HANDLED;
	}

	if ( item_data[item_id][CHAR_DURABILITY] < 1 )
	{
		client_printcolor(id, "該道具無法修復!");
		return PLUGIN_HANDLED
	}

	new item_r_cost = (item_data[item_id][CHAR_DURABILITY] - g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] + 1) * (item_data[item_id][CHAR_COST] + 1) / (item_data[item_id][CHAR_DURABILITY] + 1) / 2;

	if ( g_Coins[id][g_CurrentChar[id]] >= item_r_cost )
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			client_printcolor(id, "/y道具 /g%s /y已修復!", item_name[item_id] );

			Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - item_r_cost);

			g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id] = item_data[item_id][CHAR_DURABILITY];
		}
	} 
	else
	{
		client_printcolor(id, "/y金錢不足!你的錢:/g%d/y, 道具修復價格:/g%d", g_Coins[id][g_CurrentChar[id]], item_r_cost)
	}
	
	main_charsi_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_charsi_info_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "選擇要查看的 \r道具資訊")

	new menu = menu_create(szInfo , "charsi_info_menu");
	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		num_to_str(item_id, szTempid, 31);

		new szItems[60];
		formatex(szItems, 59, "%s", item_name[item_id])
		menu_additem(menu, szItems, szTempid, 0);
	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public charsi_info_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_charsi(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	client_printcolor(id, "/y============== /ctr%s /y==============", item_name[item_id]);
	client_printcolor(id, "/y編號:/g%d/y, 種類:/g%s/y, 價格:/g%d$", item_id, Items_Type_Name[item_data[item_id][CHAR_TYPE]], item_data[item_id][CHAR_COST]);
	client_printcolor(id, "/ctr裝備素質 /y傷害/g+%d/y, 防禦力/g+%d/y, 體力/g+%d/y, 能量/g+%d/y, 爆擊率/g+%d", 
		item_data[item_id][CHAR_DAMAGE], item_data[item_id][CHAR_ARMOR], item_data[item_id][CHAR_GIVEVIT], 
		item_data[item_id][CHAR_GIVEENE], item_data[item_id][CHAR_BLOCK]);
	client_printcolor(id, "/ctr裝備需求 /y等級/g%d/y 力量/g%d/y 敏捷/g%d", item_data[item_id][CHAR_LEVEL], item_data[item_id][CHAR_STR], item_data[item_id][CHAR_DEX]);
	
	main_charsi_info_menu(id);
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 藥水商人選單.
public main_akara_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d] - \r藥水商人", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "akara_menu");

	menu_additem(menu , "購買藥水", "1" , 0); 
	menu_additem(menu , "販賣藥水", "2" , 0); 

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public akara_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_akara(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			main_akara_buy_menu(id);
		}
		case 2:
		{
			main_akara_sell_menu(id);
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_akara_buy_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d]", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "akara_buy_menu");

	new szTempid[32];

	for (new item_id = 0; item_id < MAX_POTIONS; item_id++)
	{
		num_to_str(item_id, szTempid, 31);

		new szItems[60];
		formatex(szItems, 59, "%s \d( \y%d \d)", Potion_Names[item_id], Potion_Cost[item_id])

		menu_additem(menu, szItems, szTempid, 0);

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public akara_buy_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_akara(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_Potions(id) >= MAX_PLAYER_POTIONS )
	{
		client_printcolor(id, "無法購買,藥水數量已達最大限制!")

		return PLUGIN_HANDLED;
	}

	if ( g_Coins[id][g_CurrentChar[id]] >= Potion_Cost[item_id] )
	{
		client_printcolor(id, "/g%s /y已購買!", Potion_Names[item_id] );

		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - Potion_Cost[item_id]);

		g_iPlayerPotions[id][g_CurrentChar[id]][item_id]++;
	} 
	else
	{
		client_printcolor(id, "/y金錢不足! 你的錢:/g%d/y, 藥水價格:/g%d", g_Coins[id][g_CurrentChar[id]], Potion_Cost[item_id])
	}
	
	main_akara_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
public main_akara_sell_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "錢 \d[ \y%d \d]", g_Coins[id][g_CurrentChar[id]])

	new menu = menu_create(szInfo , "akara_sell_menu");

	new szTempid[32];

	for (new item_id = 0; item_id < MAX_POTIONS; item_id++)
	{
		if ( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s \d( \y%d \d) - \r%d", Potion_Names[item_id], (Potion_Cost[item_id] / 2), g_iPlayerPotions[id][g_CurrentChar[id]][item_id])

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public akara_sell_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_akara(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_Potions(id) <= 0 )
	{
		client_printcolor(id, "無法販賣,你沒有要水!")
		return PLUGIN_HANDLED;
	}

	if ( g_Coins[id][g_CurrentChar[id]] < MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] && g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/y/g%s /y已販賣!", Potion_Names[item_id])

		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] + (Potion_Cost[item_id] / 2) );

		g_iPlayerPotions[id][g_CurrentChar[id]][item_id]--;
	} 
	
	main_akara_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_miyu_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "萬用商人", g_PlayerChars[id])
	new menu = menu_create(szInfo , "miyu_menu");
	menu_additem(menu , "購買角色", "1" , 0);
	menu_additem(menu , "製作道具", "2" , 0);
	menu_additem(menu , "BP商店", "3" , 0);
	menu_additem(menu , "重置能力值", "4" , 0);
	menu_additem(menu , "和他瑟瑟", "5" , 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public miyu_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_miyu(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);
	switch(key) {
		case 1: main_buychar_menu(id);
		case 2: main_maker_menu(id);
		case 3: main_bpshop_menu(id);
		case 4: main_reset_apsp_menu(id);
		case 5: do_miyu_h(id);
	}
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_maker_menu(id)
{
	// new szItems[60];
	new menu = menu_create("要製作哪一件道具?" , "maker_menu");
	menu_additem(menu, "爪刀", "1", 0);
	menu_additem(menu, "表面硬化爪刀", "2", 0);
	menu_additem(menu, "參考刀", "3", 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}

public maker_menu(id , menu , item) 
{
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);
	switch(key) {
		case 1: main_make_karambit_menu(id);
		case 2: main_make_karambithardened_menu(id);
		case 3: main_make_refknife_menu(id);
	}
}

public main_make_karambithardened_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r表面硬化爪刀 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w爪刀 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\wA 血液 \yx100 ^n");
	add(szMenu, sizeof(szMenu), "\w殭屍王之心 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w高階催化劑 \yx10 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_karambit_hardened_menu");
}

public make_karambit_hardened_menu(id, num) 
{
	if( num == N8 )
	{
		if (!is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS)
		{
			client_printcolor(id, "無法製作,道具數量達到最大限制!")
			return PLUGIN_HANDLED;
		}

		const karambit_id = 11;
		const karambit_hardened_id = 12;

		if( g_iPlayerDrops[id][g_CurrentChar[id]][A_BOOLD] >= 100 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] >= 10 &&
			g_iPlayerDrops[id][g_CurrentChar[id]][GONOME_HEART] >= 1 &&
			g_iPlayerItem[id][g_CurrentChar[id]][karambit_id] >= 1 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][A_BOOLD] -= 100;
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] -= 10;
			g_iPlayerDrops[id][g_CurrentChar[id]][GONOME_HEART] -= 1;
			g_iPlayerItem[id][g_CurrentChar[id]][karambit_id]--;

			if ( g_iPlayerItem[id][g_CurrentChar[id]][karambit_id] < 1 && g_iPlayerItemWorn[id][g_CurrentChar[id]][karambit_id] )
			{
				g_iPlayerItemWorn[id][g_CurrentChar[id]][karambit_id] = ITEM_NOT_WORN;
				ExecuteForward( g_iItemTook, g_iReturn, id, karambit_id);
			}

			give_player_item(id, karambit_hardened_id)

			new names[32];
			get_user_name(id, names, 31)
			client_printcolor(0, "/y%s/g製作了 /ctr表面硬化爪刀/y!", names)
		}
		else
		{
			client_printcolor(id, "/ctr製作/g所需的素材數量不足/y!")
		}
	}

	return PLUGIN_CONTINUE;
}

public main_make_karambit_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r爪刀 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w食腦蟲鱗片 \yx200 ^n");
	add(szMenu, sizeof(szMenu), "\w綠色的皮 \yx40 ^n");
	add(szMenu, sizeof(szMenu), "\w高階催化劑 \yx5 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y50,000 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_karambit_menu");
}

public make_karambit_menu(id, num) 
{
	if( num == N8 )
	{
		if (!is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS)
		{
			client_printcolor(id, "無法製作,道具數量達到最大限制!")
			return PLUGIN_HANDLED;
		}

		if( g_iPlayerDrops[id][g_CurrentChar[id]][HEADCRAB_SCALES] >= 200 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][GREEN_LEATHER] >= 40 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] >= 5 &&
			g_Coins[id][g_CurrentChar[id]] >= 50000 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][HEADCRAB_SCALES] -= 200;
			g_iPlayerDrops[id][g_CurrentChar[id]][GREEN_LEATHER] -= 40;
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] -= 5;
			g_Coins[id][g_CurrentChar[id]] -= 50000;

			const item_id = 11;
			give_player_item(id, item_id)
			client_printcolor(id, "/y成功/g製作了 /ctr爪刀/y!")
		}
		else
		{
			client_printcolor(id, "/ctr製作/g所需的素材數量不足/y!")
		}
	}

	return PLUGIN_CONTINUE;
}

public main_make_refknife_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r參考刀 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y100,000 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_refknife_menu");
}

public make_refknife_menu(id, num) 
{
	if( num == N8 )
	{
		if (!is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS)
		{
			client_printcolor(id, "無法製作,道具數量達到最大限制!")
			return PLUGIN_HANDLED;
		}

		if( g_Coins[id][g_CurrentChar[id]] >= 100000 ) 
		{
			Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - 100000)

			const item_id = 10;
			give_player_item(id, item_id)

			new names[32];
			get_user_name(id, names, 31)
			client_printcolor(0, "/y%s /g製作了/ctr參考刀/y!", names)
		}
		else
		{
			client_printcolor(id, "/ctr製作/g所需的素材數量不足/y!")
		}
	}
	return PLUGIN_CONTINUE;
}

public main_bpshop_menu(id)
{
	new menu = menu_create("\rBOSSPOINT商店 - \w選擇分類" , "bpshop_menu");
	menu_additem(menu, "購買物品", "1", 0);
	menu_additem(menu, "購買裝備", "2", 0);
	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}

public bpshop_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_miyu(id) )
		return PLUGIN_HANDLED;

	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);
	switch(key) 
	{
		case 1: main_bpshop_drops_menu(id);
		// case 2: 
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_bpshop_drops_menu(id)
{
	new szTitle[60];
	formatex(szTitle, 59, "\rBP 物品商店 \y- \w目前BP \d[ \y%d \d]", g_Bosspoints[id][g_CurrentChar[id]])
	new menu = menu_create(szTitle , "bpshop_drops_menu");

	new szTempid[16], szItems[60];
	for (new i = 0; i < MAX_BPSHOP_DROPS; i++)
	{
		num_to_str(Bpshop_Drops_Id[i], szTempid, 31);
		formatex(szItems, 59, "%s \d( \y%d \d)", Drops_Name[Bpshop_Drops_Id[i]], Bpshop_Drops_Cost[i])
		menu_additem(menu, szItems, szTempid, 0);
	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}

public bpshop_drops_menu(id , menu , item) 
{
	if ( !native_get_p_near_miyu(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);
	if( g_Bosspoints[id][g_CurrentChar[id]] >= Bpshop_Drops_Cost[item] )	
	{
		g_iPlayerDrops[id][g_CurrentChar[id]][item_id]++;
		Set_Player_Bosspoint(id, g_Bosspoints[id][g_CurrentChar[id]] - Bpshop_Drops_Cost[item]);
		client_printcolor(id, "/g已購買 /ctr%s /y!", Drops_Name[item_id]);
	}
	else
	{
		client_printcolor(id, "/ctr%s /y需要 /ctr%d /gBossPoint", Drops_Name[item_id], Bpshop_Drops_Cost[item]);
	}

	menu_destroy(menu);
	main_bpshop_menu(id);

	return PLUGIN_HANDLED;
}

public main_buychar_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "角色數量 \d[ \y%d \d] - \r購買角色", g_PlayerChars[id])
	new menu = menu_create(szInfo , "buychar_menu");
	menu_additem(menu , "購買 錦木千束", "1" , 0);
	menu_additem(menu , "購買 草上飛", "2" , 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public buychar_menu(id , menu , item) 
{
	if ( !native_get_p_near_miyu(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);
	switch(key)
	{
		case 1: main_buy_chisato_menu(id);
		case 2: main_buy_grasswonder_menu(id);
	}
	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_buy_grasswonder_menu(id)
{
	new szMenu[256], szItem[64];
	formatex(szItem, 63, "\w確定購買 \r%s \w?^n", HEROES[NECROMANCER]);

	add(szMenu, sizeof(szMenu), szItem);
	add(szMenu, sizeof(szMenu), "\w需要 \r2000 \wBossPoint^n^n");
	add(szMenu, sizeof(szMenu), "\y7. \w確定^n");
	add(szMenu, sizeof(szMenu), "\y8. \w不要");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "buy_grasswonder_menu");
}

public buy_grasswonder_menu(id, num) 
{
	if( num == N7 )
	{
		if( g_Bosspoints[id][g_CurrentChar[id]] < 2000 ) {
			client_printcolor(id, "/g購買條件不足/y!");
			return;
		}
		
		new Val;
		g_PlayerChars[id]++;
		for (new hero_id = 0; hero_id < g_PlayerChars[id]; hero_id++)
		{
			if ( !g_PlayerCharActive[id][hero_id] )
				Val = hero_id;
		}

		g_PlayerCharActive[id][Val] = CHAR_ACTIVE;
		g_PlayerHero[id][Val] = NECROMANCER;
		g_Vitality[id][Val] = 10;
		g_Energy[id][Val] = 10;
		
		g_Bosspoints[id][g_CurrentChar[id]] -= 2000;
		client_printcolor( id, "/y角色 /g%s /y以創造, 角色數量 : /ctr%d", HEROES[NECROMANCER], g_PlayerChars[id]);
	}	
}

public main_buy_chisato_menu(id)
{
	new szMenu[256], szItem[64];
	formatex(szItem, 63, "\w確定購買 \r%s \w?^n", HEROES[ASSASSIN]);

	add(szMenu, sizeof(szMenu), szItem);
	add(szMenu, sizeof(szMenu), "\w需要 \r100,000 \w金錢^n^n");
	add(szMenu, sizeof(szMenu), "\y7. \w確定^n");
	add(szMenu, sizeof(szMenu), "\y8. \w不要");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "buy_chisato_menu");
}

public buy_chisato_menu(id, num) 
{
	switch(num) {
		case N7:
		{
			if( g_Coins[id][g_CurrentChar[id]] < 100000 ) {
				client_printcolor(id, "/g購買條件不足/y!");
				return;
			}
		/*
			for(new i = 0; i <= MAX_CHARS; ++i) {
				if( g_PlayerHero[id][i] == ASSASSIN && g_PlayerCharActive[id][i] ) {
					client_printcolor(id, "/y你已經有這個角色了! 無法再購買")
					return;
				}
			}
		*/
			new Val;
			g_PlayerChars[id]++;
			for (new hero_id = 0; hero_id < g_PlayerChars[id]; hero_id++)
			{
				if ( !g_PlayerCharActive[id][hero_id] )
					Val = hero_id;
			}

			g_PlayerCharActive[id][Val] = CHAR_ACTIVE;
			g_PlayerHero[id][Val] = ASSASSIN;
			g_Vitality[id][Val] = 10;
			g_Energy[id][Val] = 10;
			
			Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - 100000);
			client_printcolor( id, "/y角色 /g%s /y以創造, 角色數量 : /ctr%d", HEROES[ASSASSIN], g_PlayerChars[id]);
		}
	}
}

public main_reset_apsp_menu(id)
{
	new szMenu[256];
	new szTitle[64];

	format(szTitle, sizeof(szTitle), "\w確定重置你的\r技能點\w與\r能力點 \w?^n");
	add(szMenu, sizeof(szMenu), szTitle);
	add(szMenu, sizeof(szMenu), "\w需要 \r10,000 \w金錢^n^n");
	add(szMenu, sizeof(szMenu), "\y7. \w確定^n");
	add(szMenu, sizeof(szMenu), "\y8. \w不要");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "ResetApspMenu");
}

public reset_apsp_menu(id, num) 
{
	switch(num) {
		case N7:
		{
			if( g_Coins[id][g_CurrentChar[id]] < 10000 ) {
				client_printcolor(id, "/y金錢不足! 你的錢:/g%d", g_Coins[id][g_CurrentChar[id]])
				return;
			}

			// 重置能力點
			g_Strength[id][g_CurrentChar[id]] = 0;
			g_Dexterity[id][g_CurrentChar[id]] = 0;
			g_Vitality[id][g_CurrentChar[id]] = 10;
			g_Energy[id][g_CurrentChar[id]] = 10;
			
			// 加回已穿裝備點數
			for (new item_id = 0; item_id <= g_charcounter; item_id++)
			{
				if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] )
				{
					Set_Player_Energy_Item(id, item_id);
					Set_Player_Vitality_Item(id, item_id);
				}
			}

			g_PlayerStPoints[id][g_CurrentChar[id]] = g_PlayerLevel[id][g_CurrentChar[id]] * get_pcvar_num(d2_stat_points_levelup);

			// 重置技能點
			for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
			{
				if ( g_PlayerHero[id][g_CurrentChar[id]] == g_skillhero[skill_id] )
				{
					g_iSkills[id][g_CurrentChar[id]][skill_id] = 0;
				}
			}
			// 加回已穿裝備技能
			for (new item_id = 0; item_id <= g_charcounter; item_id++)
			{
				if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] )
				{
					Set_Player_Skills_Item(id, item_id);
				}
			}

			g_PlayerSkPoints[id][g_CurrentChar[id]] = g_PlayerLevel[id][g_CurrentChar[id]];

			Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - 10000);
			client_printcolor(id, "/y已重置技能點與能力點");
		}
	}
}

public main_worn_menu(id)
{
	if ( !g_iLogged[id] )
		return PLUGIN_HANDLED;

	new szInfo[60];
	formatex(szInfo, 59, "裝備選單" )

	new menu = menu_create(szInfo , "worn_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[80];
			formatex(szItems, 79, "%s \d( %s \d)", item_name[item_id], (g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] ? "\r已穿上" : "\y未穿上") )

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
	
	return PLUGIN_HANDLED;
}
public worn_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_alive(id) || Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "你沒有裝備或是你已經死了!")
		return PLUGIN_HANDLED;
	}

	if ( g_PlayerLevel[id][g_CurrentChar[id]] < item_data[item_id][CHAR_LEVEL] )
	{
		client_printcolor(id, "等級不足!")
		return PLUGIN_HANDLED;
	}

	if ( g_Strength[id][g_CurrentChar[id]] < item_data[item_id][CHAR_STR] )
	{
		client_printcolor(id, "力量不足!")
		return PLUGIN_HANDLED;
	}

	if ( g_Dexterity[id][g_CurrentChar[id]] < item_data[item_id][CHAR_DEX] )
	{
		client_printcolor(id, "敏捷不足!")
		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
	{
		if ( g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] )
		{
			client_printcolor(id, "/y道具 /g%s /y已脫下.", item_name[item_id])

			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;
			
			if ( item_data[item_id][CHAR_TYPE] == TYPE_WEAPON )
			{
				entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
				entity_set_string(id, EV_SZ_weaponmodel, "");
			}

			ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
		}
		else
		{
			if ( Player_WItemTypeValue(id, TYPE_WEAPON, item_id) > 0 )
			{
				client_printcolor(id, "你想裝備的武器太多了!")
				return PLUGIN_HANDLED
			}
			
			if ( Player_WItemTypeValue(id, TYPE_BOLTS, item_id) > 0 )
			{
				client_printcolor(id, "你想裝備的箭失太多了!")
				return PLUGIN_HANDLED
			}
			
			if ( Player_WItemTypeValue(id, TYPE_ARMOR, item_id) > 0 || Player_WItemTypeValue(id, TYPE_GLOVES, item_id) > 0
			|| Player_WItemTypeValue(id, TYPE_BOOTS, item_id) > 0 || Player_WItemTypeValue(id, TYPE_BELT, item_id) > 0 || Player_WItemTypeValue(id, TYPE_HELM, item_id) > 0 
			|| Player_WItemTypeValue(id, TYPE_SHIELD, item_id) > 0 || Player_WItemTypeValue(id, TYPE_RING, item_id) > 0 || Player_WItemTypeValue(id, TYPE_AMULET, item_id) > 0 )
			{
				client_printcolor(id, "你想裝備的道具太多了!")
				return PLUGIN_HANDLED
			}
			
			if ( item_data[item_id][CHAR_TYPE] == TYPE_WEAPON )
			{
				if ( equal(item_vmodel[item_id], "models/", 7) )
				{
					entity_set_string(id, EV_SZ_viewmodel, item_vmodel[item_id]);
				}
				else
				{
					entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
				}
	
				if ( equal(item_pmodel[item_id], "models/", 7) )
				{
					entity_set_string(id, EV_SZ_weaponmodel, item_pmodel[item_id]);
				}
				else
				{
					entity_set_string(id, EV_SZ_weaponmodel, "");
				}
			}

			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_WORN;

			Set_Player_Armor_Item(id, item_id);

			Set_Player_Energy_Item(id, item_id);
			Set_Player_Vitality_Item(id, item_id);
			Set_Player_Skills_Item(id, item_id);

			client_printcolor(id, "/y道具 /g%s /y已穿上!", item_name[item_id])
		}
	}
	
	main_worn_menu(id);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 倉庫選單.
public main_inventory_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "倉庫的錢 \d[ \y%d / %d \d]", g_CoinsInv[id][g_CurrentChar[id]], MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]])

	new menu = menu_create(szInfo , "inventory_menu");

	menu_additem(menu , "存道具", "1" , 0); 
	menu_additem(menu , "領道具", "2" , 0); 
	menu_additem(menu , "存藥水", "3" , 0); 
	menu_additem(menu , "領藥水", "4" , 0); 
	menu_additem(menu , "存所有錢", "5" , 0); 
	menu_additem(menu , "領所有錢", "6" , 0);

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public inventory_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_inventory(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	switch(key)
	{
		case 1:
		{
			main_inventory_deposit_menu(id);
		}
		case 2:
		{
			main_inventory_withdraw_menu(id);
		}
		case 3:
		{
			main_inventory_depositp_menu(id);
		}
		case 4:
		{
			main_inventory_withdrawp_menu(id);
		}
		case 5:
		{
			new inv_space = MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]] - g_CoinsInv[id][g_CurrentChar[id]]

			if ( g_Coins[id][g_CurrentChar[id]] > inv_space )
			{
				client_printcolor(id, "無法全部存入,倉庫的錢太多了!" );
				g_Coins[id][g_CurrentChar[id]] -= inv_space;
				g_CoinsInv[id][g_CurrentChar[id]] =  MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]];
				return PLUGIN_HANDLED;
			}

			if ( g_Coins[id][g_CurrentChar[id]] <= inv_space )
			{
				g_CoinsInv[id][g_CurrentChar[id]] += g_Coins[id][g_CurrentChar[id]];
				g_Coins[id][g_CurrentChar[id]] -= g_Coins[id][g_CurrentChar[id]];
			}
			
			main_inventory_menu(id);
		}
		case 6:
		{
			new p_space = MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] - g_Coins[id][g_CurrentChar[id]]

			if ( g_CoinsInv[id][g_CurrentChar[id]] > p_space )
			{
				client_printcolor(id, "無法全部領出,身上的錢太多了!" );
				g_CoinsInv[id][g_CurrentChar[id]] -= p_space;
				g_Coins[id][g_CurrentChar[id]] = MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]];
				return PLUGIN_HANDLED;
			}

			if ( g_CoinsInv[id][g_CurrentChar[id]] <= p_space )
			{
				g_Coins[id][g_CurrentChar[id]] += g_CoinsInv[id][g_CurrentChar[id]];
				g_CoinsInv[id][g_CurrentChar[id]] -= g_CoinsInv[id][g_CurrentChar[id]];
			}
			
			main_inventory_menu(id);
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_inventory_deposit_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "想存哪個道具?")

	new menu = menu_create(szInfo , "inventory_deposit_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			num_to_str(item_id, szTempid, 31);

			new szItems[60];
			formatex(szItems, 59, "%s - \r%d", item_name[item_id], g_iPlayerItem[id][g_CurrentChar[id]][item_id])

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public inventory_deposit_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_inventory(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_ItemsInv(id) >= MAX_INVENTORY_ITEMS )
	{
		client_printcolor(id, "倉庫的道具達到最大上限!" );		

		return PLUGIN_HANDLED;
	}

	if ( Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "你沒有道具!" );		

		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/y道具 /g%s /y已存入!", item_name[item_id] );

		g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;
		g_iPlayerItemInv[id][g_CurrentChar[id]][item_id]++;

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] < 1 )
		{
			if ( item_data[item_id][CHAR_TYPE] == TYPE_WEAPON
			&& g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] )
			{
				entity_set_string(id, EV_SZ_viewmodel, g_brassknuckles);
				entity_set_string(id, EV_SZ_weaponmodel, "");
			}

			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;

			ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
		}
	} 
	
	main_inventory_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_inventory_withdraw_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "想領哪個道具?")

	new menu = menu_create(szInfo , "inventory_withdraw_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItemInv[id][g_CurrentChar[id]][item_id] > 0 )
		{
			num_to_str(item_id, szTempid, 31);

			new szItems[60];
			formatex(szItems, 59, "%s - \r%d", item_name[item_id], g_iPlayerItemInv[id][g_CurrentChar[id]][item_id])

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public inventory_withdraw_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_inventory(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_ItemsInv(id) <= 0 )
	{
		client_printcolor(id, "無法領出,你倉庫沒有道具!" )
		return PLUGIN_HANDLED;
	}

	if ( Get_Player_Items(id) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(id, "無法領出,你身上的道具太多了!" )
		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerItemInv[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/y道具 /g%s /y已領出!", item_name[item_id] );

		g_iPlayerItem[id][g_CurrentChar[id]][item_id]++;
		g_iPlayerItemInv[id][g_CurrentChar[id]][item_id]--;

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] <= 1 )
		{
			g_iPlayerItemWorn[id][g_CurrentChar[id]][item_id] = ITEM_NOT_WORN;
		}
	} 
	
	main_inventory_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_inventory_depositp_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "想存哪個藥水?")

	new menu = menu_create(szInfo , "inventory_depositp_menu");

	new szTempid[32];

	for (new item_id = 0; item_id < MAX_POTIONS; item_id++)
	{
		if ( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
		{
			num_to_str(item_id, szTempid, 31);

			new szItems[60];
			formatex(szItems, 59, "%s - \r%d", Potion_Names[item_id], g_iPlayerPotions[id][g_CurrentChar[id]][item_id])

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public inventory_depositp_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_inventory(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_PotionsInv(id) >= MAX_INVENTORY_POTIONS )
	{
		client_printcolor(id, "無法存入,倉庫的藥水已達最大限制!" );		

		return PLUGIN_HANDLED;
	}

	if ( Get_Player_Potions(id) <= 0 )
	{
		client_printcolor(id, "你沒有藥水!" );		

		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/g%s /y已存入!", Potion_Names[item_id] );

		g_iPlayerPotions[id][g_CurrentChar[id]][item_id]--;
		g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id]++;
	} 
	
	main_inventory_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_inventory_withdrawp_menu(id)
{
	new szInfo[60];
	formatex(szInfo, 59, "想領哪個藥水?")

	new menu = menu_create(szInfo , "inventory_withdrawp_menu");

	new szTempid[32];

	for (new item_id = 0; item_id < MAX_POTIONS; item_id++)
	{
		if ( g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id] > 0 )
		{
			num_to_str(item_id, szTempid, 31);

			new szItems[60];
			formatex(szItems, 59, "%s - \r%d", Potion_Names[item_id], g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id])

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public inventory_withdrawp_menu(id , menu , item) 
{ 
	if ( !native_get_p_near_inventory(id) )
		return PLUGIN_HANDLED;

	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_connected(id) || Get_Player_PotionsInv(id) <= 0 )
	{
		client_printcolor(id, "你沒有藥水!" )
		return PLUGIN_HANDLED;
	}

	if ( Get_Player_Potions(id) >= MAX_PLAYER_POTIONS )
	{
		client_printcolor(id, "你身上的藥水太多了!" )
		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id] > 0 )
	{
		client_printcolor(id, "/y/g%s /y已領出!", Potion_Names[item_id] );

		g_iPlayerPotions[id][g_CurrentChar[id]][item_id]++;
		g_iPlayerPotionsInv[id][g_CurrentChar[id]][item_id]--;
	} 
	
	main_inventory_menu(id)

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 給予道具選單.
public main_give_menu(id)
{
	if ( !g_iLogged[id] )
		return PLUGIN_HANDLED;

	new szMsg[60]
	formatex(szMsg, 59, "\y你想給誰道具?")

	new menu = menu_create(szMsg , "give_menu");

	new szName[32], szTempid[32];

	for (new target = 1; target <= g_iMaxPlayers; target++)
	{
		if ( is_user_connected(target) && id != target && g_iFakeplayer != target )
		{
			get_user_name(target, szName, 31);
			num_to_str(target, szTempid, 31);

			new szPlayers[60];
			formatex(szPlayers, 59, "%s", szName )

			menu_additem(menu, szPlayers, szTempid, 0);
		}
	}
	
	menu_display(id , menu , 0); 
	
	return PLUGIN_HANDLED;
}
public give_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new target = str_to_num(data);

	if ( !is_user_connected(target) )
		return PLUGIN_HANDLED;

	g_TradeInviter[target] = id;
	g_TradeTarget[id] = target;
	g_ItemsDeposited[target] = false;
	g_ItemsDeposited[id] = false;
	client_printcolor(id, "給予請求已發送.");
	main_give_invite_menu(target);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 發送請求.
public main_give_invite_menu(target)
{
	new szMsg[60]
	static name[32] ; get_user_name(g_TradeInviter[target], name, charsmax(name));

	formatex(szMsg, 59, "\r%s \y想給你道具", name)

	new menu = menu_create(szMsg , "give_invite_menu");

	menu_additem(menu, "接受!", "1", 0);
	menu_additem(menu, "拒絕!", "2", 0);
	
	menu_display(target , menu , 0); 
}
public give_invite_menu(target , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	if ( !is_user_connected(g_TradeInviter[target]) )
		return PLUGIN_HANDLED;

	static name[32] ; get_user_name(target, name, charsmax(name));

	switch(key)
	{
		case 1:
		{
			client_printcolor(g_TradeInviter[target], "/g%s /y接受.", name);
			main_give_accept_menu(g_TradeInviter[target])

			client_printcolor(target, "接受.");

		}
		case 2:
		{
			client_printcolor(g_TradeInviter[target], "/g%s /y拒絕.", name);
			client_printcolor(target, "拒絕!");
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_give_accept_menu(id)
{
	new szMsg[60]
	static name[32] ; get_user_name(g_TradeTarget[id], name, charsmax(name));

	formatex(szMsg, 59, "\y你想給\r%s什麼道具?", name)

	new menu = menu_create(szMsg , "give_accept_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s", item_name[item_id] )

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public give_accept_menu(id , menu , item) 
{ 
	if ( !is_user_connected(g_TradeTarget[id]) )
	{
		client_printcolor(id, "對方沒有連線!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		g_ItemsDeposited[id] = false;
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "你沒有任何道具!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(g_TradeTarget[id]) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(id, "對方的道具太多了!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}

	static name[32] ; get_user_name(id, name, charsmax(name));
	
	client_printcolor(g_TradeTarget[id], "/ctr%s 給你 %s", name, item_name[item_id]);

	g_iPlayerItem[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][item_id]++;
	g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;
	
	g_iPlayerItemRepair[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][item_id] = g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id];
		
	if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] <= 0 )
	{
		ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
	}

	client_printcolor(id, "/y道具 /g%s /y成功給他了.", item_name[item_id]);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
// 交易系統.
public main_trade_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new szMsg[60]
	formatex(szMsg, 59, "\y你想跟誰交易?")

	new menu = menu_create(szMsg , "trade_menu");

	new szName[32], szTempid[32];

	for (new target = 1; target <= g_iMaxPlayers; target++)
	{
		if ( is_user_connected(target) && id != target && g_iFakeplayer != target )
		{
			get_user_name(target, szName, 31);
			num_to_str(target, szTempid, 31);

			new szPlayers[60];
			formatex(szPlayers, 59, "%s", szName )

			menu_additem(menu, szPlayers, szTempid, 0);
		}
	}
	
	menu_display(id , menu , 0); 
}
public trade_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new target = str_to_num(data);

	if ( !is_user_connected(target) )
		return PLUGIN_HANDLED;

	g_TradeInviter[target] = id;
	g_TradeTarget[id] = target;
	g_ItemsDeposited[target] = false;
	g_ItemsDeposited[id] = false;
	client_printcolor(id, "交易請求已發送.");
	main_trade_invite_menu(target);

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

// 交易請求.
public main_trade_invite_menu(target)
{
	new szMsg[60]
	static name[32] ; get_user_name(g_TradeInviter[target], name, charsmax(name));

	formatex(szMsg, 59, "\r%s \y想跟你交易", name)

	new menu = menu_create(szMsg , "trade_invite_menu");

	menu_additem(menu, "接受!", "1", 0);
	menu_additem(menu, "拒絕!", "2", 0);
	
	menu_display(target , menu , 0); 
}
public trade_invite_menu(target , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new key = str_to_num(data);

	if ( !is_user_connected(g_TradeInviter[target]) )
		return PLUGIN_HANDLED;

	static name[32] ; get_user_name(target, name, charsmax(name));

	switch(key)
	{
		case 1:
		{
			client_printcolor(g_TradeInviter[target], "/g%s /y接受.", name);
			main_trade_accept_menu(g_TradeInviter[target])
			main_trade_accept_menuT(target)

			client_printcolor(target, "接受.");

		}
		case 2:
		{
			client_printcolor(g_TradeInviter[target], "/g%s /y拒絕.", name);
			client_printcolor(target, "拒絕!");
		}
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_trade_accept_menu(id)
{
	new szMsg[60]
	static name[32] ; get_user_name(g_TradeTarget[id], name, charsmax(name));

	formatex(szMsg, 59, "\y你想給\r%s什麼?", name)

	new menu = menu_create(szMsg , "trade_accept_menu");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s", item_name[item_id] )

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}
public trade_accept_menu(id , menu , item) 
{ 
	if ( !is_user_connected(g_TradeTarget[id]) )
	{
		client_printcolor(id, "對方沒有連線!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		g_ItemsDeposited[id] = false;
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( Get_Player_Items(id) <= 0 )
	{
		client_printcolor(id, "你沒有任何道具!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(g_TradeTarget[id]) <= 0 )
	{
		client_printcolor(id, "對方沒有任何道具!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(id) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(id, "你的道具太多了!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(g_TradeTarget[id]) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(id, "對方的道具太多了!");
		g_ItemsDeposited[id] = false;
		return PLUGIN_HANDLED;
	}

	static name[32] ; get_user_name(id, name, charsmax(name));
	
	g_iPlayerTradeItem[id] = item_id;
	client_printcolor(g_TradeTarget[id], "/ctr%s 給你 %s", name, item_name[item_id]);

	g_ItemsDeposited[id] = true;

	if ( g_ItemsDeposited[id] && g_ItemsDeposited[g_TradeTarget[id]] )
	{
		g_iPlayerItem[id][g_CurrentChar[id]][g_iPlayerTradeItem[g_TradeTarget[id]]]++;
		g_iPlayerItem[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][item_id]++;
		g_iPlayerItem[id][g_CurrentChar[id]][item_id]--;
		g_iPlayerItem[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][g_iPlayerTradeItem[g_TradeTarget[id]]]--;
		
		g_iPlayerItemRepair[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][item_id] = g_iPlayerItemRepair[id][g_CurrentChar[id]][item_id];
		g_iPlayerItemRepair[id][g_CurrentChar[id]][g_iPlayerTradeItem[g_TradeTarget[id]]] = g_iPlayerItemRepair[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][g_iPlayerTradeItem[g_TradeTarget[id]]];

		if ( g_iPlayerItem[id][g_CurrentChar[id]][item_id] <= 0 )
		{
			ExecuteForward( g_iItemTook, g_iReturn, id, item_id);
		}
		if ( g_iPlayerItem[g_TradeTarget[id]][g_CurrentChar[g_TradeTarget[id]]][g_iPlayerTradeItem[g_TradeTarget[id]]] <= 0 )
		{
			ExecuteForward( g_iItemTook, g_iReturn, g_TradeTarget[id], g_iPlayerTradeItem[g_TradeTarget[id]]);
		}

		client_printcolor(id, "交易成功!");
		client_printcolor(g_TradeTarget[id], "交易成功!");
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_trade_accept_menuT(target)
{
	new szMsg[60]
	static name[32] ; get_user_name(g_TradeInviter[target], name, charsmax(name));

	formatex(szMsg, 59, "\y你想給 \r%s什麼?", name)

	new menu = menu_create(szMsg , "trade_accept_menuT");

	new szTempid[32];

	for (new item_id = 0; item_id <= g_charcounter; item_id++)
	{
		if ( g_iPlayerItem[target][g_CurrentChar[target]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s", item_name[item_id] )

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(target , menu , 0); 
}
public trade_accept_menuT(target , menu , item) 
{ 
	if ( !is_user_connected(g_TradeInviter[target]) )
	{
		client_printcolor(target, "對方沒有連線!");
		g_ItemsDeposited[target] = false;
		return PLUGIN_HANDLED;
	}

	if(item == MENU_EXIT) 
	{ 
		g_ItemsDeposited[target] = false;
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( Get_Player_Items(target) <= 0 )
	{
		client_printcolor(target, "你沒有任合道具!");
		g_ItemsDeposited[target] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(g_TradeInviter[target]) <= 0 )
	{
		client_printcolor(target, "對方沒有任何道具!");
		g_ItemsDeposited[target] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(target) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(target, "你的道具太多了!");
		g_ItemsDeposited[target] = false;
		return PLUGIN_HANDLED;
	}
	else if ( Get_Player_Items(g_TradeInviter[target]) >= MAX_PLAYER_ITEMS )
	{
		client_printcolor(target, "對方的道具太多了!");
		g_ItemsDeposited[target] = false;
		return PLUGIN_HANDLED;
	}

	static name[32] ; get_user_name(target, name, charsmax(name));
	
	g_iPlayerTradeItem[target] = item_id;
	client_printcolor(g_TradeInviter[target], "/ctr%s 給你 %s", name, item_name[item_id]);

	g_ItemsDeposited[target] = true;

	if ( g_ItemsDeposited[target] && g_ItemsDeposited[g_TradeInviter[target]] )
	{
		g_iPlayerItem[target][g_CurrentChar[target]][g_iPlayerTradeItem[g_TradeInviter[target]]]++;
		g_iPlayerItem[g_TradeInviter[target]][g_CurrentChar[g_TradeInviter[target]]][item_id]++;
		g_iPlayerItem[target][g_CurrentChar[target]][item_id]--;
		g_iPlayerItem[g_TradeInviter[target]][g_CurrentChar[g_TradeInviter[target]]][g_iPlayerTradeItem[g_TradeInviter[target]]]--;
		
		g_iPlayerItemRepair[g_TradeInviter[target]][g_CurrentChar[g_TradeInviter[target]]][item_id] = g_iPlayerItemRepair[target][g_CurrentChar[target]][item_id];
		g_iPlayerItemRepair[target][g_CurrentChar[target]][g_iPlayerTradeItem[g_TradeInviter[target]]] = g_iPlayerItemRepair[g_TradeInviter[target]][g_CurrentChar[g_TradeInviter[target]]][g_iPlayerTradeItem[g_TradeInviter[target]]];

		if ( g_iPlayerItem[target][g_CurrentChar[target]][item_id] <= 0 )
		{
			ExecuteForward( g_iItemTook, g_iReturn, target, item_id);
		}
		if ( g_iPlayerItem[g_TradeInviter[target]][g_CurrentChar[g_TradeInviter[target]]][g_iPlayerTradeItem[g_TradeInviter[target]]] <= 0 )
		{
			ExecuteForward( g_iItemTook, g_iReturn, g_TradeInviter[target], g_iPlayerTradeItem[g_TradeInviter[target]]);
		}

		client_printcolor(target, "交易成功!");
		client_printcolor(g_TradeInviter[target], "交易成功!");
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}

public main_drops_menu(id)
{
	if ( !g_iLogged[id] )
		return;

	new szTempid[16], szItems[60];

	new menu = menu_create("其他物品", "drops_menu");
	for (new drop_id = 0; drop_id < MAX_DROPS; drop_id++)
	{
		new drops_count = g_iPlayerDrops[id][g_CurrentChar[id]][drop_id];
		if( drops_count > 0 )
		{
			formatex(szItems, 59, "%s \yx%d", Drops_Name[drop_id], drops_count);
			num_to_str(drop_id, szTempid, 15);
			menu_additem(menu, szItems, szTempid, 0);
		}
	}

	if( menu_items(menu) < 1 ) {
		client_printcolor(id, "/ctr你的背包完全是空的!");
		return;
	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
}

public drops_menu(id , menu , item)
{
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	}

	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);
	new key = str_to_num(data);

	client_print(id, print_chat, "%s - %d", Drops_Name[key], g_iPlayerDrops[id][g_CurrentChar[id]][key])

	main_drops_menu(id);
	menu_destroy(menu); 

	return PLUGIN_HANDLED;
}

// 藥水選單.
public main_potion_menu(id)
{
	if ( !g_iLogged[id] )
		return PLUGIN_HANDLED;

	new szInfo[60];
	formatex(szInfo, 59, "選擇你要使用的藥水" )

	new menu = menu_create(szInfo , "potion_menu");

	new szTempid[32];

	for (new item_id = 0; item_id < MAX_POTIONS; item_id++)
	{
		if ( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
		{
			new szItems[60];
			formatex(szItems, 59, "%s - \r%d", Potion_Names[item_id], g_iPlayerPotions[id][g_CurrentChar[id]][item_id] )

			num_to_str(item_id, szTempid, 31);

			menu_additem(menu, szItems, szTempid, 0);
		}

	}

	menu_setprop(menu , MPROP_EXIT , MEXIT_ALL);
	menu_display(id , menu , 0); 
	
	return PLUGIN_HANDLED;
}
public potion_menu(id , menu , item) 
{ 
	if(item == MENU_EXIT) 
	{ 
		menu_destroy(menu); 
		return PLUGIN_HANDLED;
	} 
	new data[6], iName[64];
	new access, callback;
	menu_item_getinfo(menu, item, access, data,5, iName, 63, callback);

	new item_id = str_to_num(data);

	if ( !is_user_alive(id) || Get_Player_Potions(id) <= 0 || !g_iLogged[id] )
	{
		client_printcolor(id, "你沒有藥水或是你已經死亡或是你沒有登入!")
		return PLUGIN_HANDLED;
	}

	if ( g_iPlayerPotions[id][g_CurrentChar[id]][item_id] > 0 )
	{
		g_iPlayerPotions[id][g_CurrentChar[id]][item_id]--;
		client_printcolor(id, "感覺好多了!");
		main_potion_menu(id);
		Set_Player_Potion_HP(id, item_id);
	}

	menu_destroy(menu); 
	return PLUGIN_HANDLED;
}
public force_player_not_joined(id)
{
	if ( !is_user_connected(id) || g_Player_FirstTime_Connected[id] )
		return PLUGIN_HANDLED;
		
	if ( is_user_alive(id) )
	{
		user_silentkill(id);
	}
	
	cs_set_user_team( id, CS_TEAM_SPECTATOR);

	main_hero_menu_connect(id);

	remove_task(id + TASKID_HUDINFO);

	g_Player_Info[id] = false;
	g_iLogged[id] = false;
	ExecuteForward( g_iLog, g_iReturn, id, UNLOGGED);
		
	return PLUGIN_HANDLED;
}