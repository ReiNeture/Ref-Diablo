// 讀取/儲存玩家資料.

Save_Data(id)
{
	new szKey[66];
	Save_GetKey( id, szKey, charsmax( szKey ) );

	new iLen = 0;
	static szData[8000];
	
	iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "%d %d#", g_PlayerChars[id], g_CurrentMana[id] );

	for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
	{
		iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "%d %d %d %d %d %d %d %d %d %d %d %d#", g_PlayerLevel[id][hero_id], g_PlayerXp[id][hero_id], g_PlayerSkPoints[id][hero_id], g_PlayerStPoints[id][hero_id], g_Strength[id][hero_id], g_Dexterity[id][hero_id], g_Vitality[id][hero_id], g_Energy[id][hero_id], 
		g_Coins[id][hero_id], g_CoinsInv[id][hero_id], g_PlayerHero[id][hero_id], g_PlayerCharActive[id][hero_id] )
	}	
	for( new iSkill = 0; iSkill <= g_skillcounter; iSkill++ ) 
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "%d#", g_iSkills[id][hero_id][iSkill] );
		}	
	}
	
	nvault_set( g_Nvault, szKey, szData );

	iLen = 0;
	szData = "";

	iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "#" );

	for( new iPotions = 0; iPotions < MAX_POTIONS ; iPotions++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "%d %d#",  g_iPlayerPotions[id][hero_id][iPotions], g_iPlayerPotionsInv[id][hero_id][iPotions] )
		}	
	}

	nvault_set( g_Nvault2, szKey, szData );
	
	iLen = 0;
	szData = "";
	
	iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "#" );
	
	for( new iItems = 0; iItems <= g_charcounter ; iItems++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen += formatex( szData[ iLen ], charsmax(szData) - iLen, "%d %d %d %d %d#",  g_iPlayerItem[id][hero_id][iItems], g_iPlayerItemInv[id][hero_id][iItems], g_iPlayerItemBolts[id][hero_id][iItems], 
			g_iPlayerItemWorn[id][hero_id][iItems], g_iPlayerItemRepair[id][hero_id][iItems] )
		}
	}
	
	nvault_set( g_Nvault3, szKey, szData );
}

