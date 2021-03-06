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

#include <QFileDialog>

#include "ShadowSimulation.hpp"

ShadowSimulation::ShadowSimulation()
{
	// setup this window
	ui.setupUi(this);

    // connect stuff
    connect( ui.load, SIGNAL(clicked()), this, SLOT(loadModel()) );
    connect( ui.compute, SIGNAL(clicked()), this, SLOT(updateShadows()) );
}


void ShadowSimulation::loadModel()
{
    // load model
    std::string filename = QFileDialog::getOpenFileName(this, "Choose a file to open", ".", "Wavefront OBJ (*.obj);;Data Exchange File (*.dxf)").toStdString();
    if( filename.size() > 0 )
        m_sc.loadModel( filename );

    // update shadows
    updateShadows();
}


void ShadowSimulation::updateShadows()
{
    // set start & end
    m_sc.setStartDate( ui.start_date->date().year(), ui.start_date->date().month(), ui.start_date->date().day(),
                       ui.start_time->time().hour(), ui.start_time->time().minute(),ui.start_time->time().second() );
    m_sc.setEndDate( ui.end_date->date().year(), ui.end_date->date().month(), ui.end_date->date().day(),
                     ui.end_time->time().hour(), ui.end_time->time().minute(),ui.end_time->time().second() );
    m_sc.setLongitude( ui.long_deg->value(), ui.long_min->value(), ui.long_sec->value() );
    m_sc.setLongitude( ui.lat_deg->value(), ui.lat_min->value(), ui.lat_sec->value() );
    m_sc.setTimeZone( ui.timezone->value() );

    // set shadow params
    switch( ui.face_res->currentIndex() )
    {
        case 0 : m_sc.setShadowParams( 8 ); break;
        default :
        case 1 : m_sc.setShadowParams( 16 ); break;
        case 2 : m_sc.setShadowParams( 32 ); break;
        case 3 : m_sc.setShadowParams( 64 ); break;
    }

    // compue the shadows
    m_sc.compute( ui.slides->value(), ui.smooth_shading->isChecked() );
}


//void ShadowView::openModel()
//{
//    QString s = QFileDialog::getOpenFileName(this, "Choose a file to open", "/home", "Wavefront OBJ (*.obj);;Data Exchange File (*.dxf)");
//	if(s==NULL)	return;
//	char *fileName = new char[s.size()+1];
//	for(int i=0; i<s.size();i++)
//	{
//		fileName[i] = (s.data()[i]).toAscii();
//	}
//	fileName[s.size()] = '\0';

//	// restore old values in model adjustment dialog
//	rotX = 0.0;
//	rotY = 0.0;
//	rotZ = 0.0;;
//	invertFaces = false;
//	invertNormals = false;
//	smoothShading = false;
//	normalsCalculated = false;
//	sceneRenderingMode = 0;

//	mDialog->rotXSpin->setValue(rotX);
//	mDialog->rotYSpin->setValue(rotY);
//	mDialog->rotZSpin->setValue(rotZ);

//	if(invertFaces)	mDialog->invertFacesCheck->setCheckState(Qt::Checked);
//	else	mDialog->invertFacesCheck->setCheckState(Qt::Unchecked);

//	if(invertNormals)	mDialog->invertNormalsCheck->setCheckState(Qt::Checked);
//	else	mDialog->invertNormalsCheck->setCheckState(Qt::Unchecked);

//	if(smoothShading)	mDialog->smoothShadingCheck->setCheckState(Qt::Checked);
//	else	mDialog->smoothShadingCheck->setCheckState(Qt::Unchecked);

//	sceneRenderingMode = 0;
//	mDialog->shadowsRadio->setChecked(true);
//	frameQT->setSceneRenderingMode((float)sceneRenderingMode);
	
//	frameQT->initModel(fileName);
//	frameQT->initFBOs();
//	frameQT->updateMeshShadowTexture();
//}

//void ShadowView::aboutProgram()
//{
//	aboutDialog->show();
//}

//void ShadowView::showShadowingPeriod()
//{
//	shadowingDialog->show();
//}

//void ShadowView::okShadowingPeriod()
//{
//	// set new start date and time
//	frameQT->setStartDate( sDialog->startDate->date().year(),
//	                       sDialog->startDate->date().month(),
//	                       sDialog->startDate->date().day(),
//						   sDialog->startTime->time().hour(),
//	                       sDialog->startTime->time().minute(),
//	                       sDialog->startTime->time().second() );

//	// set new end date and time
//	frameQT->setEndDate( sDialog->endDate->date().year(),
//	                     sDialog->endDate->date().month(),
//	                     sDialog->endDate->date().day(),
//						 sDialog->endTime->time().hour(),
//	                     sDialog->endTime->time().minute(),
//	                     sDialog->endTime->time().second() );
	
