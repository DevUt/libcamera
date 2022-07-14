/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * slider.h - qcam - Slider
 */

#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

class Slider : public QSlider
{
	Q_OBJECT

public:
	Slider(QWidget *parent = nullptr)
		: QSlider(parent){};
	~Slider() = default;

	virtual QString maximumValueStr()
	{
		return QString::number(maximum());
	}

	virtual QString minimumValueStr()
	{
		return QString::number(minimum());
	}

	virtual QString currValueStr()
	{
		return QString::number(value());
	}
};

class FloatSlider : public Slider
{
	Q_OBJECT

	const float INT_TO_FLOAT = 100.0;

public:
	FloatSlider(QWidget *parent = nullptr)
		: Slider(parent)
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

	QString maximumValueStr() override
	{
		return QString::number(maximum());
	}

	QString minimumValueStr() override
	{
		return QString::number(minimum());
	}

	QString currValueStr() override
	{
		return QString::number(value());
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

class SliderLayout : public QWidget
{
	Q_OBJECT

public:
	SliderLayout(Slider *slider, QWidget *parent = nullptr)
		: QWidget(parent)
	{
		QVBoxLayout *mainVLayout = new QVBoxLayout(this);

		QHBoxLayout *labelHLayout = new QHBoxLayout;

		QLabel *minLabel = new QLabel(slider->minimumValueStr());
		QLabel *currValueLabel = new QLabel(slider->currValueStr());
		QLabel *maxLabel = new QLabel(slider->maximumValueStr());

		connect(slider, &Slider::valueChanged,
			this, [currValueLabel, slider]() {
				currValueLabel->setText(slider->currValueStr());
			});

		labelHLayout->addWidget(minLabel);
		labelHLayout->addStretch();
		labelHLayout->addWidget(currValueLabel);
		labelHLayout->addStretch();
		labelHLayout->addWidget(maxLabel);

		mainVLayout->addLayout(labelHLayout);
		mainVLayout->addWidget(slider);
	}
};
