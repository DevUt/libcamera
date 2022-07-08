/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#include "controls.h"

#include <memory>

#include <libcamera/camera.h>

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QtDebug>

#include "float_slider.h"

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QWidget(parent)
{
	// TODO: Fix using rebase  parent of formlayout
	QGridLayout *controlGLayout = new QGridLayout(this);

	controlList_ = std::make_shared<ControlList>();

	int controlcount = 0;
	for (const auto &[control, info] : camera_->controls()) {
		ControlsIndv *controlItem = new ControlsIndv(control, info);
		controlsMap_[control->id()] = controlItem;

		controlGLayout->addLayout(controlItem->controlNameLayout_(),controlcount,0);
		controlGLayout->addWidget(controlItem->controlItemHLayout_(),controlcount,1);

		controlItem->updateValue(info.def());
		controlcount++;

		connect(controlItem, &ControlsIndv::indvCntrlChanged,
			this, &ControlsTab::changeCntrlList);
	}

}

void ControlsTab::changeCntrlList(const libcamera::ControlId *control,
				  const ControlValue value)
{
	controlList_->set(control->id(), value);
	Q_EMIT cntrlListChanged(controlList_);
}

void ControlsTab::unpackControls(const libcamera::ControlList controlList)
{
	for (auto &[controlId, controlValue] : controlList)
		controlsMap_[controlId]->updateValue(controlValue);
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
		connect(qCheckBox_,&QCheckBox::stateChanged,
			this,&ControlsIndv::controlChange);
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

QLayout *ControlsIndv::controlNameLayout_()
{
	QVBoxLayout *nameLayout = new QVBoxLayout();

	currValueLabel_ = new QLabel();

	nameLayout->addWidget(new QLabel(QString::fromStdString(control_->name())));
	nameLayout->addWidget(currValueLabel_);

	return nameLayout;
}

void ControlsIndv::controlChange()
{
	ControlValue controlVal = ControlValue();

	switch (control_->type()) {
	case ControlTypeBool:
		qInfo()<<"Changing bool value";
		if(qCheckBox_->checkState() == Qt::CheckState::Checked)
			controlVal.set<bool>(true);
		else
			controlVal.set<bool>(false);
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

void ControlsIndv::updateValue(const libcamera::ControlValue controlValue)
{
	switch (control_->type()) {
	case ControlTypeBool:
		if (controlValue.get<bool>())
			currValueLabel_->setText("True");
		else
			currValueLabel_->setText("False");
		break;

	case ControlTypeFloat:
		currValueLabel_->setText(QString::number(controlValue.get<float>()));
		break;

	case ControlTypeInteger32:
		currValueLabel_->setText(QString::number(controlValue.get<int32_t>()));
		break;

	default:
		return;
	}
}
