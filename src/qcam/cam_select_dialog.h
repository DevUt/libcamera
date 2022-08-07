/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Utkarsh Tiwari <utkarsh02t@gmail.com>
 *
 * cam_select_dialog.h - qcam - Camera Selection dialog
 */

#pragma once

#include <string>

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/controls.h>
#include <libcamera/property_ids.h>

#include <QDialog>
#include <QFileInfo>
#include <QString>

class QComboBox;
class QLabel;
class QToolButton;

class CameraSelectorDialog : public QDialog
{
	Q_OBJECT
public:
	CameraSelectorDialog(libcamera::CameraManager *cameraManager,
			     std::string scriptPath, QWidget *parent);

	~CameraSelectorDialog();

	std::string getCameraId();
	std::string getCaptureScript() { return scriptPath_; };

	/* Hotplug / Unplug Support. */
	void addCamera(QString cameraId);
	void removeCamera(QString cameraId);

	/* Camera Information */
	void updateCamInfo(QString cameraId);

	/* Capture script support. */
	void selectCaptureScript();
	void resetCaptureScript();

	void accept() override;
	void reject() override;

Q_SIGNALS:
	void stopCaptureScript();

private:
	libcamera::CameraManager *cm_;

	std::string scriptPath_;
	std::string selectedScriptPath_;
	QFileInfo scriptFileInfo_;
	/* UI elements. */
	QComboBox *cameraIdComboBox_;
	QLabel *cameraLocation_;
	QLabel *cameraModel_;
	QLabel *scriptFileLine_;
	QPushButton *chooseCaptureScriptButton_;
};
