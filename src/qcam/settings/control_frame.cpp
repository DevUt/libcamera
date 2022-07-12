/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * control_frame.cpp - qcam - Control frame
 */

#include "control_frame.h"

#include <libcamera/controls.h>

#include <QFrame>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

using namespace libcamera;

ControlFrame::ControlFrame(const ControlId *control,
			   const ControlInfo &controlInfo, QWidget *parent)
	: QFrame(parent), control_(control), controlInfo_(controlInfo)
{
	/* Main layout for the frame */
	QVBoxLayout *frameVLayout = new QVBoxLayout(this);

	frameVLayout->addWidget(new QLabel(QString::fromStdString(control_->name())));

	/*
	 * No need to pass parents to widgets, as QVBoxLayout transfers
	 * ownership to its parent widget.
	 */
	frameVLayout->addWidget(defaultValueLabel());

	setFrameStyle(QFrame::StyledPanel);
}

/* -----------------------------------------------------------------------------
 * Widgets
 */

QLabel *ControlFrame::defaultValueLabel(QWidget *parent)
{
	QLabel *defaultValLabel = new QLabel(parent);

	defaultValLabel->setText("Default Value: " + getDefaultValueQStr());

	return defaultValLabel;
}

/* -----------------------------------------------------------------------------
 * Helpers
 */

QString ControlFrame::getDefaultValueQStr()
{
	switch (control_->type()) {
	case ControlTypeBool:
		if (controlInfo_.def().get<bool>()) {
			return QString("True");
		} else {
			return QString("False");
		}
	case ControlTypeInteger32:
		return QString::number(controlInfo_.def().get<int32_t>());
	case ControlTypeInteger64:
		return QString::number(controlInfo_.def().get<int64_t>());
	case ControlTypeFloat:
		return QString::number(controlInfo_.def().get<float>());
	default:
		return QString("Unavailable");
	}
}
