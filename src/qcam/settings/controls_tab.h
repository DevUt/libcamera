/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls_tab.h - qcam - Controls Tab
 */

#pragma once

#include <memory>

#include <libcamera/camera.h>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

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

private:
	std::shared_ptr<libcamera::ControlList> controlList_;
};
