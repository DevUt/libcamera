/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#include "controls.h"

#include <memory>

#include <libcamera/camera.h>

#include <QLabel>
#include <QString>
#include <QVBoxLayout>

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *controlVLayout = new QVBoxLayout(this);

	for (const auto &[control, info] : camera_->controls())
		controlVLayout->addWidget(new QLabel(QString::fromStdString(control->name()), this));

	controlVLayout->setSpacing(20);
	setLayout(controlVLayout);
}
