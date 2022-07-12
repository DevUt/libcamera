/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls_tab.cpp - qcam - Controls Tab
 */

#include "controls_tab.h"

#include <memory>

#include <libcamera/camera.h>

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "control_frame.h"

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_,
			 QWidget *parent)
	: QWidget(parent)
{
	/* Main Layout for the tab */
	QGridLayout *controlGLayout = new QGridLayout(this);

	int controlCount = 0;
	for (auto &[control, info] : camera_->controls()) {
		ControlFrame *controlFrame = new ControlFrame(control, this);

		controlGLayout->addWidget(controlFrame, controlCount / 2,
					  controlCount % 2);
		controlCount++;
	}

	if (controlCount == 0)
		controlGLayout->addWidget(new QLabel("No controls available"));
}
