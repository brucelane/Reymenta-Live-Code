#include "ReymentaLiveCodeApp.h"


void ReymentaLiveCodeApp::prepareSettings(Settings *settings)
{
	settings->setTitle("Reymenta Live Code");
	mRenderWidth = 1280;
	mRenderHeight = 720;
	settings->setWindowSize(mRenderWidth, mRenderHeight);
	settings->setFullScreen(false);
	settings->setResizable(false); // keep the screen size constant for a sender
	settings->setFrameRate(120.0f);
}

void ReymentaLiveCodeApp::setup()
{
	iGlobalTime = 1.0f;
	iResolution = Vec3f(mRenderWidth, mRenderHeight, 1.0);
	// spout
	glEnable(GL_TEXTURE_2D);
	gl::enableDepthRead();
	gl::enableDepthWrite();
	g_Width = mRenderWidth;
	g_Height = mRenderHeight;
	// Set up the texture we will use to send out
	// We grab the screen so it has to be the same size
	spoutTexture = gl::Texture(g_Width, g_Height);
	strcpy_s(SenderName, "Reymenta Live Code"); // we have to set a sender name first
	bInitialized = InitSender(SenderName, g_Width, g_Height, bTextureShare);
	// bTextureShare informs us whether Spout initialized for texture share or memory share
	//mSpoutFbo = gl::Fbo(g_Width, g_Height);

	// Create CodeEditor
	mCodeEditor = CodeEditor::create("shaders/simple.frag", CodeEditor::Settings().autoSave().codeCompletion());

	mCodeEditor->registerCodeChanged( "shaders/simple.frag", [this](const string& frag) {
		try {
			mShader = gl::GlslProg( NULL, frag.c_str() );
			mCodeEditor->clearErrors();
}
		catch (gl::GlslProgCompileExc exc) {
			mCodeEditor->setError("Simple: " + string(exc.what()));
		}
	} );
	mCodeEditor->setTheme("dark");
	mCodeEditor->setOpacity(0.9f);
	mCodeEditor->enableLineWrapping(false);

}

void ReymentaLiveCodeApp::shutdown()
{
	ReleaseSender();
}

void ReymentaLiveCodeApp::update()
{
	app::getWindow()->setTitle( "Reymenta Live Code - " + toString(getAverageFps()) );
	iGlobalTime = getElapsedSeconds();
}

void ReymentaLiveCodeApp::draw()
{
	// clear out the window with black
	gl::clear(Color(0, 0, 0));

	if (mShader){
		gl::enableAlphaBlending();
		mShader.bind();
		mShader.uniform("iGlobalTime", iGlobalTime);
		// to move inside the shader it's ok: aShader.uniform("iResolution", Vec3f(mParameterBag->mRenderResoXY.x, mParameterBag->mRenderResoXY.y, 1.0));
		mShader.uniform("iResolution", Vec3f(mRenderWidth, mRenderHeight, 1.0));
		gl::drawSolidRect(getWindowBounds());
		mShader.unbind();
		gl::disableAlphaBlending();
	}
	// -------- SPOUT -------------
	if (bInitialized) {

		// Grab the screen (current read buffer) into the local spout texture
		spoutTexture.bind();
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
		spoutTexture.unbind();

		// Send the texture for all receivers to use
		SendTexture(spoutTexture.getId(), spoutTexture.getTarget(), g_Width, g_Height, true); // true meaning invert

	}
	//Spout::SendTexture(mSpoutFbo.getTexture().getId(), mSpoutFbo.getTexture().getTarget(), mSpoutFbo.getWidth(), mSpoutFbo.getHeight(), false);

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
