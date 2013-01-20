inline void FrameQT::renderBackground()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(0);

	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f); 
		glColor3f( 0.75, 0.75, 1);
		glVertex3f(-2.0f,-1.5f,-2.0f);
		glVertex3f(+2.0f,-1.5f,-2.0f);
		glColor3f( 0, 0, 0);
		glVertex3f(+2.0f,+1.5f,-2.0f);
		glVertex3f(-2.0f,+1.5f,-2.0f);
	glEnd();

	glDepthMask(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

inline void FrameQT::renderSun()
{
	// rotate the Light Source
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glRotatef(-g_fLightAzimuth,0,1,0);	// rotate azimuth
	//glRotatef(g_fLightElevation,-1,0,0);	// rotate elevation
	//glTranslatef(0,0,g_fLightTransZ*1.1);

	// Modelview Transformation
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//glTranslatef( 0, 0, g_fCameraZoom); // Zoom
	glRotatef(g_fCameraElevation,1,0,0);  // rotate elevation
	glRotatef(g_fCameraAzimuth,0,1,0);    // rotate azimuth
	//glTranslatef( 0, -g_fGroundLevel, 0); 
	
	//Calculate Current Light Position (for Shadows)

	// render a sprite for the Light 
	glPointSize(67.0f); 
	glColor3f( 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D,g_iSunTex);
	
	float quadratic[3] = {1.0f,0.1f,0.01f};
	glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB,quadratic);
	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
	
	glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
	glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
	glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, 67.0f );
	
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable( GL_POINT_SPRITE_ARB );
	glDepthMask(false);
	
	initShadowingPeriod();

	for(int i=0;i<g_iSteps;i++)
	{
		if(i!=0)	updateCurrentDate();
		updateSunPosition();
		updateLightPosition();

		// draw the sprite	
		glBegin(GL_POINTS); 
			glVertex3f(g_pfLightPos[0]*g_fLightTransZ,g_pfLightPos[1]*g_fLightTransZ,g_pfLightPos[2]*g_fLightTransZ);
		glEnd();
	}

	
	// restore stuff
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable( GL_POINT_SPRITE_ARB );
	glDisable(GL_BLEND);
	glDepthMask(true);
}

