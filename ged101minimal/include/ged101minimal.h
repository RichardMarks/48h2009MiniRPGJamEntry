
// CODESTYLE: v2.0

// ged101minimal.h
// ged101 minimal headers master inclusion header ()
// Author: Richard Marks

/**
 * \file ged101minimal.h
 * \brief Include this to have access to all the ged101minimal classes - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __GED101MINIMAL_H__
#define __GED101MINIMAL_H__

	// include the common headers
	#if !defined(GED101_NO_COMMON_HEADERS)
		// C++ C-style library
		#include <cstdio>
		#include <cstdlib>
		#include <cstring>
		#include <cstdarg>
		#include <cmath>
		#include <cctype>
		#include <ctime>

		// STL
		#include <string>
		#include <vector>
		#include <map>
		#include <algorithm>
	#endif

	// include allegro
	#include <allegro.h>

	// include the ged101 headers

	// DEBUGGING
	#if !defined(GED101_NO_DEBUG_HEADERS)
		#include "DebugReport.h"
	#endif

	// GRAPHICS
	#if !defined(GED101_NO_GRAPHICS_HEADERS)
		#include "GraphicsDevice.h"
		#include "ColorRGB.h"
		#include "ImageResource.h"
		#include "BitmapFont.h"
		#include "ImageList.h"
	#endif

	// GUI
	#if !defined(GED101_NO_GUI_HEADERS)
		#include "GUIListBox.h"
		#include "GUITextEntryDialog.h"
		#include "GUIMessageBox.h"
	#endif

	// UTILITY
	#if !defined(GED101_NO_UTILITY_HEADERS)
		#include "Settings.h"
		#include "TimeStamp.h"
		#include "StringTokenizer.h"
		#include "TextFileParser.h"
		#include "FileUtilities.h"
	#endif

	// INPUT
	#if !defined(GED101_NO_INPUT_HEADERS)
		#include "InputDevice.h"
	#endif

	// SYSTEM
	#if !defined(GED101_NO_SYSTEM_HEADERS)
		#include "GameTimer.h"
		#include "NameDirectory.h"
	#endif

	// fully qualified namespace
	#if defined(GED101_USE_SIMPLE_NAMESPACES)
		using namespace ENGINE;
	#endif

#endif

