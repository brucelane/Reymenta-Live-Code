
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
	// parameters
	mParameterBag = ParameterBag::create();

	iGlobalTime = 1.0f;
	iResolution = Vec3f(mRenderWidth, mRenderHeight, 1.0);
	// spout
	glEnable(GL_TEXTURE_2D);
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// Create CodeEditor
	//mCodeEditor = CodeEditor::create("shaders/simple.frag", CodeEditor::Settings().window(mCodeEditorWindow).autoSave().codeCompletion());
	//mCodeEditor = CodeEditor::create("shaders/simple.frag", CodeEditor::Settings().autoSave().codeCompletion());
	mCodeEditor = CodeEditor::create(list_of<string>("simple.frag").convert_to_container<vector<fs::path>>(), CodeEditor::Settings().autoSave().codeCompletion());

	mCodeEditor->registerCodeChanged("simple.frag", [this](const string& frag) {
		try {
			mShader = gl::GlslProg(vert.c_str(), frag.c_str());
			mCodeEditor->clearErrors();
		}
		catch (gl::GlslProgCompileExc exc) {
			mCodeEditor->setError("E: " + string(exc.what()));
		}
	});
	mCodeEditor->setTheme("dark");
	mCodeEditor->setOpacity(0.9f);
	mCodeEditor->enableLineWrapping(false);
}

void ReymentaLiveCodeApp::shutdown()
{
	
}

void ReymentaLiveCodeApp::update()
{
	app::getWindow()->setTitle("Reymenta Live Code - " + toString(getAverageFps()));
	iGlobalTime = getElapsedSeconds();
}

void ReymentaLiveCodeApp::draw()
{
	// clear out the window with transparent
	gl::clear(ColorAf(0.0f, 0.0f, 0.0f, 1.0f));

	if (mShader){
		gl::enableAlphaBlending();
		mShader.bind();
		mShader.uniform("iGlobalTime", iGlobalTime);
		// to move inside the shader it's ok: aShader.uniform("iResolution", Vec3f(mParameterBag->mRenderResoXY.x, mParameterBag->mRenderResoXY.y, 1.0));
		mShader.uniform("iResolution", Vec3f(mRenderWidth, mRenderHeight, 1.0));
		//gl::drawSolidRect(getWindowBounds());
		gl::drawSolidRect(Rectf(100.0,100.0,300.0,300.0));
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
