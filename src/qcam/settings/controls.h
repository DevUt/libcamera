/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#pragma once

#include <libcamera/camera.h>
#include <libcamera/controls.h>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QWidget>

using namespace libcamera;

class ControlsTab : public QWidget
{
	Q_OBJECT

public:
	ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent = nullptr);
	~ControlsTab(){};

};
