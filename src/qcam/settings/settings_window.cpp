/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings.cpp - qcam - Settings Window
 */

#include "settings_window.h"

#include <libcamera/camera.h>

#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>

#include "controls.h"

SettingsWindow::SettingsWindow(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("Settings");
	settingTabs = new QTabWidget;

	/* Add the controls tab */
	ControlsTab *controlTabInstance = new ControlsTab(camera_, this);
	settingTabs->addTab(controlTabInstance, QString::fromStdString("Controls"));
	connect(controlTabInstance, &ControlsTab::cntrlListChanged,
		this, &SettingsWindow::controlsChanged);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(settingTabs);
	setLayout(mainLayout);
}
