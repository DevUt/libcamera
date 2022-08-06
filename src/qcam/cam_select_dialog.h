
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * cam_select_dialog.h - qcam - Camera Selection dialog
 */

#pragma once

#include <string>

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QString>

class CamSelectDialog : public QDialog
{
	Q_OBJECT
public:
	CamSelectDialog(libcamera::CameraManager *cameraManager, QWidget *parent)
		: QDialog(parent), cm_(cameraManager)
	{
		/* Use a QFormLayout for the dialog. */
		QFormLayout *camSelectDialogLayout = new QFormLayout(this);

		/* Setup the camera id combo-box. */
		cameraIdComboBox_ = new QComboBox;
		for (const auto &cam : cm_->cameras())
			cameraIdComboBox_->addItem(QString::fromStdString(cam->id()));

		/* Setup the QDialogButton Box */
		QDialogButtonBox *dialogButtonBox =
			new QDialogButtonBox(QDialogButtonBox::Ok |
					     QDialogButtonBox::Cancel);

		connect(dialogButtonBox, &QDialogButtonBox::accepted,
			this, &QDialog::accept);
		connect(dialogButtonBox, &QDialogButtonBox::rejected,
			this, &QDialog::reject);

		/* Set the layout. */
		camSelectDialogLayout->addRow("Camera: ", cameraIdComboBox_);
		camSelectDialogLayout->addWidget(dialogButtonBox);
	}

	~CamSelectDialog() = default;

	std::string getCameraId()
	{
		return cameraIdComboBox_->currentText().toStdString();
	}

private:
	libcamera::CameraManager *cm_;

	/* UI elements. */
	QComboBox *cameraIdComboBox_;
};