//	// set new longitude
//	frameQT->setLongitude( sDialog->longitudeDeg->value(),
//	                       sDialog->longitudeMin->value(),
//	                       sDialog->longitudeSec->value() );
	
//	// set new latitude
//	frameQT->setLatitude( sDialog->latitudeDeg->value(),
//	                      sDialog->latitudeMin->value(),
//						  sDialog->latitudeSec->value() );
	
//	// set new timezone
//	frameQT->setTimeZone( sDialog->timeZone->value());
	
//	// set new slides count
//	frameQT->setSlidesCount(sDialog->slidesCount->value());

//	// update data
//	frameQT->updateMeshShadowTexture();
	
//	shadowingDialog->close();
//}

//// TODO needs work
//void ShadowView::cancelShadowingPeriod()
//{
//	// TODO set all back to previous

//	shadowingDialog->close();
//}


//void ShadowView::showModelAdjustment()
//{
//	modelAdjustmentDialog->show();
//}


//void ShadowView::okModelAdjustment()
//{
//	bool modelStateChanged = false;

//	// handle the Rotation
//	double rotXNew = mDialog->rotXSpin->value();
//	double rotYNew = mDialog->rotYSpin->value();
//	double rotZNew = mDialog->rotZSpin->value();
//	if( (rotXNew - rotX != 0.0) || (rotYNew - rotY  != 0.0) || (rotZNew - rotZ  != 0.0) )
//	{
//		modelStateChanged = true;

//		printf("rotX = %f\trotY = %f\t rotZ%f\n", rotXNew - rotX, rotYNew - rotY, rotZNew - rotZ);

//		frameQT->getModel()->rotateMesh((float)rotXNew - rotX, (float)rotYNew - rotY, (float)rotZNew - rotZ);


//		rotX = rotXNew;
//		rotY = rotYNew;
//		rotZ = rotZNew;
//	}

//	// handle the invert faces checkBox
//	bool invertFacesNew = mDialog->invertFacesCheck->checkState() == Qt::Checked;
//	if(invertFaces != invertFacesNew)
//	{
//		invertFaces = !invertFaces;
//		modelStateChanged = true;
//		frameQT->getModel()->invertFaces();
//	}

//	// handle the invert normals checkBox
//	bool invertNormalsNew = mDialog->invertNormalsCheck->checkState() == Qt::Checked;
//	if(invertNormals != invertNormalsNew)
//	{
//		invertNormals = !invertNormals;
//		modelStateChanged = true;
//		frameQT->getModel()->invertNormals();
//	}

//	// handle the smooth shading checkBox
//	bool smoothShadingNew = mDialog->smoothShadingCheck->checkState() == Qt::Checked;
//	if(smoothShading != smoothShadingNew)
//	{
//		smoothShading = !smoothShading;
//		modelStateChanged = true;
//		frameQT->setSmoothShading(smoothShading);

//		normalsCalculated = true;
//		// prevent from calculating the normals more then once for the same model
//		if(!normalsCalculated)	frameQT->getModel()->calculateNormals();
//		normalsCalculated = true;
//	}

//	// handle the radio buttons
//	if(mDialog->shadowsRadio->isChecked())	sceneRenderingMode = 0;
//	else if(mDialog->shadowsColorRadio->isChecked())	sceneRenderingMode = 1;
//	else if(mDialog->shadowsAndLightingRadio->isChecked())	sceneRenderingMode = 2;
//	else if(mDialog->shadowsAndLightingColorRadio->isChecked())	sceneRenderingMode = 3;
//	frameQT->setSceneRenderingMode((float)sceneRenderingMode);

//	// update model
//	if(modelStateChanged)
//	{
//		frameQT->updateModel();
//		frameQT->updateMeshShadowTexture();
//	}

//	modelAdjustmentDialog->close();
//}


//void ShadowView::cancelModelAdjustment()
//{
//	// restore old values
//	mDialog->rotXSpin->setValue(rotX);
//	mDialog->rotYSpin->setValue(rotY);
//	mDialog->rotZSpin->setValue(rotZ);

//	if(invertFaces)	mDialog->invertFacesCheck->setCheckState(Qt::Checked);
//	else	mDialog->invertFacesCheck->setCheckState(Qt::Unchecked);

//	if(invertNormals)	mDialog->invertNormalsCheck->setCheckState(Qt::Checked);
//	else	mDialog->invertNormalsCheck->setCheckState(Qt::Unchecked);

//	if(smoothShading)	mDialog->smoothShadingCheck->setCheckState(Qt::Checked);
//	else	mDialog->smoothShadingCheck->setCheckState(Qt::Unchecked);

//	sceneRenderingMode = 0;
//	mDialog->shadowsRadio->setChecked(true);
//	frameQT->setSceneRenderingMode((float)sceneRenderingMode);

//	modelAdjustmentDialog->close();
//}


//void ShadowView::exitProgram()
//{
//	exit(0);
//}

