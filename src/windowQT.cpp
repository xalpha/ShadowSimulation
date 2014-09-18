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

#include "windowQT.h"

WindowQT::WindowQT()
{
	// setup this window
	ui.setupUi(this);
	QObject::connect(ui.actionExit, SIGNAL(activated()), this, SLOT(exitProgram()));
	QObject::connect(ui.actionOpen, SIGNAL(activated()), this, SLOT(openModel()));
	QObject::connect(ui.actionShadowing_Period, SIGNAL(activated()), this, SLOT(showShadowingPeriod()));
	QObject::connect(ui.actionModel_Adjustment, SIGNAL(activated()), this, SLOT(showModelAdjustment()));
	//QObject::connect(ui.actionConfigure_Shadows, SIGNAL(activated()), this, SLOT());
	QObject::connect(ui.actionAbout, SIGNAL(activated()), this, SLOT(aboutProgram()));
	
	// add the QGLWidget a.k.a. the main program ;)
	frameQT = new FrameQT();
	setCentralWidget(frameQT);

	// add the about dialog
	aboutDialog = new QDialog(this);
	Ui_AboutDialog adialog;
	adialog.setupUi(aboutDialog);

	// add the shadowing Period dialog
	shadowingDialog = new QDialog(this);
	sDialog = new Ui_ShadowingDialog();
	sDialog->setupUi(shadowingDialog);
	QObject::connect(sDialog->okButton, SIGNAL(clicked()), this, SLOT(okShadowingPeriod()));
	QObject::connect(sDialog->cancelButton, SIGNAL(clicked()), this, SLOT(cancelShadowingPeriod()));

	// add the model adjustment dialog
	modelAdjustmentDialog = new QDialog(this);
	mDialog = new Ui_ModelAdjustmentDialog();
	mDialog->setupUi(modelAdjustmentDialog);
	QObject::connect(mDialog->okButton, SIGNAL(clicked()), this, SLOT(okModelAdjustment()));
	QObject::connect(mDialog->cancelButton, SIGNAL(clicked()), this, SLOT(cancelModelAdjustment()));
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;;
	invertFaces = false;
	invertNormals = false;
	smoothShading = false;
	normalsCalculated = false;
	sceneRenderingMode=0;

	setWindowTitle(tr("Sun in the city"));
}


void WindowQT::openModel()
{
    QString s = QFileDialog::getOpenFileName(this, "Choose a file to open", "/home", "Data Exchange File (*.dxf);;Wavefront OBJ (*.obj)");
	if(s==NULL)	return;
	char *fileName = new char[s.size()+1];
	for(int i=0; i<s.size();i++)
	{
		fileName[i] = (s.data()[i]).toAscii();
	}
	fileName[s.size()] = '\0';

	// restore old values in model adjustment dialog
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;;
	invertFaces = false;
	invertNormals = false;
	smoothShading = false;
	normalsCalculated = false;
	sceneRenderingMode = 0;

	mDialog->rotXSpin->setValue(rotX);
	mDialog->rotYSpin->setValue(rotY);
	mDialog->rotZSpin->setValue(rotZ);

	if(invertFaces)	mDialog->invertFacesCheck->setCheckState(Qt::Checked);
	else	mDialog->invertFacesCheck->setCheckState(Qt::Unchecked);

	if(invertNormals)	mDialog->invertNormalsCheck->setCheckState(Qt::Checked);
	else	mDialog->invertNormalsCheck->setCheckState(Qt::Unchecked);

	if(smoothShading)	mDialog->smoothShadingCheck->setCheckState(Qt::Checked);
	else	mDialog->smoothShadingCheck->setCheckState(Qt::Unchecked);

	sceneRenderingMode = 0;
	mDialog->shadowsRadio->setChecked(true);
	frameQT->setSceneRenderingMode((float)sceneRenderingMode);
	
	frameQT->initModel(fileName);
	frameQT->initFBOs();
	frameQT->updateMeshShadowTexture();
}

void WindowQT::aboutProgram()
{
	aboutDialog->show();
}

void WindowQT::showShadowingPeriod()
{
	shadowingDialog->show();
}

