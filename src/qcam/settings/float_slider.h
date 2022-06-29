/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings.cpp - qcam - Float Slider
 */

#pragma once

#include <QSlider>

class FloatSlider : public QSlider
{
	Q_OBJECT

public:
	FloatSlider(QWidget *parent = 0)
		: QSlider(parent)
	{
		connect(this, &QSlider::valueChanged,
			this, &FloatSlider::notifyValueChanged);
	}

	void setValue(float value)
	{
		QSlider::setValue(value * 100);
	}

	void setRange(float min, float max)
	{
		QSlider::setRange(min * 100, max * 100);
	}

	float value()
	{
		return QSlider::value() / 100.0;
	}

Q_SIGNALS:
	void valueChanged(float value);

public Q_SLOTS:
	void notifyValueChanged(int value)
	{
		float floatValue = value / 100.0;
		Q_EMIT valueChanged(floatValue);
	}
};