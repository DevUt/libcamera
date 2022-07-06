/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * settings.cpp - qcam - Float Slider
 */

#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class Slider : public QSlider
{
public:
	Slider(QWidget *parent)
		: QSlider(parent){};
	~Slider(){};

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

public:
	FloatSlider(QWidget *parent = 0)
		: Slider(parent)
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

	QString maximumValueStr() override
	{
		return QString::number(maximum() / 100.0);
	}

	QString minimumValueStr() override
	{
		return QString::number(minimum() / 100.0);
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
		float floatValue = value / 100.0;
		Q_EMIT valueChanged(floatValue);
	}
};

class SliderLayout : public QWidget
{
public:
	SliderLayout(Slider *slider, QWidget *parent)
		: QWidget(parent)
	{
		QVBoxLayout *mainLayout = new QVBoxLayout(this);
		QHBoxLayout *hlayout = new QHBoxLayout();

		QLabel *maxLabel = new QLabel(slider->maximumValueStr(), this);
		QLabel *minLabel = new QLabel(slider->minimumValueStr(), this);
		QLabel *currValueLabel = new QLabel(slider->currValueStr(), this);

		connect(slider, &Slider::valueChanged,
			this, [currValueLabel, slider]() {
				currValueLabel->setText(slider->currValueStr());
			});

		hlayout->addWidget(minLabel);
		hlayout->addStretch();
		hlayout->addWidget(currValueLabel);
		hlayout->addStretch();
		hlayout->addWidget(maxLabel);

		mainLayout->addLayout(hlayout);
		mainLayout->addWidget(slider);
	};
	~SliderLayout(){};
};