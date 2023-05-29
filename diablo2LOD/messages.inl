// Diablo II LOD 訊息.
public Message_ClCorpse()
{
	new id = get_msg_arg_int(12)
	if( g_szModel[id][0] )
	{
		set_msg_arg_string(1, g_szModel[id])
	}
}
public Message_StatusIcon(iMsgId, MSG_DEST, id) 
{ 
	static szIcon[5] 
	get_msg_arg_string(2, szIcon, 4) 
	if( szIcon[0] == 'b' && szIcon[2] == 'y' && szIcon[3] == 'z' ) 
	{ 
		if( get_msg_arg_int(1)) 
		{ 
			fm_cs_set_user_nobuy(id) 
			return PLUGIN_HANDLED;
		} 
	}  
	
	return PLUGIN_CONTINUE;
}
public Message_VGUIMenu(msgid, dest, id) 
{
	if ( get_msg_arg_int(1) != TEAM_SELECT_VGUI_MENU_ID )
		return PLUGIN_CONTINUE

	static param_menu[1];
	param_menu[0] = msgid;

	set_task( 0.1, "Task_VGUI", id, param_menu, sizeof param_menu)

	return PLUGIN_HANDLED
}
public Task_VGUI(param_menu[], id)
{
	if ( !is_user_connected(id) )
		return;
	
	static Msg_Block;
	Msg_Block = get_msg_block( param_menu[0] );
	set_msg_block( param_menu[0], BLOCK_SET);
	engclient_cmd( id, "jointeam", "6");
	set_msg_block( param_menu[0], Msg_Block);
}

stock client_printcolor(const id, const input[], any:...)
{
	new count = 1, players[32];

	static msg[191];
	vformat(msg,190,input,3);

	replace_all(msg,190,"/g","^4");// 綠色文字.
	replace_all(msg,190,"/y","^1");// 橘色文字.
	replace_all(msg,190,"/ctr","^3");// 隊伍顏色文字.

	if (id) players[0] = id; 
	else get_players(players,count,"ch");

	for (new i=0;i<count;i++)
	{
		if (is_user_connected(players[i]))
		{
			message_begin(MSG_ONE_UNRELIABLE, MsgSayText, _, players[i]);
			write_byte(players[i]);
			write_string(msg);
			message_end();
		}
	}
}
stock Create_SpriteTrail(start[3], end[3], iSprite, count, life, scale, velocity, random ){

	message_begin( MSG_BROADCAST,SVC_TEMPENTITY)
	write_byte( TE_SPRITETRAIL )
	write_coord( start[0] )				// start position (X)
	write_coord( start[1] )				// start position (Y)
	write_coord( start[2] )				// start position (Z)
	write_coord( end[0] )				// end position (X)
	write_coord( end[1] )				// end position (Y)
	write_coord( end[2] )				// end position (Z)
	write_short( iSprite )				// sprite index
	write_byte( count )					// count
	write_byte( life)					// life in 0.1's
	write_byte( scale)					// scale in 0.1's
	write_byte( velocity )				// velocity along vector in 10's
	write_byte( random )				// randomness of velocity in 10's
	message_end()
}