void WindowQT::okShadowingPeriod()
{
	// set new start date and time
	frameQT->setStartDate( sDialog->startDate->date().year(),
	                       sDialog->startDate->date().month(),
	                       sDialog->startDate->date().day(),
						   sDialog->startTime->time().hour(),
	                       sDialog->startTime->time().minute(),
	                       sDialog->startTime->time().second() );

	// set new end date and time
	frameQT->setEndDate( sDialog->endDate->date().year(),
	                     sDialog->endDate->date().month(),
	                     sDialog->endDate->date().day(),
						 sDialog->endTime->time().hour(),
	                     sDialog->endTime->time().minute(),
	                     sDialog->endTime->time().second() );
	
	// set new longitude
	frameQT->setLongitude( sDialog->longitudeDeg->value(),
	                       sDialog->longitudeMin->value(),
	                       sDialog->longitudeSec->value() );
	
	// set new latitude
	frameQT->setLatitude( sDialog->latitudeDeg->value(),
	                      sDialog->latitudeMin->value(),
						  sDialog->latitudeSec->value() );
	
	// set new timezone
	frameQT->setTimeZone( sDialog->timeZone->value());
	
	// set new slides count
	frameQT->setSlidesCount(sDialog->slidesCount->value());

	// update data
	frameQT->updateMeshShadowTexture();
	
	shadowingDialog->close();
}

// TODO needs work
void WindowQT::cancelShadowingPeriod()
{
	// TODO set all back to previous

	shadowingDialog->close();
}


void WindowQT::showModelAdjustment()
{
	modelAdjustmentDialog->show();
}


void WindowQT::okModelAdjustment()
{
	bool modelStateChanged = false;

	// handle the Rotation
	double rotXNew = mDialog->rotXSpin->value();
	double rotYNew = mDialog->rotYSpin->value();
	double rotZNew = mDialog->rotZSpin->value();
	if( (rotXNew - rotX != 0.0) || (rotYNew - rotY  != 0.0) || (rotZNew - rotZ  != 0.0) )
	{
		modelStateChanged = true;

		printf("rotX = %f\trotY = %f\t rotZ%f\n", rotXNew - rotX, rotYNew - rotY, rotZNew - rotZ);

		frameQT->getModel()->rotateMesh((float)rotXNew - rotX, (float)rotYNew - rotY, (float)rotZNew - rotZ);


		rotX = rotXNew;
		rotY = rotYNew;
		rotZ = rotZNew;
	}

	// handle the invert faces checkBox
	bool invertFacesNew = mDialog->invertFacesCheck->checkState() == Qt::Checked;
	if(invertFaces != invertFacesNew)
	{
		invertFaces = !invertFaces;
		modelStateChanged = true;
		frameQT->getModel()->invertFaces();
	}

	// handle the invert normals checkBox
	bool invertNormalsNew = mDialog->invertNormalsCheck->checkState() == Qt::Checked;
	if(invertNormals != invertNormalsNew)
	{
		invertNormals = !invertNormals;
		modelStateChanged = true;
		frameQT->getModel()->invertNormals();
	}

	// handle the smooth shading checkBox
	bool smoothShadingNew = mDialog->smoothShadingCheck->checkState() == Qt::Checked;
	if(smoothShading != smoothShadingNew)
	{
		smoothShading = !smoothShading;
		modelStateChanged = true;
		frameQT->setSmoothShading(smoothShading);

		normalsCalculated = true;
		// prevent from calculating the normals more then once for the same model
		if(!normalsCalculated)	frameQT->getModel()->calculateNormals();
		normalsCalculated = true;
	}

	// handle the radio buttons
	if(mDialog->shadowsRadio->isChecked())	sceneRenderingMode = 0;
	else if(mDialog->shadowsColorRadio->isChecked())	sceneRenderingMode = 1;
	else if(mDialog->shadowsAndLightingRadio->isChecked())	sceneRenderingMode = 2;
	else if(mDialog->shadowsAndLightingColorRadio->isChecked())	sceneRenderingMode = 3;
	frameQT->setSceneRenderingMode((float)sceneRenderingMode);

	// update model
	if(modelStateChanged)
	{
		frameQT->updateModel();
		frameQT->updateMeshShadowTexture();
	}

	modelAdjustmentDialog->close();
}


void WindowQT::cancelModelAdjustment()
{
	// restore old values
	mDialog->rotXSpin->setValue(rotX);
	mDialog->rotYSpin->setValue(rotY);
	mDialog->rotZSpin->setValue(rotZ);

	if(invertFaces)	mDialog->invertFacesCheck->setCheckState(Qt::Checked);
	else	mDialog->invertFacesCheck->setCheckState(Qt::Unchecked);

	if(invertNormals)	mDialog->invertNormalsCheck->setCheckState(Qt::Checked);
	else	mDialog->invertNormalsCheck->setCheckState(Qt::Unchecked);

	if(smoothShading)	mDialog->smoothShadingCheck->setCheckState(Qt::Checked);
	else	mDialog->smoothShadingCheck->setCheckState(Qt::Unchecked);

	sceneRenderingMode = 0;
	mDialog->shadowsRadio->setChecked(true);
	frameQT->setSceneRenderingMode((float)sceneRenderingMode);

	modelAdjustmentDialog->close();
}


void WindowQT::exitProgram()
{
	exit(0);
}