Load_Data(id)
{
	new szKey[66];
	Save_GetKey( id, szKey, charsmax( szKey ) );

	static szData[8000], szAllItems[8000], szData2[8000], szAllItems2[8000], szData3[8000], szAllItems3[8000];
	new szClassLevel[512], szSkills[256], szItems[400], szPotions[400];
	new szHero[126], szLevel[4], szXp[10], szSk[4], szSt[5], szCurMana[6], szStr[5], szDex[5], szVit[5], szEne[5], szCoin[10], szCoinInv[11],
	szPItem[3], szPItemInv[3], szPItemW[3], szPItemRep[5], szItemB[5], szPotion[4], szPotionInv[4], szPlayerChars[4],
	szActive[2];
	nvault_get(g_Nvault, szKey, szData, charsmax(szData) );
	nvault_get(g_Nvault2, szKey, szData2, charsmax(szData2) );
	nvault_get(g_Nvault3, szKey, szData3, charsmax(szData3) );
	
	strtok(szData, szData, sizeof( szData ) - 1, szAllItems, sizeof( szAllItems ) - 1, '#')
	strtok(szData2, szData2, sizeof( szData2 ) - 1, szAllItems2, sizeof( szAllItems2 ) - 1, '#')
	strtok(szData3, szData3, sizeof( szData3 ) - 1, szAllItems3, sizeof( szAllItems3 ) - 1, '#')
	
	parse(szData, szPlayerChars, charsmax(szPlayerChars), szCurMana, charsmax(szCurMana) );

	g_PlayerChars[id] = str_to_num( szPlayerChars );
	g_CurrentMana[id] = str_to_num( szCurMana );
	
	for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
	{
		strtok(szAllItems, szClassLevel, sizeof( szClassLevel ) - 1, szAllItems, sizeof( szAllItems ) - 1, '#')
		strtok(szClassLevel, szLevel, sizeof( szLevel ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szXp, sizeof( szXp ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szSk, sizeof( szSk ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szSt, sizeof( szSt ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szStr, sizeof( szStr ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szDex, sizeof( szDex ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szVit, sizeof( szVit ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szEne, sizeof( szEne ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szCoin, sizeof( szCoin ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szCoinInv, sizeof( szCoinInv ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, ' ')
		strtok(szClassLevel, szHero, sizeof( szHero ) - 1, szActive, sizeof( szActive ) - 1, ' ')
		
		g_PlayerLevel[id][hero_id] = str_to_num( szLevel )
		g_PlayerXp[id][hero_id] = str_to_num( szXp )
		g_PlayerSkPoints[id][hero_id] = str_to_num( szSk )
		g_PlayerStPoints[id][hero_id] = str_to_num( szSt )
		g_Strength[id][hero_id] = str_to_num( szStr )
		g_Dexterity[id][hero_id] = str_to_num( szDex )
		g_Vitality[id][hero_id] = str_to_num( szVit )
		g_Energy[id][hero_id] = str_to_num( szEne )
		g_Coins[id][hero_id] = str_to_num( szCoin )
		g_CoinsInv[id][hero_id] = str_to_num( szCoinInv )
		g_PlayerHero[id][hero_id] = str_to_num( szHero );
		g_PlayerCharActive[id][hero_id] = str_to_num( szActive );
	}	
	
	for( new iSkill = 0; iSkill <= g_skillcounter; iSkill++ ) 
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			strtok(szAllItems, szClassLevel, sizeof( szClassLevel ) - 1, szAllItems, sizeof( szAllItems ) - 1, '#')
			strtok(szClassLevel, szSkills, sizeof( szSkills ) - 1, szClassLevel, sizeof( szClassLevel ) - 1, '#')

			g_iSkills[id][hero_id][iSkill] = str_to_num( szSkills );
		}	
	}
	for( new iItems = 0; iItems <= g_charcounter ; iItems++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			strtok(szAllItems3, szItems, sizeof( szItems ) - 1, szAllItems3, sizeof( szAllItems3 ) - 1, '#')
	
			strtok(szItems, szPItem, sizeof( szPItem ) - 1, szItems, sizeof( szItems ) - 1, ' ')
			strtok(szItems, szPItemInv, sizeof( szPItemInv ) - 1, szItems, sizeof( szItems ) - 1, ' ')
			strtok(szItems, szItemB, sizeof( szItemB ) - 1, szItems, sizeof( szItems ) - 1, ' ')
			strtok(szItems, szPItemW, sizeof( szPItemW ) - 1, szPItemRep, sizeof( szPItemRep ) - 1, ' ')
				
			g_iPlayerItem[id][hero_id][iItems] = str_to_num( szPItem );
			g_iPlayerItemInv[id][hero_id][iItems] = str_to_num( szPItemInv );
			g_iPlayerItemBolts[id][hero_id][iItems] = str_to_num( szItemB );
			g_iPlayerItemWorn[id][hero_id][iItems] = str_to_num( szPItemW );
			g_iPlayerItemRepair[id][hero_id][iItems] = str_to_num( szPItemRep );
		}	
	}
	for( new iPotions = 0; iPotions < MAX_POTIONS ; iPotions++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			strtok(szAllItems2, szPotions, sizeof( szPotions ) - 1, szAllItems2, sizeof( szAllItems2 ) - 1, '#')
					
			strtok( szPotions, szPotion, sizeof( szPotion ) - 1, szPotionInv, sizeof( szPotionInv ) - 1, ' ')

			g_iPlayerPotions[id][hero_id][iPotions] = str_to_num( szPotions )
			g_iPlayerPotionsInv[id][hero_id][iPotions] = str_to_num( szPotionInv )
		}	
	}
} 