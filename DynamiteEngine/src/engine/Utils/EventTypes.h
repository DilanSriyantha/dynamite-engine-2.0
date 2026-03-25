#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H	

#include "FirebaseAuthManager.h"

struct GamePauseEvent {
	bool state;
};

struct GameQuitRequestEvent {};

struct GameQuitEvent {};

enum MenuPanel {
	PNL_MENU_OPTIONS,
	PNL_ABOUT,
	PNL_LEADERBOARD,
	PNL_PAUSE,
	PNL_CHALLENGE,
	PNL_GAME_OVER,
	PNL_REGISTER,
	PNL_LOGIN
};

struct MenuPanelRequestEvent {
	MenuPanel pnl;
	bool open;
};

struct AboutPanelCloseEvent {};

struct MouseLeftClickEvent {
public:
	SDL_Point mouse;
};

struct MouseRightClickEvent {
	SDL_Point mouse;
};

struct PlayerTakeDamageEvent {
	float damage_percentage;
};

struct PlayerUseLifeLineEvent {};

struct PlayerHealthResetEvent {};

struct PlayerMoveRightEvent {};

struct PlayerMoveLeftEvent {};

struct PlayerStoppedEvent {};

struct EnemyTakeDamageEvent {
	float damage_percentage;
};

struct ChallengeLoadedEvent {};

struct UserLoggedInEvent {
	AuthResult auth_result;
};

struct LoadingEvent {
	bool is_loading;
};

#endif