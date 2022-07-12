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

ControlFrame::ControlFrame(const libcamera::ControlId *control, QWidget *parent)
	: QFrame(parent), control_(control)
{
	/* Main layout for the frame */
	QVBoxLayout *frameVLayout = new QVBoxLayout(this);

	frameVLayout->addWidget(new QLabel(QString::fromStdString(control_->name())));

	setFrameStyle(QFrame::StyledPanel);
}
