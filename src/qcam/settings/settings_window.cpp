/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings.cpp - qcam - Settings Window
 */

#include "settings_window.h"

#include <libcamera/camera.h>

#include <QLabel>
#include <QSlider>
#include <QString>
#include <QTabWidget>
#include <QVBoxLayout>

#include "controls.h"

SettingsWindow::SettingsWindow(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("Settings");

	QTabWidget *settingTabs = new QTabWidget(this);

	ControlsTab *controlTabInstance = new ControlsTab(camera_, this);
	settingTabs->addTab(controlTabInstance, QString::fromStdString("Controls"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(settingTabs);
	setLayout(mainLayout);
}
