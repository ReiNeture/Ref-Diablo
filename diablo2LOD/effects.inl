// Diablo II LOD 效果.
new Float:miyu_h_time = 0.0;
public do_miyu_h(id)
{
    if( get_gametime() < miyu_h_time )
    {
        client_printcolor(id, "/g有人剛剛才跟他瑟瑟過，讓她休息一下/y! /ctr[還需要%f秒]", miyu_h_time - get_gametime() );
        return PLUGIN_CONTINUE;
    }

    new Float:Porigin[3];
    entity_get_vector( id, EV_VEC_origin, Porigin);

    new ent = -1
    while( (ent = find_ent_in_sphere( ent, Porigin, DISTANCE_INVENTORY_PLAYER)) != 0) 
    {
        static classname[32];
        pev(ent, pev_classname, classname, charsmax(classname));
        if( equal(classname, "Miyu") )
        {
            miyu_h_time = get_gametime() + 10.0;
            emit_sound(ent, CHAN_WEAPON, h_sound, VOL_NORM, ATTN_NONE, 0, PITCH_NORM);

            if(task_exists(id + 1481))
		        remove_task(id + 1481);
            set_task(0.2, "semen_spray", id + 1481, _, _, "a", 25);

            // get_user_name(id, names, 31);
            // client_printcolor(0, "/ctr%s /g居然在跟萬用商人瑟瑟= =", names);

            client_cmd(id, "say 跟萬用商人瑟瑟好舒服啊!!!")

            break;
        }
    }

    return PLUGIN_HANDLED;
}

public semen_spray(id)
{
    id = id - 1481;
    const Float:speed = 2.0;
    new Float:Miyu_Origin[3], Float:Id_Origin[3], Float:velocityvec[3];
    fm_get_aim_origin(id, Miyu_Origin)
    pev(id, pev_origin, Id_Origin);

    velocityvec[0] = Miyu_Origin[0] - Id_Origin[0]
    velocityvec[1] = Miyu_Origin[1] - Id_Origin[1]
    velocityvec[2] = Miyu_Origin[2] - Id_Origin[2]
    new Float:num = floatsqroot(speed*speed / (velocityvec[0]*velocityvec[0] + velocityvec[1]*velocityvec[1] + velocityvec[2]*velocityvec[2]))
    velocityvec[0] *= num
    velocityvec[1] *= num
    velocityvec[2] *= num

    message_begin(MSG_BROADCAST, SVC_TEMPENTITY);
    write_byte(TE_BLOODSTREAM);
    engfunc(EngFunc_WriteCoord, Id_Origin[0]);
    engfunc(EngFunc_WriteCoord, Id_Origin[1]);
    engfunc(EngFunc_WriteCoord, Id_Origin[2]);
    engfunc(EngFunc_WriteCoord, velocityvec[0]);
    engfunc(EngFunc_WriteCoord, velocityvec[1]);
    engfunc(EngFunc_WriteCoord, velocityvec[2]);
    write_byte(8); // color
    write_byte(140); // speed
    message_end();
}