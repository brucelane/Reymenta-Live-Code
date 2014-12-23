#include "cinder/ImageIO.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

// transparent window
#include "dwmapi.h"
// parameters
#include "ParameterBag.h"

#include "CodeEditor.h"
#include "UIController.h"
#include "UIElement.h"
#if defined( CINDER_COCOA )
#include "cinderSyphon.h"
#else
#include "Spout.h"
#endif
// VisualStudio does'nt seem to support initializer_list
// yet so let's use boost::assign::list_of instead
#if defined( CINDER_MSW )
#include "boost/assign/list_of.hpp"
using namespace boost::assign;
#endif
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace MinimalUI;
using namespace Reymenta;

class ReymentaLiveCodeApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);

	void setup();
	void shutdown();
	void update();
	void draw();

	void mouseMove(MouseEvent event);
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseWheel(MouseEvent event);

	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
private:
	int						mRenderWidth;
	int						mRenderHeight;
	float					iGlobalTime;        // shader playback time (in seconds)
	Vec3f					iResolution;        // viewport resolution (in pixels)
	gl::GlslProg			mShader;
	CodeEditorRef			mCodeEditor;
	// Parameters
	ParameterBagRef				mParameterBag;

};