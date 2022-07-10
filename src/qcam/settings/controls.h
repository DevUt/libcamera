/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * controls.cpp - qcam - Controls Tab
 */

#pragma once

#include <map>
#include <string>

#include <libcamera/camera.h>
#include <libcamera/controls.h>

#include <QCheckBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

#include "float_slider.h"

using namespace libcamera;

class ControlsIndv;

class ControlsTab : public QScrollArea
{
	Q_OBJECT

public:
	ControlsTab(std::shared_ptr<libcamera::Camera> camera_, QWidget *parent = nullptr);
	~ControlsTab(){};

private:
	std::shared_ptr<ControlList> controlList_;
	std::map<const unsigned int, ControlsIndv *> controlsMap_;

public Q_SLOTS:
	void unpackControls(const libcamera::ControlList);

private Q_SLOTS:
	void changeCntrlList(const libcamera::ControlId *control, const ControlValue value);

Q_SIGNALS:
	void cntrlListChanged(std::shared_ptr<ControlList> controls);
};

class ControlsIndv : public QWidget
{
	Q_OBJECT

public:
	ControlsIndv(const libcamera::ControlId *control, const libcamera::ControlInfo info);
	~ControlsIndv(){};

	QFrame *controlFrame();
	QLayout *controlNameLayout_();
	QWidget *controlItemHLayout_();

	void updateValue(const libcamera::ControlValue controlValue);
	void setDefaultValue();

private:
	const libcamera::ControlId *control_;
	const libcamera::ControlInfo info_;

	FloatSlider *fSlider_;
	QCheckBox *qCheckBox_;
	QLabel *currValueLabel_;
	QLabel *defaultValueLabel_;
	Slider *iSlider_;

private Q_SLOTS:
	void controlChange();
Q_SIGNALS:
	void indvCntrlChanged(const libcamera::ControlId *control, const libcamera::ControlValue value);
};