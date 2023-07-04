public main_make_surveyor_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r調查兵團武器 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w殭屍王証明 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w深海礦核 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y130,000 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_surveyor_menu");
}

public make_surveyor_menu(id, num) 
{
	if( num == N8 )
	{
		if (!is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS)
		{
			client_printcolor(id, "無法製作,道具數量達到最大限制!")
			return PLUGIN_HANDLED;
		}

		const surveyor_id = 17;

		if( g_iPlayerDrops[id][g_CurrentChar[id]][GONOME_PROVE] >= 1 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][PELAGITE] >= 1 &&
			g_Coins[id][g_CurrentChar[id]] >= 130000 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][GONOME_PROVE] -= 1;
			g_iPlayerDrops[id][g_CurrentChar[id]][PELAGITE] -= 1;
			g_Coins[id][g_CurrentChar[id]] -= 130000;

			give_player_item(id, surveyor_id);

			new names[32];
			get_user_name(id, names, 31);
			client_printcolor(0, "/y%s/g製作了 /ctr調查兵團武器/y!", names);
			log_message("%s 製作了調查兵團武器, item_id=%d", names, surveyor_id);
		}
		else
		{
			client_printcolor(id, "/ctr製作/g所需的素材數量不足/y!");
		}
	}

	return PLUGIN_CONTINUE;
}

public main_make_karambithardened_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r表面硬化爪刀 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w爪刀 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\wA 血液 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w鱷魚牙齒 \yx3 ^n");
	add(szMenu, sizeof(szMenu), "\w高階催化劑 \yx10 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y100,000 ^n^n");

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

		if( g_iPlayerDrops[id][g_CurrentChar[id]][A_BOOLD] >= 1 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] >= 10 &&
			g_iPlayerDrops[id][g_CurrentChar[id]][BULLSQUID_TEETH] >= 3 &&
			g_iPlayerItem[id][g_CurrentChar[id]][karambit_id] >= 1 && 
			g_Coins[id][g_CurrentChar[id]] >= 100000 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][A_BOOLD] -= 1;
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] -= 10;
			g_iPlayerDrops[id][g_CurrentChar[id]][BULLSQUID_TEETH] -= 3;
			g_iPlayerItem[id][g_CurrentChar[id]][karambit_id]--;
			g_Coins[id][g_CurrentChar[id]] -= 100000;

			if ( g_iPlayerItem[id][g_CurrentChar[id]][karambit_id] < 1 && g_iPlayerItemWorn[id][g_CurrentChar[id]][karambit_id] )
			{
				g_iPlayerItemWorn[id][g_CurrentChar[id]][karambit_id] = ITEM_NOT_WORN;
				ExecuteForward( g_iItemTook, g_iReturn, id, karambit_id);
			}

			give_player_item(id, karambit_hardened_id)

			new names[32];
			get_user_name(id, names, 31)
			client_printcolor(0, "/y%s/g製作了 /ctr表面硬化爪刀/y!", names);
			log_message("%s 製作了表面硬化爪刀, item_id=%d", names, karambit_hardened_id);
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
	add(szMenu, sizeof(szMenu), "\w食腦蟲鱗片 \yx100 ^n");
	add(szMenu, sizeof(szMenu), "\w百目狗的皮 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w高階催化劑 \yx5 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y30,000 ^n^n");

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

		if( g_iPlayerDrops[id][g_CurrentChar[id]][HEADCRAB_SCALES] >= 100 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][GREEN_LEATHER] >= 1 && 
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] >= 5 &&
			g_Coins[id][g_CurrentChar[id]] >= 30000 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][HEADCRAB_SCALES] -= 100;
			g_iPlayerDrops[id][g_CurrentChar[id]][GREEN_LEATHER] -= 1;
			g_iPlayerDrops[id][g_CurrentChar[id]][CATALYST] -= 5;
			g_Coins[id][g_CurrentChar[id]] -= 30000;

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

public main_make_deepspace_menu(id)
{
	new szMenu[256];
	add(szMenu, sizeof(szMenu), "\y- 製作^n");
	add(szMenu, sizeof(szMenu), "\r深海之心 \yx1^n^n");

	add(szMenu, sizeof(szMenu), "\y- 需要素材^n");
	add(szMenu, sizeof(szMenu), "\w小灰通行證 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w深海礦核 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y180,000 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_deepspace_menu");
}

