#pragma once

#ifndef _VIDEOGAME_DEFS_H_
#define _VIDEOGAME_DEFS_H_

#define INIT_GUI			false
#define INIT_RAILS			false

#define WM_GAME_PROCESS		( WM_USER + 1 )
#define WM_GUI_PROCESS		( WM_USER + 2 )
#define WM_EXIT_GAME		( WM_USER + 3 )

#define OPTIONS		COptions::GetSingletonPtr()

#endif