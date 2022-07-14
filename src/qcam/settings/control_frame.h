/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * control_frame.h - qcam - Control frame
 */

#pragma once

#include <libcamera/controls.h>

#include <QCheckBox>
#include <QFrame>
#include <QLabel>
#include <QString>
#include <QWidget>

#include "slider.h"

class ControlFrame : public QFrame
{
	Q_OBJECT

public:
	ControlFrame(const libcamera::ControlId *control,
		     const libcamera::ControlInfo &controlInfo,
		     QWidget *parent);
	~ControlFrame() = default;

	void setCurrentValue(const libcamera::ControlValue controlValue);
Q_SIGNALS:
	void controlChanged(const libcamera::ControlId *controlId,
			    const libcamera::ControlValue);

private Q_SLOTS:
	void notifyControlChange();

private:
	const libcamera::ControlId *control_;
	const libcamera::ControlInfo &controlInfo_;

	/* Widgets */
	QWidget *controlInteraction(QWidget *parent = nullptr);
	QWidget *currentValueLabel(QWidget *parent = nullptr);
	QLabel *defaultValueLabel(QWidget *parent = nullptr);

	QCheckBox *controlCheckBox_;
	QLabel *currentValue_;
	FloatSlider *floatSlider_;

	/* Helper Hunctions */
	QString getDefaultValueQStr();
};
