#ifndef REGISTER_PANEL_H
#define REGISTER_PANEL_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "FirebaseAuthManager.h"

class RegisterPanel : public IScript {
public:
	RegisterPanel(AssetManager& asset_manager, EntityManager& entity_manager) : asset_manager(asset_manager), entity_manager(entity_manager)
	{}

	void init() override
	{
		auto& container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& lbl_email = entity_manager.addEntity().addComponent<Label>();
		auto& lbl_password = entity_manager.addEntity().addComponent<Label>();
		auto& txt_email = entity_manager.addEntity().addComponent<TextInputBox>(asset_manager.getFont("calibri-regular-24"));
		auto& txt_password = entity_manager.addEntity().addComponent<TextInputBox>(asset_manager.getFont("calibri-regular-24"));
		auto& btn_register = entity_manager.addEntity().addComponent<Button>();
		auto& btn_login = entity_manager.addEntity().addComponent<Button>();
		auto& lbl_error = entity_manager.addEntity().addComponent<Label>();

		container.setSize(948, 881);
		container.setScale(.5f);
		container.setTexture(asset_manager.getTexture("high-score-panel-bg"));
		container.center({ 0, 0, 1920, 1080 });
		container.setVisible(false);

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("header-register"));
		header.dockOnTopCenter(container);
		header.paddingTop(-100);
		header.setVisible(false);

		lbl_email.setFont(asset_manager.getFont("def-font-24"));
		lbl_email.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		lbl_email.setText("E-mail");
		lbl_email.dockOnTopLeft(container);
		lbl_email.dockBelow(header);
		lbl_email.paddingLeft(30);
		lbl_email.setVisible(false);

		txt_email.setSize(399, 46);
		txt_email.dockOnTopLeft(container);
		txt_email.dockBelow(lbl_email);
		txt_email.paddingLeft(30);
		txt_email.setVisible(false);

		lbl_password.setFont(asset_manager.getFont("def-font-24"));
		lbl_password.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		lbl_password.setText("Password");
		lbl_password.dockOnTopLeft(container);
		lbl_password.dockBelow(txt_email);
		lbl_password.paddingLeft(30);
		lbl_password.paddingTop(10);
		lbl_password.setVisible(false);

		txt_password.setPasswordMode(true);
		txt_password.setSize(399, 46);
		txt_password.dockOnTopLeft(container);
		txt_password.dockBelow(lbl_password);
		txt_password.paddingLeft(30);
		txt_password.setVisible(false);

		btn_register.setSize(308, 107);
		btn_register.setScale(.7f);
		btn_register.addTexture("idle", asset_manager.getTexture("btn-pill"), true);
		btn_register.setFont(asset_manager.getFont("def-font-24"));
		btn_register.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_register.setText("Register");
		btn_register.dockOnBottomCenter(container);
		btn_register.paddingTop(-75);
		btn_register.setVisible(false);
		btn_register.setOnClickListener([&]() {
			Game::getInstance().getEventBus().emit<LoadingEvent>({ true });

			FirebaseAuthManager auth;

			auth.signUp(txt_email.getText(), txt_password.getText(), [&](AuthResult result) {
				if (result.success)
				{
					std::cout << "user registered: " << result.local_id << "\n";

					if (lbl_error.getVisible())
						lbl_error.setVisible(false);

					Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_REGISTER, false });
					Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_LOGIN, true });

					Game::getInstance().getEventBus().emit<LoadingEvent>({ false });
				}
				else
				{
					std::cout << "sign up failed: " << result.error_message << "\n";

					lbl_error.setVisible(true);
					lbl_error.setText("Sign up failed:\n" + std::string(result.error_message));

					Game::getInstance().getEventBus().emit<LoadingEvent>({ false });
				}

				txt_email.setText("");
				txt_password.setText("");
			});
		});

		btn_login.setFont(asset_manager.getFont("def-font-24"));
		btn_login.setFontColor({ 0x34, 0x1c, 0x02, 0xff });
		btn_login.setText("Already have an account?", true);
		btn_login.center(container);
		btn_login.dockBelow(btn_register);
		btn_login.paddingTop(10);
		btn_login.setVisible(false);
		btn_login.setOnClickListener([&]() {
			if (lbl_error.getVisible())
				lbl_error.setVisible(false);

			Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_REGISTER, false });
			Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_LOGIN, true });
		});

		lbl_error.setFont(asset_manager.getFont("calibri-regular-24"));
		lbl_error.setFontColor({ 0xff, 0x00, 0x00, 0xff });
		lbl_error.setText("sample error");
		lbl_error.dockBelow(container);
		lbl_error.dockOnBottomLeft(container);
		lbl_error.paddingTop(40);
		lbl_error.paddingLeft(10);
		lbl_error.setVisible(false);

		auto onRegisterPanelRequest = [&](const MenuPanelRequestEvent& e) {
			if (e.pnl != PNL_REGISTER) return;

			if (e.open)
			{
				container.setVisible(true);
				header.setVisible(true);
				lbl_email.setVisible(true);
				lbl_password.setVisible(true);
				txt_email.setVisible(true);
				txt_password.setVisible(true);
				btn_register.setVisible(true);
				btn_login.setVisible(true);
			}
			else
			{
				container.setVisible(false);
				header.setVisible(false);
				lbl_email.setVisible(false);
				lbl_password.setVisible(false);
				txt_email.setVisible(false);
				txt_password.setVisible(false);
				btn_register.setVisible(false);
				btn_login.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onRegisterPanelRequest);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;
};

#endif