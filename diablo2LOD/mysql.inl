// 玩家讀取/存檔的資料庫.
public MYSQLX_Init()
{
	if ( !get_pcvar_num(d2_save_type) )
		return;

	// 確認資料庫訊息.
	new szHost[64], szUser[32], szPass[32], szDB[128], szError[256], iErrNum;

	get_pcvar_string( mysqlx_host, szHost, charsmax( szHost ) );
	get_pcvar_string( mysqlx_user, szUser, charsmax( szUser ) );
	get_pcvar_string( mysqlx_pass, szPass, charsmax( szPass ) );
	get_pcvar_string( mysqlx_db, szDB, charsmax( szDB ) );

	// 成立位元組.
	g_DBTuple = SQL_MakeDbTuple( szHost, szUser, szPass, szDB );

	// 嘗試連接.
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, charsmax( szError ) );

	if ( !g_DBConn )
	{
		log_amx( "[MYSQLX] 數據庫連接失敗: [%d] %s", iErrNum, szError );

		return;
	}

	SQL_ThreadQuery( g_DBTuple, "QueryCreateTable", gTableCreateQuery)
}
public QueryCreateTable( iFailState, Handle:hQuery, szError[ ], iError, iData[ ], iDataSize, Float:fQueueTime ) 
{ 
	if( iFailState == TQUERY_CONNECT_FAILED 
	|| iFailState == TQUERY_QUERY_FAILED ) 
	{ 
		log_amx( "[MYSQLX] %s", szError ); 
		
		return;
	} 
}

