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
#include <QScrollArea>
#include <QWidget>

#include "control_frame.h"

using namespace libcamera;

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_,
			 QWidget *parent)
	: QScrollArea(parent), controlList_(std::make_shared<ControlList>())
{
	QWidget *containerWidget = new QWidget(this);
	/* Main Layout for the tab */
	QGridLayout *controlGLayout = new QGridLayout(containerWidget);

	int controlCount = 0;
	for (auto &[control, info] : camera_->controls()) {
		ControlFrame *controlFrame = new ControlFrame(control, info, this);
		controlFrameMap_[control->id()] = controlFrame;
		connect(controlFrame, &ControlFrame::controlChanged,
			this, &ControlsTab::controlChanged);

		controlGLayout->addWidget(controlFrame, controlCount / 2,
					  controlCount % 2);
		controlCount++;
	}

	if (controlCount == 0)
		controlGLayout->addWidget(new QLabel("No controls available"));

	/* Set widget and policies for the scrollarea */
	setWidget(containerWidget);
	setWidgetResizable(true);
}

/* -----------------------------------------------------------------------------
 * Qt Slots
 */

void ControlsTab::notifyControlFrame(std::shared_ptr<const libcamera::ControlList> controlList)
{
	for (auto &[id, controlValue] : *(controlList))
		controlFrameMap_[id]->setCurrentValue(controlValue);
}

void ControlsTab::controlChanged(const libcamera::ControlId *controlId,
				 const libcamera::ControlValue controlValue)
{
	controlList_->clear();

	controlList_->set(controlId->id(), controlValue);
	Q_EMIT controlListChanged(controlList_);
}
