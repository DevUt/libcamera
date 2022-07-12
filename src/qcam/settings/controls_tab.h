/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls_tab.h - qcam - Controls Tab
 */

#pragma once

#include <memory>

#include <libcamera/camera.h>

#include <QVBoxLayout>
#include <QWidget>

class ControlsTab : public QWidget
{
	Q_OBJECT

public:
	ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent);
	~ControlsTab() = default;
};
