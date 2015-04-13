#include "ReymentaLiveCodeApp.h"

void ReymentaLiveCodeApp::prepareSettings(Settings *settings)
{
	settings->setTitle("Reymenta Live Code");
	// parameters
	mParameterBag = ParameterBag::create();
	// utils
	mBatchass = Batchass::create(mParameterBag);

	settings->setWindowSize(mParameterBag->mMainWindowWidth, mParameterBag->mMainWindowHeight);
	settings->setWindowPos(Vec2i(10, 50));
	settings->setFrameRate(60.0f);
}

void ReymentaLiveCodeApp::setup()
{
	// instanciate the WebSockets class
	mWebSockets = WebSockets::create(mParameterBag, mBatchass);

	// setup shaders and textures
	mBatchass->setup();
	// instanciate the spout class
	mSpout = SpoutWrapper::create(mParameterBag, mBatchass->getTexturesRef());
	mParameterBag->mLiveCode = true;

	mParameterBag->iResolution.x = mParameterBag->mFboWidth;
	mParameterBag->iResolution.y = mParameterBag->mFboHeight;
	mParameterBag->mRenderResolution = Vec2i(mParameterBag->mFboWidth, mParameterBag->mFboHeight);

	// spout
	//glEnable(GL_TEXTURE_2D);
	//gl::enableDepthRead();
	//gl::enableDepthWrite();

	// Create CodeEditor
	mCodeEditor = CodeEditor::create(list_of<string>("live.frag")("live.vert").convert_to_container<vector<fs::path>>(), CodeEditor::Settings().autoSave().codeCompletion());

	mCodeEditor->registerCodeChanged("live.frag", "live.vert", [this](const string& frag, const string& vert) 
	{
		try {
			mShader = gl::GlslProg(vert.c_str(), frag.c_str());
			mCodeEditor->clearErrors();
			mWebSockets->write(frag);
		}
		catch (gl::GlslProgCompileExc exc) {
			mCodeEditor->setError("err: " + string(exc.what()));
		}
	});
	mCodeEditor->setTheme("dark");
	mCodeEditor->setOpacity(0.8f);
	mCodeEditor->blur();

	mCodeEditor->enableLineWrapping(false);
	// set ui window and io events callbacks
	ui::connectWindow(getWindow());
	ui::initialize();

}

void ReymentaLiveCodeApp::shutdown()
{
	mParameterBag->save();
}

void ReymentaLiveCodeApp::update()
{
	mWebSockets->update();
	mParameterBag->iFps = getAverageFps();
	mParameterBag->sFps = toString(floor(mParameterBag->iFps));
	getWindow()->setTitle("(" + mParameterBag->sFps + " fps) Reymenta Live Code");
	mParameterBag->iChannelTime[0] = getElapsedSeconds();
	mParameterBag->iChannelTime[1] = getElapsedSeconds() - 1;
	mParameterBag->iChannelTime[3] = getElapsedSeconds() - 2;
	mParameterBag->iChannelTime[4] = getElapsedSeconds() - 3;

	mParameterBag->iGlobalTime = getElapsedSeconds();
	mBatchass->update();

}

void ReymentaLiveCodeApp::draw()
{
	// draw the fbos
	mBatchass->getTexturesRef()->draw();
	gl::setViewport(getWindowBounds());
	gl::setMatricesWindow(getWindowWidth(), getWindowHeight());

	// clear out the window with black
	gl::clear(ColorAf(0.0f, 0.0f, 0.0f, 1.0f));

	if (mShader)
	{
		gl::enableAlphaBlending();
		mShader.bind();
		mShader.uniform("iGlobalTime", mParameterBag->iGlobalTime);
		mShader.uniform("iResolution", mParameterBag->iResolution);
		
		gl::drawSolidRect(Rectf(600.0, 400.0, 600.0 + mParameterBag->mFboWidth, 400.0 + mParameterBag->mFboHeight));
		mShader.unbind();
		gl::disableAlphaBlending();
	}
	// imgui
#pragma region style
	// our theme variables
	ImGuiStyle& style = ui::GetStyle();
	style.WindowRounding = 4;
	style.WindowPadding = ImVec2(3, 3);
	style.FramePadding = ImVec2(2, 2);
	style.ItemSpacing = ImVec2(3, 3);
	style.ItemInnerSpacing = ImVec2(3, 3);
	style.WindowMinSize = ImVec2(mParameterBag->mPreviewFboWidth, mParameterBag->mPreviewFboHeight);
	style.Alpha = 0.6f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.89f, 0.92f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.38f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.4f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.22f, 0.22f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.04f, 0.04f, 0.04f, 0.22f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.65f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.8f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9f, 0.45f, 0.45f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.65f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.65f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.8f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.65f, 0.25f, 0.25f, 1.00f);
#pragma endregion style
	ui::SetNextWindowSize(ImVec2(mParameterBag->mFboWidth, mParameterBag->mFboHeight*2), ImGuiSetCond_Once);
	ui::SetNextWindowPos(ImVec2(getWindowWidth() - mParameterBag->mFboWidth, 0), ImGuiSetCond_Once);
	ui::Begin("fbo");
	{
		if (ui::Button("Ping"))
		{
			mWebSockets->ping();
		}
		ui::Image((void*)mBatchass->getTexturesRef()->getFboTextureId(mParameterBag->mLiveFboIndex), Vec2i(mParameterBag->mFboWidth, mParameterBag->mFboHeight));
	}
	ui::End();
}

void ReymentaLiveCodeApp::resize()
{

}

void ReymentaLiveCodeApp::mouseMove(MouseEvent event)
{

}

void ReymentaLiveCodeApp::mouseDown(MouseEvent event)
{

}

void ReymentaLiveCodeApp::mouseDrag(MouseEvent event)
{

}

void ReymentaLiveCodeApp::mouseUp(MouseEvent event)
{

}

void ReymentaLiveCodeApp::mouseWheel(MouseEvent event)
{

}

void ReymentaLiveCodeApp::keyDown(KeyEvent event)
{
	if (event.isAccelDown() && event.getCode() == KeyEvent::KEY_f){
		setFullScreen(!isFullScreen());
	}
}

void ReymentaLiveCodeApp::keyUp(KeyEvent event)
{

}

CINDER_APP_BASIC(ReymentaLiveCodeApp, RendererGl)
