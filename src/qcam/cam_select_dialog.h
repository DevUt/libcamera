
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * cam_select_dialog.h - qcam - Camera Selection dialog
 */

#pragma once

#include <memory>
#include <string>

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/controls.h>
#include <libcamera/property_ids.h>

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
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

		/* Set camera information labels. */
		cameraLocation_ = new QLabel;
		cameraModel_ = new QLabel;

		updateCamInfo(cm_->get(getCurrentCamera()));
		connect(cameraIdComboBox_, &QComboBox::currentTextChanged,
			this, [&]() {
				updateCamInfo(cm_->get(
					cameraIdComboBox_->currentText().toStdString()));
			});

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
		camSelectDialogLayout->addRow("Location: ", cameraLocation_);
		camSelectDialogLayout->addRow("Model: ", cameraModel_);
		camSelectDialogLayout->addWidget(dialogButtonBox);
	}

	~CamSelectDialog() = default;

	std::string getCameraId()
	{
		return cameraIdComboBox_->currentText().toStdString();
	}

	std::string getCurrentCamera()
	{
		return cameraIdComboBox_->currentText().toStdString();
	}

	/* Hotplug / Unplug Support. */
	void cameraAdded(libcamera::Camera *camera)
	{
		cameraIdComboBox_->addItem(QString::fromStdString(camera->id()));
	}

	void cameraRemoved(libcamera::Camera *camera)
	{
		int cameraIndex = cameraIdComboBox_->findText(
			QString::fromStdString(camera->id()));

		cameraIdComboBox_->removeItem(cameraIndex);
	}

	/* Camera Information */
	void updateCamInfo(const std::shared_ptr<libcamera::Camera> &camera)
	{
		if (camera == nullptr)
			return;

		const libcamera::ControlList &cameraProperties = camera->properties();

		const auto &location =
			cameraProperties.get(libcamera::properties::Location);
		if (location) {
			switch (*location) {
			case libcamera::properties::CameraLocationFront:
				cameraLocation_->setText("Internal front camera ");
				break;
			case libcamera::properties::CameraLocationBack:
				cameraLocation_->setText("Internal back camera ");
				break;
			case libcamera::properties::CameraLocationExternal:
				cameraLocation_->setText("External camera ");
				break;
			default:
				cameraLocation_->setText(QString());
			}
		} else {
			cameraLocation_->setText(QString());
		}

		const auto &model = cameraProperties.get(libcamera::properties::Model);

		if (model)
			cameraModel_->setText(QString::fromStdString(*model));
		else
			cameraModel_->setText(QString());
	}

private:
	libcamera::CameraManager *cm_;

	/* UI elements. */
	QComboBox *cameraIdComboBox_;
	QLabel *cameraLocation_;
	QLabel *cameraModel_;
};
