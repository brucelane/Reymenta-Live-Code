#include "ReymentaLiveCodeApp.h"

void ReymentaLiveCodeApp::setup()
{
	// parameters
	mParameterBag = ParameterBag::create();
	// utils
	mBatchass = Batchass::create(mParameterBag);
	CI_LOG_V("reymenta setup");

	setWindowSize(mParameterBag->mMainWindowWidth, mParameterBag->mMainWindowHeight);
	setWindowPos(ivec2(1610, 50));
	setFrameRate(60.0f);

	// setup shaders and textures
	mBatchass->setup();

	mParameterBag->mLiveCode = true;

	mParameterBag->iResolution.x = mParameterBag->mFboWidth;
	mParameterBag->iResolution.y = mParameterBag->mFboHeight;
	mParameterBag->mRenderResolution = ivec2(mParameterBag->mFboWidth, mParameterBag->mFboHeight);

	mTexture = gl::Texture::create(loadImage(loadAsset("reymenta.jpg")));
	//mLiveShader = mBatchass->getShadersRef()->getLiveShader();
	// Create CodeEditor
	mCodeEditor = CodeEditor::create(list_of<string>("live.frag")("live.vert").convert_to_container<vector<fs::path>>(), CodeEditor::Settings().autoSave().codeCompletion().lineNumbers());

	mCodeEditor->registerCodeChanged("live.frag", "live.vert", [this](const string& frag, const string& vert)
	{
		try {
			mShader = gl::GlslProg::create(vert.c_str(), frag.c_str());
			mCodeEditor->clearErrors();
			mBatchass->wsWriteText(frag);
		}
		catch (gl::GlslProgCompileExc exc) {
			mCodeEditor->setError("CodeEditor: " + string(exc.what()));
		}
	});
	mCodeEditor->setTheme("dark");
	mCodeEditor->setOpacity(0.8f);
	mCodeEditor->blur();
	mCodeEditor->setFontSize(16);
	mCodeEditor->enableLineWrapping(false);
	// set ui window and io events callbacks
	ui::initialize();
	margin = 3;
	largeW = (mParameterBag->mPreviewFboWidth + margin) * 4;
	largeH = (mParameterBag->mPreviewFboHeight + margin) * 5;
	xPos = mParameterBag->mMainWindowWidth - largeW*2 - 30;
	yPos = mParameterBag->mMainWindowHeight / 2;
	removeUI = false;
}

void ReymentaLiveCodeApp::shutdown()
{
	mParameterBag->save();
}

void ReymentaLiveCodeApp::update()
{
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
	//mBatchass->getTexturesRef()->draw();
	gl::setMatricesWindow(getWindowWidth(), getWindowHeight());

	// clear out the window with black
	gl::clear(ColorAf(0.0f, 0.0f, 0.0f, 1.0f));
	if (mShader)
	{
		mTexture->bind();

		gl::enableAlphaBlending();
		mShader->bind();
		mShader->uniform("iGlobalTime", mParameterBag->iGlobalTime);
		mShader->uniform("iResolution", mParameterBag->iResolution);
		mShader->uniform("iChannel0", 0);
		mShader->uniform("iChannel1", 0);
		mShader->uniform("iChannel2", 0);
		mShader->uniform("iChannel3", 0);
		mShader->uniform("iChannel4", 0);
		mShader->uniform("iChannel5", 0);
		mShader->uniform("iChannel6", 0);
		mShader->uniform("iChannel7", 0);

		gl::drawSolidRect(Rectf(0, yPos, mParameterBag->mFboWidth, yPos + mParameterBag->mFboHeight));
		gl::disableAlphaBlending();
		mTexture->unbind();
		
	}
	if (removeUI) return;
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

#pragma region WebSockets
	// websockets
	ui::SetNextWindowSize(ImVec2(largeW, largeH), ImGuiSetCond_Once);
	ui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiSetCond_Once);
	ui::Begin("WebSockets server");
	{
		if (mParameterBag->mIsWebSocketsServer)
		{
			ui::Text("WS Server %d", mParameterBag->mWebSocketsPort);
			ui::Text("IP %s", mParameterBag->mWebSocketsHost.c_str());
		}
		else
		{
			ui::Text("WS Client %d", mParameterBag->mWebSocketsPort);
			ui::Text("IP %s", mParameterBag->mWebSocketsHost.c_str());
		}
		if (ui::Button("Connect")) { mBatchass->wsConnect(); }
		ui::SameLine();
		if (ui::Button("Ping")) { mBatchass->wsPing(); }
	}

	ui::End();
	xPos += largeW + margin;
#pragma endregion WebSockets
	ui::SetNextWindowSize(ImVec2(largeW, largeH), ImGuiSetCond_Once);
	ui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiSetCond_Once);
	ui::Begin("console");
	{
		ui::Text("%s", mParameterBag->mMsg.c_str());
		ui::SameLine();

		ui::Image((void*)mBatchass->getTexturesRef()->getFboTextureId(mParameterBag->mLiveFboIndex), ivec2(mParameterBag->mFboWidth, mParameterBag->mFboHeight));

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

CINDER_APP(ReymentaLiveCodeApp, RendererGl)
