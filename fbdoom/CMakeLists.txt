cmake_minimum_required(VERSION 3.26)
project(rmdoom LANGUAGES CXX C)

add_executable(rmdoom)

target_sources(rmdoom PRIVATE
# am_map.c   doomstat.c  i_cdmus.c      
# # i_pcsound.c
#       i_video_fbdev.cpp  m_misc.c    p_maputl.c  p_telept.c  r_sky.c     tables.c                  z_zone.c
# d_event.c  dstrings.c  icon.c         i_scale.c        m_argv.c         m_random.c  p_mobj.c    p_tick.c    r_things.c  v_video.c
# d_items.c  dummy.c     i_endoom.c     
# # i_sdlmusic.c
#      m_bbox.c         p_ceilng.c  p_plats.c   p_user.c    sha1.c      w_checksum.c
# d_iwad.c   f_finale.c

# # i_input_sdl.c  i_sdlsound.c


# m_cheat.c        p_doors.c   p_pspr.c    r_bsp.c     sounds.c    w_file.c
# d_loop.c   f_wipe.c    i_input_tty.c  i_sound.c        m_config.c       p_enemy.c   p_saveg.c   r_data.c    s_sound.c   w_file_stdc.c
# d_main.c   g_game.c    i_joystick.c   i_sound_dummy.c  m_controls.c     p_floor.c   p_setup.c   r_draw.c    statdump.c  w_file_stdc_unbuffered.c
# d_mode.c   gusconf.c   i_main.c       i_system.c       memio.c          p_inter.c   p_sight.c   r_main.c    st_lib.c    wi_stuff.c
# d_net.c    hu_lib.c    info.c         i_timer.c        m_fixed.c        p_lights.c  p_spec.c    r_plane.c   st_stuff.c  w_main.c
# doomdef.c  hu_stuff.c  i_oplmusic.c   i_video.c        m_menu.c         p_map.c     p_switch.c  r_segs.c    stubs.c     w_wad.c


i_video_fbdev.c

# i_video.c 
i_input_tty.c


i_main.c dummy.c am_map.c doomdef.c doomstat.c dstrings.c d_event.c d_items.c d_iwad.c d_loop.c d_main.c d_mode.c d_net.c
 f_finale.c f_wipe.c g_game.c hu_lib.c hu_stuff.c info.c i_cdmus.c i_endoom.c i_joystick.c i_scale.c i_sound.c i_system.c
  i_timer.c memio.c m_argv.c m_bbox.c m_cheat.c m_config.c m_controls.c m_fixed.c m_menu.c m_misc.c m_random.c p_ceilng.c
   p_doors.c p_enemy.c p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c p_pspr.c p_saveg.c p_setup.c p_sight.c
    p_spec.c p_switch.c p_telept.c p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c r_segs.c r_sky.c r_things.c sha1.c
     sounds.c statdump.c st_lib.c st_stuff.c s_sound.c
 tables.c v_video.c wi_stuff.c w_checksum.c w_file.c w_file_stdc_unbuffered.c w_main.c w_wad.c z_zone.c
)

find_package(Qt6 COMPONENTS Core Gui)

target_link_libraries(rmdoom PRIVATE
    epframebuffer
)

target_compile_definitions(rmdoom
    PUBLIC
        NOSDL
        NORMALUNIX LINUX SNDSERV
)
