/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * control_frame.h - qcam - Control frame
 */

#pragma once

#include <libcamera/controls.h>

#include <QLabel>
#include <QFrame>
#include <QWidget>

class ControlFrame : public QFrame
{
	Q_OBJECT

public:
	ControlFrame(const libcamera::ControlId *control,
		     const libcamera::ControlInfo &controlInfo,
		     QWidget *parent);
	~ControlFrame() = default;

private:
	const libcamera::ControlId *control_;
	const libcamera::ControlInfo &controlInfo_;

	/* Widgets */
	QLabel *defaultValueLabel(QWidget *parent = nullptr);

	/* Helper Hunctions */
	QString getDefaultValueQStr();
};
