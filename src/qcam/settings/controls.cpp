/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#include "controls.h"

#include <memory>

#include <libcamera/camera.h>

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QtDebug>

#include "float_slider.h"

ControlsTab::ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent)
	: QScrollArea(parent)
{
	// TODO: Fix using rebase  parent of formlayout

	setWidgetResizable(true);

	QWidget *mainContainerWidget = new QWidget();
	QVBoxLayout *controlVLayout = new QVBoxLayout(mainContainerWidget);
	// controlVLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	controlList_ = std::make_shared<ControlList>();

	int controlcount = 0;
	for (const auto &[control, info] : camera_->controls()) {
		ControlsIndv *controlItem = new ControlsIndv(control, info);
		controlsMap_[control->id()] = controlItem;

		controlItem->setDefaultValue();
		controlVLayout->addWidget(controlItem->controlFrame());
		controlcount++;

		connect(controlItem, &ControlsIndv::indvCntrlChanged,
			this, &ControlsTab::changeCntrlList);
	}
	setWidget(mainContainerWidget);
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

QFrame *ControlsIndv::controlFrame()
{
	QFrame *mainControlFrame = new QFrame(this);
	QVBoxLayout *frameVLayout = new QVBoxLayout(this);
	frameVLayout->addLayout(controlNameLayout_());
	frameVLayout->addWidget(controlItemHLayout_());
	mainControlFrame->setFrameShape(QFrame::StyledPanel);
	mainControlFrame->setLayout(frameVLayout);
	return mainControlFrame;
}

QWidget *ControlsIndv::controlItemHLayout_()
{
	switch (control_->type()) {
	case ControlTypeBool: {
		qCheckBox_ = new QCheckBox(this);
		qCheckBox_->setTristate(false);
		if (info_.def().get<bool>())
			qCheckBox_->setCheckState(Qt::Checked);
		else
			qCheckBox_->setCheckState(Qt::Unchecked);
		connect(qCheckBox_, &QCheckBox::stateChanged,
			this, &ControlsIndv::controlChange);

		QWidget *hideWidget = new QWidget(this);
		QHBoxLayout *boolControlLabel = new QHBoxLayout();
		boolControlLabel->setAlignment(Qt::AlignLeft);
		boolControlLabel->setMargin(0);
		boolControlLabel->addWidget(new QLabel("Enabled: "));
		boolControlLabel->addWidget(qCheckBox_);
		hideWidget->setLayout(boolControlLabel);
		return hideWidget;
	}
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

	QHBoxLayout *currentValueLayout = new QHBoxLayout();
	currentValueLayout->setAlignment(Qt::AlignLeft);
	currentValueLayout->addWidget(new QLabel(QString("Current Value: ")));
	currentValueLayout->addWidget(currValueLabel_);

	QHBoxLayout *defaultValueLayout = new QHBoxLayout();
	defaultValueLayout->setAlignment(Qt::AlignLeft);
	defaultValueLayout->addWidget(new QLabel(QString("Default Value: ")));
	defaultValueLayout->addWidget(defaultValueLabel_);

	nameLayout->addLayout(defaultValueLayout);
	nameLayout->addLayout(currentValueLayout);

	return nameLayout;
}

void ControlsIndv::controlChange()
{
	ControlValue controlVal = ControlValue();

	switch (control_->type()) {
	case ControlTypeBool:
		if (qCheckBox_->checkState() == Qt::CheckState::Checked)
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

void ControlsIndv::setDefaultValue()
{
	defaultValueLabel_ = new QLabel();
	switch (control_->type()) {
	case ControlTypeBool:
		if (info_.def().get<bool>())
			defaultValueLabel_->setText("True");
		else
			defaultValueLabel_->setText("False");
		break;

	case ControlTypeFloat:
		defaultValueLabel_->setText(QString::number(info_.def().get<float>()));
		break;

	case ControlTypeInteger32:
		defaultValueLabel_->setText(QString::number(info_.def().get<int32_t>()));
		break;

	default:
		break;
	}
}