/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls_tab.h - qcam - Controls Tab
 */

#pragma once

#include <map>
#include <memory>

#include <libcamera/camera.h>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "control_frame.h"

class ControlsTab : public QScrollArea
{
	Q_OBJECT

public:
	ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent);
	~ControlsTab() = default;

Q_SIGNALS:
	void controlListChanged(const std::shared_ptr<const libcamera::ControlList>);

public Q_SLOTS:
	void controlChanged(const libcamera::ControlId *controlId,
			    const libcamera::ControlValue controlValue);
	void notifyControlFrame(std::shared_ptr<const libcamera::ControlList> controlList);

private:
	std::shared_ptr<libcamera::ControlList> controlList_;
	std::map<const unsigned int, ControlFrame *> controlFrameMap_;
};
