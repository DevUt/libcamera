/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * cam_select_dialog.cpp - qcam - Camera Selection dialog
 */

#include "cam_select_dialog.h"

#include <memory>

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QWidget>

CameraSelectorDialog::CameraSelectorDialog(libcamera::CameraManager *cameraManager,
					   std::string scriptPath, QWidget *parent)
	: QDialog(parent), cm_(cameraManager), scriptPath_(scriptPath)
{
	/* Use a QFormLayout for the dialog. */
	QFormLayout *layout = new QFormLayout(this);

	/* Setup the camera id combo-box. */
	cameraIdComboBox_ = new QComboBox;
	for (const auto &cam : cm_->cameras())
		cameraIdComboBox_->addItem(QString::fromStdString(cam->id()));

	/* Set camera information labels. */
	cameraLocation_ = new QLabel;
	cameraModel_ = new QLabel;

	updateCamInfo(cameraIdComboBox_->currentText());
	connect(cameraIdComboBox_, &QComboBox::currentTextChanged,
		this, &CameraSelectorDialog::updateCamInfo);

	/* Set capture script selection / removal button. */
	QWidget *captureWidget = new QWidget(this);
	QHBoxLayout *captureLayout = new QHBoxLayout(captureWidget);

	scriptFileLine_ = new QLabel;
	scriptFileLine_->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

	chooseCaptureScriptButton_ = new QPushButton;
	chooseCaptureScriptButton_->setIcon(QIcon::fromTheme("document-open",
							     QIcon(":upload.svg")));
	connect(chooseCaptureScriptButton_, &QPushButton::clicked,
		this, &CameraSelectorDialog::selectCaptureScript);

	QPushButton *stopCaptureScriptButton = new QPushButton;
	stopCaptureScriptButton->setIcon(QIcon::fromTheme("edit-clear",
							  QIcon(":delete.svg")));
	connect(stopCaptureScriptButton, &QPushButton::clicked,
		this, &CameraSelectorDialog::resetCaptureScript);

	captureLayout->addWidget(scriptFileLine_);
	captureLayout->addWidget(chooseCaptureScriptButton_);
	captureLayout->addWidget(stopCaptureScriptButton);
	captureLayout->setMargin(0);

	/* Set the file name of the capture script. */
	if (scriptPath_.empty()) {
		scriptFileLine_->setText("No File Selected");
	} else {
		scriptFileInfo_.setFile(QString::fromStdString(scriptPath_));
		scriptFileLine_->setText(scriptFileInfo_.fileName());
		scriptFileLine_->setToolTip(scriptFileInfo_.filePath());
	}

	/* Setup the QDialogButton Box */
	QDialogButtonBox *buttonBox =
		new QDialogButtonBox(QDialogButtonBox::Ok |
				     QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted,
		this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected,
		this, &QDialog::reject);

	/* Set the layout. */
	layout->addRow("Camera:", cameraIdComboBox_);
	layout->addRow("Location:", cameraLocation_);
	layout->addRow("Model:", cameraModel_);
	layout->addRow("Capture Script:", captureWidget);
	layout->addWidget(buttonBox);
}

CameraSelectorDialog::~CameraSelectorDialog() = default;

std::string CameraSelectorDialog::getCameraId()
{
	return cameraIdComboBox_->currentText().toStdString();
}

/* Hotplug / Unplug Support. */
void CameraSelectorDialog::addCamera(QString camearaId)
{
	cameraIdComboBox_->addItem(camearaId);
}

void CameraSelectorDialog::removeCamera(QString cameraId)
{
	int cameraIndex = cameraIdComboBox_->findText(cameraId);
	cameraIdComboBox_->removeItem(cameraIndex);
}

/* Camera Information */
void CameraSelectorDialog::updateCamInfo(QString cameraId)
{
	const std::shared_ptr<libcamera::Camera> &camera =
		cm_->get(cameraId.toStdString());

	if (!camera)
		return;

	const libcamera::ControlList &properties = camera->properties();

	const auto &location = properties.get(libcamera::properties::Location);
	if (location) {
		switch (*location) {
		case libcamera::properties::CameraLocationFront:
			cameraLocation_->setText("Internal front camera");
			break;
		case libcamera::properties::CameraLocationBack:
			cameraLocation_->setText("Internal back camera");
			break;
		case libcamera::properties::CameraLocationExternal:
			cameraLocation_->setText("External camera");
			break;
		default:
			cameraLocation_->setText("Unknown");
		}
	} else {
		cameraLocation_->setText("Unknown");
	}

	const auto &model = properties
				    .get(libcamera::properties::Model)
				    .value_or("Unknown");

	cameraModel_->setText(QString::fromStdString(model));
}

/* Capture script support. */
void CameraSelectorDialog::selectCaptureScript()
{
	selectedScriptPath_ = QFileDialog::getOpenFileName(this,
							   "Run Capture Script", QDir::currentPath(),
							   "Capture Script (*.yaml)")
				      .toStdString();

	if (!selectedScriptPath_.empty()) {
		scriptFileInfo_.setFile(QString::fromStdString(selectedScriptPath_));
		scriptFileLine_->setText(scriptFileInfo_.fileName());
		scriptFileLine_->setToolTip(scriptFileInfo_.filePath());
	} else {
		selectedScriptPath_ = scriptPath_;
	}
}

void CameraSelectorDialog::resetCaptureScript()
{
	Q_EMIT stopCaptureScript();
	scriptPath_.clear();
	selectedScriptPath_.clear();
	scriptFileLine_->setText("No File Selected");
}

void CameraSelectorDialog::accept()
{
	scriptPath_ = selectedScriptPath_;
	QDialog::accept();
}

void CameraSelectorDialog::reject()
{
	if (scriptPath_.empty()) {
		scriptFileLine_->setText("No File Selected");
	} else {
		scriptFileInfo_.setFile(QString::fromStdString(scriptPath_));
		scriptFileLine_->setText(scriptFileInfo_.fileName());
		scriptFileLine_->setToolTip(scriptFileInfo_.filePath());
	}
	QDialog::reject();
}