inline void FrameQT::renderGround() {

	//glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f); // needs only to be set if it changes.
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(-2.0f,0.0f,-2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(-2.0f,0.0f,+2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(+2.0f,0.0f,+2.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(+2.0f,0.0f,-2.0f);
	glEnd();
}

inline void FrameQT::renderMesh() {

	//glColor3f(1.0f,1.0f,1.0f);
	g_oMeshVBO.draw();

	// visualize the cameraPositions
//	glLineWidth(15.0);
//	glDisable(GL_LIGHTING);
//	glColor3f(0,1,0);
//	cameras.draw();
	
}

inline void FrameQT::renderMeshTex() 
{
	// render the model
	int rest = g_iVertexCount / 4;
	int length=0;

	for(int i=0;i<g_iShadowTextureCount;i++)
	{
		// determine how many vertices to render
		if(rest < (g_iShadowTexRowCount*g_iShadowTexRowCount) )	length = rest;
		else length = g_iShadowTexRowCount*g_iShadowTexRowCount;

		// render the mesh
		glBindTexture(GL_TEXTURE_2D, g_ipShadowTextures[i]);
		g_oMeshVBO.draw(i*4*g_iShadowTexRowCount*g_iShadowTexRowCount,4*length) ;
		rest -= g_iShadowTexRowCount*g_iShadowTexRowCount;
	}

	// render Ground
	glBindTexture(GL_TEXTURE_2D,g_oGroundTexFBO.colorBufferTex);
	//glBindTexture(GL_TEXTURE_2D, g_ipShadowTextures[0]);
	//glBindTexture(GL_TEXTURE_2D, g_oShadowFBO.colorBufferTex);
	renderGround();
	glBindTexture(GL_TEXTURE_2D, 0);
}



inline void FrameQT::renderLights()
{
	glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
}


inline void FrameQT::initShadowingPeriod()
{
	
	g_sCurrentDate.year   = g_sStartDate.year;
	g_sCurrentDate.month  = g_sStartDate.month;
	g_sCurrentDate.day    = g_sStartDate.day;
	g_sCurrentDate.hour   = g_sStartDate.hour;
	g_sCurrentDate.minute = g_sStartDate.minute;
	g_sCurrentDate.second = g_sStartDate.second;

	g_dStartDate = sunPosition.dateToJulian(g_sCurrentDate);
	g_dCurrentDate = g_dStartDate;
	g_dStopDate = sunPosition.dateToJulian(g_sStopDate);
	
	g_dShadowingPeriod = g_dStopDate - g_dStartDate;

	g_dTimePerFrame = g_dShadowingPeriod / (double)g_iSteps;
}

// update the current time according to the elapsed time
inline void FrameQT::updateCurrentDate()
{	
	if(g_dShadowingPeriod <= 0  )
	{	
		return;
		//if(!g_bToggleLoopAnimation)	return;
		//
		//// reinit g_dDeltaMinutes
		//g_dDeltaMinutes = g_dStopDate - g_dStartDate;
	} 
	else 
	{
		g_dShadowingPeriod -= g_dTimePerFrame;
		g_dCurrentDate += g_dTimePerFrame;
		g_sCurrentDate = sunPosition.julianToDate(g_dCurrentDate);
	}
	
	//printf("%i %i %i %i %i %i %s %f\n",g_ipCurrentDate[0],g_ipCurrentDate[1],g_ipCurrentDate[2],g_ipCurrentDate[3],g_ipCurrentDate[4],g_ipCurrentDate[5],g_cpWeekdays[(int)fmod(g_dCurrentDate,7.0)],g_dCurrentDate);
}

// calculate new sun position based on the current date
inline void FrameQT::updateSunPosition()
{
	// update sunPosition
	sunPosition.setDate(g_sCurrentDate);
	sunPosition.calculate();
	
	// update local azimuth and elevation
	g_fLightAzimuth = (float)sunPosition.azimuthOut;
	g_fLightElevation = (float)sunPosition.elevationOut;
	
	updateLightPosition();
}

// set the Light position Array, by projecting the azimuth and elevation angles
inline void FrameQT::updateLightPosition()
{
	// x component
	g_pfLightPos[0]	= cos(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));
	
	// y component
	g_pfLightPos[1] = sin(degToRad(g_fLightElevation));
	
	// z component
	g_pfLightPos[2] = sin(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));

	// normalize Light vector
	float vLength = sqrt(g_pfLightPos[0]*g_pfLightPos[0] + g_pfLightPos[1]*g_pfLightPos[1] + g_pfLightPos[2]*g_pfLightPos[2]);
	g_pfLightPos[0] /= vLength;
	g_pfLightPos[1] /= vLength;
	g_pfLightPos[2] /= vLength;

	// set the light position in the openGL state
	float *lightVector = new float[4];
	lightVector[0] = g_pfLightPos[0];
	lightVector[1] = g_pfLightPos[2];
	lightVector[2] = g_pfLightPos[1];
	lightVector[3] = 1.0f;
	glLightfv( GL_LIGHT0, GL_POSITION, lightVector);
}


inline void FrameQT::setupUserViewProj() {
	glViewport(0,0,g_iWindowWidth,g_iWindowHeight);	// and set new dimensions
	glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
	glLoadIdentity();
	gluPerspective(60.0,g_fAspect,0.002,20.0);  
	glMatrixMode(GL_MODELVIEW);
}


inline void FrameQT::setupCameraTexOrtho() {
	glViewport(0,0,g_iShadowTexSize,g_iShadowTexSize);	// and set new dimensions
    glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
    glLoadIdentity();
}

inline Date FrameQT::getStartDate()
{
        return g_sStartDate;
}
inline void FrameQT::setStartDate(int y, int m, int d, int h, int min, int s)
{
        g_sStartDate.year = y;
        g_sStartDate.month = m;
        g_sStartDate.day = d;
        g_sStartDate.hour = h;
        g_sStartDate.minute = min;
        g_sStartDate.second = s;
}

inline Date FrameQT::getEndDate()
{
        return g_sStopDate;
}
inline void FrameQT::setEndDate(int y, int m, int d, int h, int min, int s)
{
        g_sStopDate.year = y;
        g_sStopDate.month = m;
        g_sStopDate.day = d;
        g_sStopDate.hour = h;
        g_sStopDate.minute = min;
        g_sStopDate.second = s;
}

inline int *FrameQT::getLongitude()
{
        return g_ipLongitude;
}
inline void FrameQT::setLongitude(int deg, int min, int sec)
{
        g_ipLongitude[0]=deg;
        g_ipLongitude[1]=min;
        g_ipLongitude[2]=sec;
}

inline int *FrameQT::getLatitude()
{
        return g_ipLatitude;
}
inline void FrameQT::setLatitude(int deg, int min, int sec)
{
        g_ipLatitude[0]=deg;
        g_ipLatitude[1]=min;
        g_ipLatitude[2]=sec;
}

inline int FrameQT::getTimeZone()
{
        return g_fTimeZone;
}
inline void FrameQT::setTimeZone(int inputTimeZone)
{
        g_fTimeZone = inputTimeZone;
}

inline int FrameQT::getSlidesCount()
{
        return g_iSteps;
}
inline void FrameQT::setSlidesCount(int slides)
{
        g_iSteps = slides;
}

inline bool FrameQT::getSmoothShading()
{
	return g_bSmoothShading;
}


inline void FrameQT::setSmoothShading(bool input)
{
	g_bSmoothShading = input;
}


inline MeshDXF *FrameQT::getModel()
{
	return g_oMesh;
}


inline void FrameQT::setSceneRenderingMode(float input)
{
	g_fSceneRenderingMode = input;
}
