/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings_dialog.h - qcam - Settings Dialog
 */

#pragma once

#include <memory>

#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "controls_tab.h"
class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(std::shared_ptr<libcamera::Camera> camera, QWidget *parent)
		: QDialog(parent)
	{
		/*Main layout for dialog */
		QVBoxLayout *settingVLayout = new QVBoxLayout(this);

		/* Main TabWidget which would hold other tabs */
		QTabWidget *settingTabWidget = new QTabWidget;
		settingVLayout->addWidget(settingTabWidget);

		ControlsTab *controlsTab = new ControlsTab(camera, this);
		settingTabWidget->addTab(controlsTab, "Controls");

		setWindowTitle("Settings");
	}
	~SettingsDialog() = default;
};
