////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of ShadowSimulation, a C++ app for schadow simulation    //
//                                                                            //
// Copyright (C) 2006 Alexandru Duliu                                         //
//                                                                            //
// ShadowSimulation is free software; you can redistribute it and/or          //
// modify it under the terms of the GNU  General Public License               //
// as published by the Free Software Foundation; either version 3             //
// of the License, or (at your option) any later version.                     //
//                                                                            //
// ShadowSimulation is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with ShadowSimulation. If not, see <http://www.gnu.org/licenses/>.   //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////

#ifndef WINDOWQT_H
#define WINDOWQT_H

#include <QtGui>
#include "frame.h"
#include "ui_gui_mainWindow.h"
#include "ui_gui_aboutWindow.h"
#include "ui_gui_shadowingPeriod.h"
#include "ui_gui_modelAdjustment.h"
#include <QWidget>
#include <QFileDialog>

class WindowQT : public QMainWindow
{
	Q_OBJECT

public:
	WindowQT();

public slots:
	void showShadowingPeriod();
	void okShadowingPeriod();
	void cancelShadowingPeriod();

	void showModelAdjustment();
	void okModelAdjustment();
	void cancelModelAdjustment();

	//void setConfigureShadows();
	void openModel();
	void exitProgram();
	void aboutProgram();

private:
	FrameQT *frameQT;
	Ui_MainWindow ui;

	QDialog *aboutDialog;

	QDialog *shadowingDialog;
	Ui_ShadowingDialog *sDialog;

	double rotX, rotY, rotZ;
	bool invertFaces, invertNormals, smoothShading;
	bool normalsCalculated;
	int sceneRenderingMode;

	QDialog *modelAdjustmentDialog;
	Ui_ModelAdjustmentDialog *mDialog;
};

#endif