public make_deepspace_menu(id, num) 
{
	if( num == N8 )
	{
		if (!is_user_connected(id) || Get_Player_Items(id) >= MAX_PLAYER_ITEMS)
		{
			client_printcolor(id, "無法製作,道具數量達到最大限制!")
			return PLUGIN_HANDLED;
		}

		if( g_iPlayerDrops[id][g_CurrentChar[id]][BABYGARG_PASS] >= 1 && 
				g_iPlayerDrops[id][g_CurrentChar[id]][PELAGITE] >= 1 && 
				g_Coins[id][g_CurrentChar[id]] >= 180000 ) 
		{
			g_iPlayerDrops[id][g_CurrentChar[id]][BABYGARG_PASS]--;
			g_iPlayerDrops[id][g_CurrentChar[id]][PELAGITE]--;
			Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - 180000);

			const deepspace_id = 15;
			give_player_item(id, deepspace_id);

			new names[32];
			get_user_name(id, names, 31);
			client_printcolor(0, "/y%s /g製作了/ctr深海之心/y!", names);
			log_message("%s 製作了深海之心, item_id=%d", names, deepspace_id);
		}
		else
		{
			client_printcolor(id, "/ctr製作/g所需的素材數量不足/y!");
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
	add(szMenu, sizeof(szMenu), "\w鱷魚牙齒 \yx10 ^n");
	add(szMenu, sizeof(szMenu), "\w殭屍王証明 \yx5 ^n");
	add(szMenu, sizeof(szMenu), "\w小灰通行證 \yx3 ^n");
	add(szMenu, sizeof(szMenu), "\w大媽蛋蛋 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w大藍心臟 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w鷹爪嘴巴 \yx1 ^n");
	add(szMenu, sizeof(szMenu), "\w金錢 \y400,000 ^n^n");

	add(szMenu, sizeof(szMenu), "\y8. \w製作^n");
	add(szMenu, sizeof(szMenu), "\y0. \w取消");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "make_refknife_menu");
}
public make_refknife_menu(id, num) 
{
	if( num == N8 )
	{
		client_print(id, print_chat, "1");
	}
}

/* *************************************** 購買腳色 *************************************** */
public main_buy_grasswonder_menu(id)
{
	new szMenu[256], szItem[64];
	formatex(szItem, 63, "\w確定購買 \r%s \w?^n", HEROES[NECROMANCER]);

	add(szMenu, sizeof(szMenu), szItem);
	add(szMenu, sizeof(szMenu), "\w需要 \r500 \wBossPoint^n^n");
	add(szMenu, sizeof(szMenu), "\y7. \w確定^n");
	add(szMenu, sizeof(szMenu), "\y8. \w不要");
	show_menu(id, gBuyCharacterMenu, szMenu, -1, "buy_grasswonder_menu");
}

public buy_grasswonder_menu(id, num) 
{
	if( num == N7 )
	{
		if( g_Bosspoints[id][g_CurrentChar[id]] < 500 ) {
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
		
		g_Bosspoints[id][g_CurrentChar[id]] -= 500;

		new names[32];
		get_user_name(id, names, 31);
		client_printcolor(id, "/y角色 /g%s /y以創造, 角色數量 : /ctr%d", HEROES[NECROMANCER], g_PlayerChars[id]);
		client_printcolor(0, "/g%s /y以購買角色: /ctr%s", names, HEROES[NECROMANCER]);
		log_message("%s 以購買角色: %s", names, HEROES[NECROMANCER]);
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

			new names[32];
			get_user_name(id, names, 31);
			client_printcolor(id, "/y角色 /g%s /y以創造, 角色數量 : /ctr%d", HEROES[ASSASSIN], g_PlayerChars[id]);
			client_printcolor(0, "/g%s /y以購買角色: /ctr%s", names, HEROES[ASSASSIN]);
			log_message("%s以購買角色: %s", names, HEROES[ASSASSIN]);

		}
	}
}