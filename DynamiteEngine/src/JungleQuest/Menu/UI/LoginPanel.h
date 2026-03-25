#ifndef LOGIN_PANEL_H
#define LOGIN_PANEL_H

#define USER_CACHE_FILE "tmp_user.bin"

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "FirebaseAuthManager.h"

class LoginPanel : public IScript {
public:
	LoginPanel(AssetManager& asset_manager, EntityManager& entity_manager) : asset_manager(asset_manager), entity_manager(entity_manager)
	{}

	void init() override
	{
		auto& container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& lbl_email = entity_manager.addEntity().addComponent<Label>();
		auto& lbl_password = entity_manager.addEntity().addComponent<Label>();
		auto& txt_email = entity_manager.addEntity().addComponent<TextInputBox>(asset_manager.getFont("calibri-regular-24"));
		auto& txt_password = entity_manager.addEntity().addComponent<TextInputBox>(asset_manager.getFont("calibri-regular-24"));
		auto& btn_login = entity_manager.addEntity().addComponent<Button>();
		auto& btn_register = entity_manager.addEntity().addComponent<Button>();
		auto& lbl_error = entity_manager.addEntity().addComponent<Label>();

		container.setSize(948, 881);
		container.setScale(.5f);
		container.setTexture(asset_manager.getTexture("high-score-panel-bg"));
		container.center({ 0, 0, 1920, 1080 });

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("header-login"));
		header.dockOnTopCenter(container);
		header.paddingTop(-100);

		lbl_email.setFont(asset_manager.getFont("def-font-24"));
		lbl_email.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		lbl_email.setText("E-mail");
		lbl_email.dockOnTopLeft(container);
		lbl_email.dockBelow(header);
		lbl_email.paddingLeft(30);

		txt_email.setSize(399, 46);
		txt_email.dockOnTopLeft(container);
		txt_email.dockBelow(lbl_email);
		txt_email.paddingLeft(30);

		lbl_password.setFont(asset_manager.getFont("def-font-24"));
		lbl_password.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		lbl_password.setText("Password");
		lbl_password.dockOnTopLeft(container);
		lbl_password.dockBelow(txt_email);
		lbl_password.paddingLeft(30);
		lbl_password.paddingTop(10);

		txt_password.setPasswordMode(true);
		txt_password.setSize(399, 46);
		txt_password.dockOnTopLeft(container);
		txt_password.dockBelow(lbl_password);
		txt_password.paddingLeft(30);

		btn_login.setSize(308, 107);
		btn_login.setScale(.7f);
		btn_login.addTexture("idle", asset_manager.getTexture("btn-pill"), true);
		btn_login.setFont(asset_manager.getFont("def-font-24"));
		btn_login.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_login.setText("Login");
		btn_login.dockOnBottomCenter(container);
		btn_login.paddingTop(-75);
		btn_login.setOnClickListener([&]() {
			Game::getInstance().getEventBus().emit<LoadingEvent>({ true });

			FirebaseAuthManager auth;
			auto& file_system_manager = Game::getInstance().getFileSystemManager();

			auth.signIn(txt_email.getText(), txt_password.getText(), [&](AuthResult result) {
				if (result.success)
				{
					std::cout << "user logged in: " << result.local_id << "\n";

					if (lbl_error.getVisible())
						lbl_error.setVisible(false);

					Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_LOGIN, false });
					Game::getInstance().getEventBus().emit<UserLoggedInEvent>({ result });

					Game::getInstance().getEventBus().emit<LoadingEvent>({ false });
				}
				else
				{
					std::cout << "sign in failed: " << result.error_message << "\n";

					lbl_error.setVisible(true);
					lbl_error.setText("Sign in failed:\n" + std::string(result.error_message));

					Game::getInstance().getEventBus().emit<LoadingEvent>({ false });
				}

				txt_email.setText("");
				txt_password.setText("");
			});
		});

		btn_register.setFont(asset_manager.getFont("def-font-24"));
		btn_register.setFontColor({ 0x34, 0x1c, 0x02, 0xff });
		btn_register.setText("Create an account?", true);
		btn_register.center(container);
		btn_register.dockBelow(btn_login);
		btn_register.paddingTop(10);
		btn_register.setOnClickListener([&]() {
			if (lbl_error.getVisible())
				lbl_error.setVisible(false);

			Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_LOGIN, false });
			Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_REGISTER, true });
		});

		lbl_error.setFont(asset_manager.getFont("calibri-regular-24"));
		lbl_error.setFontColor({ 0xff, 0x00, 0x00, 0xff });
		lbl_error.setText("sample error");
		lbl_error.dockBelow(container);
		lbl_error.dockOnBottomLeft(container);
		lbl_error.paddingTop(40);
		lbl_error.paddingLeft(10);
		lbl_error.setVisible(false);

		auto onLoginPanelRequest = [&](const MenuPanelRequestEvent& e) {
			if (e.pnl != PNL_LOGIN) return;

			if (e.open)
			{
				container.setVisible(true);
				header.setVisible(true);
				lbl_email.setVisible(true);
				lbl_password.setVisible(true);
				txt_email.setVisible(true);
				txt_password.setVisible(true);
				btn_login.setVisible(true);
				btn_register.setVisible(true);
			}
			else
			{
				container.setVisible(false);
				header.setVisible(false);
				lbl_email.setVisible(false);
				lbl_password.setVisible(false);
				txt_email.setVisible(false);
				txt_password.setVisible(false);
				btn_login.setVisible(false);
				btn_register.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onLoginPanelRequest);

		checkCurrentUser();
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	void checkCurrentUser()
	{
		FirebaseAuthManager auth;
		auto& event_bus = Game::getInstance().getEventBus();

		event_bus.emit<LoadingEvent>({ true });

		auth.checkLogin(
			[&event_bus](AuthResult user)
			{
				std::cout << "user exists\n";
				event_bus.emit<LoadingEvent>({ false });
				event_bus.emit<MenuPanelRequestEvent>({ PNL_LOGIN, false });
				event_bus.emit<UserLoggedInEvent>({ user });
			},
			[&event_bus]()
			{
				std::cout << "user not logged in\n";
				event_bus.emit<LoadingEvent>({ false });
			}
		);
	}
};

#endif