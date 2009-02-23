
// CODESTYLE: v2.0

// ged101minimal.h
// ged101 minimal headers master inclusion header ()
// Author: Richard Marks

#ifndef __GED101MINIMAL_H__
#define __GED101MINIMAL_H__

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <allegro.h>

// include the minimal ged101 headers
#include "DebugReport.h"
#include "GraphicsDevice.h"
#include "ImageResource.h"
#include "ColorRGB.h"
#include "BitmapFont.h"
#include "NameDirectory.h"
#include "ImageList.h"
#include "InputDevice.h"
#include "GameTimer.h"

#if 0
// this is purely useless for now -- much needs to be re-thought out.
#include "NativeGUITextEntry.h"
#endif

#include "GUIListBox.h"
#include "GUITextEntryDialog.h"

#if defined(GED101_USE_SIMPLE_NAMESPACES)
using namespace ENGINE;
#endif

#endif

