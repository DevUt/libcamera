/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * slider.h - qcam - Slider
 */

#pragma once

#include <QSlider>
#include <QWidget>

class FloatSlider : public QSlider
{
	Q_OBJECT

	const float INT_TO_FLOAT = 100.0;

public:
	FloatSlider(QWidget *parent)
		: QSlider(parent)
	{
		connect(this, &QSlider::valueChanged,
			this, &FloatSlider::notifyValueChanged);
	}

	~FloatSlider() = default;

	void setValue(float value)
	{
		QSlider::setValue(value * INT_TO_FLOAT);
	}

	void setRange(float min, float max)
	{
		QSlider::setRange(min * INT_TO_FLOAT, max * INT_TO_FLOAT);
	}

	float minimum()
	{
		return QSlider::minimum() / INT_TO_FLOAT;
	}

	float maximum()
	{
		return QSlider::maximum() / INT_TO_FLOAT;
	}

	float value()
	{
		return QSlider::value() / INT_TO_FLOAT;
	}

Q_SIGNALS:
	void valueChanged(float value);

public Q_SLOTS:
	void notifyValueChanged(int value)
	{
		float floatVal = value / INT_TO_FLOAT;
		Q_EMIT valueChanged(floatVal);
	}
};
