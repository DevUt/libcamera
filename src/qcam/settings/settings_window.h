/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings.cpp - qcam - Settings Window
 */

#pragma once

#include <memory>

#include <libcamera/camera.h>

#include <QDialog>

using namespace libcamera;
class SettingsWindow : public QDialog
{
	Q_OBJECT

public:
	SettingsWindow(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent);
	~SettingsWindow(){};
};
