#include "ReymentaLiveCodeApp.h"


void ReymentaLiveCodeApp::prepareSettings(Settings *settings)
{
	settings->setTitle("Reymenta Live Code");
	settings->setWindowSize( 1280, 720 );
}

void ReymentaLiveCodeApp::setup()
{
	// Create CodeEditor
	mCodeEditor = CodeEditor::create("shaders/simple.frag");

	mCodeEditor->registerCodeChanged( "shaders/simple.frag", [this](const string& frag) {
		try {
			mShader = gl::GlslProg( NULL, frag.c_str() );
			mCodeEditor->clearErrors();
}
		catch (gl::GlslProgCompileExc exc) {
			mCodeEditor->setError("Simple: " + string(exc.what()));
		}
	} );
}

void ReymentaLiveCodeApp::shutdown()
{
	
}

void ReymentaLiveCodeApp::update()
{
	app::getWindow()->setTitle( "Reymenta Live Code - " + toString(getAverageFps()) );
	
}

void ReymentaLiveCodeApp::draw()
{
	// clear out the window with black
	gl::clear(Color(0, 0, 0));

	if (mShader){
		gl::enableAlphaBlending();
		mShader.bind();
		gl::drawSolidRect(getWindowBounds());
		mShader.unbind();
		gl::disableAlphaBlending();
	}
}

void ReymentaLiveCodeApp::resize()
{
	
}

void ReymentaLiveCodeApp::mouseMove( MouseEvent event )
{

}

void ReymentaLiveCodeApp::mouseDown( MouseEvent event )
{
	
}

void ReymentaLiveCodeApp::mouseDrag( MouseEvent event )
{
	
}

void ReymentaLiveCodeApp::mouseUp( MouseEvent event )
{

}

void ReymentaLiveCodeApp::mouseWheel( MouseEvent event )
{

}

void ReymentaLiveCodeApp::keyDown( KeyEvent event )
{
	
}

void ReymentaLiveCodeApp::keyUp( KeyEvent event )
{
	
}

CINDER_APP_BASIC( ReymentaLiveCodeApp, RendererGl )
