/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#include "controls.h"

#include <memory>

#include <libcamera/camera.h>

#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>

#include "float_slider.h"

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QWidget(parent)
{
	// TODO: Fix using rebase  parent of formlayout
	QFormLayout *controlFLayout = new QFormLayout();

	controlList_ = std::make_shared<ControlList>();

	for (const auto &[control, info] : camera_->controls()) {
		ControlsIndv *controlItem = new ControlsIndv(control, info);
		controlFLayout->addRow(QString::fromStdString(control->name()),
				       controlItem->controlItemHLayout_());
		connect(controlItem, &ControlsIndv::indvCntrlChanged,
			this, &ControlsTab::changeCntrlList);
	}

	controlFLayout->setSpacing(20);
	controlFLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	setLayout(controlFLayout);
}

void ControlsTab::changeCntrlList(const libcamera::ControlId *control,
				  const ControlValue value)
{
	controlList_->set(control->id(), value);
	Q_EMIT cntrlListChanged(controlList_);
}

ControlsIndv::ControlsIndv(const libcamera::ControlId *control,
			   const libcamera::ControlInfo info)
	: control_(control), info_(info){};

QWidget *ControlsIndv::controlItemHLayout_()
{
	switch (control_->type()) {
	case ControlTypeBool:
		qCheckBox_ = new QCheckBox(this);
		qCheckBox_->setTristate(false);
		if (info_.def().get<bool>())
			qCheckBox_->setCheckState(Qt::Checked);
		else
			qCheckBox_->setCheckState(Qt::Unchecked);
		return qCheckBox_;

	case ControlTypeFloat: {
		fSlider_ = new FloatSlider(this);
		fSlider_->setOrientation(Qt::Horizontal);
		fSlider_->setRange(info_.min().get<float>(), info_.max().get<float>());
		fSlider_->setValue(info_.def().get<float>());
		connect(fSlider_, &FloatSlider::valueChanged,
			this, &ControlsIndv::controlChange);
		SliderLayout *slideLayout = new SliderLayout(fSlider_, this);
		return slideLayout;
	}

	case ControlTypeInteger32: {
		iSlider_ = new Slider(this);
		iSlider_->setOrientation(Qt::Horizontal);
		iSlider_->setRange(info_.min().get<int32_t>(), info_.max().get<int32_t>());
		iSlider_->setValue(info_.def().get<int32_t>());
		connect(iSlider_, &Slider::valueChanged,
			this, &ControlsIndv::controlChange);
		SliderLayout *slideLayout = new SliderLayout(iSlider_, this);
		return slideLayout;
	}
	default:
		return (new QLabel(QString::fromStdString("Currently not supported"), this));
	}
}

void ControlsIndv::controlChange()
{
	ControlValue controlVal = ControlValue();

	switch (control_->type()) {
	case ControlTypeBool:
		controlVal.set<bool>(qCheckBox_->checkState());
		break;

	case ControlTypeFloat:
		controlVal.set<float>(fSlider_->value());
		break;

	case ControlTypeInteger32:
		controlVal.set<int32_t>(iSlider_->value());
		break;
	default:
		return;
	}

	Q_EMIT indvCntrlChanged(control_, controlVal);
}
