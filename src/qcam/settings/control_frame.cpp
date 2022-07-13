/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * control_frame.cpp - qcam - Control frame
 */

#include "control_frame.h"

#include <libcamera/controls.h>

#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

using namespace libcamera;

ControlFrame::ControlFrame(const libcamera::ControlId *control,
			   const libcamera::ControlInfo &controlInfo,
			   QWidget *parent)
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
	frameVLayout->addWidget(controlInteraction());

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

QWidget *ControlFrame::controlInteraction(QWidget *parent)
{
	QWidget *containerWidget = new QWidget(parent);

	switch (control_->type()) {
	case ControlTypeBool: {
		QHBoxLayout *HCheckBoxLayout = new QHBoxLayout(containerWidget);

		HCheckBoxLayout->addWidget(new QLabel("Enabled :"));

		controlCheckBox_ = new QCheckBox;
		/*
		 * In the start we are not sure what is exactly the state of
		 * the control. Do not assume. Set in partially checked state.
		 */
		controlCheckBox_->setCheckState(Qt::PartiallyChecked);

		connect(controlCheckBox_, &QCheckBox::stateChanged,
			this, &ControlFrame::notifyControlChange);

		HCheckBoxLayout->addWidget(controlCheckBox_);

		/* Align it with the name of the control. */
		HCheckBoxLayout->setAlignment(Qt::AlignLeft);
		HCheckBoxLayout->setMargin(0);
		return containerWidget;
	}
	default:
		return (new QLabel("Currently Unavailable"));
	}
}

/* -----------------------------------------------------------------------------
 * Qt Slots
 */

void ControlFrame::notifyControlChange()
{
	ControlValue controlValue = ControlValue();

	switch (control_->type()) {
	case ControlTypeBool:

		/*
		 * When clicked for the first time, the switch comes from a
		 * partially checked state. Turn the triset off so we can have
		 * only enabled and disabled states.
		 */
		controlCheckBox_->setTristate(false);
		/*
		 * When this function is invoked, the checkbox can only be in
		 * the Checked or Unchecked State (after the first time).
		 */
		if (controlCheckBox_->checkState() == Qt::CheckState::Checked)
			controlValue.set<bool>(true);
		else
			controlValue.set<bool>(false);

		break;
	default:
		/* Nothing to emit so return */
		return;
	}

	Q_EMIT controlChanged(control_, controlValue);
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
