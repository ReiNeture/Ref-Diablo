// Diablo II LOD 玩家指令.

public clcmd_changetm(id)
{
	if ( g_iLogged[id] )
	{
		main_mod_menu(id);
	}
	else
	{
		main_hero_menu_connect(id);
	}

	return PLUGIN_HANDLED;
}
public handle_say(id)
{
	if ( !g_iLogged[id] )
		return PLUGIN_HANDLED;

	static Text[32]
	read_args( Text, charsmax(Text) );

	remove_quotes(Text)

	static szArg1[32], szArg2[32];
	parse(Text, szArg1, charsmax(szArg1), szArg2, charsmax(szArg2))

	new Value = str_to_num(szArg2)

	if( equali( szArg1, "/deposit") )
	{
		if ( !native_get_p_near_inventory(id) )
		{
			client_print(id, print_chat, "你需要靠近倉庫!")
			return PLUGIN_HANDLED;
		}

		if(szArg2[0] == '-') 
		{
			client_print(id, print_chat, "無法使用'-'這個值!")
			return PLUGIN_HANDLED;
		}

		if ( Value > g_Coins[id][g_CurrentChar[id]] )
		{
			client_print(id, print_chat, "你沒有那麼多錢!")
			return PLUGIN_HANDLED;
		}

		if ( Value > MAX_PLAYER_MONEY_INV[g_PlayerLevel[id][g_CurrentChar[id]]] )
		{
			client_print(id, print_chat, "你沒辦法存那麼多錢!")
			return PLUGIN_HANDLED;
		}

		client_printcolor(id, "/g%d /y已存入!", Value)
		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] - Value);
		Set_Player_CoinsInv(id, g_CoinsInv[id][g_CurrentChar[id]] + Value);

		return PLUGIN_HANDLED;
	}
	else if ( equali( szArg1, "/withdraw") )
	{
		if ( !native_get_p_near_inventory(id) )
		{
			client_print(id, print_chat, "你需要靠近倉庫!")
			return PLUGIN_HANDLED;
		}

		if(szArg2[0] == '-') 
		{
			client_print(id, print_chat, "無法使用'-'這個值!")
			return PLUGIN_HANDLED;
		}

		if ( Value > g_CoinsInv[id][g_CurrentChar[id]] )
		{
			client_print(id, print_chat, "你的倉庫沒有那麼多錢!")
			return PLUGIN_HANDLED;
		}

		if ( Value > MAX_PLAYER_MONEY[g_PlayerLevel[id][g_CurrentChar[id]]] )
		{
			client_print(id, print_chat, "你沒辦法領那麼多錢!")	
			return PLUGIN_HANDLED;
		}

		client_printcolor(id, "/g%d /y已領出!", Value)
		Set_Player_Coins(id, g_Coins[id][g_CurrentChar[id]] + Value);
		Set_Player_CoinsInv(id, g_CoinsInv[id][g_CurrentChar[id]] - Value);

		return PLUGIN_HANDLED;
	}

	return PLUGIN_CONTINUE;
}
public cmd_set_level(id, level, cid) 
{
	if( !cmd_access(id, level, cid, 3) )
	{
		return PLUGIN_HANDLED;
	}
	
	new Arg1[64], Target
	read_argv(1, Arg1, 63)
	Target = cmd_target(id, Arg1, 0)
	
	new iLevel[32], Value
	read_argv(2, iLevel, 31)
	Value = str_to_num(iLevel)
	
	if(iLevel[0] == '-') 
	{
		console_print(id, "無法使用'-'這個值!")
		return PLUGIN_HANDLED;
	}
	
	if(!Target) 
	{
		console_print(id, "沒有找到該玩家!")
		return PLUGIN_HANDLED;
	}

	if ( !g_iLogged[Target] )
	{
		console_print(id, "玩家未登入!")
		return PLUGIN_HANDLED;
	}
	
	if(Value > MAX_LEVELS-1)
	{
		console_print(id, "數值無法比 %d 還高!", MAX_LEVELS-1)
		return PLUGIN_HANDLED;
	}
	
	if(Value < 1)
	{
		console_print(id, "數值無法小於1!")
		return PLUGIN_HANDLED;
	}
	
	new AdminName[32]
	get_user_name(id, AdminName, 31)
		
	new TargetName[32]
	get_user_name(Target, TargetName, 31)
	
	g_PlayerLevel[Target][g_CurrentChar[Target]] = Value
	g_PlayerXp[Target][g_CurrentChar[Target]] = LEVELS[g_PlayerLevel[Target][g_CurrentChar[Target]]]
	
	g_PlayerStPoints[Target][g_CurrentChar[Target]] = get_pcvar_num(d2_stat_points_levelup) * g_PlayerLevel[Target][g_CurrentChar[Target]];

	g_PlayerSkPoints[Target][g_CurrentChar[Target]] = g_PlayerLevel[Target][g_CurrentChar[Target]];
	
	client_printcolor(Target, "/g管理員: /ctr%s /y把你的等級調到 /g%d", AdminName, Value)

	return PLUGIN_HANDLED;
}
public cmd_set_gold(id, level, cid) 
{
	if( !cmd_access(id, level, cid, 3) )
	{
		return PLUGIN_HANDLED;
	}
	
	new Arg1[64], Target
	read_argv(1, Arg1, 63)
	Target = cmd_target(id, Arg1, 0)
	
	new iLevel[32], Value
	read_argv(2, iLevel, 31)
	Value = str_to_num(iLevel)
	
	if(iLevel[0] == '-') 
	{
		console_print(id, "無法使用'-'這個值!")
		return PLUGIN_HANDLED;
	}
	
	if(!Target) 
	{
		console_print(id, "沒有找到該玩家!")
		return PLUGIN_HANDLED;
	}

	if ( !g_iLogged[Target] )
	{
		console_print(id, "玩家未登入!")
		return PLUGIN_HANDLED;
	}

	Set_Player_Coins(Target, Value);
	
	new AdminName[32]
	get_user_name(id, AdminName, 31)
		
	new TargetName[32]
	get_user_name(Target, TargetName, 31)
	
	client_printcolor(Target, "/g管理員: /ctr%s /y把你的錢調到 /g%d", AdminName, Value)

	return PLUGIN_HANDLED;
}
public skill_handle(id)
{
	if ( !is_user_alive(id) || g_FreezeTime || !g_iLogged[id] )
		return PLUGIN_HANDLED;

	for (new skill_id = 0; skill_id <= g_skillcounter; skill_id++)
	{
		if ( g_PlayerHero[id][g_CurrentChar[id]] == g_skillhero[skill_id] )
			ExecuteForward( g_SkillFired, g_iReturn, id);
	}

	return PLUGIN_HANDLED;
}