public MYSQLX_Save_T( id ) 
{ 
	// 確保連線.
	if ( !g_DBConn ) 
	{ 
		return; 
	} 

    // 儲存玩家經驗. 
	new iLen[3] = 0; 
	static szData[10000], szQuery[12000], szData2[10000], szData3[10000]; 
	
	iLen[0] += formatex( szData[ iLen[0] ], charsmax(szData) - iLen[0], "%d %d#", g_PlayerChars[id], g_CurrentMana[id] );
	
	for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
	{
		iLen[0] += formatex ( szData[ iLen[0] ], charsmax(szData) - iLen[0], "%d %d %d %d %d %d %d %d %d %d %d %d#", g_PlayerLevel[id][hero_id], g_PlayerXp[id][hero_id], g_PlayerSkPoints[id][hero_id], g_PlayerStPoints[id][hero_id], g_Strength[id][hero_id],
		g_Dexterity[id][hero_id], g_Vitality[id][hero_id], g_Energy[id][hero_id], 
		g_Coins[id][hero_id], g_CoinsInv[id][hero_id], g_PlayerHero[id][hero_id], g_PlayerCharActive[id][hero_id] )
	}	
	for( new iSkill = 0; iSkill <= g_skillcounter; iSkill++ ) 
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen[0] += formatex( szData[ iLen[0] ], charsmax(szData) - iLen[0], "%d#", g_iSkills[id][hero_id][iSkill] );
		}	
	}

	iLen[1] += formatex( szData2[ iLen[1] ], charsmax(szData2) - iLen[1], "#" );
	
	for( new iPotions = 0; iPotions < MAX_POTIONS ; iPotions++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen[1] += formatex( szData2[ iLen[1] ], charsmax(szData2) - iLen[1], "%d %d#",  g_iPlayerPotions[id][hero_id][iPotions], g_iPlayerPotionsInv[id][hero_id][iPotions] )
		}	
	}

	iLen[2] += formatex( szData3[ iLen[2] ], charsmax(szData3) - iLen[2], "#" );
	
	for( new iItems = 0; iItems <= g_charcounter ; iItems++ )
	{
		for(new hero_id = 0; hero_id < MAX_CHARS + 1 ; hero_id++)
		{
			iLen[2] += formatex( szData3[ iLen[2] ], charsmax(szData3) - iLen[2], "%d %d %d %d %d#",  g_iPlayerItem[id][hero_id][iItems], g_iPlayerItemInv[id][hero_id][iItems], g_iPlayerItemBolts[id][hero_id][iItems], 
			g_iPlayerItemWorn[id][hero_id][iItems], g_iPlayerItemRepair[id][hero_id][iItems] )
		}
	}

	new szKey[66], szKeyName[32];
	Save_GetKey( id, szKey, charsmax( szKey ) );
	Save_GetKeyName( szKeyName, charsmax( szKeyName ) );
	

	formatex( szQuery, charsmax(szQuery), "REPLACE INTO d2_data ( player_data, player_data2, player_data3, %s ) VALUES ( '%s', '%s', '%s', '%s' );", szKeyName, szData, szData2, szData3, szKey ); 
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery);
	
	return; 
} 
public _MYSQLX_Save_T( failstate, Handle:query, error[], errnum, data[], size ) 
{ 
	if( failstate == TQUERY_CONNECT_FAILED  
	|| failstate == TQUERY_QUERY_FAILED )  
	{  
		log_amx( "[MYSQLX] %s 222", error );  
         
		return; 
	}  
} 
public MYSQLX_GetAllData( id ) 
{ 
	// 確保連線.
	if ( !g_DBConn ) 
	{ 
		return; 
	} 
	
	new szKey[66], szKeyName[32];
	Save_GetKey( id, szKey, charsmax( szKey ) );
	Save_GetKeyName( szKeyName, charsmax( szKeyName ) );
		 
	new szQuery[ 512 ], iData[ 1 ];  
	formatex( szQuery, charsmax( szQuery ), "SELECT * FROM d2_data WHERE %s = '%s';", szKeyName, szKey );  
		  
	iData[ 0 ] = id; 
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_GetAllData", szQuery, iData, 1 ); 

	return; 
} 
public _MYSQLX_GetAllData( iFailState, Handle:hQuery, szError[ ], iError, iData[ ], iDataSize, Float:fQueueTime )  
{  
	if( iFailState == TQUERY_CONNECT_FAILED  
	|| iFailState == TQUERY_QUERY_FAILED )  
	{  
		log_amx( "[MYSQLX] %s 111", szError ); 
         
		return; 
	}  
	else  
	{  
		new id = iData[ 0 ]; 
         
		static szData[20000], szAllItems[20000], szData2[8000], szAllItems2[8000], szData3[8000], szAllItems3[8000]
		new szClassLevel[256], szSkills[256], szItems[256], szPotions[256];
		new szHero[126], szLevel[4], szXp[10], szSk[4], szSt[5], szCurMana[6], szStr[5], szDex[5], szVit[5], szEne[5], szCoin[10], szCoinInv[11],
		szPItem[3], szPItemInv[3], szPItemW[3], szPItemRep[5], szItemB[5], szPotion[3], szPotionInv[4], szPlayerChars[4],
		szActive[2];

		new ColData = SQL_FieldNameToNum(hQuery, "player_data")  
		new ColData2 = SQL_FieldNameToNum(hQuery, "player_data2")  
		new ColData3 = SQL_FieldNameToNum(hQuery, "player_data3")  

		while (SQL_MoreResults(hQuery))  
		{ 
			SQL_ReadResult(hQuery, ColData, szData, charsmax( szData ) ); 
			SQL_ReadResult(hQuery, ColData2, szData2, charsmax( szData2 ) ); 
			SQL_ReadResult(hQuery, ColData3, szData3, charsmax( szData3 ) ); 

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
         
			SQL_NextRow(hQuery) 
		} 
	}  
} 
// 將數值以設定的方式給玩家.
public Save_GetKey( id, szKey[], len )
{
	switch( get_pcvar_num( d2_save_by ) )
	{
		case 2:
		{
			get_user_name( id, szKey, len );

			replace_all( szKey, len, "'", "\'" );
		}

		case 1:	get_user_ip( id, szKey, len, 1 );
		case 0:	get_user_authid( id, szKey, len );
	}
}

// 將方式返回給數值.
public Save_GetKeyName( szKeyName[], len )
{
	switch( get_pcvar_num( d2_save_by ) )
	{
		case 2:	copy( szKeyName, len, "player_name" );
		case 1:	copy( szKeyName, len, "player_ip" );
		case 0:	copy( szKeyName, len, "player_steamid" );
	}
}